#include "ofxOscCrypto.h"

#include "ofLog.h"

#include <openssl/evp.h>
#include <openssl/rand.h>

#include <limits>
#include <memory>
#include <stdexcept>

namespace ofxOsc {
namespace crypto {

namespace {

const unsigned char MAGIC[2] = { 'O', 'S' };
const unsigned char VERSION = 1;
constexpr int kDefaultIterations = 200000;

using CipherContext = std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)>;

const unsigned char * bytes(const std::string & value) {
	return reinterpret_cast<const unsigned char *>(value.data());
}

std::string randomBytes(std::size_t count) {
	std::string out(count, '\0');
	if (count > 0 && RAND_bytes(reinterpret_cast<unsigned char *>(out.data()), static_cast<int>(count)) != 1) {
		throw std::runtime_error("ofxOsc: RAND_bytes failed");
	}
	return out;
}

std::string sha256(const std::string & data) {
	std::string out(32, '\0');
	unsigned int length = 0;
	EVP_Digest(data.data(), data.size(), reinterpret_cast<unsigned char *>(out.data()), &length, EVP_sha256(), nullptr);
	out.resize(length);
	return out;
}

std::string toHex(const std::string & raw) {
	static const char * digits = "0123456789abcdef";
	std::string out;
	out.reserve(raw.size() * 2);
	for (unsigned char c : raw) {
		out.push_back(digits[c >> 4]);
		out.push_back(digits[c & 15]);
	}
	return out;
}

std::string deriveKey(const std::string & passphrase, const std::string & salt, int iterations, std::size_t keyLength) {
	std::string key(keyLength, '\0');
	if (PKCS5_PBKDF2_HMAC(passphrase.data(), static_cast<int>(passphrase.size()), bytes(salt), static_cast<int>(salt.size()),
			iterations, EVP_sha256(), static_cast<int>(keyLength), reinterpret_cast<unsigned char *>(key.data())) != 1) {
		throw std::runtime_error("ofxOsc: PBKDF2 failed");
	}
	return key;
}

void appendCounter(std::string & out, std::uint64_t counter) {
	for (int shift = 56; shift >= 0; shift -= 8) {
		out.push_back(static_cast<char>((counter >> shift) & 0xff));
	}
}

} // namespace

Cipher::Cipher(const Cipher & other) {
	copyFrom(other);
}

Cipher & Cipher::operator=(const Cipher & other) {
	if (this != &other) {
		copyFrom(other);
	}
	return *this;
}

void Cipher::copyFrom(const Cipher & other) {
	if (this == &other) {
		return;
	}
	std::scoped_lock lock(mutex, other.mutex);
	passphrase = other.passphrase;
	iterations = other.iterations;
	session = other.session;
	key = other.key;
	fingerprint = other.fingerprint;
	nextCounter = other.nextCounter;
}

void Cipher::setPassphrase(const std::string & newPassphrase, int newIterations) {
	std::lock_guard<std::mutex> lock(mutex);
	passphrase.clear();
	iterations = newIterations < 1 ? kDefaultIterations : newIterations;
	session.clear();
	key.clear();
	fingerprint.clear();
	nextCounter = 1;
	if (newPassphrase.empty()) {
		return;
	}
	try {
		session = randomBytes(SALT_SIZE);
		key = deriveKey(newPassphrase, "ofxOsc seal v1", iterations, 32);
		fingerprint = toHex(sha256(deriveKey(newPassphrase, "ofxOsc fingerprint", iterations, 32))).substr(0, 12);
		passphrase = newPassphrase;
	} catch (const std::exception & error) {
		session.clear();
		key.clear();
		fingerprint.clear();
		ofLogError("ofxOsc") << error.what();
	}
}

bool Cipher::isEnabled() const {
	std::lock_guard<std::mutex> lock(mutex);
	return !passphrase.empty();
}

std::string Cipher::getFingerprint() const {
	std::lock_guard<std::mutex> lock(mutex);
	return fingerprint;
}

bool Cipher::isEnvelope(const std::string & data) {
	return data.size() >= OVERHEAD && static_cast<unsigned char>(data[0]) == MAGIC[0]
		&& static_cast<unsigned char>(data[1]) == MAGIC[1] && static_cast<unsigned char>(data[2]) == VERSION;
}

std::string Cipher::saltOf(const std::string & envelope) {
	if (!isEnvelope(envelope)) {
		return {};
	}
	return envelope.substr(3, SALT_SIZE);
}

std::uint64_t Cipher::counterOf(const std::string & envelope) {
	if (envelope.size() < HEADER_SIZE) {
		return 0;
	}
	std::uint64_t value = 0;
	for (std::size_t i = 0; i < COUNTER_SIZE; ++i) {
		value = (value << 8) | static_cast<unsigned char>(envelope[COUNTER_OFFSET + i]);
	}
	return value;
}

std::optional<std::string> Cipher::seal(const std::string & plaintext) {
	std::lock_guard<std::mutex> lock(mutex);
	if (passphrase.empty()) {
		return plaintext;
	}
	try {
		const std::string sealed = encryptLocked(plaintext);
		if (sealed.empty()) {
			return std::nullopt;
		}
		return sealed;
	} catch (const std::exception & error) {
		ofLogError("ofxOsc") << error.what();
		return std::nullopt;
	}
}

std::string Cipher::encryptLocked(const std::string & plaintext) {
	if (plaintext.size() > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
		ofLogError("ofxOsc") << "datagram too large to seal";
		return "";
	}
	const std::string nonce = randomBytes(NONCE_SIZE);
	const std::uint64_t counter = nextCounter == 0 ? 1 : nextCounter;
	std::string out;
	out.reserve(OVERHEAD + plaintext.size());
	out.append(reinterpret_cast<const char *>(MAGIC), 2);
	out.push_back(static_cast<char>(VERSION));
	out += session;
	out += nonce;
	appendCounter(out, counter);

	CipherContext ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);
	std::string ciphertext(plaintext.size() + EVP_MAX_BLOCK_LENGTH, '\0');
	int headerLength = 0;
	int cipherLength = 0;
	int finalLength = 0;
	if (!ctx
		|| EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_gcm(), nullptr, nullptr, nullptr) != 1
		|| EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_SET_IVLEN, static_cast<int>(NONCE_SIZE), nullptr) != 1
		|| EVP_EncryptInit_ex(ctx.get(), nullptr, nullptr, bytes(key), bytes(nonce)) != 1
		|| EVP_EncryptUpdate(ctx.get(), nullptr, &headerLength, bytes(out), static_cast<int>(out.size())) != 1
		|| (!plaintext.empty() && EVP_EncryptUpdate(ctx.get(), reinterpret_cast<unsigned char *>(ciphertext.data()), &cipherLength,
			bytes(plaintext), static_cast<int>(plaintext.size())) != 1)
		|| EVP_EncryptFinal_ex(ctx.get(), reinterpret_cast<unsigned char *>(ciphertext.data()) + cipherLength, &finalLength) != 1) {
		ofLogError("ofxOsc") << "encryption failed";
		return "";
	}
	ciphertext.resize(static_cast<std::size_t>(cipherLength + finalLength));
	std::string tag(TAG_SIZE, '\0');
	if (EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_GET_TAG, static_cast<int>(TAG_SIZE), tag.data()) != 1) {
		ofLogError("ofxOsc") << "encryption failed (tag)";
		return "";
	}
	out += ciphertext;
	out += tag;
	nextCounter = counter + 1;
	if (nextCounter == 0) {
		nextCounter = 1;
	}
	return out;
}

std::optional<std::string> Cipher::decrypt(const std::string & envelope) {
	std::lock_guard<std::mutex> lock(mutex);
	try {
		return decryptLocked(envelope);
	} catch (const std::exception & error) {
		ofLogError("ofxOsc") << error.what();
		return std::nullopt;
	}
}

std::optional<std::string> Cipher::decryptLocked(const std::string & envelope) {
	if (passphrase.empty() || !isEnvelope(envelope)) {
		return std::nullopt;
	}
	if (key.size() != 32) {
		return std::nullopt;
	}
	const std::string header = envelope.substr(0, HEADER_SIZE);
	const std::string nonce = envelope.substr(3 + SALT_SIZE, NONCE_SIZE);
	const std::string ciphertext = envelope.substr(HEADER_SIZE, envelope.size() - OVERHEAD);
	std::string tag = envelope.substr(envelope.size() - TAG_SIZE);

	CipherContext ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);
	std::string plaintext(ciphertext.size() + EVP_MAX_BLOCK_LENGTH, '\0');
	int headerLength = 0;
	int plainLength = 0;
	int finalLength = 0;
	if (!ctx
		|| EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_gcm(), nullptr, nullptr, nullptr) != 1
		|| EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_SET_IVLEN, static_cast<int>(NONCE_SIZE), nullptr) != 1
		|| EVP_DecryptInit_ex(ctx.get(), nullptr, nullptr, bytes(key), bytes(nonce)) != 1
		|| EVP_DecryptUpdate(ctx.get(), nullptr, &headerLength, bytes(header), static_cast<int>(header.size())) != 1
		|| (!ciphertext.empty() && EVP_DecryptUpdate(ctx.get(), reinterpret_cast<unsigned char *>(plaintext.data()), &plainLength,
			bytes(ciphertext), static_cast<int>(ciphertext.size())) != 1)
		|| EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_SET_TAG, static_cast<int>(TAG_SIZE), tag.data()) != 1
		|| EVP_DecryptFinal_ex(ctx.get(), reinterpret_cast<unsigned char *>(plaintext.data()) + plainLength, &finalLength) != 1) {
		return std::nullopt;
	}
	plaintext.resize(static_cast<std::size_t>(plainLength + finalLength));
	return plaintext;
}

} // namespace crypto
} // namespace ofxOsc

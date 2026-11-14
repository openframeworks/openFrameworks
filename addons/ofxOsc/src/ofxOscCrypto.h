#pragma once

// Optional datagram seal for ofxOsc, on openFrameworks' OpenSSL.
//
// The passphrase is stretched once, with PBKDF2-HMAC-SHA256, into an AES-256
// key. Each UDP payload is then sealed with AES-256-GCM under a fresh nonce.
// The header, including an 8-byte counter, is authenticated. A wrong
// passphrase or any change on the way makes decrypt() fail. Decrypt does not
// stretch the passphrase again, so a stranger cannot stall the listener by
// sending new salts.
//
// Envelope (bytes): 'O' 'S' version(1) | session(16) | nonce(12) | counter(8) | ciphertext | tag(16)
//
// The session id is random for each setPassphrase. The counter starts at 1
// and increases by one per sealed datagram. ofxOscReceiver remembers recent
// counters per sender and session, so an exact copy is dropped, and a new
// session is not treated as a replay of the previous one. This hides the OSC
// address and arguments. It does not hide who sent the datagram, its size,
// or its timing, and it has no forward secrecy.

#include <cstdint>
#include <mutex>
#include <optional>
#include <string>

namespace ofxOsc {
namespace crypto {

class Cipher {
public:
	static constexpr std::size_t SALT_SIZE = 16;
	static constexpr std::size_t NONCE_SIZE = 12;
	static constexpr std::size_t TAG_SIZE = 16;
	static constexpr std::size_t COUNTER_SIZE = 8;
	static constexpr std::size_t COUNTER_OFFSET = 3 + SALT_SIZE + NONCE_SIZE;
	static constexpr std::size_t HEADER_SIZE = COUNTER_OFFSET + COUNTER_SIZE;
	static constexpr std::size_t OVERHEAD = HEADER_SIZE + TAG_SIZE;

	Cipher() = default;
	Cipher(const Cipher & other);
	Cipher & operator=(const Cipher & other);

	/// Empty turns sealing off. iterations below 1 uses 200000.
	void setPassphrase(const std::string & passphrase, int iterations = 200000);
	bool isEnabled() const;
	/// 12 hex characters. The same passphrase and stretch match on every machine.
	std::string getFingerprint() const;

	/// The plaintext when sealing is off, the envelope when it is on. Nothing when sealing fails.
	std::optional<std::string> seal(const std::string & plaintext);
	std::optional<std::string> decrypt(const std::string & envelope);

	static bool isEnvelope(const std::string & data);
	static std::string saltOf(const std::string & envelope);
	static std::uint64_t counterOf(const std::string & envelope);

private:
	void copyFrom(const Cipher & other);
	std::string encryptLocked(const std::string & plaintext);
	std::optional<std::string> decryptLocked(const std::string & envelope);

	mutable std::mutex mutex;
	std::string passphrase;
	int iterations = 200000;
	std::string session;
	std::string key;
	std::string fingerprint;
	std::uint64_t nextCounter = 1;
};

} // namespace crypto
} // namespace ofxOsc

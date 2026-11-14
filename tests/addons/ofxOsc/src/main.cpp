#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"
#include "ofxOsc.h"
#include "ofxOscParameterSync.h"

#include <chrono>
#include <cstring>
#include <thread>

#if !defined(TARGET_WIN32)
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

// Localhost checks. Nothing leaves the machine.

namespace {

const int kRounds = 1000;
const std::string kPass = "osc-room";

bool take(ofxOscReceiver & receiver, ofxOscMessage & message, int timeoutMs) {
	const auto start = ofGetElapsedTimeMillis();
	while (ofGetElapsedTimeMillis() - start < static_cast<uint64_t>(timeoutMs)) {
		if (receiver.getNextMessage(message)) {
			return true;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
	return false;
}

bool quiet(ofxOscReceiver & receiver, int timeoutMs) {
	ofxOscMessage message;
	return !take(receiver, message, timeoutMs);
}

#if !defined(TARGET_WIN32)
int bindLoopback(int port) {
	const int fd = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		return -1;
	}
	sockaddr_in address;
	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	address.sin_port = htons(static_cast<uint16_t>(port));
	const timeval wait { 1, 0 };
	::setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &wait, sizeof(wait));
	if (::bind(fd, reinterpret_cast<sockaddr *>(&address), sizeof(address)) != 0) {
		::close(fd);
		return -1;
	}
	return fd;
}

bool sendFrom(int fd, int port, const std::string & bytes) {
	sockaddr_in address;
	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	address.sin_port = htons(static_cast<uint16_t>(port));
	const ssize_t sent = ::sendto(fd, bytes.data(), bytes.size(), 0, reinterpret_cast<sockaddr *>(&address), sizeof(address));
	return sent == static_cast<ssize_t>(bytes.size());
}
#endif

} // namespace

class ofApp: public ofxUnitTestsApp {
	void test_messages_constructors(){
		const char * chars = "/const_char";
		ofxOscMessage m_chars {chars};
		ofxTest(m_chars.getAddress() ==  "/const_char","ofxOscMessage{const char *}");

		const std::string cstr { "/const_string" };
		ofxOscMessage m_cstr{cstr};
		ofxTest(m_cstr.getAddress() ==  "/const_string","ofxOscMessage{const std::string}");

		std::string str { "/string" };
		ofxOscMessage m_str {str};
		ofxTest(m_str.getAddress() ==  "/string","ofxOscMessage{std::string");

		std::string & strref = str;
		ofxOscMessage m_strref {strref};
		ofxTest(m_strref.getAddress() ==  "/string","ofxOscMessage{std::string &}");

		const std::string & cstrref = cstr;
		ofxOscMessage m_cstrref {cstrref};
		ofxTest(m_cstrref.getAddress() ==  "/const_string","ofxOscMessage{const std::string &}");
		
		std::string mv { "/string_for_move" };
		ofxOscMessage m_mv {std::move(mv)};
		ofxTest(m_mv.getAddress() ==  "/string_for_move","ofxOscMessage{std::string &&}");
		
		std::unique_ptr<ofxOscReceiver> r = std::make_unique<ofxOscReceiver>();
		r = std::make_unique<ofxOscReceiver>();
		r = std::make_unique<ofxOscReceiver>();
		r = std::make_unique<ofxOscReceiver>();
		r = std::make_unique<ofxOscReceiver>();
		r->setup(2222);

		ofxTest(r.get()->getPort() == 2222, "ofxOscReceiver: port test");
		
	}

	void testFingerprint() {
		ofxOscSender sender;
		ofxOscReceiver receiver;
		sender.setPassphrase(kPass, kRounds);
		receiver.setPassphrase(kPass, kRounds);
		ofxTest(sender.isEncrypting(), "sender seals when a passphrase is set");
		ofxTestEq(sender.getFingerprint(), receiver.getFingerprint(), "fingerprint matches");
		sender.setPassphrase("");
		ofxTest(!sender.isEncrypting(), "empty passphrase turns sealing off");
	}

	void testPlaintext() {
		ofxOscSender sender;
		ofxOscReceiver receiver;
		ofxTest(receiver.setup(60141), "plaintext receiver listens");
		ofxTest(sender.setup("127.0.0.1", 60141), "plaintext sender");
		ofxOscMessage message;
		message.setAddress("/braid");
		message.addStringArg("open");
		ofxTest(sender.sendMessage(message), "plaintext send");
		ofxOscMessage got;
		ofxTest(take(receiver, got, 2000), "plaintext arrives");
		ofxTestEq(got.getAddress(), std::string("/braid"), "plaintext address");
		if (got.getNumArgs() == 1) {
			ofxTestEq(got.getArgAsString(0), std::string("open"), "plaintext body");
		}
		receiver.stop();
		sender.clear();
	}

	void testSealed() {
		ofxOscSender sender;
		ofxOscReceiver receiver;
		ofxTest(receiver.setup(60142), "sealed receiver listens");
		ofxTest(sender.setup("127.0.0.1", 60142), "sealed sender");
		sender.setPassphrase(kPass, kRounds);
		receiver.setPassphrase(kPass, kRounds);
		receiver.requireEncryption = true;
		ofxOscMessage message;
		message.setAddress("/braid");
		message.addStringArg("sealed");
		ofxTest(sender.sendMessage(message), "sealed send");
		ofxOscMessage got;
		ofxTest(take(receiver, got, 2000), "matching passphrase arrives");
		if (got.getNumArgs() == 1) {
			ofxTestEq(got.getArgAsString(0), std::string("sealed"), "sealed body");
		}

		message.clear();
		message.setAddress("/braid");
		message.addStringArg("again");
		ofxTest(sender.sendMessage(message), "second sealed send");
		ofxTest(take(receiver, got, 2000), "a new counter still arrives");

		sender.setPassphrase(kPass, kRounds);
		message.clear();
		message.setAddress("/braid");
		message.addStringArg("fresh");
		ofxTest(sender.sendMessage(message), "a new session is sent");
		ofxTest(take(receiver, got, 2000), "a restarted counter is not a replay");
		if (got.getNumArgs() == 1) {
			ofxTestEq(got.getArgAsString(0), std::string("fresh"), "new session body");
		}
		receiver.stop();
		sender.clear();
	}

	void testRejected() {
		ofxOscSender sender;
		ofxOscReceiver receiver;
		receiver.requireEncryption = true;
		receiver.setPassphrase(kPass, kRounds);
		ofxTest(receiver.setup(60143), "strict receiver listens");
		ofxTest(sender.setup("127.0.0.1", 60143), "plain sender to a strict receiver");
		ofxOscMessage message;
		message.setAddress("/braid");
		message.addStringArg("plain");
		ofxTest(sender.sendMessage(message), "plaintext is sent");
		ofxTest(quiet(receiver, 300), "plaintext is dropped when encryption is required");

		sender.setPassphrase("other-room", kRounds);
		message.clear();
		message.setAddress("/braid");
		message.addStringArg("wrong");
		ofxTest(sender.sendMessage(message), "wrong passphrase is sent");
		ofxTest(quiet(receiver, 300), "wrong passphrase is dropped");

		sender.setPassphrase(kPass, kRounds * 2);
		message.clear();
		message.setAddress("/braid");
		message.addStringArg("stretch");
		ofxTest(sender.sendMessage(message), "different stretch is sent");
		ofxTest(quiet(receiver, 300), "a different stretch does not open the datagram");

		sender.setPassphrase(kPass, kRounds);
#if !defined(TARGET_WIN32)
		const int noise = ::socket(AF_INET, SOCK_DGRAM, 0);
		ofxTest(noise >= 0, "noise socket opens");
		std::string junk(80, 'x');
		junk[0] = 'O';
		junk[1] = 'S';
		junk[2] = 1;
		for (int i = 0; i < 64; ++i) {
			junk[20] = static_cast<char>(i);
			sendFrom(noise, 60143, junk);
		}
		if (noise >= 0) {
			::close(noise);
		}
#endif
		message.clear();
		message.setAddress("/braid");
		message.addStringArg("after");
		ofxTest(sender.sendMessage(message), "a real datagram follows the noise");
		ofxOscMessage got;
		ofxTest(take(receiver, got, 2000), "noise does not block the next sealed datagram");
		if (got.getNumArgs() == 1) {
			ofxTestEq(got.getArgAsString(0), std::string("after"), "datagram after noise");
		}
		receiver.stop();
		sender.clear();
	}

	void testBundle() {
		ofxOscSender sender;
		ofxOscReceiver receiver;
		ofxTest(receiver.setup(60144), "bundle receiver listens");
		ofxTest(sender.setup("127.0.0.1", 60144), "bundle sender");
		sender.setPassphrase(kPass, kRounds);
		receiver.setPassphrase(kPass, kRounds);
		ofxOscBundle bundle;
		ofxOscMessage first;
		first.setAddress("/braid/1");
		first.addIntArg(1);
		ofxOscMessage second;
		second.setAddress("/braid/2");
		second.addIntArg(2);
		bundle.addMessage(first);
		bundle.addMessage(second);
		ofxTest(sender.sendBundle(bundle), "sealed bundle send");
		ofxOscMessage got;
		ofxTest(take(receiver, got, 2000), "bundle first message");
		ofxTestEq(got.getAddress(), std::string("/braid/1"), "bundle keeps the first address");
		ofxTest(take(receiver, got, 2000), "bundle second message");
		ofxTestEq(got.getAddress(), std::string("/braid/2"), "bundle keeps the second address");
		receiver.stop();
		sender.clear();
	}

	void testReplay() {
#if !defined(TARGET_WIN32)
		const int capture = bindLoopback(60145);
		ofxTest(capture >= 0, "capture socket binds");
		if (capture < 0) {
			return;
		}
		ofxOscSender sender;
		sender.setPassphrase(kPass, kRounds);
		ofxTest(sender.setup("127.0.0.1", 60145), "sender aims at the capture socket");
		ofxOscMessage message;
		message.setAddress("/braid");
		message.addStringArg("move");
		ofxTest(sender.sendMessage(message), "sealed datagram leaves the sender");
		char buffer[2048];
		sockaddr_in from;
		socklen_t fromLength = sizeof(from);
		const ssize_t gotBytes = ::recvfrom(capture, buffer, sizeof(buffer), 0, reinterpret_cast<sockaddr *>(&from), &fromLength);
		::close(capture);
		ofxTest(gotBytes > 16, "capture reads a datagram");
		if (gotBytes <= 16) {
			return;
		}
		const std::string wire(buffer, static_cast<std::size_t>(gotBytes));
		ofxTest(wire.size() >= 2 && wire[0] == 'O' && wire[1] == 'S', "wire starts with the seal");
		ofxTest(wire.find("/braid") == std::string::npos, "the address is not in the clear");

		ofxOscReceiver receiver;
		receiver.setPassphrase(kPass, kRounds);
		receiver.requireEncryption = true;
		ofxTest(receiver.setup(60146), "replay receiver listens");
		const int forward = ::socket(AF_INET, SOCK_DGRAM, 0);
		ofxTest(forward >= 0, "forward socket opens");
		ofxTest(sendFrom(forward, 60146, wire), "first copy is forwarded");
		ofxOscMessage got;
		ofxTest(take(receiver, got, 2000), "first copy opens");
		if (got.getNumArgs() == 1) {
			ofxTestEq(got.getArgAsString(0), std::string("move"), "opened body");
		}
		ofxTest(sendFrom(forward, 60146, wire), "same datagram is sent again");
		ofxTest(quiet(receiver, 300), "the copy is dropped");

		std::string tampered = wire;
		tampered.back() = static_cast<char>(tampered.back() ^ 0x01);
		ofxTest(sendFrom(forward, 60146, tampered), "tampered datagram is sent");
		ofxTest(quiet(receiver, 300), "a changed datagram is dropped");
		if (forward >= 0) {
			::close(forward);
		}
		receiver.stop();
		sender.clear();
#else
		ofxTest(true, "replay capture is checked on posix");
#endif
	}

	void testParameterSync() {
		ofParameter<int> leftValue;
		ofParameter<int> rightValue;
		leftValue.set("count", 1);
		rightValue.set("count", 1);
		ofParameterGroup leftGroup;
		ofParameterGroup rightGroup;
		leftGroup.setName("room");
		rightGroup.setName("room");
		leftGroup.add(leftValue);
		rightGroup.add(rightValue);

		ofxOscParameterSync left;
		ofxOscParameterSync right;
		left.setup(leftGroup, 60147, "127.0.0.1", 60148);
		right.setup(rightGroup, 60148, "127.0.0.1", 60147);
		left.setPassphrase(kPass, kRounds);
		right.setPassphrase(kPass, kRounds);
		left.setRequireEncryption(true);
		right.setRequireEncryption(true);
		leftValue = 9;

		const auto start = ofGetElapsedTimeMillis();
		while (rightValue.get() != 9 && ofGetElapsedTimeMillis() - start < 2000) {
			left.update();
			right.update();
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
		ofxTestEq(rightValue.get(), 9, "parameter sync carries the sealed value");
	}

	void run(){
		test_messages_constructors();
		testFingerprint();
		testPlaintext();
		testSealed();
		testRejected();
		testBundle();
		testReplay();
		testParameterSync();
	}
};

int main( ){
    ofInit();
    auto window = std::make_shared<ofAppNoWindow>();
    auto app = std::make_shared<ofApp>();
    ofRunApp(window, app);
    return ofRunMainLoop();
}

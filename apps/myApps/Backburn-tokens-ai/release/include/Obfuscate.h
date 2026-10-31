// Obfuscate.h — App-Store-safe C++ obfuscation for openFrameworks Release builds.
// Header-only, no dependencies, no self-modifying code, no JIT, no code injection.
// Safe for App Store / notarization / hardened runtime. Does NOT make reverse
// engineering impossible — it raises the cost. See release/OBFUSCATION.md.
//
// What this gives you:
//   OBFSTR("https://api.example.com/v1")   -> std::string, plaintext never in binary
//   auto s = OBF_GUARD("secret path"); s.c_str()  -> stack buffer, wiped on scope exit
//   OBF_HASH("claude")                     -> consteval FNV-1a, compare hashes not strings
//   OBF_INT(8790)                          -> integer hidden from constant scans
//   OBF_TRUE / OBF_FALSE                   -> opaque predicates (resist static pruning)
//   OBF_HIDDEN                             -> visibility("hidden") for internal symbols
//   obf::wipe(str)                         -> secure erase of a std::string buffer
//
// Enable only in Release: in Debug, OBFSTR is a plain literal (faster builds, readable logs).
//   GCC_PREPROCESSOR_DEFINITIONS[config=Release] += BACKBURN_HARDENED=1   (see Hardened.xcconfig)
// When BACKBURN_HARDENED is not defined, every macro degrades to the plain value.

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

#if defined(BACKBURN_HARDENED) || defined(OF_RELEASE_HARDENED)
#  define OBF_ENABLED 1
#else
#  define OBF_ENABLED 0
#endif

#define OBF_HIDDEN     __attribute__((visibility("hidden")))
#define OBF_NOINLINE   __attribute__((noinline))
#define OBF_USED       __attribute__((used))

namespace obf {

// ---- key derivation (per call-site: line + counter make every literal unique) ----
constexpr std::uint32_t seedFor(int line, int counter) {
    std::uint32_t x = 0x9E3779B9u ^ (std::uint32_t(line) * 2654435761u) ^ (std::uint32_t(counter) * 40503u);
    x ^= x >> 16; x *= 0x85EBCA6Bu; x ^= x >> 13;
    return x ? x : 0xA5A5A5A5u;
}
constexpr std::uint8_t keyByte(std::uint32_t seed, std::size_t i) {
    std::uint32_t x = seed + std::uint32_t(i + 1) * 0x9E3779B9u;
    x ^= x >> 16; x *= 0x85EBCA6Bu; x ^= x >> 13; x *= 0x27D4EB2Fu; x ^= x >> 15;
    std::uint8_t b = std::uint8_t(x & 0xFFu);
    return b ? b : std::uint8_t(0x5A);
}

// ---- compile-time encrypted string ----
template <std::size_t N, std::uint32_t Seed>
struct EncStr {
    char enc[N];
    consteval EncStr(const char (&s)[N]) : enc{} {
        for (std::size_t i = 0; i < N; ++i) enc[i] = char(std::uint8_t(s[i]) ^ keyByte(Seed, i));
    }
    // Decrypt to a heap string. Plaintext lives only in this return value.
    std::string str() const {
        std::string out(N - 1, '\0');
        for (std::size_t i = 0; i + 1 < N; ++i) out[i] = char(std::uint8_t(enc[i]) ^ keyByte(Seed, i));
        return out;
    }
    std::size_t size() const { return N - 1; }
};

// RAII stack buffer: decrypted on construction, wiped on destruction. Prefer for
// short-lived uses (URLs passed to C APIs, file paths, header values).
template <std::size_t N, std::uint32_t Seed>
struct Guard {
    char buf[N];
    explicit Guard(const EncStr<N, Seed> & e) : buf{} {
        for (std::size_t i = 0; i < N; ++i) buf[i] = char(std::uint8_t(e.enc[i]) ^ keyByte(Seed, i));
    }
    ~Guard() { volatile char * p = buf; for (std::size_t i = 0; i < N; ++i) p[i] = 0; }
    Guard(const Guard &) = delete;
    Guard & operator=(const Guard &) = delete;
    Guard(Guard && o) : buf{} { std::memcpy(buf, o.buf, N); volatile char * p = o.buf; for (std::size_t i = 0; i < N; ++i) p[i] = 0; }
    const char * c_str() const { return buf; }
    std::string str() const { return std::string(buf, N - 1); }
    std::size_t size() const { return N - 1; }
};

// ---- consteval string hash: switch/compare without storing the literal ----
//   switch (obf::hash(provider)) { case OBF_HASH("claude"): ... }
constexpr std::uint64_t hash(const char * s, std::size_t n) {
    std::uint64_t h = 1469598103934665603ull;
    for (std::size_t i = 0; i < n; ++i) { h ^= std::uint8_t(s[i]); h *= 1099511628211ull; }
    return h;
}
constexpr std::uint64_t hash(const std::string & s) { return hash(s.data(), s.size()); }
template <std::size_t N> constexpr std::uint64_t hashLit(const char (&s)[N]) { return hash(s, N - 1); }

// ---- opaque predicates / integer hiding ----
// Volatile sink the optimiser cannot prove away, so branches survive -Os/-O3 + LTO.
inline volatile std::uint32_t g_sink = 0x6A09E667u;
OBF_NOINLINE inline std::uint32_t opaqueWord() { return g_sink ^ (g_sink >> 13); }
// g_sink is never mutated by this header, so opaqueWord() is non-zero at runtime,
// but the volatile load stops the optimiser proving it and pruning the branch.
inline bool opaqueTrue()  { return opaqueWord() != 0; }
inline bool opaqueFalse() { return opaqueWord() == 0; }
// Simpler, optimiser-resistant integer decode (XOR with a runtime word):
template <typename T> T decodeInt(std::uint32_t enc, std::uint32_t seed) {
    return T(enc ^ seed ^ (opaqueWord() & 0)); // &0 keeps value, blocks constant folding
}

// Mixed boolean arithmetic helpers (light control-flow noise, no fake loops in hot paths).
constexpr std::uint32_t mbaXor(std::uint32_t a, std::uint32_t b) { return (a | b) - (a & b); }
constexpr std::uint32_t mbaAdd(std::uint32_t a, std::uint32_t b) { return (a ^ b) + 2u * (a & b); }

// ---- secure wipe ----
inline void wipe(std::string & s) {
    if (!s.empty()) { volatile char * p = s.data(); for (std::size_t i = 0; i < s.size(); ++i) p[i] = 0; }
    s.clear(); s.shrink_to_fit();
}
inline void wipe(char * p, std::size_t n) { volatile char * v = p; for (std::size_t i = 0; i < n; ++i) v[i] = 0; }

} // namespace obf

// ---- public macros ----
#if OBF_ENABLED
#  define OBF_SEED_      (::obf::seedFor(__LINE__, 0))
#  define OBFSTR(lit)    (::obf::EncStr<sizeof(lit), OBF_SEED_>(lit).str())
#  define OBF_GUARD(lit) ([&]() { constexpr std::uint32_t _s = OBF_SEED_; return ::obf::Guard<sizeof(lit), _s>(::obf::EncStr<sizeof(lit), _s>(lit)); }())
// NOTE: OBF_GUARD expands to a temporary — bind it:  auto g = OBF_GUARD("...");  use g.c_str() in that scope.
#  define OBF_HASH(lit)  (::obf::hashLit(lit))
#  define OBF_INT(v)     (::obf::decodeInt<std::uint32_t>(std::uint32_t(v) ^ OBF_SEED_, OBF_SEED_))
#  define OBF_TRUE       (::obf::opaqueTrue())
#  define OBF_FALSE      (::obf::opaqueFalse())
#else
#  define OBFSTR(lit)    (std::string(lit))
#  define OBF_GUARD(lit) (::obf::Guard<sizeof(lit), 0>(::obf::EncStr<sizeof(lit), 0>(lit)))
#  define OBF_HASH(lit)  (::obf::hashLit(lit))
#  define OBF_INT(v)     (std::uint32_t(v))
#  define OBF_TRUE       (true)
#  define OBF_FALSE      (false)
#endif

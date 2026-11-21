#include <xnuva_randomx/engine.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

using xnuva::randomx_foundation::Digest;
using xnuva::randomx_foundation::LightHasher;
using xnuva::randomx_foundation::HashOnce;

namespace {
std::span<const std::byte> Bytes(std::string_view str)
{
    return std::as_bytes(std::span{str.data(), str.size()});
}

std::vector<std::byte> DecodeHex(std::string_view str)
{
    if (str.size() % 2 != 0) throw std::invalid_argument("Odd-length hex");
    auto nibble = [](char ch) -> unsigned {
        if (ch >= '0' && ch <= '9') return static_cast<unsigned>(ch - '0');
        if (ch >= 'a' && ch <= 'f') return static_cast<unsigned>(ch - 'a' + 10);
        if (ch >= 'A' && ch <= 'F') return static_cast<unsigned>(ch - 'A' + 10);
        throw std::invalid_argument("Invalid hex digit");
    };
    std::vector<std::byte> result;
    result.reserve(str.size() / 2);
    for (size_t i = 0; i < str.size(); i += 2) {
        result.push_back(std::byte((nibble(str[i]) << 4) | nibble(str[i + 1])));
    }
    return result;
}

std::string Hex(Digest bytes)
{
    constexpr char alphabet[] = "0123456789abcdef";
    std::string result;
    result.reserve(64);
    for (auto b : bytes) {
        result.push_back(alphabet[b >> 4]);
        result.push_back(alphabet[b & 0xf]);
    }
    return result;
}

void Expect(bool ok, std::string_view why)
{
    if (!ok) throw std::runtime_error(std::string(why));
}
}

int main()
{
    try {
        // Existing XNUVA V2 reference, RAW RandomX digest order.
        constexpr std::string_view key{"test key 000"};
        constexpr std::string_view input{"This is a test"};
        constexpr std::string_view raw_vector{
            "22ec6b861b3eb23686b2efbad69513c9"
            "67ecfce80983df66c9c5b4fbfb4cdb6f"
        };
        LightHasher reusable{Bytes(key)};
        const Digest first = reusable.Hash(Bytes(input));
        Expect(Hex(first) == raw_vector, "RandomX V2 reference vector mismatch");
        Expect(reusable.Hash(Bytes(input)) == first,
               "Repeated hashes must be deterministic");
        Expect(HashOnce(Bytes(key), Bytes(input)) == first,
               "Independent light context returned a different digest");

        // Existing XNUVA serialized 80-byte header and its seed block ID
        // in INTERNAL byte order. This fixture tests byte-order boundaries.
        const auto header = DecodeHex(
            "010000000000000000000000000000000000000000000000"
            "000000000000000000000000e876b6cca70e8a073c70712c"
            "baa97fa2b39b152d8c7d46b35d4f9ebe163392fbd0829c6"
            "affff7f2000000000");
        const auto seed = DecodeHex(
            "ae1a49be55e5eefabdf6f08001bbdd97"
            "b29820d1acea221149ae3237b1b3952d");
        Expect(header.size() == 80, "Fixture header size is not 80");
        Expect(seed.size() == 32, "Fixture seed size is not 32");
        LightHasher header_hasher{seed};
        const Digest h = header_hasher.Hash80ByteHeader(header);
        Expect(Hex(h) ==
               "857071f9507a5912e68825811b97134d"
               "60144c2011a13564f4787141fe9ab580",
               "80-byte header RandomX V2 reference mismatch");
        Expect(h == header_hasher.Hash(header),
               "Header and generic hash APIs disagree");

        bool wrong_size_rejected = false;
        try {
            (void)header_hasher.Hash80ByteHeader(
                std::span<const std::byte>{header}.first(79));
        } catch (const std::invalid_argument&) {
            wrong_size_rejected = true;
        }
        Expect(wrong_size_rejected, "79-byte header was not rejected");

        bool empty_seed_rejected = false;
        try {
            LightHasher bad{std::span<const std::byte>{}};
        } catch (const std::invalid_argument&) {
            empty_seed_rejected = true;
        }
        Expect(empty_seed_rejected, "Empty seed was not rejected");

        bool empty_input_rejected = false;
        try {
            (void)header_hasher.Hash(std::span<const std::byte>{});
        } catch (const std::invalid_argument&) {
            empty_input_rejected = true;
        }
        Expect(empty_input_rejected, "Empty input was not rejected");

        std::cout << "PASS: RandomX V2 string/header vectors, "
                     "cache determinism and invalid-input checks\n";
        std::cout << "NOTE: Hashing only; no coin consensus, mining "
                     "or network acceptance qualified.\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "FAIL: " << e.what() << '\n';
        return 1;
    }
}

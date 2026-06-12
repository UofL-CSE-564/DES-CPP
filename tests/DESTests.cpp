#include "des/DES.hpp"

#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

std::string blockToBytes(std::uint64_t block) {
    std::string bytes;
    bytes.reserve(8);
    for (int shift = 56; shift >= 0; shift -= 8) {
        bytes.push_back(static_cast<char>((block >> shift) & 0xffU));
    }
    return bytes;
}

} // namespace

TEST(DESKeyTest, ValidatesOddParityForEveryByte) {
    EXPECT_TRUE(DES::hasValidParity(0x133457799BBCDFF1ULL));
    EXPECT_TRUE(DES::hasValidParity(0x7CA110454A1A6E57ULL));

    EXPECT_FALSE(DES::hasValidParity(0x133457799BBCDFF0ULL));
    EXPECT_FALSE(DES::hasValidParity(0x0000000000000000ULL));
}

TEST(DESKeyTest, ConstructorsAndSetKeyEnforceParity) {
    DES default_constructed;
    EXPECT_FALSE(default_constructed.hasKey());
    EXPECT_THROW((void)default_constructed.encrypt(0x0123456789ABCDEFULL), std::logic_error);

    EXPECT_NO_THROW(default_constructed.setKey(0x133457799BBCDFF1ULL));
    EXPECT_TRUE(default_constructed.hasKey());
    EXPECT_EQ(default_constructed.key(), 0x133457799BBCDFF1ULL);

    EXPECT_NO_THROW(DES(0x133457799BBCDFF1ULL));
    EXPECT_THROW(DES(0x133457799BBCDFF0ULL), std::invalid_argument);
    EXPECT_THROW(default_constructed.setKey(0x0000000000000000ULL), std::invalid_argument);
}

TEST(DESBlockTest, EncryptsAndDecryptsKnownStandardVector) {
    DES des(0x133457799BBCDFF1ULL);

    const std::uint64_t plaintext = 0x0123456789ABCDEFULL;
    const std::uint64_t ciphertext = 0x85E813540F0AB405ULL;

    EXPECT_EQ(des.encrypt(plaintext), ciphertext);
    EXPECT_EQ(des.decrypt(ciphertext), plaintext);
}

TEST(DESBlockTest, EncryptsAndDecryptsProvidedExampleVector) {
    DES des(0x7CA110454A1A6E57ULL);

    const std::uint64_t plaintext = 0x01A1D6D039776742ULL;
    const std::uint64_t ciphertext = des.encrypt(plaintext);

    EXPECT_EQ(des.decrypt(ciphertext), plaintext);
}

TEST(DESStreamTest, EncryptsInputStreamInBigEndianEightByteChunks) {
    DES des(0x133457799BBCDFF1ULL);
    std::istringstream input(blockToBytes(0x0123456789ABCDEFULL));

    const auto encrypted = des.encrypt(input);

    ASSERT_EQ(encrypted.size(), 1U);
    EXPECT_EQ(encrypted[0], 0x85E813540F0AB405ULL);
}

TEST(DESStreamTest, PadsFinalPartialChunkWithZeroBytes) {
    DES des(0x133457799BBCDFF1ULL);
    std::string bytes;
    bytes.push_back(static_cast<char>(0x01));
    bytes.push_back(static_cast<char>(0x23));
    std::istringstream input(bytes);

    const auto encrypted = des.encrypt(input);

    ASSERT_EQ(encrypted.size(), 1U);
    EXPECT_EQ(des.decrypt(encrypted[0]), 0x0123000000000000ULL);
}

TEST(DESStreamTest, RoundTripsMultipleStreamBlocks) {
    DES des(0x133457799BBCDFF1ULL);
    const std::vector<std::uint64_t> plaintext_blocks{
        0x0123456789ABCDEFULL,
        0x1111111111111111ULL,
        0xCAFE000000000000ULL
    };

    std::string plaintext_bytes;
    plaintext_bytes += blockToBytes(plaintext_blocks[0]);
    plaintext_bytes += blockToBytes(plaintext_blocks[1]);
    plaintext_bytes.push_back(static_cast<char>(0xCA));
    plaintext_bytes.push_back(static_cast<char>(0xFE));

    std::istringstream plaintext_stream(plaintext_bytes);
    const auto encrypted_blocks = des.encrypt(plaintext_stream);

    std::string ciphertext_bytes;
    for (const auto block : encrypted_blocks) {
        ciphertext_bytes += blockToBytes(block);
    }

    std::istringstream ciphertext_stream(ciphertext_bytes);
    const auto decrypted_blocks = des.decrypt(ciphertext_stream);

    EXPECT_EQ(decrypted_blocks, plaintext_blocks);
}



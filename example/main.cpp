#include "des/DES.hpp"

#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>

int main() {
    //the values below are commonly used as DES validation tests (Found in FIPS PUB)
    const std::uint64_t key = 0x133457799BBCDFF1ULL;
    const std::uint64_t plaintext = 0x0123456789ABCDEFULL;

    DES des(key);
    const std::uint64_t ciphertext = des.encrypt(plaintext);
    const std::uint64_t decrypted = des.decrypt(ciphertext);

    std::cout << std::hex << std::setfill('0') << std::uppercase;
    std::cout << "Plaintext : 0x" << std::setw(16) << plaintext << '\n';
    std::cout << "Ciphertext: 0x" << std::setw(16) << ciphertext << '\n';
    std::cout << "Decrypted : 0x" << std::setw(16) << decrypted << '\n';

    if (ciphertext == 0x85E813540F0AB405ULL) {
        std::cout << "Ciphertext matches known DES expect value (FIPS test)\n";
    } else {
        std::cout << "Ciphertext does NOT match expected value\n";
    }

    std::ifstream input("example/sample.txt", std::ios::binary);
    if (!input) {
        std::cerr << "sample.txt not found\n";
        return 1;
    }

    input.seekg(0, std::ios::end);
    std::streamsize original_size = input.tellg();
    input.seekg(0, std::ios::beg);

    auto encrypted_blocks = des.encrypt(input);
    std::ofstream encrypted_file("example/sample.txt.des", std::ios::binary);
    for (std::uint64_t block : encrypted_blocks) {
        for (int shift = 56; shift >= 0; shift -= 8) {
            char byte = static_cast<char>((block >> shift) & 0xffU);
            encrypted_file.write(&byte, 1);
        }
    }
    encrypted_file.close();

    std::ifstream encrypted_input("example/sample.txt.des", std::ios::binary);
    auto decrypted_blocks = des.decrypt(encrypted_input);
    std::ofstream decrypted_file("example/sample.roundtrip.txt", std::ios::binary);
    for (std::uint64_t block : decrypted_blocks) {
        for (int shift = 56; shift >= 0 && original_size > 0; shift -= 8, --original_size) {
            char byte = static_cast<char>((block >> shift) & 0xffU);
            decrypted_file.write(&byte, 1);
        }
    }

    std::cout << std::dec;


    return 0;
}

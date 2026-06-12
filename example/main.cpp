#include "des/DES.hpp"

#include <cstdint>
#include <iomanip>
#include <iostream>

int main() {
    const std::uint64_t key = 0x133457799BBCDFF1ULL;
    const std::uint64_t plaintext = 0x0123456789ABCDEFULL;

    DES des(key);
    const std::uint64_t ciphertext = des.encrypt(plaintext);
    const std::uint64_t decrypted = des.decrypt(ciphertext);

    std::cout << std::hex << std::setfill('0') << std::uppercase;
    std::cout << "Plaintext : 0x" << std::setw(16) << plaintext << '\n';
    std::cout << "Ciphertext: 0x" << std::setw(16) << ciphertext << '\n';
    std::cout << "Decrypted : 0x" << std::setw(16) << decrypted << '\n';

    return decrypted == plaintext ? 0 : 1;
}


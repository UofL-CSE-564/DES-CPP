#pragma once

#include <cstdint>
#include <istream>
#include <vector>

class DES {
public:
    DES();
    explicit DES(std::uint64_t key);

    void setKey(std::uint64_t key);
    void set_key(std::uint64_t key) { setKey(key); }

    [[nodiscard]] static bool hasValidParity(std::uint64_t key) noexcept;

    [[nodiscard]] bool hasKey() const noexcept;
    [[nodiscard]] std::uint64_t key() const;

    [[nodiscard]] std::uint64_t encrypt(std::uint64_t block) const;
    [[nodiscard]] std::uint64_t decrypt(std::uint64_t block) const;

    [[nodiscard]] std::vector<std::uint64_t> encrypt(std::istream& input) const;
    [[nodiscard]] std::vector<std::uint64_t> decrypt(std::istream& input) const;

private:
    std::uint64_t key_;
    bool has_key_;

    void requireKey() const;
    [[nodiscard]] std::vector<std::uint64_t> processStream(std::istream& input, char mode) const;
};


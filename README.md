# DES-CPP

A C++17 implementation of the DES encryption standard that can be imported by other CMake projects.

## Layout

- `include/des/DES.hpp` - public `DES` class API
- `src/` - DES implementation
- `tests/` - GoogleTest unit tests
- `example/` - small executable showing block encryption/decryption

## Build and test

```zsh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

## Run the example

```zsh
./build/example/des_cpp_example
```

## Basic usage

```cpp
#include "des/DES.hpp"

#include <cstdint>

int main() {
	DES des(0x133457799BBCDFF1ULL);
	std::uint64_t ciphertext = des.encrypt(0x0123456789ABCDEFULL);
	std::uint64_t plaintext = des.decrypt(ciphertext);
}
```

`DES::setKey` and the key constructor validate DES odd parity: each of the eight key bytes must contain an odd number of `1` bits. The default constructor creates an object without a key; call `setKey` before encryption or decryption.

Stream overloads read input eight bytes at a time using big-endian block construction. A final partial block is padded with zero bytes and returned as one final encrypted/decrypted `std::uint64_t` value.

## Use from another CMake project with FetchContent

```cmake
include(FetchContent)

FetchContent_Declare(
	DES_CPP
	GIT_REPOSITORY https://github.com/YOUR_ORG/DES-CPP.git
	GIT_TAG main
)
FetchContent_MakeAvailable(DES_CPP)

target_link_libraries(your_target PRIVATE DES::DES)
```

## Use after install

```zsh
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=/path/to/install
cmake --build build
cmake --install build
```

Then, from another CMake project:

```cmake
find_package(DES-CPP CONFIG REQUIRED)
target_link_libraries(your_target PRIVATE DES::DES)
```

hash_sha256
==================

[![Build Status](https://github.com/imahjoub/hash_sha256/actions/workflows/hash_sha256.yml/badge.svg)](https://github.com/imahjoub/hash_sha256/actions)
[![Boost Software License 1.0](https://img.shields.io/badge/license-BSL%201.0-blue.svg)](https://github.com/imahjoub/hash_sha256/blob/main/LICENSE_1_0.txt)
[![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/imahjoub/hash_sha256)](https://github.com/imahjoub/hash_sha256)
[![codecov](https://codecov.io/gh/imahjoub/hash_sha256/branch/main/graph/badge.svg?token=3LIK8E96FC)](https://codecov.io/gh/imahjoub/hash_sha256)


SHA-256 is a patented cryptographic hash function that outputs a value that is 256 bits long.


## Characteristics of the SHA-256 Algorithm

  * Message Length: The length of the cleartext should be less than 264 bits. The size needs to be in the comparison area to keep the digest as random as possible.

  * Digest Length: The length of the hash digest should be 256 bits in SHA 256 algorithm, 512 bits in SHA-512, and so on. Bigger digests usually suggest significantly more calculations at the cost of speed and space.

  * Irreversible: By design, all hash functions such as the SHA 256 are irreversible. You should neither get a plaintext when you have the digest beforehand nor should the digest provide its original value when you pass it through the hash function again.


## Using the `hash_sha256`

`hash_sha256` is a header-only c++ implementation of SHA-256


* The following program shows a test example
```cpp
// alias declaration
using hash_output_type = std::array<std::uint8_t, 32U>;

// create an object of hash_sha256
hash_sha256 my_sha256;

// original message
const std::array<std::uint8_t, 3U> my_msg = { 'a', 'b', 'c'};

// update the hash with given data
my_sha256.update(my_msg.data(), my_msg.size());

// final hash result
hash_output_type digest = my_sha256.digest();
```

## Testing, continuous integration and quality
Testing and continuous integration runs on GitHub Actions.
Various OS/compiler combinations are used including
GCC/clang/MSVC.

Code coverage uses GCC/gcov/lcov and has a
quality-gate with comparison/baseline-check provided by third-party [Codecov](https://app.codecov.io/gh/imahjoub/hash_sha256).

TBD Provide use linters and describe them.

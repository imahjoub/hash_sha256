hash_sha256
==================

<p align="center">
    <a href="https://github.com/imahjoub/hash_sha256/actions">
        <img src="https://github.com/imahjoub/hash_sha256/actions/workflows/hash_sha256.yml/badge.svg" alt="Build Status"></a>
    <a href="https://github.com/imahjoub/hash_sha256/issues?q=is%3Aissue+is%3Aopen+sort%3Aupdated-desc">
        <img src="https://custom-icon-badges.herokuapp.com/github/issues-raw/imahjoub/hash_sha256?logo=github" alt="Issues" /></a>
    <a href="https://github.com/imahjoub/hash_sha256/actions/workflows/hash_sha256_codeql.yml">
        <img src="https://github.com/imahjoub/hash_sha256/actions/workflows/hash_sha256_codeql.yml/badge.svg" alt="CodeQL"></a>
    <a href="https://app.codacy.com/gh/imahjoub/hash_sha256/dashboard">
        <img src="https://app.codacy.com/project/badge/Grade/913aec26f468405ab0ff9a29beb6d400" alt="Codacy Badge" /></a>
    <a href="https://sonarcloud.io/summary/new_code?id=imahjoub_hash_sha256">
        <img src="https://sonarcloud.io/api/project_badges/measure?project=imahjoub_hash_sha256&metric=alert_status" alt="Quality Gate Status"></a>
    <a href="https://codecov.io/gh/imahjoub/hash_sha256">
        <img src="https://codecov.io/gh/imahjoub/hash_sha256/branch/main/graph/badge.svg?token=3LIK8E96FC" alt="code coverage"></a>
    <a href="https://github.com/imahjoub/hash_sha256" alt="GitHub code size in bytes">
        <img src="https://img.shields.io/github/languages/code-size/imahjoub/hash_sha256" /></a>
    <a href="https://github.com/imahjoub/hash_sha256/blob/main/LICENSE_1_0.txt">
        <img src="https://img.shields.io/badge/license-BSL%201.0-blue.svg" alt="Boost Software License 1.0"></a>
    <a href="https://github.com/imahjoub/hash_sha256" alt="Activity">
        <img src="https://img.shields.io/github/commit-activity/y/imahjoub/hash_sha256" /></a>
    <a href="https://godbolt.org/z/44Yj848To" alt="godbolt">
        <img src="https://img.shields.io/badge/try%20it%20on-godbolt-green" /></a>
</p>

A simple implementation of SHA-256 in C++ (header-only). This implementation uses little endian byte order.

## Using the `hash_sha256`

  * The following program shows a test example

  ```cpp
  // Create an object of hash_sha256
  hash_sha256 hash1;

  // Original message
  const std::array<std::uint8_t, 3U> msg1 = {'a', 'b', 'c' };

  // Initialize hash
  hash1.sha256_init();

  // Update the hash with given data
  hash1.sha256_update(msg1.data(), msg1.size());

  // Get hash result
  sha256_output_type hash_result1 = hash1.sha256_final();
  ```

## Testing, continuous integration and quality
Testing and continuous integration runs on GitHub Actions.
Various OS/compiler combinations are used including
GCC/clang/MSVC.

Code coverage uses GCC/gcov/lcov and has a
quality-gate with comparison/baseline-check provided by third-party [Codecov](https://app.codecov.io/gh/imahjoub/hash_sha256).

Additional quality checks are performed on pull-request
and merge to master using modern third party open-source services.
These include
[LGTM](https://lgtm.com/projects/g/imahjoub/hash_sha256/alerts/?mode=list) and [CodeSonar](https://sonarcloud.io/summary/new_code?id=imahjoub_hash_sha256).

## Algorithm efficiency
The following picture shows the real-time measurement of the `sha256` benchmark run on 32-bit microcontrollers (ARM Cortex™ M4) of the `NUCLEO-F446RE` board.
The hash calculation takes about 32 µs

![sha_256](https://user-images.githubusercontent.com/48915588/206875379-1ba68838-5d61-4fde-b43c-ccdc65641f6e.jpg)


## References
Algorithm specification can be found here:
* http://csrc.nist.gov/publications/fips/fips180-2/fips180-2withchangenotice.pdf

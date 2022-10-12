hash_sha256
==================
<p align="center">
    <a href="https://github.com/imahjoub/hash_sha256/actions">
        <img src="https://github.com/imahjoub/hash_sha256/actions/workflows/hash_sha256.yml/badge.svg" alt="Build Status"></a>
    <a href="https://github.com/imahjoub/hash_sha256/issues?q=is%3Aissue+is%3Aopen+sort%3Aupdated-desc">
        <img src="https://custom-icon-badges.herokuapp.com/github/issues-raw/imahjoub/hash_sha256?logo=github" alt="Issues" /></a>
    <a href="https://lgtm.com/projects/g/imahjoub/hash_sha256/context:cpp">
        <img src="https://img.shields.io/lgtm/grade/cpp/g/imahjoub/hash_sha256.svg?logo=lgtm&logoWidth=18" alt="Language grade: C/C++" /></a>
    <a href="https://app.codacy.com/gh/imahjoub/hash_sha256/dashboard">
        <img src="https://app.codacy.com/project/badge/Grade/913aec26f468405ab0ff9a29beb6d400" alt="Codacy Badge" /></a>
    <a href="https://lgtm.com/projects/g/imahjoub/hash_sha256/alerts/?mode=list">
        <img src="https://img.shields.io/lgtm/alerts/g/imahjoub/hash_sha256.svg?logo=lgtm&logoWidth=18" alt="Total alerts" /></a>
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
</p>


A simple and C++ header-only implementation of SHA-256. This implementation uses little endian byte order.

## Characteristics of the SHA-256 Algorithm

  * SHA-256 is an unkeyed cryptographic hashing function that takes an input of variable length and produces a 256-bit long hash output

  * Message Length: The length of the cleartext should be less than 264 bits. The size needs to be in the comparison area to keep the digest as random as possible.

  * Digest Length: The length of the hash digest should be 256 bits in SHA 256 algorithm, 512 bits in SHA-512, and so on. Bigger digests usually suggest significantly more calculations at the cost of speed and space.

  * Irreversible: By design, all hash functions such as the SHA 256 are irreversible. You should neither get a plaintext when you have the digest beforehand nor should the digest provide its original value when you pass it through the hash function again.


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

  // final hash result
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
[LGTM](https://lgtm.com/projects/g/imahjoub/hash_sha256/alerts/?mode=list)
and [CodeSonar](https://sonarcloud.io/summary/new_code?id=imahjoub_hash_sha256).


## References
Algorithm specification can be found here:
* http://csrc.nist.gov/publications/fips/fips180-2/fips180-2withchangenotice.pdf

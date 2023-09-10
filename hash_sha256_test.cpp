///////////////////////////////////////////////////////////////////
//                                                               //
//  Copyright Iliass Mahjoub 2022 - 2023.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
//                                                               //
///////////////////////////////////////////////////////////////////

#ifdef HASH_HASH256_PC
#include <iostream>
#include <cstring>
#endif // HASH_HASH256_PC

#include <hash_sha256/hash_sha256.h>

auto hash_sha256_test1() -> bool
{
  constexpr std::array<std::uint8_t, 3U> msg1 = {'a', 'b', 'c' };

  constexpr sha256_type expected_hash1 =
  {
    0xBAU, 0x78U, 0x16U, 0xBFU, 0x8FU, 0x01U, 0xCFU, 0xEAU,
    0x41U, 0x41U, 0x40U, 0xDEU, 0x5DU, 0xAEU, 0x22U, 0x23U,
    0xB0U, 0x03U, 0x61U, 0xA3U, 0x96U, 0x17U, 0x7AU, 0x9CU,
    0xB4U, 0x10U, 0xFFU, 0x61U, 0xF2U, 0x00U, 0x15U, 0xADU
  };

  constexpr hash_sha256 hash1(msg1.data(), msg1.size());

  constexpr sha256_type hash_result1 = hash1.sha256_result();

  static_assert(hash_result1.front() == static_cast<std::uint8_t>(UINT8_C(0xBA)), "Error: Undexpected front-byte Hash Result");
  static_assert(hash_result1.back()  == static_cast<std::uint8_t>(UINT8_C(0xAD)), "Error: Undexpected back-byte Hash Result");

  constexpr auto result_is_ok = detail::equal(hash_result1.cbegin(), hash_result1.cend(), expected_hash1.cbegin());

  static_assert(result_is_ok, "Error: Undexpected total Hash Result");

  return (hash_result1 == expected_hash1);
}

#ifdef HASH_HASH256_PC
auto hash_sha256_test2() -> bool
{
  constexpr uint8_t msg2[] =
  {
    "abcdefghijklmnopqrstuvwxyz"
    "abcdefghijklmnopqrstuvwxyz"
    "abcdefghijklmnopqrstuvwxyz"
    "abcdefghijklmnopqrstuvwxyz"
    "abcdefghijklmnopqrstuvwxyz"
    "abcdefghijklmnopqrstuvwxyz"
    "abcdefghijklmnopqrstuvwxyz"
    "abcdefghijklmnopqrstuvwxyz"
    "abcdefghijklmnopqrstuvwxyz"
    "abcdefghijklmnopqrstuvwxyz"
    "abcdefghijklmnopqrstuvwxyz"
    "abcdefghijklmnopqrstuvwxyz"
  };

  constexpr sha256_type expected_hash2 =
  {
    0x06U, 0x4EU, 0xAEU, 0x61U, 0x97U, 0x8DU, 0xDBU, 0x8CU,
    0x86U, 0x76U, 0x4DU, 0xEFU, 0xD7U, 0x87U, 0x42U, 0x09U,
    0x50U, 0xD2U, 0xE4U, 0xB1U, 0x26U, 0xE8U, 0x30U, 0x6CU,
    0xDBU, 0x56U, 0x5EU, 0x3DU, 0x08U, 0x2DU, 0x55U, 0xDDU
  };

  hash_sha256 hash2;

  hash2.sha256_init();

  hash2.sha256_update(msg2, strlen(reinterpret_cast<const char*>(msg2)));

  hash2.sha256_final();

  sha256_type hash_result2 = hash2.sha256_result();

  const auto result_is_ok = detail::equal(hash_result2.cbegin(), hash_result2.cend(), expected_hash2.cbegin());

  return result_is_ok;
}
#endif // HASH_HASH256_PC

extern "C"
{
  volatile std::uint32_t hash_sha256_gdb_result;

  auto hash_sha256_get_gdb_result(void) -> bool;

  auto hash_sha256_get_gdb_result(void) -> bool // LCOV_EXCL_LINE
  {
    hash_sha256_gdb_result =
      static_cast<std::uint32_t>
      (
        hash_sha256_test1() ? UINT32_C(0xF00DCAFE) : UINT32_C(0xFFFFFFFF)
      );

    volatile auto hash_result_is_ok =
      (hash_sha256_gdb_result == UINT32_C(0xF00DCAFE));

    return hash_result_is_ok;
  }
}

auto main() -> int
{
  bool hash_result_is_ok = false;

  #ifdef HASH_HASH256_QEMU
  hash_result_is_ok = hash_sha256_get_gdb_result();
  #else

  hash_result_is_ok = (   hash_sha256_test1()
                       && hash_sha256_test2()
                       && hash_sha256_get_gdb_result());

  std::cout << "hash_result_is_ok: " << std::boolalpha << hash_result_is_ok << std::endl;

  #endif // STD_HASH256_QEMU

  return hash_result_is_ok ? 0 : -1;
}

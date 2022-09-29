///////////////////////////////////////////////////////////////////
//                                                               //
//  Copyright Iliass Mahjoub 2022.                               //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
//                                                               //
///////////////////////////////////////////////////////////////////

#ifdef STD_HASH256_IOSTREAM
#include <iostream>
#endif

#include <hash_sha256/hash_sha256.h>

auto hash_sha256_test() -> bool
{
  hash_sha256 my_sha256;

  const std::array<std::uint8_t, 72U> my_msg =
  {
    0x61U, 0x62U, 0x63U, 0x61U, 0x62U, 0x63U, 0x61U, 0x62U,
    0x63U, 0x61U, 0x62U, 0x63U, 0x61U, 0x62U, 0x63U, 0x61U,
    0x62U, 0x63U, 0x61U, 0x62U, 0x63U, 0x61U, 0x62U, 0x63U,
    0x61U, 0x62U, 0x63U, 0x61U, 0x62U, 0x63U, 0x61U, 0x62U,
    0x63U, 0x61U, 0x62U, 0x63U, 0x61U, 0x62U, 0x63U, 0x61U,
    0x62U, 0x63U, 0x61U, 0x62U, 0x63U, 0x61U, 0x62U, 0x63U,
    0x61U, 0x62U, 0x63U, 0x61U, 0x62U, 0x63U, 0x61U, 0x62U,
    0x63U, 0x61U, 0x62U, 0x63U, 0x61U, 0x62U, 0x63U, 0x61U,
    0x62U, 0x63U, 0x61U, 0x62U, 0x63U, 0x61U, 0x62U, 0x63U
  };

  const hash_output_type my_hash =
  {
    0x07U, 0x82U, 0x7AU, 0x89U, 0x74U, 0x2CU, 0xE3U, 0x60U,
    0x90U, 0x69U, 0x03U, 0x2DU, 0x06U, 0x45U, 0x96U, 0xC9U,
    0xD5U, 0x44U, 0xA4U, 0x59U, 0x50U, 0xC5U, 0x43U, 0xA2U,
    0xD1U, 0xBDU, 0xA5U, 0x00U, 0xA9U, 0xC1U, 0x74U, 0xF5U
  };

  my_sha256.update(my_msg.data(), my_msg.size());

  hash_output_type hash_result = my_sha256.digest();

  return (hash_result == my_hash);
}

extern "C"
{
  volatile std::uint32_t hash_sha256_gdb_result;

  auto hash_sha256_get_hash_result(void) -> bool;
  auto hash_sha256_get_gdb_result(void)  -> bool;

  auto hash_sha256_get_hash_result(void) -> bool
  {
    const bool result_is_ok = hash_sha256_test();

    hash_sha256_gdb_result =
      static_cast<std::uint32_t>
      (
        result_is_ok ? UINT32_C(0xF00DCAFE) : UINT32_C(0xFFFFFFFF)
      );

    return result_is_ok;
  }

  auto hash_sha256_get_gdb_result(void) -> bool
  {
    volatile auto result_is_ok =
      (hash_sha256_gdb_result == UINT32_C(0xF00DCAFE));

    return result_is_ok;
  }
}

auto main() -> int
{
  bool hash_is_ok = false;
  hash_is_ok      = hash_sha256_test();

  #ifdef STD_HASH256_IOSTREAM
  std::cout << "hash_is_ok: " << std::boolalpha << hash_is_ok << std::endl;
  #endif // HASH_SHA256_GDB

  #ifdef STD_HASH256_GDB
  hash_is_ok += hash_sha256_get_gdb_result();
  #endif // STD_HASH256_GDB

  return hash_is_ok ? 0 : -1;
}

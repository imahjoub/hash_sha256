#include <hash_sha256/hash_sha256.h>

#include <iostream>

auto main() -> int
{
  hash_sha256 my_sha256;

  const std::array<uint8_t, 3U> my_msg = { 'a', 'b', 'c'};

  const hash_output_type my_hash =
  {
    0xBAU, 0x78U, 0x16U, 0xBFU, 0x8FU, 0x01U, 0xCFU, 0xEAU,
    0x41U, 0x41U, 0x40U, 0xDEU, 0x5DU, 0xAEU, 0x22U, 0x23U,
    0xB0U, 0x03U, 0x61U, 0xA3U, 0x96U, 0x17U, 0x7AU, 0x9CU,
    0xB4U, 0x10U, 0xFFU, 0x61U, 0xF2U, 0x00U, 0x15U, 0xADU
  };

  my_sha256.update(my_msg.data(), my_msg.size());

  hash_output_type digest = my_sha256.digest();

  const bool hash_is_ok = (digest == my_hash);

  std::cout << "hash_is_ok: " << std::boolalpha << hash_is_ok << std::endl;

  return hash_is_ok ? 0 : -1;
}

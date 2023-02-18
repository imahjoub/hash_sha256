///////////////////////////////////////////////////////////////////
//                                                               //
//  Copyright Iliass Mahjoub 2022 - 2023.                        //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
//                                                               //
///////////////////////////////////////////////////////////////////

#ifndef HASH_SHA256_2022_06_02_H
  #define HASH_SHA256_2022_06_02_H

  #include <algorithm>
  #include <array>
  #include <cstdint>

  class hash_sha256
  {
  public:
    using result_type = std::array<std::uint8_t, UINT8_C(32)>;

    constexpr hash_sha256()                       = default;
    constexpr hash_sha256(const hash_sha256&)     = default;
    constexpr hash_sha256(hash_sha256&&) noexcept = default;
    ~hash_sha256()                                = default; // LCOV_EXCL_LINE

    constexpr auto operator=(hash_sha256&&) noexcept -> hash_sha256& = default;
    constexpr auto operator=(const hash_sha256&)     -> hash_sha256& = default;

    constexpr auto hash(const std::uint8_t* msg, const size_t length) -> result_type
    {
      sha256_init();
      sha256_update(msg, length);
      return sha256_finalize();
    }

    constexpr auto sha256_init() -> void
    {
      my_datalen = 0U;
      my_bitlen  = 0U;

      transform_context[0U] = UINT32_C(0x6A09E667);
      transform_context[1U] = UINT32_C(0xBB67AE85);
      transform_context[2U] = UINT32_C(0x3C6EF372);
      transform_context[3U] = UINT32_C(0xA54FF53A);
      transform_context[4U] = UINT32_C(0x510E527F);
      transform_context[5U] = UINT32_C(0x9B05688C);
      transform_context[6U] = UINT32_C(0x1F83D9AB);
      transform_context[7U] = UINT32_C(0x5BE0CD19);
    }

    constexpr auto sha256_update(const std::uint8_t* msg, const size_t length) -> void
    {
      for (std::size_t i = 0U; i < length; ++i)
      {
        my_data[my_datalen] = msg[i];
        my_datalen++;

        if(my_datalen == 64U)
        {
          sha256_transform();
          my_datalen = 0U;
          my_bitlen += 512U;
        }
      }
    }

    constexpr auto sha256_finalize() -> result_type
    {
      std::size_t i           =  0U;
      result_type hash_result = { };

      i = my_datalen;

      // Pad whatever data is left in the buffer.
      if(my_datalen < 56U)
      {
        my_data[i++] = 0x80U;
        std::fill((my_data.begin() + i), (my_data.begin() + 56U), 0U);
      }

      else
      {
        my_data[i++] = 0x80U;
        std::fill((my_data.begin() + i), my_data.end(), 0U);
        sha256_transform();
        std::fill_n(my_data.begin(), 56U, 0U);
      }

      // Append to the padding the total message's length in bits and transform.
      my_bitlen += static_cast<std::uint64_t>(my_datalen * UINT8_C(8));

      my_data[63U] = static_cast<std::uint8_t>(my_bitlen >> UINT8_C( 0));
      my_data[62U] = static_cast<std::uint8_t>(my_bitlen >> UINT8_C( 8));
      my_data[61U] = static_cast<std::uint8_t>(my_bitlen >> UINT8_C(16));
      my_data[60U] = static_cast<std::uint8_t>(my_bitlen >> UINT8_C(24));
      my_data[59U] = static_cast<std::uint8_t>(my_bitlen >> UINT8_C(32));
      my_data[58U] = static_cast<std::uint8_t>(my_bitlen >> UINT8_C(40));
      my_data[57U] = static_cast<std::uint8_t>(my_bitlen >> UINT8_C(48));
      my_data[56U] = static_cast<std::uint8_t>(my_bitlen >> UINT8_C(56));

      sha256_transform();

      // Since this implementation uses little endian byte ordering and SHA uses big endian,
      // reverse all the bytes when copying the final transform_context to the output hash.
      for(std::size_t j = 0U; j < 4U; ++j)
      {
        hash_result[j +  0U] = ((transform_context[0U] >> (24U - (j * 8U))) & UINT32_C(0x000000FF));
        hash_result[j +  4U] = ((transform_context[1U] >> (24U - (j * 8U))) & UINT32_C(0x000000FF));
        hash_result[j +  8U] = ((transform_context[2U] >> (24U - (j * 8U))) & UINT32_C(0x000000FF));
        hash_result[j + 12U] = ((transform_context[3U] >> (24U - (j * 8U))) & UINT32_C(0x000000FF));
        hash_result[j + 16U] = ((transform_context[4U] >> (24U - (j * 8U))) & UINT32_C(0x000000FF));
        hash_result[j + 20U] = ((transform_context[5U] >> (24U - (j * 8U))) & UINT32_C(0x000000FF));
        hash_result[j + 24U] = ((transform_context[6U] >> (24U - (j * 8U))) & UINT32_C(0x000000FF));
        hash_result[j + 28U] = ((transform_context[7U] >> (24U - (j * 8U))) & UINT32_C(0x000000FF));
      }

      return hash_result;
    }

  private:
    using transform_context_type = std::array<std::uint32_t, UINT8_C(8)>;

    std::uint32_t          my_datalen { };
    std::uint64_t          my_bitlen  { };

    std::array<std::uint8_t, 64U> my_data    { };
    transform_context_type transform_context { };

    static constexpr std::array<std::uint32_t, UINT8_C(64)> transform_constants =
    {
      UINT32_C(0x428A2F98), UINT32_C(0x71374491), UINT32_C(0xB5C0FBCF), UINT32_C(0xE9B5DBA5),
      UINT32_C(0x3956C25B), UINT32_C(0x59F111F1), UINT32_C(0x923F82A4), UINT32_C(0xAB1C5ED5),
      UINT32_C(0xD807AA98), UINT32_C(0x12835B01), UINT32_C(0x243185BE), UINT32_C(0x550C7DC3),
      UINT32_C(0x72BE5D74), UINT32_C(0x80DEB1FE), UINT32_C(0x9BDC06A7), UINT32_C(0xC19BF174),
      UINT32_C(0xE49B69C1), UINT32_C(0xEFBE4786), UINT32_C(0x0FC19DC6), UINT32_C(0x240CA1CC),
      UINT32_C(0x2DE92C6F), UINT32_C(0x4A7484AA), UINT32_C(0x5CB0A9DC), UINT32_C(0x76F988DA),
      UINT32_C(0x983E5152), UINT32_C(0xA831C66D), UINT32_C(0xB00327C8), UINT32_C(0xBF597FC7),
      UINT32_C(0xC6E00BF3), UINT32_C(0xD5A79147), UINT32_C(0x06CA6351), UINT32_C(0x14292967),
      UINT32_C(0x27B70A85), UINT32_C(0x2E1B2138), UINT32_C(0x4D2C6DFC), UINT32_C(0x53380D13),
      UINT32_C(0x650A7354), UINT32_C(0x766A0ABB), UINT32_C(0x81C2C92E), UINT32_C(0x92722C85),
      UINT32_C(0xA2BFE8A1), UINT32_C(0xA81A664B), UINT32_C(0xC24B8B70), UINT32_C(0xC76C51A3),
      UINT32_C(0xD192E819), UINT32_C(0xD6990624), UINT32_C(0xF40E3585), UINT32_C(0x106AA070),
      UINT32_C(0x19A4C116), UINT32_C(0x1E376C08), UINT32_C(0x2748774C), UINT32_C(0x34B0BCB5),
      UINT32_C(0x391C0CB3), UINT32_C(0x4ED8AA4A), UINT32_C(0x5B9CCA4F), UINT32_C(0x682E6FF3),
      UINT32_C(0x748F82EE), UINT32_C(0x78A5636F), UINT32_C(0x84C87814), UINT32_C(0x8CC70208),
      UINT32_C(0x90BEFFFA), UINT32_C(0xA4506CEB), UINT32_C(0xBEF9A3F7), UINT32_C(0xC67178F2)
    };

    constexpr auto sha256_transform() -> void
    {
      std::uint32_t tmp1 = 0U;
      std::uint32_t tmp2 = 0U;

      std::array<std::uint32_t, 8U> state = { };
      std::array<std::uint32_t, 64U> m    = { };

      for(std::size_t i = 0U, j = 0U; i < 16U; ++i, j += 4U)
      {
        m[i] = static_cast<std::uint32_t>
        (
            static_cast<std::uint32_t>(static_cast<std::uint32_t>(my_data[j + 0U]) << 24U)
          | static_cast<std::uint32_t>(static_cast<std::uint32_t>(my_data[j + 1U]) << 16U)
          | static_cast<std::uint32_t>(static_cast<std::uint32_t>(my_data[j + 2U]) <<  8U)
          | static_cast<std::uint32_t>(static_cast<std::uint32_t>(my_data[j + 3U]) <<  0U)
        );
      }

      for(std::size_t i = 16U ; i < 64U; ++i)
      {
        m[i] = ssig1(m[i - 2U]) + m[i - 7U] + ssig0(m[i - 15U]) + m[i - 16U];
      }

      std::copy(transform_context.begin(), transform_context.end() , state.begin());

      for(std::size_t i = 0U; i < 64U; ++i)
      {
        tmp1 = state[7U] + bsig1(state[4U]) + ch(state[4U], state[5U], state[6U]) + transform_constants[i] + m[i];

        tmp2 = bsig0(state[0U]) + maj(state[0U], state[1U], state[2U]);

        state[7U] = state[6U];
        state[6U] = state[5U];
        state[5U] = state[4U];
        state[4U] = state[3U] + tmp1;
        state[3U] = state[2U];
        state[2U] = state[1U];
        state[1U] = state[0U];
        state[0U] = tmp1 + tmp2;
      }

      transform_context[0U] += state[0U];
      transform_context[1U] += state[1U];
      transform_context[2U] += state[2U];
      transform_context[3U] += state[3U];
      transform_context[4U] += state[4U];
      transform_context[5U] += state[5U];
      transform_context[6U] += state[6U];
      transform_context[7U] += state[7U];
    }

    // circular left shift ROTR^n(x)
    static constexpr auto rotl(std::uint32_t a, unsigned b)                      -> std::uint32_t { return (static_cast<std::uint32_t>(a << b) | static_cast<std::uint32_t>(a >> (static_cast<unsigned>(UINT8_C(32)) - b))); }
    static constexpr auto rotr(std::uint32_t a, unsigned b)                      -> std::uint32_t { return (static_cast<std::uint32_t>(a >> b) | static_cast<std::uint32_t>(a << (static_cast<unsigned>(UINT8_C(32)) - b))); }
    static constexpr auto ch (std::uint32_t x, std::uint32_t y, std::uint32_t z) -> std::uint32_t { return (static_cast<std::uint32_t>(x & y) ^ static_cast<std::uint32_t>(~x & z)); }
    static constexpr auto maj(std::uint32_t x, std::uint32_t y, std::uint32_t z) -> std::uint32_t { return (static_cast<std::uint32_t>(x & y) ^ static_cast<std::uint32_t>(x & z) ^ static_cast<std::uint32_t>(y & z)); }

    static constexpr auto bsig0(std::uint32_t x) -> std::uint32_t { return (rotr(x, static_cast<unsigned>(UINT8_C( 2))) ^ rotr(x, static_cast<unsigned>(UINT8_C(13))) ^ rotr(x,   static_cast<unsigned>(UINT8_C(22)))); }
    static constexpr auto bsig1(std::uint32_t x) -> std::uint32_t { return (rotr(x, static_cast<unsigned>(UINT8_C( 6))) ^ rotr(x, static_cast<unsigned>(UINT8_C(11))) ^ rotr(x,   static_cast<unsigned>(UINT8_C(25)))); }
    static constexpr auto ssig0(std::uint32_t x) -> std::uint32_t { return (rotr(x, static_cast<unsigned>(UINT8_C( 7))) ^ rotr(x, static_cast<unsigned>(UINT8_C(18))) ^     (x >> static_cast<unsigned>(UINT8_C( 3)))); }
    static constexpr auto ssig1(std::uint32_t x) -> std::uint32_t { return (rotr(x, static_cast<unsigned>(UINT8_C(17))) ^ rotr(x, static_cast<unsigned>(UINT8_C(19))) ^     (x >> static_cast<unsigned>(UINT8_C(10)))); }
  };
#endif // HASH_SHA256_2022_06_02_H

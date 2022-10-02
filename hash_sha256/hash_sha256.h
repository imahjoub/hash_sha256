///////////////////////////////////////////////////////////////////
//                                                               //
//  Copyright Iliass Mahjoub 2022.                               //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
//                                                               //
///////////////////////////////////////////////////////////////////

#ifndef HASH_SHA256_2022_06_02_H
  #define HASH_SHA256_2022_06_02_H

  #include <algorithm>
  #include <array>

  using hash_output_type = std::array<std::uint8_t, 32U>;

  class hash_sha256
  {
  public:
    hash_sha256():
      m_data {0U}
      {
        m_init_hash_val[0U] = UINT32_C(0x6A09E667);
        m_init_hash_val[1U] = UINT32_C(0xBB67AE85);
        m_init_hash_val[2U] = UINT32_C(0x3C6EF372);
        m_init_hash_val[3U] = UINT32_C(0xA54FF53A);
        m_init_hash_val[4U] = UINT32_C(0x510E527F);
        m_init_hash_val[5U] = UINT32_C(0x9B05688C);
        m_init_hash_val[6U] = UINT32_C(0x1F83D9AB);
        m_init_hash_val[7U] = UINT32_C(0x5BE0CD19);
      }
    
    hash_sha256(const hash_sha256&) = delete;
    hash_sha256(hash_sha256&&)      = delete;
    virtual ~hash_sha256()          = default; // LCOV_EXCL_LINE

    auto operator=(const hash_sha256&) -> hash_sha256& = delete;
    auto operator=(hash_sha256&&) -> hash_sha256&      = delete;
    
    auto update(const std::uint8_t* data, std::size_t length) -> void
    {
      for(std::size_t i = 0 ; i < length ; ++i)
      {
        m_data[m_blocklen++] = data[i]; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

        if(m_blocklen == 64U)
        {
          transform();

          // End of the block
          m_bitlen  += 512U;
          m_blocklen = 0U;
        }
      }
    }

    auto digest() -> hash_output_type
    {
      hash_output_type hash = {0U};

      pad();

      convert(hash.data());

      return hash;
    }

  private:
    std::uint64_t m_bitlen   = 0U;
    std::uint32_t m_blocklen = 0U;

    std::array<std::uint8_t,  64U> m_data          = {0U};
    std::array<std::uint32_t,  8U> m_init_hash_val = {0U};

    static constexpr std::array<std::uint32_t, 64U> K =
    {
      0x428A2F98U, 0x71374491U, 0xB5C0FBCFU, 0xE9B5DBA5U,
      0x3956C25BU, 0x59F111F1U, 0x923F82A4U, 0xAB1C5ED5U,
      0xD807AA98U, 0x12835B01U, 0x243185BEU, 0x550C7DC3U,
      0x72BE5D74U, 0x80DEB1FEU, 0x9BDC06A7U, 0xC19BF174U,
      0xE49B69C1U, 0xEFBE4786U, 0x0FC19DC6U, 0x240CA1CCU,
      0x2DE92C6FU, 0x4A7484AAU, 0x5CB0A9DCU, 0x76F988DAU,
      0x983E5152U, 0xA831C66DU, 0xB00327C8U, 0xBF597FC7U,
      0xC6E00BF3U, 0xD5A79147U, 0x06CA6351U, 0x14292967U,
      0x27B70A85U, 0x2E1B2138U, 0x4D2C6DFCU, 0x53380D13U,
      0x650A7354U, 0x766A0ABBU, 0x81C2C92EU, 0x92722C85U,
      0xA2BFE8A1U, 0xA81A664BU, 0xC24B8B70U, 0xC76C51A3U,
      0xD192E819U, 0xD6990624U, 0xF40E3585U, 0x106AA070U,
      0x19A4C116U, 0x1E376C08U, 0x2748774CU, 0x34B0BCB5U,
      0x391C0CB3U, 0x4ED8AA4AU, 0x5B9CCA4FU, 0x682E6FF3U,
      0x748F82EEU, 0x78A5636FU, 0x84C87814U, 0x8CC70208U,
      0x90BEFFFAU, 0xA4506CEBU, 0xBEF9A3F7U, 0xC67178F2U
    };

    // circular right shift ROTR^n(x)
    static auto rotr(std::uint32_t x, std::uint32_t n) -> std::uint32_t
    {
      return (static_cast<std::uint32_t>(x >> n) | static_cast<std::uint32_t>(x << (32U - n)));
    }

    static auto sha_ch(std::uint32_t e, std::uint32_t f, std::uint32_t g) -> std::uint32_t
    {
      return (static_cast<std::uint32_t>(e & f) ^ static_cast<std::uint32_t>(~e & g));
    }

    static auto sha_maj(std::uint32_t a, std::uint32_t b, std::uint32_t c) -> std::uint32_t
    {
      return (static_cast<std::uint32_t>(a & (b | c)) | static_cast<std::uint32_t>(b & c));
    }

    static auto bsig0(std::uint32_t x) -> std::uint32_t
    {
      return (rotr(x, 2U) ^ rotr(x, 13U) ^ rotr(x, 22U));
    }

    static auto bsig1(std::uint32_t x) -> std::uint32_t
    {
      return (rotr(x, 6U) ^ rotr(x, 11U) ^ rotr(x, 25U));
    }

    static auto ssig0(std::uint32_t x) -> std::uint32_t
    {
      return (rotr(x, 7U) ^ rotr(x, 18U) ^ (x >> 3U));
    }

    static auto ssig1(std::uint32_t x) -> std::uint32_t
    {
      return (rotr(x, 17U) ^ rotr(x, 19U) ^ (x >> 10U));
    }

    auto transform() -> void
    {
      std::uint32_t sum   = 0U;
      std::uint32_t tmp1 = 0U;
      std::uint32_t tmp2 = 0U;

      std::array<std::uint32_t, 64> m     = {0U};
      std::array<std::uint32_t, 8U> state = {0U};

      std::size_t j = 0U;

      for(std::uint8_t i = 0U; i < 16U; ++i)
      {
        // Split data in 32 bit blocks for the 16 first words
        m[i] = static_cast<std::uint32_t>(   static_cast<std::uint32_t>(static_cast<std::uint32_t>(m_data[j + 0U]) << 24U)
                                           | static_cast<std::uint32_t>(static_cast<std::uint32_t>(m_data[j + 1U]) << 16U)
                                           | static_cast<std::uint32_t>(static_cast<std::uint32_t>(m_data[j + 2U]) <<  8U)
                                           | static_cast<std::uint32_t>(static_cast<std::uint32_t>(m_data[j + 3U]) <<  0U));
        j += 4U;
      }

      for(std::uint8_t k = 16U; k < 64U; ++k)
      {
        // Remaining 48 blocks
        m[k] = ssig1(m[k - 2U]) + m[k - 7U] + ssig0(m[k - 15U]) + m[k - 16U];
      }

      std::copy(m_init_hash_val.begin(), m_init_hash_val.end(), state.begin());

      for(std::uint8_t i = 0U; i < 64U; ++i)
      {
        sum   = (  static_cast<std::uint32_t>(m[i])
                 + static_cast<std::uint32_t>(hash_sha256::K[i])
                 + static_cast<std::uint32_t>(state[7U])
                 + static_cast<std::uint32_t>(sha_ch(state[4U], state[5U], state[6U]))
                 + static_cast<std::uint32_t>(bsig1(state[4U])));

        tmp1  = (  static_cast<std::uint32_t>(bsig0(state[0U]))
                 + static_cast<std::uint32_t>(sha_maj(state[0U], state[1U], state[2U]))
                 + static_cast<std::uint32_t>(sum));

        tmp2  = (  static_cast<std::uint32_t>(state[3U])
                 + static_cast<std::uint32_t>(sum));

        state[7U] = state[6U];
        state[6U] = state[5U];
        state[5U] = state[4U];
        state[4U] = tmp2;
        state[3U] = state[2U];
        state[2U] = state[1U];
        state[1U] = state[0U];
        state[0U] = tmp1;
      }

      for(std::uint8_t i = 0U; i < 8U; ++i)
      {
        m_init_hash_val[i] += state[i];
      }

    }

    auto pad() -> void
    {
            std::uint64_t i  = m_blocklen;
      const std::uint8_t end = (m_blocklen < 56U) ? 56U : 64U;

      m_data[i++] = 0x80U;  // Append a bit 1

      std::fill((m_data.begin() + i), (m_data.begin() + end), 0U);


      if(m_blocklen >= 56U)
      {
        transform();

        std::fill_n(m_data.begin(), 56U, 0U);
      }

      // Append to the padding the total message's length in bits and transform.
      m_bitlen   += static_cast<std::uint64_t>(static_cast<std::uint32_t>(m_blocklen) * UINT8_C(8U));
      m_data[63U] = static_cast<std::uint8_t> (static_cast<std::uint64_t>(m_bitlen)  >> UINT8_C(0U));
      m_data[62U] = static_cast<std::uint8_t> (static_cast<std::uint64_t>(m_bitlen)  >> UINT8_C(8U));
      m_data[61U] = static_cast<std::uint8_t> (static_cast<std::uint64_t>(m_bitlen)  >> UINT8_C(16U));
      m_data[60U] = static_cast<std::uint8_t> (static_cast<std::uint64_t>(m_bitlen)  >> UINT8_C(24U));
      m_data[59U] = static_cast<std::uint8_t> (static_cast<std::uint64_t>(m_bitlen)  >> UINT8_C(32U));
      m_data[58U] = static_cast<std::uint8_t> (static_cast<std::uint64_t>(m_bitlen)  >> UINT8_C(40U));
      m_data[57U] = static_cast<std::uint8_t> (static_cast<std::uint64_t>(m_bitlen)  >> UINT8_C(48U));
      m_data[56U] = static_cast<std::uint8_t> (static_cast<std::uint64_t>(m_bitlen)  >> UINT8_C(56U));

      transform();
    }

    static auto u8from32(const std::uint32_t& src32, std::uint8_t* dst8) -> void
    {
      dst8[0U] = static_cast<std::uint8_t>(src32 >> 24U); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
      dst8[1U] = static_cast<std::uint8_t>(src32 >> 16U); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
      dst8[2U] = static_cast<std::uint8_t>(src32 >>  8U); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
      dst8[3U] = static_cast<std::uint8_t>(src32 >>  0U); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }

    auto convert(std::uint8_t* hash) -> void
    {
      std::size_t j = 0U;

      std::for_each(std::begin(m_init_hash_val), std::end(m_init_hash_val),
        [&hash, &j](auto &elem)
        {
          u8from32(elem, &hash[j + 0U]); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
          j += 4U;
        });
    }
  };

#endif // HASH_SHA256_2022_06_02_H

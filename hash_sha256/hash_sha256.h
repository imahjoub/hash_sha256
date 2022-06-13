///////////////////////////////////////////////////////////////////////////////
//  Copyright Iliass Mahjoub 2022.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#ifndef HASH_SHA256_2022_06_02_H
  #define HASH_SHA256_2022_06_02_H

  #include <array>
  #include <cstdint>
  #include <cstring>
  #include <iomanip>
  #include <sstream>

  using hash_output_type = std::array<std::uint8_t, 32U>;

  class hash_sha256
  {
  public:
    hash_sha256():
      m_data {0U}
      {
        m_init_hash_val[0U] = 0x6A09E667U;
        m_init_hash_val[1U] = 0xBB67AE85U;
        m_init_hash_val[2U] = 0x3C6EF372U;
        m_init_hash_val[3U] = 0xA54FF53AU;
        m_init_hash_val[4U] = 0x510E527FU;
        m_init_hash_val[5U] = 0x9B05688CU;
        m_init_hash_val[6U] = 0x1F83D9ABU;
        m_init_hash_val[7U] = 0x5BE0CD19U;
      }
    
    hash_sha256(const hash_sha256&) = delete;

    hash_sha256(hash_sha256&&) = delete;

    virtual ~hash_sha256() = default;    

    auto operator=(const hash_sha256&) -> hash_sha256& = delete;

    auto operator=(hash_sha256&&) -> hash_sha256& = delete;
    
    auto update(const std::uint8_t* data, std::size_t length) -> void
    {
      for(std::size_t i = 0 ; i < length ; i++)
      {
        m_data[m_blocklen++] = data[i]; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

        if(m_blocklen == 64U)
        {
          transform();

          // End of the block
          m_bitlen += 512;
          m_blocklen = 0U;
        }
      }
    }

    auto digest() -> hash_output_type
    {
      hash_output_type hash;

      hash.fill(0U);

      pad();

      revert(hash.data());

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

    static auto rotr(std::uint32_t x, std::uint32_t n) -> std::uint32_t
    {
      return (x >> n) | (x << (32U - n));
    }

    static auto choose(std::uint32_t e, std::uint32_t f, std::uint32_t g) -> std::uint32_t
    {
      return (e & f) ^ (~e & g);
    }

    static auto majority(std::uint32_t a, std::uint32_t b, std::uint32_t c) -> std::uint32_t
    {
      return (a & (b | c)) | (b & c);
    }

    static auto sig0(std::uint32_t x) -> std::uint32_t
    {
      return rotr(x, 7U) ^ rotr(x, 18U) ^ (x >> 3U);
    }

    static auto sig1(std::uint32_t x) -> std::uint32_t
    {
      return rotr(x, 17U) ^ rotr(x, 19U) ^ (x >> 10U);
    }

    auto transform() -> void
    {
      std::uint32_t maj   = 0U;
      std::uint32_t xor_a = 0U;
      std::uint32_t ch    = 0U;
      std::uint32_t xor_e = 0U;
      std::uint32_t sum   = 0U;
      std::uint32_t new_a = 0U;
      std::uint32_t new_e = 0U;

      std::array<std::uint32_t, 64> m     = {0U};
      std::array<std::uint32_t, 8U> state = {0U};

      for(std::uint8_t i = 0U, j = 0U; i < 16U; ++i, j += 4U)
      {
        // Split data in 32 bit blocks for the 16 first words
        m[i] = static_cast<std::uint32_t>(   static_cast<std::uint32_t>(static_cast<std::uint32_t>(m_data[j + 0U]) << 24U)
                                           | static_cast<std::uint32_t>(static_cast<std::uint32_t>(m_data[j + 1U]) << 16U)
                                           | static_cast<std::uint32_t>(static_cast<std::uint32_t>(m_data[j + 2U]) <<  8U)
                                           | static_cast<std::uint32_t>(static_cast<std::uint32_t>(m_data[j + 3U]) <<  0U));
      }

      for(std::uint8_t k = 16U; k < 64U; ++k)
      {
        // Remaining 48 blocks
        m[k] = sig1(m[k - 2U]) + m[k - 7U] + sig0(m[k - 15U]) + m[k - 16U];
      }

      for(std::uint8_t i = 0U; i < 8U; ++i)
      {
        state[i] = m_init_hash_val[i]; 
      }

      for(std::uint8_t i = 0U; i < 64U; ++i)
      {
        maj    = majority(state[0U], state[1U], state[2U]);
        xor_a  = (rotr(state[0U], 2U) ^ rotr(state[0U], 13U) ^ rotr(state[0U], 22U));
        ch     = choose(state[4U], state[5U], state[6U]);
        xor_e  = rotr(state[4U], 6U) ^ rotr(state[4U], 11U) ^ rotr(state[4U], 25U);
        sum    = (m[i] + hash_sha256::K[i] + state[7U] + ch + xor_e);

        new_a  = (xor_a + maj + sum);
        new_e  = (state[3U] + sum);

        state[7U] = state[6U];
        state[6U] = state[5U];
        state[5U] = state[4U];
        state[4U] = new_e;
        state[3U] = state[2U];
        state[2U] = state[1U];
        state[1U] = state[0U];
        state[0U] = new_a;
      }

      for(std::uint8_t i = 0U; i < 8U; ++i)
      {
        m_init_hash_val[i] += state[i];
      }

    }

    auto pad() -> void
    {
      std::uint64_t i  = m_blocklen;
      std::uint8_t end = (m_blocklen < 56) ? 56 : 64;

      m_data[i++] = 0x80U;  // Append a bit 1

      while (i < end)
      {
        m_data[i++] = 0x00U; // Pad with zeros
      }

      if(m_blocklen >= 56U)
      {
        transform();
        memset(m_data.data(), 0U, 56U);
      }

      // Append to the padding the total message's length in bits and transform.
      m_bitlen   += static_cast<std::uint64_t>(m_blocklen * 8U);
      m_data[63U] = static_cast<std::uint8_t>(m_bitlen >>  0U);
      m_data[62U] = static_cast<std::uint8_t>(m_bitlen >>  8U);
      m_data[61U] = static_cast<std::uint8_t>(m_bitlen >> 16U);
      m_data[60U] = static_cast<std::uint8_t>(m_bitlen >> 24U);
      m_data[59U] = static_cast<std::uint8_t>(m_bitlen >> 32U);
      m_data[58U] = static_cast<std::uint8_t>(m_bitlen >> 40U);
      m_data[57U] = static_cast<std::uint8_t>(m_bitlen >> 48U);
      m_data[56U] = static_cast<std::uint8_t>(m_bitlen >> 56U);

      transform();
    }

    auto revert(std::uint8_t* hash) -> void
    {
      // SHA uses big endian byte ordering
      // Revert all bytes
      for(std::uint8_t i = 0U; i < 4U; ++i)
      {
        for(std::uint8_t j = 0U; j < 8U; ++j)
        {
          hash[i + (j * 4U)] = ((m_init_hash_val[j] >> (24U - i * 8U)) & 0X000000FFU); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        }
      }
    }
  };

#endif // HASH_SHA256_2022_06_02_H

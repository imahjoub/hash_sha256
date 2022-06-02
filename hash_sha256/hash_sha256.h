#ifndef HASH_SHA256_2022_06_02_H
  #define HASH_SHA256_2022_06_02_H

  #include <array>
  #include <cstdint>
  #include <cstring>
  #include <iomanip>
  #include <string>
  #include <sstream>

  using hash_output_type = std::array<std::uint8_t, 32U>;

  class hash_sha256
  {
  public:
    hash_sha256():
     m_blocklen(0U),
     m_bitlen  (0U)
     {
       m_state[0U] = 0x6A09E667U;
       m_state[1U] = 0xBB67AE85U;
       m_state[2U] = 0x3C6EF372U;
       m_state[3U] = 0xA54FF53AU;
       m_state[4U] = 0x510E527FU;
       m_state[5U] = 0x9B05688CU;
       m_state[6U] = 0x1F83D9ABU;
       m_state[7U] = 0x5BE0CD19U;
    }

    void update(const std::uint8_t* data, std::size_t length)
    {
      for (std::size_t i = 0 ; i < length ; i++)
      {
        m_data[m_blocklen++] = data[i];

        if (m_blocklen == 64)
        {
          transform();

         // End of the block
         m_bitlen += 512;
         m_blocklen = 0;
        }
      }
    }

    hash_output_type digest()
    {
      hash_output_type hash; //= new std::uint8_t[32];

      pad();

      revert(hash.data());

      return hash;
    }

    static std::string toString(const std::uint8_t * digest)
    {
      std::stringstream s;

      s << std::setfill('0') << std::hex;

      for(std::uint8_t i = 0 ; i < 32 ; i++)
      {
        s << std::setw(2) << (unsigned int) digest[i];
      }

      return s.str();
    }

  private:
    std::uint8_t  m_data[64U];
    std::uint32_t m_blocklen;
    std::uint64_t m_bitlen;
    std::uint32_t m_state[8U]; //A, B, C, D, E, F, G, H

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

    static std::uint32_t rotr(std::uint32_t x, std::uint32_t n)
    {
      return (x >> n) | (x << (32 - n));
    }

    static std::uint32_t choose(std::uint32_t e, std::uint32_t f, std::uint32_t g)
    {
      return (e & f) ^ (~e & g);
    }
    static std::uint32_t majority(std::uint32_t a, std::uint32_t b, std::uint32_t c)
    {
      return (a & (b | c)) | (b & c);
    }

    static std::uint32_t sig0(std::uint32_t x)
    {
      return hash_sha256::rotr(x, 7) ^ hash_sha256::rotr(x, 18) ^ (x >> 3);
    }

    static std::uint32_t sig1(std::uint32_t x)
    {
      return hash_sha256::rotr(x, 17) ^ hash_sha256::rotr(x, 19) ^ (x >> 10);
    }

    void transform()
    {
      uint32_t maj, xorA, ch, xorE, sum, newA, newE, m[64];
      uint32_t state[8];

      for (std::uint8_t i = 0, j = 0; i < 16; i++, j += 4)
      {
        // Split data in 32 bit blocks for the 16 first words
        m[i] = (m_data[j] << 24) | (m_data[j + 1] << 16) | (m_data[j + 2] << 8) | (m_data[j + 3]);
      }

      for (std::uint8_t k = 16 ; k < 64; k++)
      {
        // Remaining 48 blocks
        m[k] = hash_sha256::sig1(m[k - 2]) + m[k - 7] + hash_sha256::sig0(m[k - 15]) + m[k - 16];
      }

      for(std::uint8_t i = 0 ; i < 8 ; i++)
      {
        state[i] = m_state[i];
      }

      for (std::uint8_t i = 0; i < 64; i++)
      {
        maj   = hash_sha256::majority(state[0], state[1], state[2]);
        xorA  = hash_sha256::rotr(state[0], 2) ^ hash_sha256::rotr(state[0], 13) ^ hash_sha256::rotr(state[0], 22);

        ch = choose(state[4], state[5], state[6]);

        xorE  = hash_sha256::rotr(state[4], 6) ^ hash_sha256::rotr(state[4], 11) ^ hash_sha256::rotr(state[4], 25);

        sum  = m[i] + K[i] + state[7] + ch + xorE;
        newA = xorA + maj + sum;
        newE = state[3] + sum;

        state[7] = state[6];
        state[6] = state[5];
        state[5] = state[4];
        state[4] = newE;
        state[3] = state[2];
        state[2] = state[1];
        state[1] = state[0];
        state[0] = newA;
      }

      for(std::uint8_t i = 0 ; i < 8 ; i++)
      {
        m_state[i] += state[i];
      }

    }

    void pad()
    {
      uint64_t i       = m_blocklen;
      std::uint8_t end = m_blocklen < 56 ? 56 : 64;

      m_data[i++] = 0x80; // Append a bit 1

      while (i < end)
      {
        m_data[i++] = 0x00; // Pad with zeros
      }

      if(m_blocklen >= 56)
      {
        transform();
        memset(m_data, 0, 56);
      }

      // Append to the padding the total message's length in bits and transform.
      m_bitlen   += static_cast<std::uint8_t>(m_blocklen * 8);
      m_data[63U] = static_cast<std::uint8_t>(m_bitlen);
      m_data[62U] = static_cast<std::uint8_t>(m_bitlen >> 8);
      m_data[61U] = static_cast<std::uint8_t>(m_bitlen >> 16);
      m_data[60U] = static_cast<std::uint8_t>(m_bitlen >> 24);
      m_data[59U] = static_cast<std::uint8_t>(m_bitlen >> 32);
      m_data[58U] = static_cast<std::uint8_t>(m_bitlen >> 40);
      m_data[57U] = static_cast<std::uint8_t>(m_bitlen >> 48);
      m_data[56U] = static_cast<std::uint8_t>(m_bitlen >> 56);

      transform();
    }

    void revert(std::uint8_t* hash)
    {
      // SHA uses big endian byte ordering
      // Revert all bytes
      for (std::uint8_t i = 0 ; i < 4 ; i++)
      {
        for(std::uint8_t j = 0 ; j < 8 ; j++)
        {
          hash[i + (j * 4)] = ((m_state[j] >> (24 - i * 8)) & 0X000000FFU);
        }
      }
    }
  };

#endif // HASH_SHA256_2022_06_02_H

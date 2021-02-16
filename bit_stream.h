#pragma once
#include <cstdint>
#include <cassert>

class BitStream
{
public:
    BitStream(uint8_t *p, int64_t numBytes = INT64_MAX)
        : m_p(p)
        , m_numBytes(numBytes)
    {
        Reset();
    }

    uint8_t GetBits(unsigned int n)
    {
        assert(n >=0 && n<=8);

        uint8_t ret = 0;

        while(n--)
        {
            if(m_mask)
            {
                ret <<= 1;
                ret |= (m_byte & m_mask) >> m_pos;
                m_mask >>= 1;
                m_pos--;
            }
            else
            {
                m_p++;
                m_numBytes--;
                Reset();
            }
        }

        return ret;
    }

    bool ByteAligned()
    {
        return m_mask == 0x80; // Or m_pos==7
    }

    bool MoreDataInByteStream()
    {
        return m_numBytes != 0;
    }

    void Reset()
    {
        m_byte = *m_p;
        m_pos = 7;
        m_mask = 0x80;
    }

    uint8_t *m_p;
    uint8_t m_mask;
    uint8_t m_pos;
    uint8_t m_byte;
    int64_t m_numBytes;
};
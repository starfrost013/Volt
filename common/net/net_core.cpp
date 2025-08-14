#include <common/common.hpp>

//
// net_core.cpp:
// Network Core implementation
// NOTE: This doesn't currently implement any kind of a network, it's so the engine can be easily extended later.
//


namespace Volt
{
    // The current network type.
    NetworkType net_type = NetworkType::Loopback;

    bool net_initialised;

    void Net_Init(NetworkType network_type)
    {
        net_type = network_type;

        // Initialise based on the network type.
        switch (net_type)
        {
            case NetworkType::Loopback:
                Logging_LogChannel("******** Net_Init start: Loopback ********", LogChannel::Debug);
                Loopback_Init();
                Logging_LogChannel("******** Net_Init done: Loopback ********", LogChannel::Debug);
                break;
        }

    }

    void Net_Shutdown()
    {

    }

    /* Message functions */
    uint8_t* Net_ReadMessage(uint32_t length)
    {
        switch (net_type)
        {
            case NetworkType::Loopback:
                return Loopback_ReadMessage(length);
                break;
        }

        return nullptr; 
    }

    void Net_WriteMessage(uint8_t* msg, uint32_t length)
    {
        switch (net_type)
        {
            case NetworkType::Loopback:
                Loopback_WriteMessage(msg, length);
                break;
        }
    }

    /* Message API functions */

    /* Reading */
    uint8_t Net_ReadByte()
    {
        return *Net_ReadMessage(1);
    }

    uint16_t Net_ReadShort()
    {
        return *(uint16_t*)Net_ReadMessage(2);
    }

    uint32_t Net_ReadInt()
    {
        return *(uint32_t*)Net_ReadMessage(4);
    }

    uint64_t Net_ReadLong()
    {
        return *(uint64_t*)Net_ReadMessage(4);
    }

    int8_t Net_ReadByteS()
    {
        return (int8_t)*Net_ReadMessage(1);
    }

    int16_t Net_ReadShortS()
    {
        return *(int16_t*)Net_ReadMessage(2);
    }

    int32_t Net_ReadIntS()
    {
        return *(int32_t*)Net_ReadMessage(4);
    }

    int64_t Net_ReadLongS()
    {
        return *(int64_t*)Net_ReadMessage(8);

    }

    // Reads until 00, or max_length (0 to disable - very dangerous)
    char* Net_ReadString(char* buf, uint32_t max_length)
    {
        // return the buffer unchanged in the case of an error 

        if (max_length > MAX_STRING_LENGTH)
        {
            Logging_LogChannel("Net_ReadString: Your buffer is too big (%d > %d), shrink it or increase MAX_STRING_LENGTH",
                 LogChannel::Warning, max_length, MAX_STRING_LENGTH);
            return buf; 
        }

        if (max_length <= 0)
        {
            Logging_LogChannel("Net_ReadString: Your buffer is too small (%d <= 0). Fix it",
                LogChannel::Warning, max_length, MAX_STRING_LENGTH);
            return buf;
        }

        if (max_length)
        {
            for (uint32_t position = 0; position < max_length; position++)
            {
                int32_t byte_next = Net_ReadByte();

                buf[position] = byte_next; 

                // ensures null terminated strings by checking after the set
                if (!byte_next)
                    break; 
            
            }

            return buf; 
        }
        else // no max length, read until 00
        {
            int32_t byte_next = 0, position = 0; 
            
            while (byte_next)
            {
                byte_next = Net_ReadByte();
                buf[position] = byte_next; 

                position++;

                if (position >= MAX_STRING_LENGTH)
                    break; 
            }

            return buf;
        }
    } 

    /* Writing */
    void Net_WriteByte(uint8_t value)
    {
        Net_WriteMessage(&value, 1);
    }

    void Net_WriteShort(uint16_t value)
    {
        Net_WriteMessage((uint8_t*)&value, 2);
    }
    
    void Net_WriteInt(uint32_t value)
    {
        Net_WriteMessage((uint8_t*)&value, 4);
    }

    void Net_WriteLong(uint64_t value)
    {
        Net_WriteMessage((uint8_t*)&value, 8);
    }

    void Net_WriteByteS(int8_t value)
    {
        Net_WriteMessage((uint8_t*)&value, 1);
    }

    void Net_WriteShortS(int16_t value)
    {
        Net_WriteMessage((uint8_t*)&value, 2);
    }

    void Net_WriteIntS(int32_t value)
    {
        Net_WriteMessage((uint8_t*)&value, 4);
    }

    void Net_WriteLongS(int64_t value)
    {
        Net_WriteMessage((uint8_t*)&value, 8);
    }

    void Net_WriteString(char* value, uint32_t max_length)
    {
        auto real_length = strlen(value);

        if (real_length <= 0 
        || real_length >= max_length)
        {
            Logging_LogChannel("Net_WriteString: invalid string length (0-%d, was %d!)", LogChannel::Warning, max_length, real_length);
            return;
        }

        for (uint32_t id = 0; id < real_length; id++)
        {
            Net_WriteByte(value[id]);
        }
        
        // ensure a string terminator gets written, harmless (if slow) if we write an extra one. 
        Net_WriteByte(0x00);

    } // Writes until 00 or max_length

}

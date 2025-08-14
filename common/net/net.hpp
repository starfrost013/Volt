#pragma once 
#include <Volt.hpp>
//
// net.hpp:
// Network Core
// NOTE: This doesn't currently implement any kind of a network, it's so the engine can be easily extended later.
//

namespace Volt 
{
    // Stolen from quake 3, we can always increase it later.
    #define MAX_LOOPBACK        16
    #define MAX_PACKET          4096 //4kb seems reasonable
    #define MAX_STRING_LENGTH   1024 

    typedef enum 
    {
        // Defines a network where the client and server share a buffer.
        Loopback = 0,

        // Defines a dedicated server.
        DedicatedServer = 1,

        // Defines a listen server.
        ListenServer = 2,

        // Defines a client connected to a server.
        ClientOnly = 3,
    } NetworkType;

    extern NetworkType net_type;

    typedef enum 
    {
        // A ping
        Ping = 0,

        // Ping ack from the client
        PingAck = 1,

    } NetworkMessageType;

    typedef struct
    {
        uint32_t data_length;
        uint8_t message[MAX_PACKET];
    } NetworkMessage;

    // Determines if the network system has been initialised.
    extern bool net_initialised;

    void Net_Init(NetworkType network_type);

    /* Core packet writing functions */
    uint8_t* Net_ReadMessage(uint32_t length);
    void Net_WriteMessage(uint8_t* msg, uint32_t length);

    /* 
        Read or write the byte for the message type first. 
        Then write the information that you need.

        All of this uses the above internal functions to read N bytes from the buffer before casting (strings have some other processing)
    */

    uint8_t Net_ReadByte();
    uint16_t Net_ReadShort();
    uint32_t Net_ReadInt();
    uint64_t Net_ReadLong();
    int8_t Net_ReadByteS();
    int16_t Net_ReadShortS();
    int32_t Net_ReadIntS();
    int64_t Net_ReadLongS();
    char* Net_ReadString(char* buf, uint32_t max_length = MAX_STRING_LENGTH); // Reads until 00, or max_length (0 to disable - very dangerous)

    void Net_WriteByte(uint8_t value);
    void Net_WriteShort(uint16_t value);
    void Net_WriteInt(uint32_t value);
    void Net_WriteLong(uint64_t value);
    void Net_WriteByteS(int8_t value);
    void Net_WriteShortS(int16_t value);
    void Net_WriteIntS(int32_t value);
    void Net_WriteLongS(int64_t value);
    void Net_WriteString(char* value, uint32_t max_length = MAX_STRING_LENGTH); // Reads until 00, or max_length (0 to disable - very dangerous)

    void Net_Shutdown();

    //
    // Network types
    //

    /* Loopback info */
    typedef struct 
    {
        uint8_t get;
        uint8_t put;
        NetworkMessage messages[MAX_LOOPBACK];
    } NetworkLoopback;

    extern NetworkLoopback loopback;

    void Loopback_Init();
    uint8_t* Loopback_ReadMessage(uint32_t length);
    void Loopback_WriteMessage(uint8_t* msg, uint32_t length);
}
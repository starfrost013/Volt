#include <common/common.hpp>

namespace Volt
{   
    /* Variables used for each type of network channel */
    NetworkLoopback loopback;

    void Loopback_Init()
    {

    }

    uint8_t* Loopback_ReadMessage(uint32_t length)
    {
        return nullptr; 
    }

    void Loopback_WriteMessage(uint8_t* msg, uint32_t length)
    {

    }
}

// 
// VOLT
// Copyright © 2025 starfrost
//
// config.hpp: Defines the machine config system
//

#pragma once
#include <common/common.hpp>
#include <external/pdjson/pdjson.hpp>

namespace Volt
{
    void Config_Load();

    const char* Config_GetString(const char* key);
    uint32_t Config_GetInt32(const char* key);
    bool Config_GetBool(const char* key);
    double Config_GetDouble(const char* key);

    // Low level read stuff
    int32_t Config_Get(void* user);
    int32_t Config_Peek(void* user);

    void Config_Shutdown();
}
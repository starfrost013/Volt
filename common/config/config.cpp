// 
// VOLT
// Copyright © 2025 starfrost
//
// config.cpp: Implements the config system
//

#include <common/config/config.hpp>
#include <utility>


namespace Volt
{
    Cvar* config_file_name; 
    VoltFile* config_stream;        // so that it can be opened

    std::unordered_map<const char*, const char*> config;

    #define NUMERICAL_BUF_LENGTH    32       // longest possible 64 bit = 26 + 1 for sign + some extra to be safee

    void Config_Load()
    {
        // this is only used within this function
        json_stream config_json; 
        
        config_file_name = Cvar_Get("config_file_name", "Config.volt", false);

        if (!config_file_name)
        {
            Logging_LogChannel("Failed to open config file at %s", LogChannel::Fatal, config_file_name->value);
            return; 
        }

        config_stream = Filesystem_OpenFile(config_file_name->string, VoltFileMode::Text, false);
        

        json_open_user(&config_json, Config_Get, Config_Peek, nullptr);

        enum json_type type; 

        bool first = false; // two DONEs = done
        bool keep_going = true; 
        bool error = false; // an error occurred

        const char* key;
        const char* value[MAX_STRING_LENGTH] = {0};
        char num_buf[NUMERICAL_BUF_LENGTH];

        // stupid stupid pdjson stupid
        size_t max = MAX_STRING_LENGTH;

        while (keep_going)
        {
            type = json_next(&config_json);

            switch (type)
            {
                case JSON_STRING:
                    key = json_get_string(&config_json, &max);
                    type = json_next(&config_json);

                    // see what is next
                    switch (type)
                    {
                        case JSON_DONE:
                            first = true;
                            break; 
                        case JSON_ERROR:
                            error = true;
                            break;
                        case JSON_STRING:
                            config.insert(std::make_pair(key, json_get_string(&config_json, &max)));
                            break; 
                        case JSON_TRUE:
                            config.insert(std::make_pair(key, "true"));
                            break;
                        case JSON_FALSE:
                            config.insert(std::make_pair(key, "false"));
                            break;
                        case JSON_NUMBER:
                            snprintf(num_buf, NUMERICAL_BUF_LENGTH, "%f", json_get_number(&config_json));
                            config.insert(std::make_pair(key, num_buf));
                            break; 
                    }

                    
                    // set up the config now
                    break;

                //we don't care about most types currently
                case json_type::JSON_ERROR:
                    error = true;
                    break; 
                default:
                    break;
            }

            if (error)
                break; 
        }

        json_close(&config_json);

        // We're done so close
        Filesystem_CloseFile(config_stream);

        // shut down if an error was found
        if (error)
        {
            Logging_LogChannel("Invalid config file at %s: %s", LogChannel::Fatal, config_file_name->value);
            return;             
        }
    }

    // I wonder if we should just make a C++ PDjson.

    // Low level function called by PD-JSON for get.
    int32_t Config_Get(void* user)
    {
        int32_t ret = 0x00;

        config_stream->file >> ret;

        return ret; 
    }

    // Low level function called by PD-JSON for peek.
    int32_t Config_Peek(void* user)
    {
        int32_t ret = 0x00;

        config_stream->file >> ret;
        config_stream->file.seekg(-sizeof(int32_t), std::ios_base::cur);

        return ret; 
    }


    const char* Config_GetString(const char* key)
    {
        std::unordered_map<const char*, const char*>::const_iterator iter = config.find(key);

        if (iter == config.end())
            return nullptr; 
        else
            return iter->first;
    }

    uint32_t Config_GetInt32(const char* key)
    {
        std::unordered_map<const char*, const char*>::const_iterator iter = config.find(key);

        if (iter == config.end())
            return 0; 
        else
            return atoi(iter->first);       
    }

    bool Config_GetBool(const char* key)
    {
        std::unordered_map<const char*, const char*>::const_iterator iter = config.find(key);

        if (iter == config.end())
            return false; 
        else
        {
            // the lowercase strings of "true" and "false"are used
            if (!strncmp(iter->first, "true", MAX_STRING_LENGTH))
                return true;
            else
                return false; 
        }
    }
    
    double Config_GetDouble(const char* key)
    {
        std::unordered_map<const char*, const char*>::const_iterator iter = config.find(key);

        if (iter == config.end())
            return 0.00f; 
        else
            return atof(iter->first);
    }

    
    void Config_Shutdown()
    {
        
    }
}
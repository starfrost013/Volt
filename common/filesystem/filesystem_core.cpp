//
// VOLT
// Copyright © 2024-2025 Connor Hyde
//

#include <common/filesystem/filesystem.hpp>
#include <fstream>

namespace Volt
{
    // Convars
    Cvar* fs_type;
    Cvar* fs_basedir;

    // Globals
    bool fs_initialised = false;
    VoltFilesystem* filesystem;

    // NOT thread safe...Default buffer to read strings into.
    char fs_string_buf[FS_MAX_STRING_LENGTH] = {0}; 

    void Filesystem_Init(const char* filesystem_path)
    {
        // the type for now

        fs_type = Cvar_Get("type", "0", true);
        fs_basedir = Cvar_Get("fs_basedir", ".", true);

        filesystem = Memory_Alloc<VoltFilesystem>(TAG_FILESYSTEM);

        filesystem->type = static_cast<VoltFilesystemType>(fs_type->value);

        if (filesystem->type == VoltFilesystemType::FS_Folder)
            Logging_LogChannel("Filesystem Init: Type = FS_Folder", LogChannel::Message);

        fs_initialised = true;
    }
    
    void Filesystem_Shutdown()
    {
        if (filesystem->head)
        {
            // walk through the linked list of open files and close them all
            VoltFile* entry = filesystem->head;
            
            while (entry->next) 
            {
                entry->file.close();

                VoltFile* to_be_freed = entry; 

                entry = entry->next;
                
                Memory_Free<VoltFile>(to_be_freed);
            }

            filesystem->head = filesystem->tail = nullptr;
        }

        memset(&filesystem, 0x00, sizeof(filesystem));

        fs_initialised = false; 
    }


    // File open/close functions

    // Open a file and, if the cached parameter is true, optionally cache it/
    //
    VoltFile* Filesystem_OpenFile(const char* path, VoltFileMode mode, bool cached)
    {
        // allocate a new open file entry
        VoltFile* entry = Memory_Alloc<VoltFile>(TAG_FILESYSTEM);
        entry->mode = mode; 
        
        strncpy(entry->path, path, FS_MAX_PATH);

        char real_path[FS_MAX_PATH] = {0};
        
        // Determine what to do on the basis of the filesystem type
        snprintf(real_path, FS_MAX_PATH, "%s/%s", fs_basedir->string, path);    

        if (filesystem->type == VoltFilesystemType::FS_Folder)
        {
            // open the file in binary mode, check if it works
            std::ios_base::openmode mode_stream = std::ios::out | std::ios::in;

            if (mode == VoltFileMode::Binary)
                mode_stream |= std::ios::binary;

            entry->file.open(real_path, mode_stream);

            if (!entry->file.good())
            {
                Logging_LogChannel("Failed to open file %s using FS_Folder method\n", LogChannel::Warning, path);
                Memory_Free<VoltFile>(entry);
                return nullptr; 
            }   

            // turn off whitespace skip
            entry->file >> std::noskipws;
        }

        // set up the first entry
        if (!filesystem->head)
        {
            filesystem->head = filesystem->tail = entry;
            // Since we use calloc which automatically clears to zero we don't need to explicitly set the prev/next pointers
        } 
        else
        {
            VoltFile* prev = filesystem->tail;
            filesystem->tail->next = entry;
            filesystem->tail = entry; 
            entry->next = nullptr; 
            entry->prev = prev;
        }

        Logging_LogChannel("Opened file %s", LogChannel::Debug, path);

        return entry;
    }

    //
    // Closes a file that the game has already opened.,
    //
    void Filesystem_CloseFile(VoltFile* entry)
    {
        assert(entry);

        if (static_cast<int32_t>(fs_type->value) == VoltFilesystemType::FS_Folder)
        {
            entry->file.close();
        }

        // free the entry, first check if its the only entry
        if (entry == filesystem->head 
        && filesystem->head == filesystem->tail)
        {
            filesystem->head = filesystem->tail = nullptr;
        }

        // now check if it is the head/tail to keep the GameImage structures consistent
        if (entry == filesystem->tail)   
        {
            filesystem->tail = entry->prev; 
            filesystem->tail->next = nullptr;
        }
        else if (entry == filesystem->head)
        {
            filesystem->head = entry->next; 
            filesystem->head->prev = nullptr; 
        }
        
        // we aren't the head of the tail, so just make sure the linked list is consistent
        if (entry->prev && entry->next)
        {
            entry->prev->next = entry->next;
            entry->next->prev = entry->prev; 
        }

        Logging_LogChannel("Closed file %s", LogChannel::Debug, entry->path);

        Memory_Free<VoltFile>(entry);
    }


    void Filesystem_ReadString(VoltFile* entry, char* fs_buf, uint32_t n)
    {
        // fs_buf DEFAULT is fs_string_buf (NOT compatible with MULTI-THREADING!!!!!)

        // THIS CODE SUCKS. MAKE IT USE GAMEIMAGEFILEENTRY OVERLOADS!

        // Pascal styled-strings are used here in order to get around the issue of 
        // not knowing the length of what we're going to read.
        // Also, we can't do operator overrides on pointers.
        std::fstream& stream = entry->file;

        uint32_t length = strlen(fs_buf);

        // terminate the string
        if (fs_buf[0] != '\0'
            && length < n)
        {
            //clear the previous string that was there
            memset(fs_buf, 0x00, length);
        }
            

        if (entry->mode == VoltFileMode::Binary)
            stream >> n;
    
        char next_str_byte = 0x00; 
        
        bool line_done = false; 

        for (uint32_t byte = 0; byte < n; byte++)
        {
            // check if we are about to overflow
            if (byte >= n)
                break;
                
            stream >> fs_buf[byte];

            if (stream.eof())
            {
                entry->eof = true;
                return;
            }

            // if we reached a new line and it is text mode, end
            if (entry->mode == Text)
            {
                if (fs_buf[byte] == '\r'
                || fs_buf[byte] == '\n')
                    break;
            }
        }

        // we somehow reached the end of the buffer without ending the string. whoops
         
    }

    void Filesystem_WriteString(VoltFile* entry, char* string)
    {
        if (!string)
            return; 

        uint32_t length = ARRAY_SIZE(string);                  // ONLY valid for SINGLE-BYTE ENCODING!

        if (entry->mode == VoltFileMode::Binary)
            entry->file << length;

        // blast the whole string at once for efficiency reasons
        entry->file.write(string, length);
    }   

    // Write to the filesystem.

    void Filesystem_Flush(VoltFile* entry)
    {
        assert(entry);

        entry->file.flush();
    }


}
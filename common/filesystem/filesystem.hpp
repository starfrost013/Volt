#pragma once
#include <common/common.hpp>

//
// filesystem.hpp
// Filesystem & Game Image Loading (7 March 2025)
//

/*
    This is a filesystem component intended to be able to abstract away different
    sources of file loading and also to allow commonly used files to be cached
    in memory for faster access.

    It uses c++ streams and memory mapped files that are nicely abstracted behind
    a set of clean APIs
*/
namespace Volt
{
    // Defines 

    // Increment when there is a breaking change
    #define FS_HEADER_VERSION               1   
    #define FS_MAX_PATH                     260 // for windows
    #define FS_MAX_STRING_LENGTH            256


    // Structures
    enum FilesystemType
    {
        // Filesystem mounted from a folder.
        FS_Folder = 0,
    };

    enum FileMode
    {
        FileMode_Binary = 0,

        FileMode_Text = 1,
    };

    /* Game image file header */
    struct GameImageFileHeader
    {
        // the version of the image file
        uint8_t version;

        // the engine version that was used to create this game image file
        uint8_t engine_version_major;
        uint8_t engine_version_minor;
        uint8_t engine_version_revision;
    }; 

    //
    // GameImageFileEntry
    //
    // Defines an open file in the game.
    //
    struct GameImageFileEntry 
    {
        uint32_t length;
        uint32_t start_position;                // Irrelevant if type == FS_GameImage
        FileMode mode;                          // the mode to open the file in
        char path[FS_MAX_PATH];                 // the path to the file relative to fs_basedir
        bool eof;                               // Irrelevant if type == FS_Folder
        std::fstream file;                      // Irrelevant if type == FS_GameImage
        GameImageFileEntry* prev; 
        GameImageFileEntry* next; 

        template <typename T>
        std::ostream& operator<<(T const &p)
        {
            return this->file << p; 

            if (this->file.eof())
                this->eof = true; 
        }

        template <typename T>
        std::ostream& operator>>(T const &p)
        {
            return this->file >> p; 
        }

    };

    // Game Image 
    struct GameImage
    {
        FilesystemType type;                 
        GameImageFileHeader header;             // Irrelevant if type == FS_Folder
        GameImageFileEntry* head;               // Irrelevant if type == FS_Folder
        GameImageFileEntry* tail;               // Irrelevant if type == FS_Folder
        std::fstream stream;
        bool loaded; 
    };

    // Convars
    extern Cvar* fs_type;
    extern Cvar* fs_basedir;

    // Globals
    extern bool fs_initialised;
    extern GameImage* filesystem;

    extern char fs_string_buf[FS_MAX_STRING_LENGTH]; 

    // Functions


    void Filesystem_Init(const char* filesystem_path = nullptr);
    void Filesystem_Shutdown();

    // File open/close functions
    GameImageFileEntry* Filesystem_OpenFile(const char* path, FileMode mode = FileMode_Binary, bool cached = false);
    void Filesystem_CloseFile(GameImageFileEntry* entry);

    // File read/write functions
    // For most functions, we can use the contents of the stream in order to read/write

    // READ CV1
    void Filesystem_ReadString(GameImageFileEntry* entry, char* buf = fs_string_buf, uint32_t n = FS_MAX_STRING_LENGTH);
    void Filesystem_WriteString(GameImageFileEntry* entry, char* string);
}

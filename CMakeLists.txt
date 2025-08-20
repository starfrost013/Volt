# Volt PC Emulator
# Copyright     © 2025 starfrost
# Portions      © 2023-2025 starfrost

cmake_minimum_required (VERSION 3.8)

project ("Volt")

# Add source to this project's executable.
add_executable (Volt  

# EXTERNAL
"external/glad/src/glad.c"

# CORE
"Volt.cpp"

# COMMON
"common/common.cpp" 
"common/memory/memory_alloc.cpp"
"common/cvar/cvar.cpp" 
"common/command/command.cpp"
"common/command/commands_basic.cpp" "common/command/commands_exec.cpp" "common/command/commands_memory.cpp"
"common/cmdline/cmdline.cpp"
"common/console/console.cpp"
"common/net/net_core.cpp" "common/net/types/net_loopback.cpp"
"common/filesystem/filesystem.hpp" "common/filesystem/filesystem_core.cpp"

# UTIL
"util/util.hpp" "util/util.cpp" 
"util/math/math.hpp" "util/math/math.cpp"
"util/logging/logging.cpp" 
"util/console/console.cpp"

# EMULATION

# Emulation Core
"emulation/emulation.cpp"
"emulation/components/component.cpp"
"emulation/machine/machine.cpp"

# Emulation Components 

# Storage
"emulation/components/storage/storage_address_space.cpp"

# CPU

# 8086
"emulation/components/cpu/cpu8086_core.cpp" 
"emulation/components/cpu/cpu8086_modrm.cpp"
"emulation/components/cpu/cpu8086_ops_control.cpp" 
"emulation/components/cpu/cpu8086_ops_flag.cpp"
"emulation/components/cpu/cpu8086_ops_group.cpp"
"emulation/components/cpu/cpu8086_ops_math.cpp"
"emulation/components/cpu/cpu8086_ops_move.cpp"
"emulation/components/cpu/cpu8086_ops_prefix.cpp"

# Disassembly system
"emulation/disasm/disasm.cpp"

# RENDER
"render/render_core.cpp"

# RENDERER - OPENGL 3
"render/gl3/gl3_core.cpp"

# RENDERER - NULL
"render/null/null_core.cpp"

# SYS
)

# Base include directories
include_directories(".")
# Required for glad for now
include_directories("external/glad/include")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  target_compile_definitions(Volt PRIVATE DEBUG)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  target_compile_definitions(Volt PRIVATE RELEASE)
endif()

# Libraries and compiler specific stuff

add_library(glfw3 STATIC IMPORTED)

if (WIN32)
  include_directories("external/glfw/win64/include")

  if (MSVC)
    message(FATAL_ERROR "MSVC IS DEPRECATED!")
  else()
    # glfw3 - win64
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/glfw/win64/lib-mingw-w64/libglfw3.a)

    target_link_libraries(Volt glfw3)
  endif()
else()
  message(FATAL_ERROR "Linux not supported yet...:(")
endif()


# Build system-specific files
if (WIN32)
	target_sources(Volt PUBLIC "sys/windows/sys_win32.cpp")
else()
	target_sources(Volt PUBLIC "sys/linux/sys_linux.cpp")
endif()


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Volt PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.f
 
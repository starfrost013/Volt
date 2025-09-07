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
"emulation/emulation_commands.cpp"

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
"emulation/components/cpu/cpu8086_ops_io.cpp"
"emulation/components/cpu/cpu8086_ops_math.cpp"
"emulation/components/cpu/cpu8086_ops_move.cpp"
"emulation/components/cpu/cpu8086_ops_prefix.cpp"
"emulation/components/cpu/cpu8086_ops_stack.cpp"
"emulation/components/cpu/cpu8086_ops_string.cpp"

# Disassembly system
"emulation/disasm/disasm.cpp"

# IO
"emulation/components/io/io_port.cpp"

# Support components
"emulation/components/support/i8237_dma.cpp"
"emulation/components/support/i8253_pit.cpp"
"emulation/components/support/i8255_ppi.cpp"
"emulation/components/support/i8259_pic.cpp"

# Video
"emulation/components/video/video_mda.cpp"

# RENDER
"render/render_core.cpp"
"render/shader/render_shader_manager.cpp"

# RENDERER - OPENGL 3
"render/gl4/gl4_core.cpp"

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
add_library(squirrel STATIC IMPORTED)
add_library(sqstdlib STATIC IMPORTED)

# most platforms don't use extensions for binaries
set(FINAL_BINARY_NAME "${CMAKE_PROJECT_NAME}")

if (WIN32)
  include_directories("external/glfw/win64/include")
  set(FINAL_BINARY_NAME "${CMAKE_PROJECT_NAME}.exe")
  if (MSVC)
    message(WARNING "MSVC is currently not supported, therefore compilation is likely to fail.")
  else()
    # glfw3 - win64
    set_target_properties(glfw3 PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/glfw/win64/lib-mingw-w64/libglfw3.a)
    set_target_properties(squirrel PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/squirrel/win64/libsquirrel_static.a)
    set_target_properties(sqstdlib PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/external/squirrel/win64/libsqstdlib_static.a)

    target_link_libraries(Volt glfw3)
    target_link_libraries(Volt squirrel)
    target_link_libraries(Volt sqstdlib)

  endif()
else()
  message(WARNING "Linux not supported yet...:(")
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

add_custom_command(TARGET Volt POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${CMAKE_BINARY_DIR}/${FINAL_BINARY_NAME}
  ${CMAKE_BINARY_DIR}/out/${FINAL_BINARY_NAME}
  )

add_custom_command(TARGET Volt POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/assets
  ${CMAKE_BINARY_DIR}/out
)

cmake_minimum_required(VERSION 3.0.0)

# Define LINUX
if (UNIX AND NOT APPLE)
    set(LINUX 1)
endif()

# Setup modules path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# Project name
project(tddod)

# Some compiler flags
# set(CMAKE_CXX_STANDARD 17) # C++17
# if (MSVC)
#     SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" "/MP") # Multi core in VS
# endif()

# Define _DEBUG
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-D_DEBUG)
endif()

#justwindowsthings
if (WIN32)
    add_definitions(-DNOMINMAX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Project files
file(GLOB srcfiles ./src/*.*)
file(GLOB componentsfiles ./src/components/*.*)
file(GLOB systemsfiles ./src/systems/*.*)
file(GLOB helpersfiles ./src/helpers/*.*)

list(APPEND includes PUBLIC ./src/)
# list(APPEND includes PUBLIC ./src/components/)
# list(APPEND includes PUBLIC ./src/systems/)

#------------------------------------------------------------------------------
# Third parties
#------------------------------------------------------------------------------

# OpenGL
find_package(OpenGL REQUIRED)
list(APPEND includes PUBLIC ${OPENGL_INCLUDE_DIR})
list(APPEND libs ${OPENGL_LIBRARIES})

# SDL2
set(HAVE_LIBC ON)
add_subdirectory(./thirdparty/SDL-mirror/)
list(APPEND libs SDL2-static)
list(APPEND includes PUBLIC ./thirdparty/SDL-mirror/include/)

# gl3w
list(APPEND includes PUBLIC ./thirdparty/gl3w/include/)
list(APPEND srcthirdparty ./thirdparty/gl3w/src/gl3w.c)

# EnTT
list(APPEND includes PUBLIC ./thirdparty/entt/src/)

# stb
list(APPEND includes PUBLIC ./thirdparty/stb/)

#------------------------------------------------------------------------------
# Assets
#------------------------------------------------------------------------------

file(GLOB files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/*.*")
foreach(file ${files})
    message("configuring ${file}")
    configure_file(${file} ${CMAKE_CURRENT_BINARY_DIR}/${file} COPYONLY)
endforeach()

#------------------------------------------------------------------------------
# Exe
#------------------------------------------------------------------------------

# tddod.exe, use WinMain on Windows
source_group("thirdparty" FILES ${srcthirdparty})
source_group("game" FILES ${srcfiles})
source_group("components" FILES ${componentsfiles})
source_group("systems" FILES ${systemsfiles})
source_group("helpers" FILES ${helpersfiles})
add_executable(tddod WIN32 ${srcfiles} ${componentsfiles} ${systemsfiles} ${helpersfiles} ${srcthirdparty})

# Work dir
set_property(TARGET tddod PROPERTY
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/")

# Lib/Headers
target_include_directories(tddod ${includes})
target_link_libraries(tddod ${libs})

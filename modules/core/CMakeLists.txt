# The core of the StrikeBox emulator.
# -------------------------------------------------------------------------------
# MIT License
# 
# Copyright (c) 2019 Ivan Roberto de Oliveira
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
project(strikebox-core VERSION 1.0.0 LANGUAGES CXX)

include(ExternalProject)

##############################
# Source files
#
if(WIN32)
    set(platform_path windows)
elseif(LINUX)
    set(platform_path linux)
elseif(APPLE)
    set(platform_path darwin)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

file(GLOB_RECURSE sources
    src/common/*.cpp
    src/${platform_path}/*.cpp
    src/pch.cpp
)

file(GLOB_RECURSE private_headers
    src/common/*.hpp
    src/common/*.h
    src/${platform_path}/*.hpp
    src/${platform_path}/*.h
    src/pch.hpp
)

file(GLOB_RECURSE public_headers
    include/*.hpp
    include/*.h
)

##############################
# Project structure
#

add_library(strikebox-core OBJECT ${sources} ${private_headers} ${public_headers})

set_target_properties(strikebox-core PROPERTIES DEBUG_POSTFIX "-debug")

target_include_directories(strikebox-core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/common
        ${CMAKE_CURRENT_SOURCE_DIR}/src/${platform_path}
)

# Include virt86
find_package(virt86 CONFIG REQUIRED)
target_link_libraries(strikebox-core PUBLIC virt86::virt86)

# TODO: Include Zydis
# add_definitions(-DZYDIS_STATIC_DEFINE)
# ExternalProject_Add(
#     Zydis
#     GIT_REPOSITORY "https://github.com/zyantific/zydis"
#     GIT_TAG "v2.0.0"
#     UPDATE_COMMAND ""
#     INSTALL_COMMAND "")
# ExternalProject_Get_Property(Zydis install_dir)
# target_include_directories(strikebox-core PUBLIC ${install_dir}/src/Zydis/include ${install_dir}/src/Zydis-build)
# add_dependencies(strikebox-core Zydis)
# 
# if(WIN32)
#     target_link_libraries(strikebox-core PUBLIC ${install_dir}/src/Zydis-build/${CMAKE_CFG_INTDIR}/Zydis.lib)
# else()
#     target_link_libraries(strikebox-core PUBLIC ${install_dir}/src/Zydis-build/libZydis.a)
# endif()
# 
# target_include_directories(strikebox-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${install_dir}/src/Zydis/include)

if(MSVC)
    add_precompiled_header(strikebox-core pch.hpp SOURCE_CXX "${CMAKE_CURRENT_SOURCE_DIR}/src/common/pch.cpp" FORCEINCLUDE)

    vs_set_filters(BASE_DIR src FILTER_ROOT "Sources" SOURCES ${sources})
    vs_set_filters(BASE_DIR src FILTER_ROOT "Private Headers" SOURCES ${private_headers})
    vs_set_filters(BASE_DIR include FILTER_ROOT "Public Headers" SOURCES ${public_headers})

    vs_use_edit_and_continue()

    set_target_properties(strikebox-core PROPERTIES FOLDER Libraries)
else()
    add_precompiled_header(strikebox-core src/common/pch.hpp PCH_PATH pch.hpp SOURCE_CXX "${CMAKE_CURRENT_SOURCE_DIR}/src/common/pch.cpp" FORCEINCLUDE)
endif()

##############################
# Installation
#

# Configuration
set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
set(namespace "strikebox::")

# Include module with function 'write_basic_package_version_file'
include(CMakePackageConfigHelpers)

# Configure '<PROJECT-NAME>ConfigVersion.cmake'
# Use:
#   * PROJECT_VERSION
write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)

# Configure '<PROJECT-NAME>Config.cmake'
# Use variables:
#   * TARGETS_EXPORT_NAME
#   * PROJECT_NAME
configure_package_config_file(
    "cmake/Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

# Install target library
install(TARGETS strikebox-core
    EXPORT "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

# Install public headers
install(DIRECTORY "include/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

# Install CMake Config modules
#   * <prefix>/lib/cmake/<PROJECT-NAME>/<PROJECT-NAME>Config.cmake
#   * <prefix>/lib/cmake/<PROJECT-NAME>/<PROJECT-NAME>ConfigVersion.cmake
install(FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

# Install CMake Target module
#   * <prefix>/lib/cmake/<PROJECT-NAME>/<PROJECT-NAME>Targets.cmake
install(EXPORT "${TARGETS_EXPORT_NAME}"
    NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)

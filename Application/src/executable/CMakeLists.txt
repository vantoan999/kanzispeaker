cmake_minimum_required(VERSION 3.5.1)
project(KanziSpeaker_executable)

if(MSVC)
    option(BUILD_SHARED_LIBS "Selects whether to build and link to shared or static libraries" ON)
endif()

if(NOT KANZI_ENGINE_BUILD)
    find_package(Kanzi REQUIRED CONFIG CMAKE_FIND_ROOT_PATH_BOTH
        HINTS "$ENV{KANZI_HOME}/Engine/lib/cmake")
endif()

include(kanzi-common)

include_directories(${PROJECT_SOURCE_DIR}/../../lib/VLC/sdk/include)

add_library(vlc SHARED IMPORTED)
set_target_properties(vlc PROPERTIES 
    IMPORTED_LOCATION D:/KanziWorkspace/KanziWorkspace_3_9_6_453/Projects/KanziSpeaker/Application/lib/VLC/libvlc.dll
    IMPORTED_IMPLIB ${PROJECT_SOURCE_DIR}/../../lib/VLC/sdk/lib/libvlc.lib)

add_executable(KanziSpeaker_executable src/kanzispeaker_main.cpp)

if(ANDROID)
    target_link_libraries(KanziSpeaker_executable -Wl,--whole-archive Kanzi::kzappfw -Wl,--no-whole-archive)
else()
    target_link_libraries(KanziSpeaker_executable Kanzi::kzappfw)
endif()

target_link_libraries(KanziSpeaker_executable Kanzi::kzui Kanzi::kzcoreui KanziSpeaker vlc)

# KANZI_LINK_FREETYPE and KANZI_LINK_ITYPE options determine which font backends are explicitly linked to your application. 
# By default including Kanzi config sets the following:
#  - KANZI_LINK_FREETYPE=ON in static build.
#  - Both OFF in dynamic build.
#
# In case you are working with dynamic libraries, Kanzi engine can load available font engine backend at runtime.
# Use FontEngine application configuration to choose which font engine backend Kanzi loads: 
# FontEngine=freetype|itype|none
target_link_font_engine_backends(KanziSpeaker_executable)

# [CodeBehind libs start]. Do not remove this identifier.
set(KanziSpeaker_codebehind_lib_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../Tool_project/CodeBehind/KanziSpeaker")
if(EXISTS "${KanziSpeaker_codebehind_lib_dir}")
    add_subdirectory("${KanziSpeaker_codebehind_lib_dir}" "${CMAKE_CURRENT_BINARY_DIR}/CodeBehind/KanziSpeaker")
    include_directories(${KanziSpeaker_codebehind_lib_dir}/include)
    target_link_libraries(KanziSpeaker_executable KanziSpeaker_CodeBehind)
endif()
# [CodeBehind libs end]. Do not remove this identifier.

set_target_properties(KanziSpeaker_executable PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/../../bin")
set_target_properties(KanziSpeaker_executable PROPERTIES VS_DEBUGGER_ENVIRONMENT "${KANZI_VS_DEBUGGER_ENVIRONMENT}")


#set_target_properties(vlc PROPERTIES
#    IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/../../lib/VLC/libvlc.dll
#    IMPORTED_IMPLIB ${PROJECT_SOURCE_DIR}/../../lib/VLC/sdk/libvlc.lib
#)

install_kanzi_libs_to_output_directory()
install_kzbs_to_output_directory(${CMAKE_SOURCE_DIR}/bin)
install_target_to_output_directory(KanziSpeaker_executable)

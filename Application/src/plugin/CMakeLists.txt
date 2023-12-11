cmake_minimum_required(VERSION 3.5.1)
project(KanziSpeaker_plugin)

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
    IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/../../lib/VLC/libvlc.dll
    IMPORTED_IMPLIB ${PROJECT_SOURCE_DIR}/../../lib/VLC/sdk/lib/libvlc.lib)

set(sources
    src/kanzispeaker.cpp
    src/kanzispeaker.hpp
    src/kanzispeaker.rc
    src/kanzispeaker_module.cpp
    src/kanzispeaker_module.hpp
    src/ffmpeg_manager.hpp
	src/ffmpeg_manager.cpp)

add_library(KanziSpeaker ${sources})
target_link_libraries(KanziSpeaker PUBLIC Kanzi::kzcore Kanzi::kzcoreui Kanzi::kzui vlc)

target_include_directories(KanziSpeaker PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)

if(BUILD_SHARED_LIBS AND MSVC)
    target_compile_definitions(KanziSpeaker PRIVATE "KANZISPEAKER_API=__declspec(dllexport)")
    target_compile_definitions(KanziSpeaker PRIVATE "KANZISPEAKER_API_EXPORT")
    target_compile_definitions(KanziSpeaker INTERFACE "KANZISPEAKER_API=__declspec(dllimport)")
else()
    target_compile_definitions(KanziSpeaker PUBLIC "KANZISPEAKER_API=")
endif()
target_compile_definitions(KanziSpeaker PRIVATE -Dssize_t=intptr_t)

# Copy plugin dll for the Kanzi-studio project.
if(BUILD_SHARED_LIBS AND MSVC)
    set(KANZI_PROJECT_PLUGIN_PATH "${PROJECT_SOURCE_DIR}/../../lib/${PLATFORM_TARGET}/${MSVC_TAG}_$<CONFIG>_DLL")
    add_custom_command(TARGET KanziSpeaker POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${KANZI_PROJECT_PLUGIN_PATH}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:KanziSpeaker>" "${KANZI_PROJECT_PLUGIN_PATH}")
endif()

install_target_to_output_directory(KanziSpeaker)

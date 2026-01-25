cmake_minimum_required(VERSION 3.14)
project(pawns)

# Set normal compiler flags
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated -Wall")

# Set versions of dependencies
set(PDC_VERSION 3.9)

# Include FetchContent
include(FetchContent)

message(STATUS "Using PDCurses version ${PDC_VERSION}")

# Fetch PDCurses (https://github.com/wmcbrine/PDCurses/archive/refs/tags/${PDC_VERSION}.tar.gz)

# Options to control fetching/building PDCurses
option(PDC_USE_SYSTEM "Use system-installed curses (skip fetching PDCurses)" OFF)
if(WIN32)
    set(DEFAULT_PDC_PORT "wincon")
else()
    set(DEFAULT_PDC_PORT "x11")
endif()
set(PDC_PORT "${DEFAULT_PDC_PORT}" CACHE STRING "PDCurses build port (e.g. x11, wincon, sdl2, dos, os2)")
set_property(CACHE PDC_PORT PROPERTY STRINGS x11 wincon sdl1 sdl2 dos os2)
option(PDC_BUILD_SHARED "Build PDCurses as shared library (if supported by port)" OFF)

if(PDC_USE_SYSTEM)
    message(STATUS "Using system curses (PDC_USE_SYSTEM=ON)")
else()
    include(ExternalProject)

    FetchContent_Declare(
        PDCurses
        URL https://github.com/wmcbrine/PDCurses/archive/refs/tags/${PDC_VERSION}.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(PDCurses)

    set(PDC_SOURCE_DIR "${pdcurses_SOURCE_DIR}")
    message(STATUS "Fetched PDCurses into ${PDC_SOURCE_DIR}; building port: ${PDC_PORT}")

    set(PDC_MAKE_FLAGS "")
    if(PDC_BUILD_SHARED)
        set(PDC_MAKE_FLAGS "DLL=Y")
    endif()

    # Write a small CMake script to be invoked after the build to copy the
    # produced library into a stable path inside the build dir.
    set(PDC_COPY_SCRIPT "${CMAKE_BINARY_DIR}/pdc_copy_pdcurses.cmake")
    file(WRITE "${PDC_COPY_SCRIPT}"
"file(MAKE_DIRECTORY \"${CMAKE_BINARY_DIR}/pdc_build/lib\")\n"
"set(_src_dir \"${PDC_SOURCE_DIR}/${PDC_PORT}\")\n"
"if(EXISTS \"${PDC_SOURCE_DIR}/${PDC_PORT}/libXCurses.a\")\n"
"  file(COPY \"${PDC_SOURCE_DIR}/${PDC_PORT}/libXCurses.a\" DESTINATION \"${CMAKE_BINARY_DIR}/pdc_build/lib\")\n"
"  file(RENAME \"${CMAKE_BINARY_DIR}/pdc_build/lib/libXCurses.a\" \"${CMAKE_BINARY_DIR}/pdc_build/lib/pdcurses.a\")\n"
"  message(STATUS \"Copied PDCurses library: libXCurses.a\")\n"
"elseif(EXISTS \"${PDC_SOURCE_DIR}/${PDC_PORT}/pdcurses.a\")\n"
"  file(COPY \"${PDC_SOURCE_DIR}/${PDC_PORT}/pdcurses.a\" DESTINATION \"${CMAKE_BINARY_DIR}/pdc_build/lib\")\n"
"  message(STATUS \"Copied PDCurses library: pdcurses.a\")\n"
"elseif(EXISTS \"${PDC_SOURCE_DIR}/${PDC_PORT}/libpdcurses.a\")\n"
"  file(COPY \"${PDC_SOURCE_DIR}/${PDC_PORT}/libpdcurses.a\" DESTINATION \"${CMAKE_BINARY_DIR}/pdc_build/lib\")\n"
"  file(RENAME \"${CMAKE_BINARY_DIR}/pdc_build/lib/libpdcurses.a\" \"${CMAKE_BINARY_DIR}/pdc_build/lib/pdcurses.a\")\n"
"  message(STATUS \"Copied PDCurses library: libpdcurses.a\")\n"
"elseif(EXISTS \"${PDC_SOURCE_DIR}/${PDC_PORT}/libXCurses.so\")\n"
"  file(COPY \"${PDC_SOURCE_DIR}/${PDC_PORT}/libXCurses.so\" DESTINATION \"${CMAKE_BINARY_DIR}/pdc_build/lib\")\n"
"  file(RENAME \"${CMAKE_BINARY_DIR}/pdc_build/lib/libXCurses.so\" \"${CMAKE_BINARY_DIR}/pdc_build/lib/pdcurses.a\")\n"
"  message(STATUS \"Copied PDCurses library: libXCurses.so\")\n"
"elseif(EXISTS \"${PDC_SOURCE_DIR}/${PDC_PORT}/libpdcurses.so\")\n"
"  file(COPY \"${PDC_SOURCE_DIR}/${PDC_PORT}/libpdcurses.so\" DESTINATION \"${CMAKE_BINARY_DIR}/pdc_build/lib\")\n"
"  file(RENAME \"${CMAKE_BINARY_DIR}/pdc_build/lib/libpdcurses.so\" \"${CMAKE_BINARY_DIR}/pdc_build/lib/pdcurses.a\")\n"
"  message(STATUS \"Copied PDCurses library: libpdcurses.so\")\n"
"elseif(EXISTS \"${PDC_SOURCE_DIR}/${PDC_PORT}/pdcurses.dll\")\n"
"  file(COPY \"${PDC_SOURCE_DIR}/${PDC_PORT}/pdcurses.dll\" DESTINATION \"${CMAKE_BINARY_DIR}/pdc_build/lib\")\n"
"  file(RENAME \"${CMAKE_BINARY_DIR}/pdc_build/lib/pdcurses.dll\" \"${CMAKE_BINARY_DIR}/pdc_build/lib/pdcurses.a\")\n"
"  message(STATUS \"Copied PDCurses library: pdcurses.dll\")\n"
"else()\n"
"  message(FATAL_ERROR \"PDCurses library not found in ${PDC_SOURCE_DIR}/${PDC_PORT}\")\n"
"endif()\n"
    )

    ExternalProject_Add(pdcurses-build
        SOURCE_DIR "${PDC_SOURCE_DIR}"
        CONFIGURE_COMMAND ""
        BUILD_IN_SOURCE 1
        # Use a small shell snippet to run ./configure when present, then make
        BUILD_COMMAND /bin/sh -c "cd '${PDC_SOURCE_DIR}/${PDC_PORT}' && [ -x ./configure ] && ./configure || true && (${CMAKE_MAKE_PROGRAM} -j1 libXCurses.a || ${CMAKE_MAKE_PROGRAM} -j1 pdcurses.a || ${CMAKE_MAKE_PROGRAM} -j1 libpdcurses.a || ${CMAKE_MAKE_PROGRAM} -j1 libXCurses.so || ${CMAKE_MAKE_PROGRAM} -j1 pdcurses.so || ${CMAKE_MAKE_PROGRAM} -j1 all) ${PDC_MAKE_FLAGS}"
        INSTALL_COMMAND ${CMAKE_COMMAND} -P "${PDC_COPY_SCRIPT}"
    )

    # Create an IMPORTED target that other targets can link to
    add_library(pdcurses STATIC IMPORTED GLOBAL)
    set_target_properties(pdcurses PROPERTIES
        IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/pdc_build/lib/pdcurses.a"
        INTERFACE_INCLUDE_DIRECTORIES "${PDC_SOURCE_DIR}"
    )
    add_dependencies(pdcurses pdcurses-build)
endif()

# Build the executable
file(GLOB_RECURSE PROJECT_HEADERS "src/*.h")
file(GLOB_RECURSE PROJECT_SOURCES "src/*.c")

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})

if(PDC_USE_SYSTEM)
    find_package(Curses REQUIRED)
    target_include_directories(${PROJECT_NAME} PRIVATE ${CURSES_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CURSES_LIBRARIES})
else()
    # Ensure the library is built before linking
    target_include_directories(${PROJECT_NAME} PRIVATE ${PDC_SOURCE_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE pdcurses)
    # If the x11 port is selected, attempt to find & link X11-related libraries
    if(PDC_PORT STREQUAL "x11")
        find_library(X11_LIB NAMES X11)
        find_library(XT_LIB NAMES Xt)
        find_library(XAW_LIB NAMES Xaw Xaw3d)
        find_library(XMU_LIB NAMES Xmu)
        find_library(XPM_LIB NAMES Xpm)
        set(PDC_X11_LIBS ${X11_LIB} ${XT_LIB} ${XAW_LIB} ${XMU_LIB} ${XPM_LIB})
        # remove empty entries
        list(FILTER PDC_X11_LIBS EXCLUDE REGEX "^$")
        if(PDC_X11_LIBS)
            target_link_libraries(${PROJECT_NAME} PRIVATE ${PDC_X11_LIBS})
        else()
            message(WARNING "No X11-related libs found; linking may fail for the x11 PDCurses port")
        endif()
    elseif(PDC_PORT STREQUAL "sdl2")
        # Prefer modern SDL2 CMake target if available
        find_package(SDL2 QUIET)
        if(TARGET SDL2::SDL2)
            target_include_directories(${PROJECT_NAME} PRIVATE SDL2::SDL2)
            target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
        else()
            # Fallback to pkg-config detection
            find_package(PkgConfig QUIET)
            if(PkgConfig_FOUND)
                pkg_check_modules(SDL2_PKG sdl2)
                if(SDL2_PKG_FOUND)
                    target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_PKG_INCLUDE_DIRS})
                    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_PKG_LIBRARIES})
                    target_compile_options(${PROJECT_NAME} PRIVATE ${SDL2_PKG_CFLAGS_OTHER})
                else()
                    message(WARNING "SDL2 not found via pkg-config; linking may fail for the sdl2 PDCurses port")
                endif()
            else()
                message(WARNING "PkgConfig not found; SDL2 detection skipped for sdl2 port")
            endif()
        endif()
    endif()
endif()

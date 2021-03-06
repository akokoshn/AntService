cmake_minimum_required ( VERSION 3.4.0 )

project ( AntService CXX )

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

include(ExternalProject)

if (UNIX)
    ExternalProject_Add ( libusb-1.0
        PREFIX libusb-1.0
        GIT_REPOSITORY "https://github.com/libusb/libusb.git"
        UPDATE_COMMAND ""
        PATCH_COMMAND ""
        SOURCE_DIR ${CMAKE_BINARY_DIR}/thirdparty/libusb
        CONFIGURE_COMMAND ./autogen.sh CFLAGS=-fPIC CXXFLAGS=-fPIC --enable-shared
        BUILD_COMMAND make
        BUILD_IN_SOURCE 1
        INSTALL_COMMAND ""
        LOG_DOWNLOAD 1
        LOG_BUILD 1
    )

    set ( LIBUSB_INCLUDE_DIR ${CMAKE_BINARY_DIR}/thirdparty/libusb/libusb )
else (UNIX)
    set ( LIBUSB_INCLUDE_DIR ${CMAKE_BINARY_DIR}/../lib_usb/include )
endif (UNIX)

include_directories (
  include
  API/include
  tools/include
  ${LIBUSB_INCLUDE_DIR}
)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set ( SOURCE_LIB
        API/src/TrainerControl_dll.cpp
        src/AntStick.cpp
        src/AntMessageReader.cpp
        src/AntMessageWriter.cpp
        src/SearchService.cpp
        src/TelemetryServer.cpp
        src/HeartRateMonitor.cpp
        tools/src/Tools.cpp
)

add_library( AntService SHARED ${SOURCE_LIB} )

if (UNIX)
    add_dependencies(AntService libusb-1.0)

    target_link_libraries( AntService
        udev
        ${CMAKE_BINARY_DIR}/thirdparty/libusb/libusb/.libs/libusb-1.0.a
    )
else (UNIX)
    target_link_libraries( AntService ${CMAKE_BINARY_DIR}/../lib_usb/lib/libusb-1.0.lib )
endif (UNIX)

add_subdirectory( tests )
add_subdirectory( samples )

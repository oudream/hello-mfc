cmake_minimum_required(VERSION 3.0)

project(tcp-client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
# 1: static MFC library, 2: shared MFC library
set(CMAKE_MFC_FLAG 2)

#find_package(MFC)
#if(NOT MFC_FOUND)
#    message(FATAL_ERROR "MFC is not found, please check MFC installation")
#endif()

add_executable(tcp-client mfc.cpp)

#target_sources(tcp-client PRIVATE
#        mfc.cpp
#        )
#set_target_properties(tcp-client PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

target_compile_definitions(tcp-client PRIVATE
        -DWIN32
        -D_DEBUG
        -D_WINDOWS
        -D_UNICODE
        -DUNICODE
        -D_AFXDLL
)

target_link_options(tcp-client PRIVATE
        /ENTRY:wWinMainCRTStartup
)

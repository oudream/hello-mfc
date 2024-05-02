cmake_minimum_required(VERSION 3.15)

project(filesystem-read)

# Set the use of MFC
set(CMAKE_MFC_FLAG 1) # 1 for static linking, 2 for dynamic linking

add_executable(filesystem-read WIN32
        main.cpp
        MyMFCApp.rc
)

target_compile_definitions(filesystem-read PRIVATE
        -DWIN32
        -D_DEBUG
        -D_WINDOWS
        -D_UNICODE
        -DUNICODE
        -D_AFXDLL
)

target_link_libraries(filesystem-read PRIVATE
        user32
        gdi32
        ole32
        oleaut32
        uuid
        comdlg32
)

#target_link_options(filesystem-read PRIVATE
#        /ENTRY:wWinMainCRTStartup
#)

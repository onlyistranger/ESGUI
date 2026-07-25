cmake_minimum_required(VERSION 3.16)
project(ESGUI_SDL2_Simulator LANGUAGES C)

# ================================================================
#  SDL2 依赖
# ================================================================
# 支持多种安装方式：
#   vcpkg:  find_package(SDL2 CONFIG REQUIRED)
#   system: find_package(SDL2 REQUIRED)
#   manual: -DSDL2_DIR=<path>
# ================================================================
find_package(SDL2 CONFIG QUIET)
if(NOT SDL2_FOUND)
    find_package(SDL2 REQUIRED)
endif()

# ================================================================
#  ESGUI 源码目录（相对于本 CMakeLists.txt 的上级目录）
# ================================================================
set(ESGUI_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

# ================================================================
#  头文件路径（顺序重要：SDL2/ 必须在最前面，优先找到桩 main.h）
# ================================================================
set(ESGUI_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}          # SDL2 桩 main.h（最高优先级）
    ${ESGUI_ROOT}                        # ESGUI 核心头文件
    ${ESGUI_ROOT}/BSP                    # BSP 头文件
    ${ESGUI_ROOT}/Adapter                # Adapter 头文件
    ${ESGUI_ROOT}/Font                   # 字体头文件
)

# ================================================================
#  ESGUI 核心源文件
# ================================================================
set(ESGUI_CORE_SRCS
    ${ESGUI_ROOT}/ESGUI.c
    ${ESGUI_ROOT}/ESGUI_Event.c
    ${ESGUI_ROOT}/ESGUI_Menu.c
    ${ESGUI_ROOT}/ESGUI_Widget.c
    ${ESGUI_ROOT}/ESGUI_PageDefaltVtbl.c
    ${ESGUI_ROOT}/ESGUI_Anim.c
)

# ================================================================
#  ESGUI BSP 源文件（绘制引擎）
# ================================================================
set(ESGUI_BSP_SRCS
    ${ESGUI_ROOT}/BSP/ESGUI_BSP_Canvas.c
    ${ESGUI_ROOT}/BSP/ESGUI_BSP_draw.c
    ${ESGUI_ROOT}/BSP/ESGUI_BSP_Text.c
    ${ESGUI_ROOT}/BSP/ESGUI_BSP_BMP.c
)

# ================================================================
#  ESGUI Adapter 源文件
# ================================================================
set(ESGUI_ADAPTER_SRCS
    ${ESGUI_ROOT}/Adapter/ESGUI_UseCanvas.c
)

# ================================================================
#  字体
# ================================================================
set(ESGUI_FONT_SRCS
    ${ESGUI_ROOT}/Font/eui_test_font.c
)

# ================================================================
#  SDL2 模拟器入口
# ================================================================
set(SIMULATOR_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
)

# ================================================================
#  可执行目标
# ================================================================
add_executable(esgui_sdl2
    ${ESGUI_CORE_SRCS}
    ${ESGUI_BSP_SRCS}
    ${ESGUI_ADAPTER_SRCS}
    ${ESGUI_FONT_SRCS}
    ${SIMULATOR_SRCS}
)

target_include_directories(esgui_sdl2 PRIVATE ${ESGUI_INCLUDE_DIRS})

# SDL2 链接
if(SDL2_FOUND)
    target_link_libraries(esgui_sdl2 PRIVATE SDL2::SDL2)
else()
    target_include_directories(esgui_sdl2 PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(esgui_sdl2 PRIVATE ${SDL2_LIBRARIES})
endif()

# MinGW: SDL2 默认带 -mwindows 导致 linker 找 WinMain，用 -mconsole 覆盖
if(MINGW)
    target_link_options(esgui_sdl2 PRIVATE -mconsole)
endif()

# Windows 进程常驻内存统计
if(WIN32)
    target_link_libraries(esgui_sdl2 PRIVATE psapi)
endif()

# ================================================================
#  编译选项
# ================================================================
if(MSVC)
    target_compile_options(esgui_sdl2 PRIVATE /W3 /utf-8)
else()
    target_compile_options(esgui_sdl2 PRIVATE -Wall -Wextra)
endif()

# C99 标准（ESGUI 使用 C99 特性：for 循环内声明变量等）
set_target_properties(esgui_sdl2 PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
)

# ================================================================
#  安装（可选）
# ================================================================
install(TARGETS esgui_sdl2 RUNTIME DESTINATION bin)

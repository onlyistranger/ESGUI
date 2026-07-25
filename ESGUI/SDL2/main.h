/**
 * @file main.h
 * @brief SDL2 模拟器桩头文件 —— 替代 STM32 的 main.h
 * 
 * ESGUI 源码中多处 #include "main.h" 以获取 uint8_t / bool 等类型。
 * 在 PC 上编译时，用本桩文件提供等价类型定义，无需修改 ESGUI 源码。
 */

#ifndef __MAIN_H
#define __MAIN_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* GCC weak 属性：用于 ESGUI_UseCanvasFlush 的弱定义 */
#if defined(__GNUC__) || defined(__clang__)
#define __WEAK   __attribute__((weak))
#elif defined(_MSC_VER)
#define __WEAK   __declspec(selectany)
#else
#define __WEAK
#endif

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

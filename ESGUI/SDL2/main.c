/**
 * @file main.c
 * @brief ESGUI SDL2 PC 模拟器
 * 
 * 在 PC 上使用 SDL2 模拟 OLED 128×64 屏幕的 GUI 渲染效果。
 * 复用 ESGUI 全部核心代码（Canvas / Draw / Text / BMP / Anim / Menu），
 * 仅替换底层刷屏函数为 SDL2 纹理渲染。
 *
 * 编译依赖：SDL2 (>= 2.0)
 * 按键映射：
 *   ↑/W      上一项
 *   ↓/S      下一项
 *   ←/A      减少值 / 左移
 *   →/D      增加值 / 右移
 *   Enter    确认
 *   Backspace 返回
 *   Escape   退出
 */

/*
 * SDL_MAIN_HANDLED: 告知 SDL2 不要接管 main 入口。
 * MinGW 下 SDL2 默认使用 WinMain → SDL_main 桥接（-mwindows），
 * 定义此宏后 SDL 直接使用标准 int main(int,char**)。
 */
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <stdio.h>
#include <string.h>

/* ================================================================
 *  桩头文件：ESGUI 源码依赖 "main.h"，需要让编译器先找到我们的桩
 * ================================================================ */
#include "main.h"

/* ================================================================
 *  ESGUI 头文件
 * ================================================================ */
#include "ESGUI.h"
#include "ESGUI_PageDefaltVtbl.h"
#include "ESGUI_UseCanvas.h"

/* ================================================================
 *  BMP 位图数据（精简版，来自 MY_BMP.c）
 * ================================================================ */
#include "ESGUI_BSP_BMP.h"

/* ---------- Computer 32x32 ---------- */
static const uint8_t _bmp_computer[] = {
    0x00,0x00,0x00,0x00,0xFE,0x02,0x02,0xFA,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,
    0x0A,0x0A,0x0A,0x0A,0xFA,0x52,0x22,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x80,0x7F,0x40,0x40,0x5F,0x50,0x50,0x50,0x50,0x50,0x50,0x50,0x50,
    0x50,0x50,0x50,0x50,0x5F,0x40,0x40,0x7F,0x40,0x80,0x80,0x80,0x80,0x00,0x00,0x00,
    0x00,0x7F,0x7F,0x40,0x41,0x40,0x5D,0x55,0x55,0x55,0x54,0x5C,0x40,0x40,0x5E,0x42,
    0x56,0x42,0x5E,0x40,0x5E,0x42,0x52,0x56,0x52,0x42,0x5E,0x40,0x7F,0x10,0xE0,0x00,
    0x00,0x00,0x00,0x00,0x3C,0x52,0x4A,0x43,0x7B,0x5B,0x52,0x5A,0x4A,0x7A,0x5A,0x5A,
    0x52,0x7A,0x5B,0x73,0x76,0x53,0x67,0x52,0x46,0x32,0x1E,0x18,0x04,0x03,0x01,0x00
};
static const Bitmap bmp_Computer32x32 = {32, 32, _bmp_computer};

/* ---------- File 32x32 ---------- */
static const uint8_t _bmp_file[] = {
    0x00,0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,
    0x02,0x02,0x02,0x02,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x80,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
    0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
static const Bitmap bmp_File32x32 = {32, 32, _bmp_file};

/* ---------- MCU 32x32 ---------- */
static const uint8_t _bmp_mcu[] = {
    0xFF,0xFF,0x7F,0x7F,0x0F,0xEF,0xEF,0xE3,0xE8,0xE3,0xEF,0xEF,0xE3,0xE8,0xE3,0xEF,
    0xEF,0xE3,0xE8,0xE3,0xEF,0xEF,0x63,0x68,0x63,0xEF,0xEF,0x0F,0x7F,0x7F,0xFF,0xFF,
    0xDE,0xDE,0x8C,0xAD,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0xFD,0xFC,0xFF,0xFF,0x00,0xAD,0x8C,0xDE,0xDE,
    0x7B,0x7B,0x31,0xB5,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0xB5,0x31,0x7B,0x7B,
    0xFF,0xFF,0xFE,0xFE,0xF0,0xF7,0xF7,0xC7,0x17,0xC7,0xF7,0xF7,0xC7,0x17,0xC7,0xF7,
    0xF7,0xC7,0x17,0xC7,0xF7,0xF7,0xC7,0x17,0xC7,0xF7,0xF7,0xF0,0xFE,0xFE,0xFF,0xFF
};
static const Bitmap bmp_MCU32x32 = {32, 32, _bmp_mcu};

/* ---------- Picture 32x32 ---------- */
static const uint8_t _bmp_picture[] = {
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x7F,0x7F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x1F,0x0F,0x07,0x07,0x07,0x1F,0x3F,0x7F,
    0x7F,0xFF,0xFF,0xFF,0xFF,0xE1,0xC0,0x80,0x80,0xC0,0xE0,0xF0,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0x1F,0x0F,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x01,0x1F,0x1F,0x7F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,
    0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
static const Bitmap bmp_Picture32x32 = {32, 32, _bmp_picture};

/* ---------- Setting 32x32 ---------- */
static const uint8_t _bmp_setting[] = {
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xC7,0x87,0x0F,0x0F,0x1F,0x1F,0x1F,0x03,0x01,
    0x01,0x03,0x1F,0x1F,0x1F,0x0F,0x0F,0x87,0xC7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xEF,0xC7,0xC7,0x83,0x01,0x00,0x00,0x00,0x3C,0x7E,0xFF,0xFF,
    0xFF,0xFF,0x7E,0x3C,0x00,0x00,0x00,0x81,0x83,0xC7,0xC7,0xEF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0x7F,0x7F,0x7F,0xFF,0xFF,0xCF,0xC7,0xE1,0xE0,0xF0,0xF0,0xF0,0xF0,0x80,0x00,
    0x00,0x80,0xF0,0xF0,0xF0,0xF0,0xE0,0xE1,0xC7,0xCF,0xFF,0xFF,0x7F,0x7F,0x7F,0xFF,
    0xFF,0xC0,0xC0,0xC0,0xCF,0xCF,0xCF,0xCF,0xCF,0xCF,0xCF,0xCF,0xCF,0xCF,0xCF,0xCF,
    0xCF,0xCF,0xCF,0xCF,0xCF,0xCF,0xCF,0xCF,0xCF,0xCF,0xCF,0xCF,0xC0,0xC0,0xC0,0xFF
};
static const Bitmap bmp_Setting32x32 = {32, 32, _bmp_setting};

/* ---------- Lightning 8x16 ---------- */
static const uint8_t _bmp_lightning[] = {
    0xFF,0x3F,0x0F,0x01,0x3F,0x3F,0x3F,0x3F,0xFC,0xFC,0xFC,0xFC,0x80,0xE0,0xFC,0xFF
};
static const Bitmap bmp_Lightning8x16 = {8, 16, _bmp_lightning};


/* ================================================================
 *  SDL2 全局变量
 * ================================================================ */
static SDL_Window   *g_window   = NULL;
static SDL_Renderer *g_renderer = NULL;
static SDL_Texture  *g_texture  = NULL;

/* 屏幕参数 */
#define SCREEN_W        128
#define SCREEN_H        64
#define SCREEN_SCALE    4           /* 窗口 = 128×4 × 64×4 = 512×256 */

/* 全屏帧缓冲（PC 内存充足，直接开整屏缓冲） */
static uint8_t  g_fb[SCREEN_W * (SCREEN_H / 8)];  /* page-based: stride=128, pages=8 */
static uint8_t  g_rgb[SCREEN_W * SCREEN_H * 3];    /* RGB888 CPU 缓冲 */

/* ESGUI 核心对象 */
static ESGUI_T          g_ui;
static Canvas           g_canvas;
static CanvasStripIter  g_canvas_it;

/* 测试页面 */
static ESGUI_MenuPage_T     g_text_page;
static ESGUI_MenuPage_T     g_bmp_page;
static ESGUI_PopWindow_T    g_pop_window;

/* 演示变量 */
static bool     g_bool_val   = false;
static uint16_t g_uint16_val = 0;


/* ================================================================
 *  ESGUI_UseCanvasFlush 弱定义覆盖 —— 送屏到 SDL2
 * ================================================================ */

/**
 * @brief 将 page-based 条带转为 RGB888 字节序列写入 g_rgb
 *
 * page-based: buf[page*stride + x] bit n = 像素(x, page*8+n)
 * 输出：每像素 3 字节 R,G,B（无 alpha，彻底避免透明度导致的偏色）
 */
static void strip_to_rgb888(const uint8_t *buf, int buf_w, int buf_h,
                            int buf_x0, int buf_y0)
{
    for (int py = 0; py < buf_h; py++) {
        int screen_y = buf_y0 + py;
        if (screen_y < 0 || screen_y >= SCREEN_H) continue;
        int page = py >> 3;
        int bit  = py & 7;
        uint8_t mask = (uint8_t)(1u << bit);

        for (int px = 0; px < buf_w; px++) {
            int screen_x = buf_x0 + px;
            if (screen_x < 0 || screen_x >= SCREEN_W) continue;
            uint8_t byte = buf[page * buf_w + px];
            uint8_t val = (byte & mask) ? 0xFF : 0x00;
            int idx = (screen_y * SCREEN_W + screen_x) * 3;
            g_rgb[idx + 0] = val;
            g_rgb[idx + 1] = val;
            g_rgb[idx + 2] = val;
        }
    }
}

/**
 * @brief 刷屏回调 —— 覆盖 __WEAK 默认空实现
 */
void ESGUI_UseCanvasFlush(int x0, int y0, int x1, int y1,
                          const uint8_t *buf, void *user)
{
    (void)user;
    if (g_texture == NULL) return;

    int bw = (x1 - x0 + 1);
    int bh = (y1 - y0 + 1);

    strip_to_rgb888(buf, bw, bh, x0, y0);

    SDL_Rect rect = { x0, y0, bw, bh };
    SDL_UpdateTexture(g_texture, &rect,
                      &g_rgb[(y0 * SCREEN_W + x0) * 3],
                      SCREEN_W * 3);
}


/* ================================================================
 *  测试页面定义（复用 ESGUI 默认虚函数表）
 * ================================================================ */

/* -------- 值描述符 -------- */
static uint16_t val_get_permille(void *ctx) {
    if (!ctx) return 0;
    return *(uint16_t*)ctx * 1000 / 100;
}
static uint8_t val_to_string(void *ctx, char *buf, uint16_t size) {
    if (!ctx) return 0;
    int len = snprintf(buf, size, "%d", *(uint16_t*)ctx);
    return (uint8_t)(len > 0 ? len : 0);
}
static bool val_step(void *ctx, int8_t direction) {
    if (!ctx) return false;
    uint16_t *pv = (uint16_t*)ctx;
    if (direction > 0 && *pv < 100) { (*pv)++; return true; }
    if (direction < 0 && *pv > 0)   { (*pv)--; return true; }
    return false;
}
static ESGUI_ValueDesc_T g_value_desc = {
    &g_uint16_val,
    val_get_permille,
    val_to_string,
    val_step
};

/* -------- 弹窗条目 -------- */
static ESGUI_MenuItem_T g_textlist_popup_items[] = {
    {0,0,"wen ben ",NULL,NULL,NULL},
    {0,0,"文本一",NULL,NULL,NULL},
    {0,0,"文本二",NULL,NULL,NULL},
    {0,0,"长文本演示----chang wen ben yan shi",NULL,NULL,NULL},
};

static ESGUI_MenuItem_T g_bmplist_popup_items[] = {
    {0,0,"Computer",(void*)&bmp_Computer32x32,NULL,NULL},
    {0,0,"File",    (void*)&bmp_File32x32,NULL,NULL},
    {0,0,"MCU",     (void*)&bmp_MCU32x32,NULL,NULL},
};

/* -------- 弹窗回调 -------- */
static ESGUI_MenuAction_T on_message_popup(ESGUI_MenuPage_T *page, void *arg) {
    (void)page; (void)arg;
    ESGUI_DefaultMessagePopWindowCreate(&g_pop_window, "TEST MESSAGE\n123", 100, 50, 1);
    return (ESGUI_MenuAction_T){ACT_SHOW_POPUP, &g_pop_window};
}
static ESGUI_MenuAction_T on_bool_popup(ESGUI_MenuPage_T *page, void *arg) {
    (void)page;
    ESGUI_DefaultBoolPopWindowCreate(&g_pop_window, "  Bool Pop\n  Window", 100, 50, arg);
    return (ESGUI_MenuAction_T){ACT_SHOW_POPUP, &g_pop_window};
}
static ESGUI_MenuAction_T on_value_popup(ESGUI_MenuPage_T *page, void *arg) {
    (void)page; (void)arg;
    ESGUI_DefaultValuePopWindowCreate(&g_pop_window, "  Value Pop\n  Window", 100, 50, &g_value_desc);
    return (ESGUI_MenuAction_T){ACT_SHOW_POPUP, &g_pop_window};
}
static ESGUI_MenuAction_T on_textlist_popup(ESGUI_MenuPage_T *page, void *arg) {
    (void)page; (void)arg;
    ESGUI_DefaultTextListPopWindowCreate(&g_pop_window, 100, 50,
        g_textlist_popup_items, ESGUI_ITEM_NUM_COUNT(g_textlist_popup_items));
    return (ESGUI_MenuAction_T){ACT_SHOW_POPUP, &g_pop_window};
}
static ESGUI_MenuAction_T on_bmplist_popup(ESGUI_MenuPage_T *page, void *arg) {
    (void)page; (void)arg;
    ESGUI_DefaultBMPListPopWindowCreate(&g_pop_window, "BMP W", 100, 50,
        g_bmplist_popup_items, ESGUI_ITEM_NUM_COUNT(g_bmplist_popup_items));
    return (ESGUI_MenuAction_T){ACT_SHOW_POPUP, &g_pop_window};
}

/* 前向声明 */
static ESGUI_MenuAction_T on_goto_bmp_page(ESGUI_MenuPage_T *page, void *arg);

/* -------- 文本页面条目 -------- */
static ESGUI_MenuItem_T g_text_menu_items[] = {
    {0,0,"123",               NULL,NULL,NULL},
    {0,0,"页面一\x03/0",         NULL,NULL,NULL},
    {0,0,"消息弹窗",            NULL,on_message_popup,NULL},
    {0,0,"布尔弹窗\x03/2",       NULL,on_bool_popup,&g_bool_val},
    {0,0,"值弹窗\x03/2",        NULL,on_value_popup,&g_uint16_val},
    {0,0,"文本列表弹窗\x03/2",    NULL,on_textlist_popup,&g_pop_window.focus_idx},
    {0,0,"图形页面",            NULL,on_goto_bmp_page,&g_bmp_page},
    {0,0,"长文本测试chang wen ben ce shi 12345",NULL,NULL,NULL},
};

/* -------- 图形页面条目 -------- */
static ESGUI_MenuItem_T g_bmp_menu_items[] = {
    {0,0,"Computer", (void*)&bmp_Computer32x32,  NULL,NULL},
    {0,0,"File",     (void*)&bmp_File32x32,      NULL,NULL},
    {0,0,"MCU",      (void*)&bmp_MCU32x32,       NULL,NULL},
    {0,0,"闪电",     (void*)&bmp_Lightning8x16,  NULL,NULL},
    {0,0,"Picture",  (void*)&bmp_Picture32x32,   on_bmplist_popup,NULL},
    {0,0,"Setting",  (void*)&bmp_Setting32x32,   NULL,NULL},
};

static ESGUI_MenuAction_T on_goto_bmp_page(ESGUI_MenuPage_T *page, void *arg) {
    (void)page;
    ESGUI_DefaultBMPMenuCreate(&g_bmp_page, "Label",
        g_bmp_menu_items, ESGUI_ITEM_NUM_COUNT(g_bmp_menu_items));
    return (ESGUI_MenuAction_T){ACT_PUSH_PAGE, arg};
}


/* ================================================================
 *  SDL2 初始化
 * ================================================================ */
static int sdl_init(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return -1;
    }

    g_window = SDL_CreateWindow(
        "ESGUI SDL2 Simulator",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_W * SCREEN_SCALE, SCREEN_H * SCREEN_SCALE,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!g_window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return -1;
    }

    g_renderer = SDL_CreateRenderer(g_window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!g_renderer) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return -1;
    }

    /* 设置逻辑大小：无论窗口多大，都按 128×64 逻辑坐标绘制 */
    SDL_RenderSetLogicalSize(g_renderer, SCREEN_W, SCREEN_H);
    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);

    /* g_rgb stores exactly three bytes per pixel in R, G, B order. */
    g_texture = SDL_CreateTexture(g_renderer,
        SDL_PIXELFORMAT_RGB24,
        SDL_TEXTUREACCESS_STATIC,
        SCREEN_W, SCREEN_H);
    if (!g_texture) {
        fprintf(stderr, "SDL_CreateTexture Error: %s\n", SDL_GetError());
        return -1;
    }

    /* 初始清屏 */
    memset(g_fb, 0, sizeof(g_fb));
    memset(g_rgb, 0, sizeof(g_rgb));
    SDL_UpdateTexture(g_texture, NULL, g_rgb, SCREEN_W * 3);

    return 0;
}


/* ================================================================
 *  ESGUI 初始化
 * ================================================================ */
static void esgui_init(void)
{
    /* 1. 创建默认文本菜单作为首页 */
    ESGUI_DefaltTextMenuCreate(&g_text_page,
        g_text_menu_items, "ESGUI Demo",
        ESGUI_ITEM_NUM_COUNT(g_text_menu_items));

    /* 2. 初始化 ESGUI 核心 */
    ESGUI_Init(&g_ui, &g_text_page,
               ESGUI_CanvasRefresh_CB,
               ESGUI_AnimTick_CB);

    /* 3. 绑定 Canvas（全屏帧缓冲，strip_h=64 表示整屏刷新） */
    ESGUI_BindCanvas(&g_ui, &g_canvas, &g_canvas_it,
                     g_fb, SCREEN_W, SCREEN_H, 64);
}


/* ================================================================
 *  SDL 键码 → ESGUI 事件码 映射
 * ================================================================ */
static int sdl_key_to_esgui(SDL_Keycode key, ESGUI_EventCode_t *out)
{
    switch (key) {
    case SDLK_UP:    case SDLK_w: *out = EVT_KEY_DOWN;  return 1;
    case SDLK_DOWN:  case SDLK_s: *out = EVT_KEY_UP;    return 1;
    case SDLK_LEFT:  case SDLK_a: *out = EVT_KEY_LEFT;  return 1;
    case SDLK_RIGHT: case SDLK_d: *out = EVT_KEY_RIGHT; return 1;
    case SDLK_RETURN: case SDLK_SPACE: *out = EVT_KEY_OK;   return 1;
    case SDLK_BACKSPACE: case SDLK_ESCAPE: *out = EVT_KEY_BACK; return 1;
    default: return 0;
    }
}


/* ================================================================
 *  主循环
 * ================================================================ */
int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    /* ---- SDL2 初始化 ---- */
    if (sdl_init() != 0) {
        return 1;
    }

    /* ---- ESGUI 初始化 ---- */
    esgui_init();

    /* ---- 主循环 ---- */
    int running = 1;
    uint32_t last_tick = SDL_GetTicks();

    while (running) {
        /* 1. 处理 SDL 事件 */
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
            case SDL_QUIT:
                running = 0;
                break;

            case SDL_KEYDOWN: {
                ESGUI_EventCode_t ecode;
                if (sdl_key_to_esgui(ev.key.keysym.sym, &ecode)) {
                    /* Escape 退出程序 */
                    if (ev.key.keysym.sym == SDLK_ESCAPE) {
                        /* 检查是否有弹窗：先尝试关闭弹窗，否则退出 */
                        ESGUI_FeedKey(&g_ui, EVT_KEY_BACK, SDL_GetTicks());
                    } else {
                        ESGUI_FeedKey(&g_ui, ecode, SDL_GetTicks());
                    }
                }
                break;
            }

            default:
                break;
            }
        }

        /* 2. ESGUI Tick（内部通过 UseCanvasFlush 更新 g_rgb → texture） */
        uint32_t now = SDL_GetTicks();
        ESGUI_Tick(&g_ui, now);

        /* 3. 渲染到屏幕 */
        SDL_RenderClear(g_renderer);
        SDL_RenderCopy(g_renderer, g_texture, NULL, NULL);
        SDL_RenderPresent(g_renderer);

        /* 4. 帧率控制 ~60fps */
        uint32_t elapsed = SDL_GetTicks() - last_tick;
        if (elapsed < 16) {
            SDL_Delay(16 - elapsed);
        }
        last_tick = SDL_GetTicks();
    }

    /* ---- 清理 ---- */
    SDL_DestroyTexture(g_texture);
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    SDL_Quit();

    return 0;
}

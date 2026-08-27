# ESGUI — 嵌入式简易 GUI 菜单框架

## 📋 更新日志

### V3.0.0 (2026-8-25)
- ### ↓↓↓ 新增 ↓↓↓
  - ### 弹窗升级为弹窗栈：
    - 支持多个弹窗叠放显示（`ESGUI_MAX_POPUP_DEPTH`，默认 4 层），新弹窗压入栈顶，输入优先路由到栈顶弹窗，关闭后自动回到下层弹窗；同一弹窗指针不重复压栈
    - 示例演示两层弹窗嵌套：弹窗内再开弹窗、一次按键连关多层
  - ### 延迟动作队列增加入队 API（`ESGUI_MenuCtrlQueueAction`）：
    - 将动作排队，当前动作（如弹窗滑出动画）完成后由 `ESGUI_Tick` 依次取出执行
    - 典型用法：弹窗条目回调中先把"关闭下层弹窗"入队，再返回"关闭当前弹窗"，实现一次按键连关多层弹窗
  - ### 键盘组件（`ESGUI_KeyBoard`）：
    - 字母页（QWERTY）/ 数字符号页切换、SHIFT 大小写、按钮或编码器导航
    - 键宽按行均分、总高超高时自动垂直滚动（焦点行始终可见）
  - ### 单行文本编辑框（`ESGUI_EditBox`）：
    - 插入 / 退格 / 光标移动，文本超宽自动水平滚动，光标始终可见
  - ### 多行文本编辑框（`ESGUI_MultiLineEditBox`）：
    - `\n` 换行、光标上下左右移动（上下保持目标列）、显示区自动纵向滚动
  - ### 键盘输入弹窗（`ESGUI_DefaultKeyBoardPopWindowCreate`）：
    - 顶部单行输入框 + 底部键盘，按"确定"写入目标缓冲（按容量截断），"取消"丢弃
  - ### 多行文本编辑页（`ESGUI_MultiLineEditPageCreate`）：
    - 全屏：标题栏 + 多行文本区 + 底部键盘；直接编辑用户工作缓冲，BACK 返回即保存
  - ### 无按钮长文本消息弹窗（`ESGUI_DefaultMessageLongTextPopWindowCreate`）：
    - 文本按弹窗宽度自动换行（支持 UTF-8 与 `\n`），右侧纵向进度条显示浏览进度，UP/DOWN 滚动浏览，OK/BACK 任意键关闭
  - ### 手动指定初始 focus_idx：
    - Create 之后、Push/Show 之前设置 `page.focus_idx` / `pop_window.focus_idx` 即可，首次显示即高亮指定条目（焦点框/列表滚动/进度条直接到位）；越界值自动归位到最后一个条目
  - ### 0 条目数菜单支持：
    - 条目数为 0 的页面/弹窗安全空转（不做任何操作）：不绘制、不响应除 BACK 外的输入
    - BACK 键仍可正常退出（页面 `ACT_POP_PAGE` / 弹窗 `ACT_CLOSE_POPUP`），不越界不崩溃
  - ### ESP32-S3 WiFi 联网 demo
- ### ↓↓↓ 修复 ↓↓↓
  - 修复默认弹窗忽略弹窗条目 on_enter 回调返回动作的 BUG（弹窗条目返回的 `ACT_*` 动作现在会被框架执行）
  - 修复动态菜单插入条目时不清除空的占位条目的 BUG（首次 Add/Insert 直接覆盖占位条目）


### v1.0.0
- 首次提交

### v1.1.0 (2026-07-07)
- 新增：五种默认弹窗的标题滚动版本
### V1.2.1 (2026-7-14)
- 修复了无动画运行还一直刷屏的BUG
- STM32的示例工程新增对SH1107 OLED驱动芯片的支持

### V1.2.2 (2026-7-20)
- 示例工程新增思澈芯片例程
- 例程使用思澈官方SF32lb52-nano-n16-r16开发板
- 该例程可作为在RT-Thread中使用ESGUI的参考

### V1.3.1 (2026-8-12)
- 修复了一些BUG，新增ESGUI_Def.h文件，统一类型定义，方便移植
- 将一些不太严谨的语句改严谨了

### V1.3.2 (2026-8-13)
- 修复了ESGUI类型冲突的BUG
- 新增ESP-IDF V6.02框架下的ESP32P4芯片例程
- 该例程可作为在FreeRTOS中使用ESGUI的参考

### V1.4.0 (2026-8-14)
- ESGUI自带的Canvas绘图库字体查找方式更改为二分查找
- ESGUI字体生成器适配二分查找
- 更新示例工程的ESGUI版本

### V1.4.1 (2026-8-14)
- 刚刚忘记更新字体生成器了，现在补上

### V2.0.0 (2026-8-18)
- ### ↓↓↓ 新增 ↓↓↓
  - ### 多线程支持：
  - 无锁命令队列（单生产者-单消费者），Async 函数可从任意任务/中断安全调用，UI 线程在 `ESGUI_Tick` 内统一处理（Actor 模式）
  - `ESGUI_ENABLE_MULTITHREAD` 多线程支持开关：=1 走队列，=0 退化为直接同步调用（零队列开销）

  - `ESGUI_SYNC_MODE` 双同步后端：=0 编译器原子（GCC/Clang，多核安全）；=1 纯软件 volatile（零原子内建，不支持多核）

  - 新增生产者收件箱（`ESGUI_ProducerBox_*`）：多任务调用 UI 的"单生产者收拢"方案，每个任务一个 SPSC 收件箱，UI 线程自动轮询收拢,多生产者解决方案

  - **Async 异步函数(单生产者模型)**
    - 页面栈：`ESGUI_PushPageAsync` / `ESGUI_PopPageAsync` / `ESGUI_ShowPopupAsync` / `ESGUI_ClosePopupAsync`
    - 覆盖层：`ESGUI_OverlayAddAsync` / `ESGUI_OverlayRemoveAsync` / `ESGUI_OverlaySetVisibleAsync`
    - 其他：`ESGUI_HandleActionAsync` / `ESGUI_AnimStartAsync` / `ESGUI_AnimStopAllAsync`
  - ### Overlay 覆盖层
    - 常驻组件层，独立于页面/弹窗，始终叠加在最上层（z-order = 注册顺序）
    - 支持任意任务异步增删/显隐，`always_dirty` 每帧强制重绘
  - ### 3D 线框图绘制模块（`ESGUI_3D.c/.h`）
    - 定点透视投影、零浮点；模型变换（缩放 / 绕X / 绕Y / 绕Z / 平移）
    - 内置 3D 菜单页面（`ESGUI_ENABLE_3D_MENU`），焦点模型带旋转动画
    - 3D模型数据生成AI提示词.txt,可以把提示词丢给AI，让AI帮你生成模型数据

- ### ↓↓↓重构↓↓↓
      - STM32示例工程默认不开启多线程支持
      - ESP32工程默认开启多线程支持
      - SIFLI工程默认开启多线程支持
  - `ESGUI_PageDefaltVtbl.c` 更名为 `ESGUI_DefaultConfig.c`**（默认虚函数表实现），配置头统一为 `ESGUI_DefaultConfig.h`
  - `ESGUI_T`的`draw_data`成员更名为`draw_ctx`**
  - 示例页面与`ESGUI`配置同步
  - 示例工程文件结构**
  - 新增 `page/` 目录：存放菜单页面描述（文本页/BMP页/3D页等）
  - 新增 `model/` 目录：存放 3D 模型数据（顶点/边）
  - 条件编译裁剪宏真正接线（三角形/缓动曲线可裁剪）、8 对齐透明位图快速路径、过渡遮罩去除法
- ### ↓↓↓修复↓↓↓
  - 字体生成器生成的字体文件`NULL`未定义问题，现已改为`ESGUI_NULL`
  - 长文本作为首条时进入页面不滚动的问题

### V2.1.0 (2026-8-21)
- 新增GIF组件,添加ESGUI对GIF的支持
- 新增GIF取模软件`ESGUI_GIF_Create.exe`，可以方便的生成GIF数组
- 所有示例工程已更新
- BMP菜单已适配GIF


### V2.2.0 (2026-8-22)
- ### ↓↓↓ 新增 ↓↓↓
  - ### 菜单/列表弹窗运行时增删条目：
    - `ESGUI_MenuPageAddItem` / `ESGUI_MenuPageInsertItem` / `ESGUI_MenuPageRemoveItem`：操作静态条目数组（需设置 `item_cap`，自动重排布局并修正焦点）
    - 动态文本菜单 `ESGUI_DynamicTextMenuCreate`：条目数组内部 malloc，销毁自动 free，`item_auto_expand` 容量自适应（增加不足自动 realloc 翻倍，删除超 2 倍自动缩容，实现真正的按需增删）
    - `ESGUI_MENU_RUNTIME_ITEMS` 总开关：0=整套机制剔除（API 与 on_relayout 重排回调一并裁剪）
  - ### 增删条目的异步版本（与 Async 家族一致）：
    - `ESGUI_MenuPageAddItemAsync` / `ESGUI_MenuPageInsertItemAsync` / `ESGUI_MenuPageRemoveItemAsync`：从任意任务/中断投递，UI 线程 Tick 内执行；多任务并发用 ProducerBox 消息盒投递同结构命令
  - ### 多线程方式独立开关：
    - `ESGUI_ENABLE_CMD_QUEUE`：主命令队列方式（Async 入主队列），0=Async 退化为同步直调
    - `ESGUI_ENABLE_PRODUCER_BOX`：生产者消息盒方式，0=消息盒类型/函数整体剔除
  - ### 动态内存接口宏：
    - `ESGUI_MALLOC` / `ESGUI_FREE` / `ESGUI_REALLOC`：可自定义内存分配器（如 ESP-IDF `heap_caps_*` 指定 PSRAM、自定义内存池），默认标准库
  - README 补充：菜单动态增删用法、异步增删与各功能 ProducerBox 投递示例


# ↓↓ESGUI介绍↓↓
ESGUI（Embedded Simple GUI）是一个面向单色 OLED（如 SSD1315/SSD1306）的轻量级菜单框架。
采用 **纯 C 编写**、**零浮点运算**、**零动态内存分配**、**零 RTOS 依赖**，专为资源受限的 MCU 设计。
>介绍视频:https://www.bilibili.com/video/BV1xFTw6iEbj?vd_source=605419deeeaedb82f5f8918bda063219

> 典型应用场景：128×64 单色屏、STM32/GD32 等 Cortex-M 内核、无外部显存或显存极小的嵌入式设备。

---

## ✨ 核心特性

| 特性 | 说明 |
|------|------|
| **零浮点** | 所有动画与进度使用千分比（0~1000），无 `float`/`double`，无 FPU 也能流畅运行 |
| **默认零动态内存** | 页面/弹窗/动画数据默认全部使用静态内存池；动态菜单（可选，`ESGUI_ENABLE_DYNAMIC_TEXT_MENU`）才使用 malloc，且分配器可用 `ESGUI_MALLOC` 宏自定义 |
| **零 RTOS 依赖** | 线程安全靠自研无锁命令队列实现，不依赖任何操作系统/锁/汇编 |
| **多线程支持** | Async 函数可从任意任务/中断调用；多任务场景用生产者收件箱"单生产者收拢"；主队列与消息盒可独立开关裁剪 |
| **页式显存映射** | 显存 Buffer 与 SSD1315 GDDRAM 1:1 页式映射，送屏无需转置 |
| **分块刷新** | 支持按条带（Strip）分块刷新，几 KB RAM 即可驱动大屏 |
| **动画系统** | 内置线性/缓入/缓出/回弹/冲过/弹跳等曲线，支持热更新目标值与 must_complete 阻塞 |
| **3D 线框渲染** | 定点透视投影（零浮点），模型变换 + 内置 3D 菜单页面 |
| **Overlay 覆盖层** | 常驻组件层，独立于页面/弹窗，始终叠加在最上层，支持异步增删/显隐 |
| **页面栈管理** | 最大 **8 级**菜单深度（可宏定义调整），支持 Push/Pop 过渡动画，动画期间自动屏蔽按键防误触 |
| **弹窗栈** | 【V3.0.0】弹窗升级为栈结构（默认 **4 层**），支持多弹窗叠放，输入路由到栈顶，关闭后自动回落到下层弹窗 |
| **延迟动作队列** | 【V3.0.0】动作可排队，在当前动作（弹窗滑出动画等）完成后依次执行，支持一次按键连关多层弹窗 |
| **键盘与文本编辑** | 【V3.0.0】键盘组件（字母/数字符号页）+ 单行/多行文本编辑框 + 键盘输入弹窗 + 多行编辑页 |
| **长文本弹窗** | 【V3.0.0】无按钮长文本弹窗：自动换行 + 右侧进度条 + UP/DOWN 滚动浏览 |
| **空菜单保护** | 【V3.0.0】0 条目菜单/弹窗安全空转，BACK 可正常退出，不越界不崩溃 |
| **手动初始焦点** | 【V3.0.0】Create 后、Push 前手动指定 `focus_idx`，首次显示即高亮指定条目 |
| **虚函数表架构** | 每个页面/弹窗自带 `esgui_page_vtable_t`，所有显示效果与输入处理均可被用户完全覆盖 |
| **模块化裁剪** | 通过宏开关编译时剔除不需要的页面类型、弹窗、动画曲线、3D 模块，极致压缩 Flash |
| **UTF-8 文本** | 支持中英文混排，自动换行，超长文本自动滚动 |

---

## 🏗️ 架构设计

ESGUI 采用**"核心框架 + 默认虚函数表"**的两层架构：

```
┌─────────────────────────────────────┐
│  用户自定义页面（可选）                │  ← 继承 vtable，完全重写或局部覆盖
│  ESGUI_DefaultConfig.c / .h         │  ← 默认虚函数表：文本/BMP/3D菜单 + 多种弹窗（含键盘/长文本）
│  ESGUI_DefaultConfig.h              │  ← 编译时配置（尺寸/开关/动画参数/多线程/队列）
├─────────────────────────────────────┤
│  ESGUI.c / ESGUI.h                  │  ← 框架核心：生命周期、事件路由、Tick 驱动、命令队列
│  ESGUI_Menu.c / .h                  │  ← 菜单控制器：页面栈、弹窗栈、延迟动作队列、动作分发
│  ESGUI_Anim.c / .h                  │  ← 动画引擎：静态池、千分比插值、缓动曲线
│  ESGUI_Event.c / .h                 │  ← 事件定义（按键/编码器/触摸）
│  ESGUI_Widget.c / .h                │  ← 基础控件：进度条、焦点框、复选框等
│  ESGUI_KeyBoard.c / .h              │  ← 键盘组件：字母/数字符号页、SHIFT、编码器导航
│  ESGUI_EditBox.c / .h               │  ← 单行文本编辑框：插入/退格/光标/自动水平滚动
│  ESGUI_MultiLineEditBox.c / .h      │  ← 多行文本编辑框：换行/光标移动/自动纵向滚动
│  ESGUI_3D.c / .h                    │  ← 3D 线框渲染：定点投影、模型变换
│  ESGUI_UseCanvas.c / .h             │  ← Canvas 适配层：绑定分块刷新到框架
│  BSP/                               │  ← 底层绘图库：画布、图元、文本、位图
└─────────────────────────────────────┘
```

### 虚函数表（vtable）机制

每个页面/弹窗都是一个 `ESGUI_MenuPage_T` 或 `ESGUI_PopWindow_T`，内含一个 `const esgui_page_vtable_t *vtbl`：

```c
typedef struct {
    void (*on_create)(ESGUI_MenuPage_T *page);          // 分配资源，初始化页面
    void (*on_destroy)(ESGUI_MenuPage_T *page);         // 释放资源
    void (*on_draw)(ESGUI_MenuPage_T *page);            // 绘制背景 + 所有项
    uint16_t (*special_item_draw)(...);                   // 特殊条目标记绘制
    uint16_t (*get_special_item_draw_w)(...);            // 特殊条目宽度计算
    ESGUI_MenuAction_T (*on_input)(...);                // 按键/编码器输入处理
    void (*on_focus_change)(...);                       // 焦点变化通知（启动动画）
    void (*on_page_chenge)(...);                        // 页面切换回调（Push/Pop 动画）
} esgui_page_vtable_t;
```

**这意味着：**
- 你可以只替换 `on_draw` 实现一套完全不同的视觉风格（比如从列表改为网格）。
- 可以只替换 `on_input` 实现自定义的按键逻辑（比如旋转编码器带加速度）。
- 可以继承默认实现，只覆盖其中一两个函数，复用其余代码。
- **ESGUI_DefaultConfig.c** 只是框架附带的一套"默认皮肤"，不是框架本身。你可以完全删掉它，用自己的 vtable 实现替代。

---

## 📁 项目结构

```
ESGUI_Git/
├── ESGUI/                          # 框架核心源码
│   ├── ESGUI.c / ESGUI.h           # 框架入口：Init / FeedKey / Tick / 命令队列 / Async / Overlay
│   ├── ESGUI_Menu.c / .h           # 菜单控制器：页面栈、弹窗、动作分发
│   ├── ESGUI_Event.c / .h          # 事件定义（按键/编码器/触摸）
│   ├── ESGUI_Anim.c / .h           # 动画引擎：缓动曲线、静态池、千分比插值
│   ├── ESGUI_DefaultConfig.c / .h  # 【默认虚函数表】文本/BMP/3D 菜单 + 多种弹窗（原 ESGUI_PageDefaltVtbl.c 更名）
│   ├── ESGUI_DefaultConfig.h       # 【编译配置】尺寸、开关、动画参数、多线程、命令队列、3D
│   ├── ESGUI_Widget.c / .h         # 控件：进度条、焦点框、复选框、单选框
│   ├── ESGUI_KeyBoard.c / .h       # 【V3.0.0】键盘组件：字母/数字符号页、SHIFT、编码器导航
│   ├── ESGUI_EditBox.c / .h        # 【V3.0.0】单行文本编辑框：插入/退格/光标/自动水平滚动
│   ├── ESGUI_MultiLineEditBox.c/.h # 【V3.0.0】多行文本编辑框：换行/光标/自动纵向滚动
│   ├── ESGUI_3D.c / .h             # 3D 线框渲染：定点透视投影、模型变换
│   ├── ESGUI_UseCanvas.c / .h      # Canvas 适配层：绑定分块刷新到 ESGUI
│   ├── BSP/
│   │   ├── ESGUI_BSP_Canvas.c / .h   # 画布与分块刷新引擎
│   │   ├── ESGUI_BSP_draw.c / .h     # 基础图元：点、线、矩形、圆、圆角矩形、三角形
│   │   ├── ESGUI_BSP_Text.c / .h     # UTF-8 文本渲染与页式字模
│   │   └── ESGUI_BSP_BMP.c / .h      # 1bpp 页式位图绘制
│   └── Font/
│       ├eui_test_font.c / .h    # 示例字体（12px，英文+部分中文）
│       └── ...                     # 用户生成的自定义字体
├── ESGUI字体生成器.exe              # 字体生成工具
|── ESGUI_GIF_Create.exe            #GIF取模工具
└── example/                        # 示例工程（STM32 / ESP32 / 思澈 等）
    ├── app/
    │   ├── page/                   # 【V2.0.0 新增】菜单页面描述（文本页/BMP页/3D页等）
    │   └── model/                  # 【V2.0.0 新增】3D 模型数据（顶点/边数组）
    └── ...
```

---

## ⚙️ 编译配置（ESGUI_DefaultConfig.h）

`ESGUI_DefaultConfig.h` 控制**默认虚函数表**与**框架行为**的编译内容，所有宏均可用 `-D` 在编译命令行覆盖。

### 多线程 / 命令队列

| 宏 | 默认值 | 说明 |
|---|-----|---|
| `ESGUI_ENABLE_MULTITHREAD` | 1 | 1=Async 走无锁队列（多线程）；0=退化为直接同步调用（单线程，零队列开销） |
| `ESGUI_ENABLE_CMD_QUEUE` | 1 | 【V2.2.0】主命令队列方式开关：1=Async 入主队列；0=Async 退化为同步直调，主队列类型/处理代码剔除 |
| `ESGUI_ENABLE_PRODUCER_BOX` | 1 | 【V2.2.0】生产者消息盒方式开关：0=ProducerBox 类型/函数整体剔除 |
| `ESGUI_CMD_QUEUE_SIZE` | 32 | 主命令队列大小（须 2 的幂） |
| `ESGUI_SYNC_MODE` | 0 | 0=编译器原子（GCC/Clang，多核安全）；1=纯软件 volatile（任何编译器，单核） |
| `ESGUI_MAX_PRODUCER` | 4 | 生产者收件箱最大数量（多生产者收拢） |
| `ESGUI_PRODUCER_BOX_CAP` | 8 | 单个收件箱容量（须 2 的幂） |

### 运行时条目增删 / 动态菜单 / 内存

| 宏 | 默认值 | 说明 |
|---|-----|---|
| `ESGUI_ENABLE_MENU_RUNTIME_ITEMS` | 1 | 【V2.2.0】运行时增删条目总开关：1=Add/Insert/Remove 三个 API + 各页面 on_relayout 重排；0=整套剔除 |
| `ESGUI_ENABLE_DYNAMIC_TEXT_MENU` | 1 | 【V2.2.0】动态文本菜单（`ESGUI_DynamicTextMenuCreate`）：条目数组 malloc、销毁自动 free、容量自适应；依赖 `ESGUI_ENABLE_TEXT_MENU=1` |
| `ESGUI_MALLOC` / `ESGUI_FREE` / `ESGUI_REALLOC` | 标准库 | 【V2.2.0】动态内存分配接口宏，可覆盖为自定义分配器（如 `heap_caps_malloc(..., MALLOC_CAP_SPIRAM)`） |

### 页面 / 弹窗裁剪

| 宏 | 默认值 | 说明 |
|---|-----|---|
| `ESGUI_MAX_MENU_DEPTH` | 8   | 菜单栈最大深度（可在 `ESGUI_Menu.h` 独立调整） |
| `ESGUI_ENABLE_TEXT_MENU` | 1   | 默认文本菜单页面 |
| `ESGUI_ENABLE_BMP_MENU` | 1   | 默认图形（BMP）菜单页面 |
| `ESGUI_ENABLE_POPUP_MESSAGE` | 1   | 消息弹窗 |
| `ESGUI_ENABLE_POPUP_BOOL` | 1   | 布尔弹窗（OK/Cancel） |
| `ESGUI_ENABLE_POPUP_VALUE` | 1   | 数值调节弹窗 |
| `ESGUI_ENABLE_POPUP_TEXTLIST` | 1   | 文本列表弹窗 |
| `ESGUI_ENABLE_POPUP_BMPLIST` | 1   | 图片列表弹窗 |
| `ESGUI_ENABLE_POPUP_LONGTEXT` | 1 | 【V3.0.0】无按钮长文本消息弹窗（自动换行 + 进度条 + 滚动浏览） |
| `ESGUI_LONGTEXT_POPUP_MAX_LINES` | 64 | 【V3.0.0】长文本弹窗最大行数（行偏移表大小，超出并入最后一行截断显示） |

### 键盘 / 文本编辑（V3.0.0）

| 宏 | 默认值 | 说明 |
|---|-----|---|
| `ESGUI_ENABLE_KEYBOARD` | 1 | 【V3.0.0】键盘组件 + 单行文本框 + 键盘输入弹窗总开关；0=整套剔除 |
| `ESGUI_KEY_BOARD_KEY_H` | 16 | 【V3.0.0】键盘键高（像素），键宽按每行键数自动均分；小屏（128x64）建议调小（如 10） |
| `ESGUI_KEYBOARD_EDIT_MAX_LEN` | 32 | 【V3.0.0】键盘弹窗内部编辑缓冲长度（含 '\0'），按"确定"时按目标容量截断写入 |
| `ESGUI_ENABLE_MULTILINE_EDIT` | 1 | 【V3.0.0】多行文本编辑框 + 多行编辑页总开关；0=整套剔除 |
| `ESGUI_MULTILINE_EDIT_MAX_LINES` | 32 | 【V3.0.0】多行编辑框最大行数（行偏移表大小） |
| `ESGUI_MULTILINE_EDIT_PAGE_POOL_SIZE` | 2 | 【V3.0.0】多行编辑页私有数据静态内存池槽位数（每槽约 100 字节 RAM） |

### 弹窗栈 / 延迟动作（V3.0.0）

| 宏 | 默认值 | 说明 |
|---|-----|---|
| `ESGUI_MAX_POPUP_DEPTH` | 4 | 【V3.0.0】弹窗栈最大深度（同时最多叠放的弹窗数） |
| `ESGUI_MENU_PENDING_ACT_QUEUE_SIZE` | 8 | 【V3.0.0】延迟动作队列容量（排队等待依次执行的动作数） |

### 3D / Overlay / 绘制 / 动画

| 宏 | 默认值 | 说明 |
|---|-----|---|
| `ESGUI_ENABLE_3D` | 1   | 3D 线框渲染模块 |
| `ESGUI_ENABLE_3D_MENU` | 1   | 3D 菜单页面（需同时开启 `ESGUI_ENABLE_3D`） |
| `ESGUI_MAX_OVERLAY` | 4   | Overlay 覆盖层最大数量 |
| `ESGUI_ENABLE_DRAW_TRIANGLE` | 1   | 三角形绘制图元 |
| `ESGUI_ANIM_ENABLE_OVERSHOOT` | 1   | 冲过缓动曲线 |
| `ESGUI_ANIM_ENABLE_BOUNCE` | 1   | 弹跳缓动曲线 |
| `ESGUI_ANIM_ENABLE_STEP` | 1   | 阶跃缓动曲线 |

### GIF
| 宏 | 默认值 | 说明 |
|---|--------|---|
| `ESGUI_ENABLE_GIF` | 1      | 使能 GIF 动图组件 |
| `ESGUI_BMP_MENU_MAX_ITEMS` | 16     | BMP 菜单单页最大条目数（用于 GIF 条目标志缓存的静态数组大小） |

### 显示效果

| 宏 | 默认值 | 说明 |
|---|-----|---|
| `ESGUI_PAGE_TRANSITION_TYPE` | 0   | 0=百叶窗淡入淡出，1=缩放过渡 |
| `ESGUI_PAGE_TRANSITION_ANIM_TIME` | 250 | 页面过渡动画时长（ms） |
| `ESGUI_ITEM_SPACING` | 3   | 文本条目间距（像素） |

> 在编译命令中加 `-DESGUI_ENABLE_TEXT_MENU=0 -DESGUI_ENABLE_3D=0` 即可完全剔除文本菜单与 3D 代码，显著减小 Flash。

---

## 🚀 快速开始

### 1. 准备显存与画布

```c
#include "ESGUI.h"
#include "ESGUI_UseCanvas.h"

#define SCREEN_W   128
#define SCREEN_H   64
#define STRIP_H    16                 // 条带高度（8 的倍数，越大 RAM 占用越高）

uint8_t canvas_buf[SCREEN_W * STRIP_H / 8];  // 条带显存
Canvas canvas;
CanvasStripIter canvas_iter;
ESGUI_T ui;
```

### 2. 定义菜单页面（使用默认效果）

```c
/* 回调：按 OK 进入下一页 */
ESGUI_MenuAction_T on_enter_sub(ESGUI_MenuPage_T *page, void *arg) {
    extern ESGUI_MenuPage_T sub_page;
    return (ESGUI_MenuAction_T){ACT_PUSH_PAGE, &sub_page};
}

/* 回调：按 BACK 返回 */
ESGUI_MenuAction_T on_pop(ESGUI_MenuPage_T *page, void *arg) {
    return (ESGUI_MenuAction_T){ACT_POP_PAGE, NULL};
}

/* 条目结构：{x, y, label, icon, on_enter, arg} */
ESGUI_MenuItem_T main_items[] = {
    {0, 0, "Setting",  NULL, on_enter_sub, NULL},
    {0, 0, "Info",     NULL, on_enter_sub, NULL},
    {0, 0, "Exit",     NULL, on_pop,       NULL},
};
ESGUI_MenuPage_T main_page;
```

### 3. 初始化并运行

```c
void ui_init(void) {
    /* 绑定画布（分块刷新） */
    ESGUI_BindCanvas(&ui, &canvas, &canvas_iter,
                     canvas_buf, SCREEN_W, SCREEN_H, STRIP_H);

    /* 创建默认文本菜单页面（使用默认 vtable） */
    ESGUI_DefaltTextMenuCreate(&main_page, main_items, "Main Menu", 3);

    /* 初始化 UI，传入首页面 */
    ESGUI_Init(&ui, &main_page, ESGUI_CanvasRefresh_CB, ESGUI_AnimTick_CB);
}

/* 主循环（1ms ~ 10ms 周期） */
void ui_loop(uint32_t tick_ms, ESGUI_EventCode_t key) {
    ESGUI_FeedKey(&ui, key, tick_ms);   // 输入事件（多线程模式下入队，可改由任意任务调用）
    ESGUI_Tick(&ui, tick_ms);           // 动画 + 绘制（必须由唯一 UI 线程调用）
}
```

### 4. 你需要实现的硬件接口

唯一需要用户实现的弱函数（`__WEAK`）：

```c
/* 送屏回调：将当前条带数据写入 OLED */
void ESGUI_UseCanvasFlush(int x0, int y0, int x1, int y1,
                         const uint8_t *buff, void *user) {
    // 例如：SSD1315_I2C_Write(x0, y0, x1, y1, buff);
}
```

---

## 🔀 多线程支持

### 设计模型

ESGUI 采用**单 UI 线程 + 无锁命令队列**（Actor / 消息传递模式，与 LVGL 一致）：

```
任意任务/中断 ──投递命令──> 无锁队列 ──> ESGUI_Tick（唯一消费者）──> 动画 + 绘制
```

所有跨任务交互都通过"投递命令"完成，**不共享任何 UI 状态**，因此不需要锁、不需要上下文 ID、不依赖 RTOS。库内无任何 `__asm__` / 关中断 / 硬件原子指令（纯软件模式下），完全可移植。

### 打开 / 关闭

```c
#define ESGUI_ENABLE_MULTITHREAD  1    // 1=开启（默认）；0=关闭，Async 退化为直接同步调用
```

### 同步后端（两种）

```c
#define ESGUI_SYNC_MODE  0
// 0 = 自动适配（默认）：
//     GCC/Clang/ARMCC6 → 内建原子 __atomic_*（多核安全，生产/消费可分核运行）
//     其他编译器（MSVC/IAR/ARMCC5）→ 自动退化为 volatile（单核语义）
// 1 = 纯软件：仅用 volatile，零内建原子、零 asm、零 RTOS，任何 C 编译器可用
//     （限定：单核 + 单生产者 + 单消费者）
```

### 用法一：单生产者（推荐，最简单）

只有**一个**任务/中断调用 UI 接口时，直接使用 Async 函数即可（从任意任务或中断调用，无锁、不阻塞）：

```c
/* 任意任务 / 中断中 */
ESGUI_FeedKey(&ui, EVT_KEY_OK, tick_ms);                       // 按键
ESGUI_PushPageAsync(&ui, &sub_page);                           // 入栈
ESGUI_PopPageAsync(&ui);                                       // 出栈
ESGUI_ShowPopupAsync(&ui, &popup);                             // 弹窗
ESGUI_ClosePopupAsync(&ui);                                    // 关弹窗
ESGUI_HandleActionAsync(&ui, (ESGUI_MenuAction_T){ACT_REFRESH, NULL});
ESGUI_OverlayAddAsync(&ui, &overlay);                          // 覆盖层
ESGUI_AnimStartAsync(&ui, &anim_cfg);                          // 动画

/* 【V2.2.0】运行时条目增删（等效同步版，UI 线程执行） */
ESGUI_MenuPageAddItemAsync(&ui, &page, &new_item);             // 末尾追加
ESGUI_MenuPageInsertItemAsync(&ui, &page, 2, &new_item);       // 指定位置插入
ESGUI_MenuPageRemoveItemAsync(&ui, &page, 3);                  // 删除指定条目
```

UI 线程只需照常调用 `ESGUI_Tick`，命令在 Tick 内统一出队处理。

### 用法二：多生产者收件箱（多个任务都要调 UI）

C 语言没有"当前任务"概念，固定签名的 Async 函数无法自动识别调用者，因此多任务场景采用**每任务一个 SPSC 收件箱，UI 线程统一收拢**——每个收件箱仍满足"单生产者"，同步原语不变：

```
任务A ──> boxA（SPSC）──┐
任务B ──> boxB（SPSC）──┼──> ESGUI_Tick 轮询所有收件箱 → 统一处理（唯一消费者）
任务C ──> boxC（SPSC）──┘
```

```c
static ESGUI_ProducerBox_T net_box;      /* 例：网络任务专属收件箱 */

/* init 阶段（任务启动前，单线程）执行一次 */
void app_init(void) {
    ESGUI_ProducerBoxInit(&net_box);
    ESGUI_ProducerBoxRegister(&ui, &net_box);   /* 上限 ESGUI_MAX_PRODUCER */
}

/* 网络任务内（任意时刻，无锁） */
void net_task(void *arg) {
    ...
    ESGUI_ProducerBoxPushAction(&net_box,
        (ESGUI_MenuAction_T){ACT_SHOW_POPUP, &pop_window});  /* 弹窗/切页等动作 */
    ESGUI_ProducerBoxPushKey(&net_box, EVT_KEY_OK, tick);    /* 按键事件 */
    /* 或构造任意命令：ESGUI_ProducerBoxPush(&net_box, &cmd); */
}
```

UI 线程侧无需任何额外代码，`ESGUI_Tick` 会自动"先清主队列、再轮询所有已注册收件箱"，命令与主队列走同一处理逻辑。

#### 各功能的 ProducerBox 投递示例（V2.2.0 补充）

以下命令与对应 Async 函数**完全等效**（前提：收件箱已 `ESGUI_ProducerBoxInit` + `ESGUI_ProducerBoxRegister`）：

```c
/* 1. 按键（便捷函数） */
ESGUI_ProducerBoxPushKey(box, EVT_KEY_OK, tick_ms);

/* 2. 压栈 / 出栈（动作命令，与 PushPageAsync/PopPageAsync 等效） */
ESGUI_ProducerBoxPushAction(box, (ESGUI_MenuAction_T){ACT_PUSH_PAGE, &sub_page});
ESGUI_ProducerBoxPushAction(box, (ESGUI_MenuAction_T){ACT_POP_PAGE, NULL});

/* 3. 弹窗 显示 / 关闭 */
ESGUI_ProducerBoxPushAction(box, (ESGUI_MenuAction_T){ACT_SHOW_POPUP, &popup});
ESGUI_ProducerBoxPushAction(box, (ESGUI_MenuAction_T){ACT_CLOSE_POPUP, NULL});

/* 4. 覆盖层 添加 / 移除 / 可见性（通用 Push 构造命令） */
ESGUI_Cmd_T cmd;
cmd.type = ESGUI_CMD_OVERLAY_ADD;
cmd.u.overlay.ov = &overlay;
ESGUI_ProducerBoxPush(box, &cmd);

cmd.type = ESGUI_CMD_OVERLAY_REMOVE;
cmd.u.overlay.ov = &overlay;
ESGUI_ProducerBoxPush(box, &cmd);

cmd.type = ESGUI_CMD_OVERLAY_VISIBLE;
cmd.u.overlay_vis.ov = &overlay;
cmd.u.overlay_vis.visible = false;
ESGUI_ProducerBoxPush(box, &cmd);

/* 5. 任意动作（刷新 / 退出应用等） */
ESGUI_ProducerBoxPushAction(box, (ESGUI_MenuAction_T){ACT_REFRESH, NULL});

/* 6. 动画 启动 / 停止（anim_t 配置须保持有效直到 UI 线程处理） */
cmd.type = ESGUI_CMD_ANIM_START;
cmd.u.anim_start.a = &anim_cfg;
ESGUI_ProducerBoxPush(box, &cmd);

cmd.type = ESGUI_CMD_ANIM_STOP_ALL;
cmd.u.anim_stop_all.var = &my_var;
ESGUI_ProducerBoxPush(box, &cmd);

/* 7. 运行时条目增删（条目值拷贝，label/icon 等指针须保持有效） */
cmd.type = ESGUI_CMD_MENU_ITEM;
cmd.u.menu_item.page = &page;
cmd.u.menu_item.op   = ESGUI_MENU_ITEM_OP_ADD;
cmd.u.menu_item.item = (ESGUI_MenuItem_T){0,0,"动态条目",NULL,on_enter,NULL};
ESGUI_ProducerBoxPush(box, &cmd);

cmd.u.menu_item.op   = ESGUI_MENU_ITEM_OP_INSERT;
cmd.u.menu_item.idx  = 2;   /* 在索引 2 处插入 */
cmd.u.menu_item.item = (ESGUI_MenuItem_T){0,0,"插入条目",NULL,on_enter,NULL};
ESGUI_ProducerBoxPush(box, &cmd);

cmd.u.menu_item.op   = ESGUI_MENU_ITEM_OP_REMOVE;
cmd.u.menu_item.idx  = 3;   /* 删除索引 3 */
ESGUI_ProducerBoxPush(box, &cmd);
```

> 说明：`PushKey` / `PushAction` 为便捷函数，其余功能统一用 `ESGUI_ProducerBoxPush(box, &cmd)` 构造命令；cmd 结构可复用（每次 Push 前改字段即可）。所有约束与对应 Async 版一致。

### ⚠️ 限制条件（重要）

1. **单消费者**：`ESGUI_Tick` 只能由**唯一**的 UI 线程调用；所有绘制、动画、页面/弹窗操作都在其中完成。
2. **主命令队列单生产者**：`ESGUI_FeedKey` / `ESGUI_*Async` 只能由**一个**任务/中断调用；多个任务请用收件箱（且**每个收件箱只允许其所属任务**写入，不能两个任务共用一个箱）。
3. **纯软件模式单核**：`ESGUI_SYNC_MODE=1` 时生产与消费必须在**同一 CPU 核**上；双核分核运行（如 ESP32-S3 双核）必须用 `=0`（编译器原子）。
4. **原子模式依赖编译器**：`ESGUI_SYNC_MODE=0` 需要 GCC/Clang 内建原子（GCC、Clang、ARM Compiler 6、新版本 IAR 均支持）；MSVC/老 IAR/ARMCC5 请用 `=1`。
5. **生命周期约定**：`ESGUI_AnimStartAsync` 传入的 `anim_t *` 配置必须保持有效，直到 UI 线程处理完该命令（用静态/全局变量，不要用栈上临时变量）。
6. **初始化时序**：`ESGUI_Init`、`ESGUI_BindCanvas`、`ESGUI_ProducerBoxRegister` 等共享状态写入必须在任务启动前、单线程的 init 阶段完成。
7. **队列满丢弃**：主队列/收件箱满时新命令被直接丢弃（不阻塞调用方），高频投递场景请调大容量（须 2 的幂）。
8. **关闭多线程**：`ESGUI_ENABLE_MULTITHREAD=0` 时 Async 函数直接同步执行，只能在单线程主循环内调用。

---

## 📝 菜单运行时增删条目（V2.2.0）

支持文本菜单、BMP 菜单、3D 菜单以及文本列表/BMP 列表弹窗（普通+滚动标题版），两种方式：

### 方式一：静态条目数组（`ESGUI_ENABLE_MENU_RUNTIME_ITEMS`）

条目数组由你声明（**可写、留足容量**），通过三个 API 增删，框架自动重排布局并修正焦点：

```c
ESGUI_MenuPage_T page;
ESGUI_MenuItem_T my_items[16];        /* 可写数组，容量 16 */

/* 创建后声明容量（增加/插入前必须设置；删除不依赖容量） */
ESGUI_DefaltTextMenuCreate(&page, my_items, "List", 3);
page.item_cap = 16;

/* 运行时增删（须在 UI 线程调用：on_enter 回调 / UI 任务定时器） */
ESGUI_MenuItem_T it = {0,0,"New Item",ESGUI_NULL,on_enter,ESGUI_NULL};
ESGUI_MenuPageAddItem(&page, &it);                        // 末尾追加
ESGUI_MenuPageInsertItem(&page, 1, &it);                  // 索引 1 处插入
ESGUI_MenuPageRemoveItem(&page, 2);                       // 删除索引 2（至少保留 1 条）
```

- 操作成功后自动调用 vtable 的 `on_relayout` 重排（文本菜单无需重排，BMP/3D 菜单重算布局、BMP 菜单同时重扫 GIF 标志），并返回 `ACT_REFRESH` 触发重绘；
- 从 `on_enter` 调用时直接返回 `(ESGUI_MenuAction_T){ACT_REFRESH, NULL}` 即可。

### 方式二：动态文本菜单（`ESGUI_ENABLE_DYNAMIC_TEXT_MENU`）

条目数组由框架内部 `malloc`，销毁自动 `free`，且**容量自适应**——加满自动翻倍扩容、删后超 2 倍自动缩容，无需预设上限：

```c
ESGUI_MenuPage_T dyn_page;

/* 创建（初始容量 12；内存分配器可用 ESGUI_MALLOC 宏自定义） */
if (!ESGUI_DynamicTextMenuCreate(&dyn_page, "动态列表", 12)) return;

/* 创建后仅 1 个占位条目（框架要求至少 1 条），改写它或直接增删 */
dyn_page.items[0].label = "第一项";

/* 增删与方式一同源（自动扩容/缩容） */
ESGUI_MenuPageAddItem(&dyn_page, &it);
ESGUI_MenuPageRemoveItem(&dyn_page, 1);
```

- 页面 Pop 销毁后条目数组已 `free`，再次 Push 前必须重新 `Create`；
- 需要跨任务调用时用异步版本：`ESGUI_MenuPageAddItemAsync(ui, page, &it)` 等（见多线程章节）。

### 注意事项

1. 条目数组必须**可写**且容量足够（静态方式勿用 const 数组）；
2. 静态方式增/插前必须设置 `page.item_cap`；动态方式由创建函数自动设置；
3. 所有增删 API 须在 **UI 线程**（on_enter / UI 任务）调用，跨任务用 Async 或 ProducerBox；
4. 删除至少保留 1 条（避免空菜单导致绘制越界，返回 false）；
5. 文本条目的 `label` 不能为 NULL（与既有绘制逻辑一致），且需保持有效（静态字符串/持久缓冲区）。

---

## 🪟 弹窗栈与延迟动作队列（V3.0.0）

### 弹窗栈

弹窗从"单弹窗"升级为**弹窗栈**：新弹窗压入栈顶，输入事件优先路由到栈顶弹窗，
关闭栈顶后自动回到下层弹窗（同一弹窗指针不重复压栈）。最大叠放层数由
`ESGUI_MAX_POPUP_DEPTH` 配置（默认 4）。

```c
ESGUI_PopWindow_T popup1, popup2;

/* 第一层弹窗条目回调：再打开第二层弹窗（压入弹窗栈） */
ESGUI_DefaultTextListPopWindowCreate(&popup2, 100, 45, items2, N);
return (ESGUI_MenuAction_T){ACT_SHOW_POPUP, &popup2};
```

### 延迟动作队列

`ESGUI_MenuCtrlQueueAction` 将动作排入延迟动作队列，当前动作（如弹窗滑出动画）完成后
由 `ESGUI_Tick` 依次取出执行——用于"一次按键连关多层弹窗"等需要动作衔接的场景：

```c
/* 第二层弹窗条目回调：先把"关闭第一层"入队，再返回"关闭本层" → 一次按键连关两层 */
static ESGUI_MenuAction_T stack_close_all_on_enter(ESGUI_MenuPage_T *page, void *arg) {
    ESGUI_MenuCtrlQueueAction(&ui.menu_ctrl, (ESGUI_MenuAction_T){ACT_CLOSE_POPUP, ESGUI_NULL});
    return (ESGUI_MenuAction_T){ACT_CLOSE_POPUP, ESGUI_NULL};
}
```

- 队列容量由 `ESGUI_MENU_PENDING_ACT_QUEUE_SIZE` 配置（默认 8），满时或动作为 `ACT_NONE` 时忽略入队；
- 动作触发 must_complete 动画时暂停出队，动画完成后的下一个 Tick 自动继续。

---

## ⌨️ 键盘与文本编辑（V3.0.0）

由 `ESGUI_ENABLE_KEYBOARD` / `ESGUI_ENABLE_MULTILINE_EDIT` 开关控制（默认开启，可整体裁剪）。

### 键盘组件（ESGUI_KeyBoard）

字母页（QWERTY）与数字/符号页切换、SHIFT 大小写、按钮或编码器导航；
键宽 = 区域宽 ÷ 本行键数自动均分，键盘总高超高时自动垂直滚动（焦点行始终可见）。

```c
ESGUI_KeyBoard_T kb;
ESGUI_KeyBoardInit(&kb, ESGUI_KEY_BOARD_FONT, x, y, area_w, area_h, key_h);

char ch; ESGUI_KeyAction_T act;
if (ESGUI_KeyBoardHandleEvent(&kb, e, &ch, &act)) {   /* 输入事件：焦点移动或键触发 */
    switch (act) { ... }                              /* ESGUI_KEY_CHAR / BACKSPACE / ENTER ... */
}
ESGUI_KeyBoardDraw(&c, &kb);                          /* 绘制（调用方先裁剪到键盘区域） */
```

### 单行文本编辑框（ESGUI_EditBox）

```c
char buf[32];
ESGUI_EditBox_T eb;
ESGUI_EditBoxInit(&eb, buf, sizeof(buf));   /* 用户提供缓冲，含 '\0' */
ESGUI_EditBoxInsert(&eb, 'A');              /* 光标处插入（缓冲满返回 false） */
ESGUI_EditBoxBackspace(&eb);                /* 退格 */
ESGUI_EditBoxCursorMove(&eb, -1);           /* 光标左移 */
ESGUI_EditBoxDraw(&c, &eb, x, y, w, font, caret_on);  /* 超宽自动水平滚动，光标始终可见 */
```

### 多行文本编辑框（ESGUI_MultiLineEditBox）

```c
char buf[128];
ESGUI_MultiLineEditBox_T meb;
ESGUI_MultiLineEditBoxInit(&meb, buf, sizeof(buf));
ESGUI_MultiLineEditBoxInsert(&meb, '\n');   /* 支持换行 */
ESGUI_MultiLineEditBoxCursorMove(&meb, -2); /* -2上 / -1左 / +1右 / +2下，上下保持目标列 */
ESGUI_MultiLineEditBoxDraw(&c, &meb, x, y, w, h, font, caret_on);  /* 自动纵向滚动 */
```

### 键盘输入弹窗（默认弹窗）

顶部单行输入框 + 底部键盘，宽/高由调用方指定（示例：128 全宽、下半屏 64 高）：

```c
static char kb_text[32];
ESGUI_DefaultKeyBoardPopWindowCreate(&pop_window, 128, 64, kb_text, sizeof(kb_text), kb_text);
return (ESGUI_MenuAction_T){ACT_SHOW_POPUP, &pop_window};
/* 按"确定"把内部编辑结果写入 kb_text（按目标容量截断）；"取消"丢弃，不影响目标缓冲 */
```

### 多行文本编辑页（默认页面）

全屏：标题栏 + 多行文本区 + 底部键盘，直接编辑用户工作缓冲，BACK 返回即保存：

```c
static ESGUI_MenuPage_T ml_page;
static char ml_text[128];
ESGUI_MultiLineEditPageCreate(&ml_page, "多行编辑", ml_text, sizeof(ml_text), ml_text);
return (ESGUI_MenuAction_T){ACT_PUSH_PAGE, &ml_page};
```

---

## 📄 无按钮长文本消息弹窗（V3.0.0）

文本按弹窗宽度自动换行（支持 UTF-8 与 `\n`），右侧纵向进度条显示浏览进度，
UP/DOWN 滚动浏览，OK/BACK 任意键关闭（与普通消息弹窗一致）：

```c
static const char long_msg[] =
    "无按钮长文本弹窗演示。\n文本按弹窗宽度自动换行，支持中文 UTF-8 与英文混排……";

ESGUI_DefaultMessageLongTextPopWindowCreate(&pop_window, long_msg, 110, 60);
return (ESGUI_MenuAction_T){ACT_SHOW_POPUP, &pop_window};
```

最大行数由 `ESGUI_LONGTEXT_POPUP_MAX_LINES` 配置（默认 64，超出并入最后一行截断显示）。

---

## 🎯 手动指定初始 focus_idx 与 0 条目菜单（V3.0.0）

### 手动指定初始焦点

所有默认菜单/弹窗（文本/BMP/3D 菜单、文本列表/BMP 列表/键盘/长文本弹窗）均支持
在 **Create 之后、Push/Show 之前** 手动设置初始焦点，首次显示即高亮指定条目：

```c
ESGUI_DefaultTextListPopWindowCreate(&pop_window, 100, 50, items, N);
pop_window.focus_idx = 2;                 /* 首次显示焦点框直接落在条目 2 */
return (ESGUI_MenuAction_T){ACT_SHOW_POPUP, &pop_window};
```

- 焦点框/列表滚动/进度条等布局在首次渲染时按 `focus_idx` 直接落位，无需按键触发修正；
- 越界值自动归位到最后一个条目（`focus_idx >= item_num` 时收敛到 `item_num - 1`）。

### 0 条目数菜单

条目数为 0 的页面/弹窗**安全空转**（不做任何操作）：不绘制、不响应除 BACK 外的任何输入，
BACK 键仍可正常退出（页面返回 `ACT_POP_PAGE`、弹窗返回 `ACT_CLOSE_POPUP`），不会越界或崩溃。
适用于"先进入空页面、再由运行时增删 API 填充条目"等场景。

---

## 🧊 3D 线框渲染

`ESGUI_3D.c/.h` 提供零浮点的定点透视投影线框渲染：

```c
#include "ESGUI_3D.h"

/* 模型：局部坐标顶点 + 边 */
static const ESGUI_3DPoint_T cube_pts[] = {
    {-10,-10,-10}, { 10,-10,-10}, { 10,-10, 10}, {-10,-10, 10},
    {-10, 10,-10}, { 10, 10,-10}, { 10, 10, 10}, {-10, 10, 10},
};
static const ESGUI_3DEdge_T cube_edges[] = {
    {0,1},{1,2},{2,3},{3,0},{4,5},{5,6},{6,7},{7,4},
    {0,4},{1,5},{2,6},{3,7},
};
static const ESGUI_3D_T cube = { cube_pts, 8, cube_edges, 12 };

/* 变换：平移 + 旋转 + 缩放（全定点） */
ESGUI_3DTransform_T t;
ESGUI_3DTransformInit(&t);
t.ty = 15; //沿y轴平移(右手坐标系)
t.scale_q8 = 256;   //缩放倍率为1,即模型原始大小
ESGUI_3DTransformRotate(&t, ESGUI_3D_AXIS_Y, 30); //旋转

/* 绘制（恒等变换时零变换开销） */
ESGUI_3DWireframeDiagram(&canvas, &cube, &t, 80, 128, 64, EUI_MODE_SET);
```

- 投影、三角（`ESGUI_3DTransformPoints`）、旋转全部定点整数运算，无 `float`/`sqrt`。
- 内置 3D 菜单页面：`ESGUI_Default3DMenuCreate(&page, title, items, num)`，焦点模型带旋转动画。
- 模型数据放入示例工程 `model/` 目录。

---

## 🪟 Overlay 覆盖层

覆盖层是**常驻显示组件**，独立于页面/弹窗，始终叠加在最上层（注册顺序即 z-order，先注册的在底层）：

```c
typedef struct {
    void (*on_draw)(ESGUI_Overlay_T *ov);  /* 绘制回调（必填） */
    void *render_ctx;                       /* 渲染上下文（框架注入） */
    void *user_data;                        /* 用户私有数据 */
    uint8_t visible      : 1;               /* 1=可见 */
    uint8_t always_dirty : 1;               /* 1=每帧强制重绘（常驻动态内容） */
} ESGUI_Overlay_T;
```

```c
static ESGUI_Overlay_T clock_ov = { .on_draw = clock_on_draw, .always_dirty = 1 };

/* 任意任务 / 中断（异步） */
ESGUI_OverlayAddAsync(&ui, &clock_ov);
ESGUI_OverlaySetVisibleAsync(&ui, &clock_ov, false);
ESGUI_OverlayRemoveAsync(&ui, &clock_ov);

/* 或 UI 线程内同步 */
ESGUI_OverlayAdd(&ui, &clock_ov);
```

数量上限由 `ESGUI_MAX_OVERLAY` 配置（默认 4）。

`ESGUI_Tick` 函数中，框架已把 `ui->draw_ctx` 注入`ESGUI_Overlay_T`的`render_ctx`成员：

```c
static void hud_on_draw(ESGUI_Overlay_T *ov) {
    Your_draw_ctx *c = ov->render_ctx;     /* 框架自动注入 ui->draw_ctx* */
    Your_ui_draw_hline(c, 0, 127, 10);     /* 画一条 HUD 分隔线 */
}
```

> 注意：`render_ctx` 由框架在每次刷新前自动注入，**不要手动赋值**。

---

## 🎨 自定义页面效果（进阶）

如果你不满意默认效果，**不需要修改框架核心**，只需提供自己的 vtable：

```c
/* 自定义绘制：比如把文本菜单改成横向标签页 */
void my_custom_draw(ESGUI_MenuPage_T *page) {
    // 完全自定义的绘制逻辑，可调用 BSP 层的 eui_draw_xxx
}

/* 自定义输入：比如把上下键改成左右键控制 */
ESGUI_MenuAction_T my_custom_input(ESGUI_MenuPage_T *page, ESGUI_EventCode_t e) {
    // ...
}

static const esgui_page_vtable_t my_vtable = {
    .on_create       = esgui_text_menu_defalt_on_create,  // 复用默认创建逻辑
    .on_destroy      = esgui_text_menu_defalt_on_destroy,
    .on_draw         = my_custom_draw,                      // 替换为自己的绘制
    .on_input        = my_custom_input,                     // 替换为自己的输入
    .on_focus_change = esgui_text_menu_defalt_on_focus_change,
    .on_page_chenge  = esgui_text_menu_default_on_page_change,
};

/* 创建页面时绑定自定义 vtable */
void MyTextMenuCreate(ESGUI_MenuPage_T *page, ...) {
    memset(page, 0, sizeof(*page));
    page->vtbl = &my_vtable;
    // ...
}
```

**动画系统同样可被自定义：**
- 默认效果使用 `ESGUI_Anim.c` 中的 `anim_start()` 驱动焦点框、进度条、页面过渡。
- 你可以在自定义 vtable 中调用 `anim_start()`，也可以完全不用动画系统，直接同步修改属性。

---

## 🎬 动画系统

ESGUI 内置轻量级动画引擎，所有视觉变化（焦点框移动、进度条增长、页面滑入、弹窗弹出）均由动画驱动。

```c
anim_t a = {0};
anim_set_var(&a, &my_var);              // 关联目标变量
anim_set_exec_cb(&a, my_callback);      // 每帧写入回调
anim_set_values(&a, 0, 100);            // 起始值 → 目标值
anim_set_time(&a, 300);                 // 300ms
anim_set_path(&a, ANIM_PATH_EASE_OUT);  // 缓动曲线
anim_start(&a);                         // 启动（若同变量已有动画，自动热更新终点）
```

- **must_complete**：页面退出动画可标记为必须完成，框架会阻塞真正的 `Pop/Destroy` 直到动画结束，防止画面撕裂。
- **热更新**：编码器快速转动时，同一变量不会启动多个动画，而是直接修改终点并延续。

---

## 📝 特殊条目标记

文本菜单支持在条目字符串尾部附加标记符，自动渲染控件：

```c
/* 标记格式：文本 + '\x03' + '/' + 类型码 */
{"WiFi\x03/0",  NULL, on_toggle, &wifi_en},   // 方形复选框
{"Mode\x03/1",  NULL, on_toggle, &mode_sel},  // 圆形单选框
{"Vol \x03/2",  NULL, NULL,     &volume},     // 数值显示（arg 指向 int16）
```

---

## 🔧 字体与位图生成

### 字体要求
ESGUI 使用 **页式 1bpp 字模**（与 SSD1315 GDDRAM 同布局），工具链如下：

1. 位图用 **PCtoLCD2002** / **Image2LCD** 生成常规水平字模（高位在前、从左到右、从上到下）。
2. 字体使用本仓库提供的 **`ESGUI字体生成器.exe`** 将水平字模转换为 **页式格式**。
3. 将生成的 `.c` 文件放入 `ESGUI/Font/`，并在代码中替换 `ESGUI_DEFAULT_FONT` 宏。

### 位图要求
BMP 菜单使用的图片同样需为 **页式 1bpp**，可用上述工具生成。

---

## 📦 示例工程

`example/` 目录包含多个可编译工程模板（STM32 HAL / 标准库 / ESP-IDF / 思澈），使用框架默认虚函数表，演示：

- 文本菜单 / BMP 菜单 / 3D 菜单切换
- 多种弹窗（消息/布尔/值/文本列表/图片列表/键盘输入/长文本，含滚动标题版本）
- 弹窗栈嵌套演示（弹窗内再开弹窗、延迟动作队列一次按键连关多层）
- 键盘输入、单行/多行文本编辑
- 长文本滚动与页面过渡动画
- 分块刷新绑定 SSD1315
- 多线程（Async + 生产者收件箱，基于 FreeRTOS / RT-Thread）
- 3D 线框图绘制与旋转
- 【V3.0.0】ESP32-S3 WiFi 联网 demo

V2.0.0 起，示例工程的用户代码统一按功能分目录组织：

```
example/<芯片>/app/
├── page/        # 菜单页面描述（text_page.c、bmp_page.c、model_page.c 等）
└── model/       # 3D 模型数据（顶点/边数组）
```
---

## 🖼️ 演示与介绍视频

> https://www.bilibili.com/video/BV1xFTw6iEbj?vd_source=605419deeeaedb82f5f8918bda063219

---

## 📄 许可证

本项目采用 **MIT License**，可自由用于商业或非商业项目。
详见仓库根目录 `LICENSE` 文件。

---

## 🤝 贡献与反馈

欢迎提交 Issue 或 PR。如果你制作了新的页面类型、缓动曲线、3D 模型或硬件适配层，欢迎分享！

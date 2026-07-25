# ESGUI SDL2 模拟器

在 PC 上使用 SDL2 模拟 OLED 128×64 屏幕的 ESGUI 渲染效果。

## 依赖

- **SDL2** (>= 2.0)
- **CMake** (>= 3.16)
- C99 编译器 (GCC / Clang / MSVC)

### 安装 SDL2

**Windows (vcpkg):**
```bash
vcpkg install sdl2:x64-windows
```

**Windows (MSYS2/MinGW):**
```bash
pacman -S mingw-w64-x86_64-SDL2
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt install libsdl2-dev
```

**macOS (Homebrew):**
```bash
brew install sdl2
```

## 构建

```bash
cd ESGUI/SDL2
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake   # vcpkg
# 或
cmake ..                                                                          # 系统安装的 SDL2
cmake --build .
```

## 运行

```bash
./esgui_sdl2        # Linux/macOS
.\esgui_sdl2.exe    # Windows
```

## 按键映射

| 按键          | 功能       |
|---------------|------------|
| ↑ / W         | 上一项     |
| ↓ / S         | 下一项     |
| ← / A         | 减少值/左移 |
| → / D         | 增加值/右移 |
| Enter / Space | 确认       |
| Backspace     | 返回       |
| Escape        | 关闭弹窗/退出 |

## 技术说明

本模拟器完全复用 ESGUI 核心源码，仅替换了底层刷屏函数：

- **桩头文件** `main.h`：替代 STM32 的 `main.h`，提供 `uint8_t`/`bool` 等类型定义
- **刷屏函数** `ESGUI_UseCanvasFlush`：将 page-based 帧缓冲转换为 RGB24 并更新 SDL2 纹理
- **输入处理**：SDL2 键盘事件映射到 ESGUI 事件码
- **帧缓冲**：使用全屏缓冲（128×64），strip_h=64，整屏刷新
- **运行统计**：窗口标题每秒更新实际帧率（FPS）和进程常驻内存（RSS）

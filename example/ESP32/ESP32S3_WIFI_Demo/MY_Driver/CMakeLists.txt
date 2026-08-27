# 注意：源文件用 file(GLOB) 收集，新增/删除 .c 后需重新配置（改动本文件或执行 reconfigure）才会生效。
# 2026/08/17 新增 GIF 动图组件：MY_Driver/ESGUI/ESGUI_GIF.c（改动本文件以触发 GLOB 重新收集）。
# 2026/08/25 新增 WiFi 扫描/连接/设备列表页：UI/page/page_wifi_scan.c、page_wifi_connect.c、page_wifi_ap_clients.c（改动本文件以触发 GLOB 重新收集）。
file(GLOB_RECURSE ALL_SUBDIRS LIST_DIRECTORIES true RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*")
set(MY_INCLUDE_DIRS "")
foreach(item ${ALL_SUBDIRS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${item})
        list(APPEND MY_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${item})
    endif()
endforeach()
# 追加当前目录本身
list(APPEND MY_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SRC_FILES "./*.c")
idf_component_register(SRCS ${SRC_FILES}
        REQUIRES driver
        REQUIRES esp_lcd esp_driver_gpio
        REQUIRES esp_timer esp_wifi nvs_flash
        INCLUDE_DIRS ${MY_INCLUDE_DIRS})
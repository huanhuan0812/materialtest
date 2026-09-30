cmake_minimum_required(VERSION 3.16)

project(materialtest VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_OSX_DEPLOYMENT_TARGET "26.0")

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui Svg)

qt_standard_project_setup()

# =====================================================================
# 1. 将 material-color-utilities 编成静态库
# =====================================================================
file(GLOB_RECURSE MCU_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/3rdparty/cpp/*.cc
)
# 排除单元测试文件（会引入 gtest 依赖、main 冲突）
list(FILTER MCU_SOURCES EXCLUDE REGEX "_test\\.cc$")

find_package(absl REQUIRED) 
add_library(mcu STATIC ${MCU_SOURCES})

target_include_directories(mcu PUBLIC
    ${CMAKE_SOURCE_DIR}/3rdparty
)

target_link_libraries(mcu PUBLIC
    absl::flat_hash_map
    absl::flat_hash_set
    absl::strings
    absl::base
    absl::status
    absl::statusor
    absl::synchronization
)

# MCU 源码是 C++17 写的，这里显式指定更保险
target_compile_features(mcu PUBLIC cxx_std_17)

# 静态库位置独立代码（macOS 上链接进 .app 一般不需要，但加上无害）
set_target_properties(mcu PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# =====================================================================
# 2. 主程序
# =====================================================================
qt_add_executable(materialtest
    main.cpp
    widget.cpp
    widget.h
    QtMaterial/QtMaterial.hpp
    QtMaterial/global/MaterialTheme.hpp
    QtMaterial/global/MaterialTheme.cpp
    QtMaterial/global/MaterialElement.hpp
    QtMaterial/global/MaterialElement.cpp
    QtMaterial/global/init.hpp
    QtMaterial/global/init.cpp
    QtMaterial/global/SeedColors.hpp
    QtMaterial/global/Constant.hpp
    QtMaterial/global/GlobalTheme.hpp
    QtMaterial/icon/Icon.hpp
    QtMaterial/icon/Icon.cpp
    QtMaterial/icon/textIcon/TextIcon.hpp
    QtMaterial/icon/textIcon/TextIcon.cpp
    QtMaterial/utils/stringToColor.hpp
    QtMaterialRc.qrc
    QtMaterial/window/Window.hpp
    QtMaterial/window/Window.cpp
    QtMaterial/input/Button.hpp
    QtMaterial/input/Button.cpp
    QtMaterial/text/Label.hpp
    QtMaterial/text/Label.cpp
    QtMaterial/text/Text.hpp
)

target_link_libraries(materialtest PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt6::Core
    Qt6::Gui
    Qt6::Svg
    mcu                                  # ← 链接静态库
)

# 注意：如果 mcu 的 target_include_directories 是 PUBLIC，
# 这里就不需要再 include_directories(./3rdparty) 了。
# 如果别处还有代码需要，保留也无妨：
# include_directories(./3rdparty)

set_target_properties(materialtest PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS materialtest
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
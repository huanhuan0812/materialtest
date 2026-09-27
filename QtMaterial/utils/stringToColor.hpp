#pragma once

#include <QColor>

namespace material::chores {

/**
 * @brief 将字符串转换为 QColor
 * @param str 输入的字符串
 * @param color 输出的 QColor
 * @return 转换是否成功
 */
static bool stringToColor(const QString& str, QColor& color) {
    if (!QColor::isValidColorName(str)) return false;
    
    color.fromString(str);
    return color.isValid();
}

}
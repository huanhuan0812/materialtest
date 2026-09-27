#pragma once

#include <QPixmap>
#include <QIcon>
#include "textIcon/TextIcon.hpp"
#include "../global/GlobalTheme.hpp"

namespace material::Icon{
    QIcon Icon(char32_t textIcon, QSize size = QSize(24, 24) , QColor color = globalTheme().Primary());
    QPixmap Pixmap(char32_t textIcon, QSize size = QSize(24, 24) , QColor color = globalTheme().Primary());
}
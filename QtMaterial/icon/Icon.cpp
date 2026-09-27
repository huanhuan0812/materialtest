#include "Icon.hpp"
#include <QPainter>
#include <QFont>
#include "../global/init.hpp"

namespace material::Icon {

QIcon Icon(char32_t textIcon, QSize size , QColor color) {
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);
    
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 使用 Material Icons 字体
    QFont font = MaterialFont;
    font.setPixelSize(size.height() * 0.8); // 适当缩放
    
    painter.setFont(font);
    painter.setPen(color);
    painter.drawText(pixmap.rect(), Qt::AlignCenter, QString::fromUcs4(&textIcon, 1));
    
    return QIcon(pixmap);
}

QPixmap Pixmap(char32_t textIcon, QSize size , QColor color) {
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);
    
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QFont font = MaterialFont;
    font.setPixelSize(size.height() * 0.8);
    
    painter.setFont(font);
    painter.setPen(color);
    painter.drawText(pixmap.rect(), Qt::AlignCenter, QString::fromUcs4(&textIcon, 1));
    
    return pixmap;
}

}
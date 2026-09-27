#include "TextIcon.hpp"
#include "../../global/init.hpp"
#include <QFontInfo>
#include <QDebug>

namespace material::Icon{
    TextIcon::TextIcon(Glyph glyph,int pixelSize, QWidget* parent)
        : QLabel(parent) {
        setAttribute(Qt::WA_TransparentForMouseEvents);
        const char32_t cp = static_cast<char32_t>(glyph);
        setText(QString::fromUcs4(&cp, 1));
        setAlignment(Qt::AlignCenter);
        QFont f = material::MaterialFont;
        f.setPixelSize(pixelSize);
        setFont(f);
    }
}
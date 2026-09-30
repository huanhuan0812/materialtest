// Label.cpp
#include "Label.hpp"

namespace material::text {

Label::Label(QWidget *parent)
    : material::global::MaterialElement()
    , QLabel(parent)
{
    onUpdateColor(m_theme->getScheme());
}

Label::Label(const QString &text, QWidget *parent)
    : material::global::MaterialElement()
    , QLabel(text, parent)
{
    onUpdateColor(m_theme->getScheme());
}

Label::~Label()
{
}

void Label::onUpdateColor(material::global::Scheme mode)
{
    Q_UNUSED(mode);

    if (!m_theme)
        return;

    QPalette pal = palette();
    pal.setColor(QPalette::WindowText, m_theme->OnSurface());
    pal.setColor(QPalette::Text,       m_theme->OnSurface());
    setPalette(pal);
}

} // namespace material::text
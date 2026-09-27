#include "MaterialElement.hpp"

namespace material::global{
MaterialElement::MaterialElement(MaterialTheme *theme)
    : m_theme(theme) 
{
    connectTheme();
}
MaterialElement::~MaterialElement() {
    if (m_connection) {
        QObject::disconnect(m_connection);
    }
}

void MaterialElement::setTheme(MaterialTheme *theme) {
    if (m_theme == theme)
        return;
    m_theme = theme;
    if (m_connection) {
        QObject::disconnect(m_connection);
    }
    connectTheme();
}

void MaterialElement::connectTheme() {
        m_connection = QObject::connect(m_theme, &MaterialTheme::themeChanged,
            [this](Scheme mode) {
                onUpdateColor(mode);
        });
    }

}
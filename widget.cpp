#include "widget.h"
#include "QtMaterial/global/Constant.hpp"
#include "QtMaterial/global/GlobalTheme.hpp"

Widget::Widget(QWidget *parent)
    : material::window::Window(parent)
{
    this->setGeometry(100, 100, 600, 400);
    m_textIcon = new material::Icon::TextIcon(material::Icon::Glyph::Home, 24, this);
    m_textIcon->setGeometry(0, 0, 50, 50);

    m_outlinedButton = new material::input::Button("Outlined", this);
    m_outlinedButton->setGeometry(100, 100, 150, 50);
    m_outlinedButton->setButtonType(material::input::ButtonType::Outlined);

    m_elevatedButton = new material::input::Button("Elevated", this);
    m_elevatedButton->setGeometry(100, 160, 150, 50);
    m_elevatedButton->setButtonType(material::input::ButtonType::Elevated);

    m_filledTonalButton = new material::input::Button("Filled Tonal", this);
    m_filledTonalButton->setGeometry(100, 220, 150, 50);
    m_filledTonalButton->setButtonType(material::input::ButtonType::FilledTonal);

    m_textButton = new material::input::Button("Text", this);
    m_textButton->setGeometry(100, 280, 150, 50);
    m_textButton->setButtonType(material::input::ButtonType::Text);

    connect(m_outlinedButton, &material::input::Button::clicked, this, [this]() {
        material::globalTheme().setScheme(material::globalTheme().getScheme() == material::global::Scheme::Light ? material::global::Scheme::Dark : material::global::Scheme::Light);
    });
}

Widget::~Widget()
{
}

#pragma once

#include <QWidget>
#include "QtMaterial/icon/textIcon/TextIcon.hpp"
#include "QtMaterial/QtMaterial.hpp"

class Widget : public material::window::Window
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
private:
    material::Icon::TextIcon* m_textIcon;
    material::input::Button* m_button,*m_outlinedButton,*m_elevatedButton,*m_filledTonalButton,*m_textButton;
    
};

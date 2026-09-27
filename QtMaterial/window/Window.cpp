#include "Window.hpp"

namespace material::window {
Window::Window(QWidget* parent): QWidget(parent), global::MaterialElement() {
    QPalette windowPalette = palette();
    windowPalette.setColor(QPalette::Window, m_theme->Surface());
    setPalette(windowPalette);
    if (m_contentContainer) {
        QPalette containerPalette = m_contentContainer->palette();
        containerPalette.setColor(QPalette::Window, m_theme->Surface());
        m_contentContainer->setPalette(containerPalette);
        m_contentContainer->setAutoFillBackground(true);
    }
}

Window::~Window() = default;

void Window::onUpdateColor(global::Scheme mode) {
    Q_UNUSED(mode);
    QPalette windowPalette = palette();
    windowPalette.setColor(QPalette::Window, m_theme->Surface());
    setPalette(windowPalette);
    if (m_contentContainer) {
        QPalette containerPalette = m_contentContainer->palette();
        containerPalette.setColor(QPalette::Window, m_theme->Surface());
        m_contentContainer->setPalette(containerPalette);
        m_contentContainer->setAutoFillBackground(true);
    }
    update();
}
}
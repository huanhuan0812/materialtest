#pragma once

#include "../global/MaterialElement.hpp"
#include <QWidget>

namespace material::window {
    class Window : public QWidget, public global::MaterialElement {
        Q_OBJECT
    public:
        explicit Window(QWidget* parent = nullptr);
        ~Window() override;
    private:
        QWidget* m_contentContainer = nullptr;
    protected:
        void onUpdateColor(global::Scheme mode) override;
    };
}
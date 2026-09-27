#pragma once

#include <QObject>
#include "MaterialTheme.hpp"
#include "GlobalTheme.hpp"

namespace material::global {

class MaterialElement {
public:
    explicit MaterialElement(MaterialTheme *theme = &material::globalTheme());
    
    virtual ~MaterialElement();

    void setTheme(MaterialTheme *theme);

protected:
    MaterialTheme *m_theme;
    QMetaObject::Connection m_connection;
    
    virtual void onUpdateColor(Scheme mode) = 0;

private:
    void connectTheme();
};

} // namespace material::global
#pragma once
#include "../global/MaterialElement.hpp"
#include <QLabel>

namespace material::text {

class Label :  public QLabel, public material::global::MaterialElement
{
    Q_OBJECT
public:
    explicit Label(QWidget *parent = nullptr);
    explicit Label(const QString &text, QWidget *parent = nullptr);
    ~Label() override;
protected:
    void onUpdateColor(material::global::Scheme mode) override;
};
}
#pragma once

#include <QPushButton>
#include <QPainter>
#include <QPainterPath>
#include <QVariantAnimation>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QList>
#include <QObject>
#include <QPointF>

#include "../global/MaterialElement.hpp"

namespace material::input {

/*!
 * \brief Material Design 3 按钮类型
 */
enum class ButtonType {
    Filled,
    FilledTonal,
    Outlined,
    Elevated,
    Text
};

/*!
 * \brief 涟漪数据：QObject 子类，作为 Button 的子对象管理生命周期。
 * 不需要信号槽，因此不加 Q_OBJECT。
 */
class Ripple : public QObject
{
public:
    explicit Ripple(QObject* parent = nullptr) : QObject(parent) {}
    ~Ripple() override = default;

    QPointF center;
    qreal   radius  = 0.0;
    qreal   opacity = 0.0;
    bool    isFadingOut = false;

    QVariantAnimation* radiusAnim = nullptr;
    QVariantAnimation* fadeAnim   = nullptr;
    QTimer*            fadeTimer  = nullptr;
};

/*!
 * \brief Material Design 3 按钮
 */
class Button : public QPushButton, public global::MaterialElement
{
    Q_OBJECT
    Q_PROPERTY(ButtonType buttonType READ buttonType WRITE setButtonType)
    Q_PROPERTY(QString textColorRole READ textColorRole WRITE setTextColorRole)
    Q_PROPERTY(QString backgroundColorRole READ backgroundColorRole WRITE setBackgroundColorRole)
    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius)
    Q_PROPERTY(bool enableRipple READ isRippleEnabled WRITE setEnableRipple)

public:
    explicit Button(QWidget* parent = nullptr);
    explicit Button(const QString& text, QWidget* parent = nullptr);
    ~Button() override;

    // ---- 类型 ----
    ButtonType buttonType() const { return m_buttonType; }
    void setButtonType(ButtonType type);

    // ---- 颜色角色覆盖 ----
    QString textColorRole() const { return m_textColorRole; }
    void setTextColorRole(const QString& role);

    QString backgroundColorRole() const { return m_backgroundColorRole; }
    void setBackgroundColorRole(const QString& role);

    // ---- 圆角 ----
    int  cornerRadius() const { return m_cornerRadius; }
    void setCornerRadius(int radius);

    // ---- 涟漪 ----
    bool isRippleEnabled() const { return m_rippleEnabled; }
    void setEnableRipple(bool enable);

    // ---- Material Icons 字体图标 ----
    void    setIconText(const QString& t) { m_iconText = t; updateGeometry(); update(); }
    QString iconText() const { return m_iconText; }

    // ---- padding ----
    void  setHorizontalPadding(qreal p) { m_horizontalPadding = p; updateGeometry(); update(); }
    qreal horizontalPadding() const { return m_horizontalPadding; }
    void  setVerticalPadding(qreal p) { m_verticalPadding = p; updateGeometry(); update(); }
    qreal verticalPadding() const { return m_verticalPadding; }

    // ---- size ----
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;

    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void changeEvent(QEvent* event) override;

    // MaterialElement 钩子
    void onUpdateColor(global::Scheme mode) override;

private:
    // 状态
    struct State {
        bool hovered = false;
        bool pressed = false;
        bool focused = false;
    };

    // 有效颜色
    struct EffectiveColors {
        QColor background;
        QColor text;
        QColor border;
        QColor overlay;
    };

    // 初始化
    void init();
    void initShadowEffect();
    void updateShadowEffect();

    // 颜色解析
    EffectiveColors effectiveColors() const;

    // 主题参数
    qreal cornerRadiusValue() const;
    qreal rippleOpacity() const;
    int   rippleFadeDelay() const;
    qreal elevation() const;
    qreal stateLayerOpacity() const;

    // 绘制子过程
    void drawBackground(QPainter* p, const QRectF& r, const EffectiveColors& c);
    void drawOutline   (QPainter* p, const QRectF& r, const EffectiveColors& c);
    void drawStateLayer(QPainter* p, const QRectF& r, const EffectiveColors& c);
    void drawRipples   (QPainter* p, const QRectF& r, const EffectiveColors& c);
    void drawFocusRing (QPainter* p, const QRectF& r);
    void drawContent   (QPainter* p, const QRectF& r, const EffectiveColors& c);

    // 涟漪管理
    void createRipple(const QPointF& pos);
    void startFadeOut(Ripple* ripple);
    void removeRipple(Ripple* ripple);
    void cleanupFinishedRipples();

    // 内容布局
    QRectF contentRect(const QRectF& rect) const;
    qreal  calculateContentWidth() const;
    qreal  calculateContentHeight() const;

    // ---------------- 成员 ----------------
    ButtonType m_buttonType = ButtonType::Filled;
    QString    m_textColorRole;
    QString    m_backgroundColorRole;
    int        m_cornerRadius   = 9999;
    bool       m_rippleEnabled  = true;

    QString m_iconText;
    qreal   m_horizontalPadding = 24.0;
    qreal   m_verticalPadding   = 0.0;

    State m_state;

    QGraphicsDropShadowEffect* m_shadowEffect = nullptr;
    qreal m_currentElevation = 0.0;

    QList<Ripple*> m_ripples;

    
};

} // namespace material::input
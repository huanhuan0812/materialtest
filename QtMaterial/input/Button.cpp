#include "Button.hpp"

#include <QEnterEvent>
#include <QFocusEvent>
#include <QMouseEvent>
#include <QtMath>

namespace material::input {

namespace {
// M3 按钮度量
constexpr int kButtonHeight   = 40;
constexpr int kMinButtonWidth = 48;

// M3 状态层不透明度（仅 press / hover）
constexpr qreal kHoverOpacity   = 0.08;
constexpr qreal kPressedOpacity = 0.12;
constexpr qreal kDisabledTextOpacity      = 0.38;
constexpr qreal kDisabledContainerOpacity = 0.12;

// 焦点环
constexpr qreal kFocusRingThickness = 2.0;
constexpr qreal kFocusRingInset     = 1.0;   // 距 widget 边缘的内缩

// 涟漪默认值
constexpr qreal kRippleOpacity      = 0.12;
constexpr int   kRippleFadeDelay    = 200;

// 阴影层级
constexpr qreal kShadowElevationLevel0 = 0.0;
constexpr qreal kShadowElevationLevel1 = 1.0;

// 角色名
constexpr const char* kPrimary              = "Primary";
constexpr const char* kOnPrimary            = "OnPrimary";
constexpr const char* kSecondaryContainer   = "SecondaryContainer";
constexpr const char* kOnSecondaryContainer = "OnSecondaryContainer";
constexpr const char* kSurface              = "Surface";
constexpr const char* kSurfaceContainerLow  = "SurfaceContainerLow";
constexpr const char* kOutline              = "Outline";
constexpr const char* kOnSurface            = "OnSurface";
constexpr const char* kShadow               = "Shadow";
} // namespace

// ----------------------------------------------------------------------
// 工具
// ----------------------------------------------------------------------
static QColor themeColor(global::MaterialTheme* theme, const QString& role)
{
    if (!theme) return QColor();

    if (role == QLatin1String("Primary"))              return theme->Primary();
    if (role == QLatin1String("OnPrimary"))            return theme->OnPrimary();
    if (role == QLatin1String("PrimaryContainer"))     return theme->PrimaryContainer();
    if (role == QLatin1String("OnPrimaryContainer"))   return theme->OnPrimaryContainer();
    if (role == QLatin1String("Secondary"))            return theme->Secondary();
    if (role == QLatin1String("OnSecondary"))          return theme->OnSecondary();
    if (role == QLatin1String("SecondaryContainer"))   return theme->SecondaryContainer();
    if (role == QLatin1String("OnSecondaryContainer")) return theme->OnSecondaryContainer();
    if (role == QLatin1String("Tertiary"))             return theme->Tertiary();
    if (role == QLatin1String("OnTertiary"))           return theme->OnTertiary();
    if (role == QLatin1String("Error"))                return theme->Error();
    if (role == QLatin1String("OnError"))              return theme->OnError();
    if (role == QLatin1String("Surface"))              return theme->Surface();
    if (role == QLatin1String("SurfaceContainerLow"))  return theme->SurfaceContainerLow();
    if (role == QLatin1String("SurfaceContainer"))     return theme->SurfaceContainer();
    if (role == QLatin1String("SurfaceContainerHigh")) return theme->SurfaceContainerHigh();
    if (role == QLatin1String("OnSurface"))            return theme->OnSurface();
    if (role == QLatin1String("OnSurfaceVariant"))     return theme->OnSurfaceVariant();
    if (role == QLatin1String("Outline"))              return theme->Outline();
    if (role == QLatin1String("OutlineVariant"))       return theme->OutlineVariant();
    if (role == QLatin1String("Background"))           return theme->Background();
    if (role == QLatin1String("Shadow"))               return theme->Shadow();

    return QColor();
}

static QColor alphaBlend(const QColor& fg, const QColor& bg, qreal alpha)
{
    QColor f = fg;
    f.setAlphaF(alpha);
    const qreal a = f.alphaF();
    const int r = qRound(f.red()   * a + bg.red()   * (1 - a));
    const int g = qRound(f.green() * a + bg.green() * (1 - a));
    const int b = qRound(f.blue()  * a + bg.blue()  * (1 - a));
    return QColor(r, g, b);
}

// ----------------------------------------------------------------------
// 构造 / 析构
// ----------------------------------------------------------------------
Button::Button(QWidget* parent)
    : QPushButton(parent)
    , global::MaterialElement()
{
    init();
}

Button::Button(const QString& text, QWidget* parent)
    : QPushButton(text, parent)
    , global::MaterialElement()
{
    init();
}

Button::~Button()
{
    // m_ripples 里的 Ripple 的 parent 都是 this，会随 Button 析构自动清理。
    m_ripples.clear();
}

void Button::init()
{
    m_buttonType    = ButtonType::Filled;
    m_cornerRadius  = global::cornerRadius(global::CornerSize::Full);
    m_rippleEnabled = true;

    setMinimumHeight(kButtonHeight);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_MacShowFocusRect, false);
    setAttribute(Qt::WA_Hover, true);
    setCursor(Qt::PointingHandCursor);

    initShadowEffect();

    setFont(global::typography(global::TypographyType::LabelLarge));
    update();
}

void Button::initShadowEffect()
{
    m_shadowEffect = new QGraphicsDropShadowEffect(this);
    m_shadowEffect->setBlurRadius(0);
    m_shadowEffect->setOffset(0, 0);
    m_shadowEffect->setEnabled(false);
    setGraphicsEffect(m_shadowEffect);
}

// ----------------------------------------------------------------------
// 属性
// ----------------------------------------------------------------------
void Button::setButtonType(ButtonType type)
{
    if (type == m_buttonType) return;
    m_buttonType = type;
    updateShadowEffect();
    update();
}

void Button::setTextColorRole(const QString& role)
{
    if (role == m_textColorRole) return;
    m_textColorRole = role;
    update();
}

void Button::setBackgroundColorRole(const QString& role)
{
    if (role == m_backgroundColorRole) return;
    m_backgroundColorRole = role;
    update();
}

void Button::setCornerRadius(int radius)
{
    radius = qMax(0, radius);
    if (radius == m_cornerRadius) return;
    m_cornerRadius = radius;
    update();
}

void Button::setEnableRipple(bool enable)
{
    if (enable == m_rippleEnabled) return;
    m_rippleEnabled = enable;
    if (!enable) {
        for (Ripple* r : m_ripples) r->deleteLater();
        m_ripples.clear();
        update();
    }
}

// ----------------------------------------------------------------------
// 尺寸
// ----------------------------------------------------------------------
QSize Button::sizeHint() const
{
    const qreal contentW = calculateContentWidth();
    const qreal contentH = calculateContentHeight();

    const int w = qMax(static_cast<int>(contentW + m_horizontalPadding * 2), kMinButtonWidth);
    const int h = qMax(static_cast<int>(contentH + m_verticalPadding * 2), kButtonHeight);

    return QSize(w, h);
}

QSize Button::minimumSizeHint() const
{
    return QSize(kMinButtonWidth, kButtonHeight);
}

qreal Button::calculateContentWidth() const
{
    QFontMetrics fm(font());
    const qreal textW = text().isEmpty()     ? 0.0 : fm.horizontalAdvance(text());
    const qreal iconW = m_iconText.isEmpty() ? 0.0 : fm.horizontalAdvance(m_iconText);

    if (!m_iconText.isEmpty() && !text().isEmpty())
        return iconW + 8.0 + textW;
    return iconW + textW;
}

qreal Button::calculateContentHeight() const
{
    QFontMetrics fm(font());
    const qreal iconH = m_iconText.isEmpty() ? 0.0 : fm.height();
    const qreal textH = text().isEmpty()     ? 0.0 : fm.height();
    return qMax(iconH, textH);
}

// ----------------------------------------------------------------------
// 颜色解析
// ----------------------------------------------------------------------
Button::EffectiveColors Button::effectiveColors() const
{
    EffectiveColors c;
    c.background = Qt::transparent;
    c.text       = Qt::transparent;
    c.border     = Qt::transparent;
    c.overlay    = Qt::transparent;

    auto col = [this](const QString& role) { return themeColor(m_theme, role); };

    switch (m_buttonType) {
    case ButtonType::Filled:
        c.background = m_backgroundColorRole.isEmpty() ? col(kPrimary) : col(m_backgroundColorRole);
        c.text       = m_textColorRole.isEmpty()       ? col(kOnPrimary) : col(m_textColorRole);
        c.overlay    = col(kOnPrimary);
        break;

    case ButtonType::FilledTonal:
        c.background = m_backgroundColorRole.isEmpty() ? col(kSecondaryContainer) : col(m_backgroundColorRole);
        c.text       = m_textColorRole.isEmpty()       ? col(kOnSecondaryContainer) : col(m_textColorRole);
        c.overlay    = col(kOnSecondaryContainer);
        break;

    case ButtonType::Outlined:
        c.background = m_backgroundColorRole.isEmpty() ? col(kSurface) : col(m_backgroundColorRole);
        c.text       = m_textColorRole.isEmpty()       ? col(kPrimary) : col(m_textColorRole);
        c.border     = col(kOutline);
        c.overlay    = col(kPrimary);
        break;

    case ButtonType::Elevated:
        c.background = m_backgroundColorRole.isEmpty() ? col(kSurfaceContainerLow) : col(m_backgroundColorRole);
        c.text       = m_textColorRole.isEmpty()       ? col(kPrimary) : col(m_textColorRole);
        c.overlay    = col(kPrimary);
        break;

    case ButtonType::Text:
        c.background = Qt::transparent;
        c.text       = m_textColorRole.isEmpty() ? col(kPrimary) : col(m_textColorRole);
        c.overlay    = col(kPrimary);
        break;
    }

    if (!isEnabled()) {
        const QColor onSurface = col(kOnSurface);
        if (c.background != Qt::transparent) {
            c.text       = alphaBlend(onSurface, c.background, kDisabledTextOpacity);
            c.background = alphaBlend(onSurface, c.background, kDisabledContainerOpacity);
        } else {
            QColor fg = onSurface;
            fg.setAlphaF(kDisabledTextOpacity);
            c.text = fg;
        }
        c.border = Qt::transparent;
    }

    return c;
}

// ----------------------------------------------------------------------
// 主题参数
// ----------------------------------------------------------------------
qreal Button::cornerRadiusValue() const { return static_cast<qreal>(m_cornerRadius); }
qreal Button::rippleOpacity() const     { return kRippleOpacity; }
int   Button::rippleFadeDelay() const   { return kRippleFadeDelay; }

qreal Button::elevation() const
{
    if (!isEnabled()) return kShadowElevationLevel0;
    return (m_buttonType == ButtonType::Elevated) ? kShadowElevationLevel1
                                                  : kShadowElevationLevel0;
}

// ★ 修改点：不再因为 focused 而叠加整块状态层
qreal Button::stateLayerOpacity() const
{
    if (!isEnabled()) return 0.0;
    if (m_state.pressed) return kPressedOpacity;
    if (m_state.hovered) return kHoverOpacity;
    return 0.0;
}

// ----------------------------------------------------------------------
// 阴影
// ----------------------------------------------------------------------
void Button::updateShadowEffect()
{
    const qreal e = elevation();
    m_currentElevation = e;

    if (e <= 0 || !isEnabled()) {
        m_shadowEffect->setEnabled(false);
        return;
    }

    m_shadowEffect->setEnabled(true);

    const qreal blur    = e * 0.06 * 32;
    const qreal offset  = e * 1.5;
    const qreal opacity = 0.15 + e * 0.04;

    m_shadowEffect->setBlurRadius(qMax(blur, 1.0));
    m_shadowEffect->setOffset(0, offset);

    QColor shadowColor = themeColor(m_theme, kShadow);
    if (!shadowColor.isValid()) shadowColor = QColor(0, 0, 0);
    shadowColor.setAlphaF(qMin(opacity, 1.0));
    m_shadowEffect->setColor(shadowColor);
}

// ----------------------------------------------------------------------
// 绘制
// ----------------------------------------------------------------------
void Button::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    const QRectF bounds = QRectF(rect());
    const EffectiveColors colors = effectiveColors();

    updateShadowEffect();

    drawBackground(&painter, bounds, colors);
    drawOutline   (&painter, bounds, colors);
    drawStateLayer(&painter, bounds, colors);
    drawRipples   (&painter, bounds, colors);
    drawFocusRing (&painter, bounds);            // ★ 独立焦点环
    drawContent   (&painter, bounds, colors);
}

void Button::drawBackground(QPainter* p, const QRectF& r, const EffectiveColors& c)
{
    if (c.background == Qt::transparent || c.background.alpha() == 0) return;

    const qreal radius = qMin(cornerRadiusValue(), qMin(r.width(), r.height()) / 2.0);
    QPainterPath path;
    path.addRoundedRect(r, radius, radius);

    p->save();
    p->setPen(Qt::NoPen);
    p->setBrush(c.background);
    p->drawPath(path);
    p->restore();
}

void Button::drawOutline(QPainter* p, const QRectF& r, const EffectiveColors& c)
{
    if (c.border == Qt::transparent || c.border.alpha() == 0) return;

    const qreal radius = qMin(cornerRadiusValue(), qMin(r.width(), r.height()) / 2.0);
    QPainterPath path;
    path.addRoundedRect(r.adjusted(0.5, 0.5, -0.5, -0.5),
                        qMax(0.0, radius - 0.5),
                        qMax(0.0, radius - 0.5));

    p->save();
    p->setPen(QPen(c.border, 1.0));
    p->setBrush(Qt::NoBrush);
    p->drawPath(path);
    p->restore();
}

void Button::drawStateLayer(QPainter* p, const QRectF& r, const EffectiveColors& c)
{
    const qreal opacity = stateLayerOpacity();
    if (opacity <= 0 || c.overlay == Qt::transparent) return;

    QColor stateColor = c.overlay;
    stateColor.setAlphaF(opacity);

    const qreal radius = qMin(cornerRadiusValue(), qMin(r.width(), r.height()) / 2.0);
    QPainterPath path;
    path.addRoundedRect(r, radius, radius);

    p->save();
    p->setPen(Qt::NoPen);
    p->setBrush(stateColor);
    p->drawPath(path);
    p->restore();
}

void Button::drawRipples(QPainter* p, const QRectF& r, const EffectiveColors& c)
{
    if (m_ripples.isEmpty() || c.overlay == Qt::transparent) return;

    const qreal radius = qMin(cornerRadiusValue(), qMin(r.width(), r.height()) / 2.0);
    QPainterPath clipPath;
    clipPath.addRoundedRect(r, radius, radius);

    p->save();
    p->setClipPath(clipPath);

    for (Ripple* ripple : m_ripples) {
        if (ripple->radius <= 0.0 || ripple->opacity <= 0.0) continue;

        QColor rippleColor = c.overlay;
        rippleColor.setAlphaF(ripple->opacity);

        p->setPen(Qt::NoPen);
        p->setBrush(rippleColor);
        p->drawEllipse(ripple->center, ripple->radius, ripple->radius);
    }

    p->restore();
}

// ★ 新增：独立焦点环
void Button::drawFocusRing(QPainter* p, const QRectF& r)
{
    return;
    // if (!m_state.focused || !isEnabled())
    //     return;
    // if (focusPolicy() == Qt::NoFocus)
    //     return;

    // QColor fc = m_theme ? m_theme->Primary() : QColor(0, 0, 0);
    // if (!fc.isValid()) fc = QColor(0, 0, 0);

    // // 焦点环画在 widget 边缘内缩 1px 处，圆角半径 = 半高，避免圆弧叠加
    // QRectF fr = r.adjusted(kFocusRingInset, kFocusRingInset,
    //                        -kFocusRingInset, -kFocusRingInset);
    // qreal frr = fr.height() / 2.0;

    // p->save();
    // p->setPen(QPen(fc, kFocusRingThickness));
    // p->setBrush(Qt::NoBrush);
    // p->drawRoundedRect(fr, frr, frr);
    // p->restore();
}

void Button::drawContent(QPainter* p, const QRectF& r, const EffectiveColors& c)
{
    if (text().isEmpty() && m_iconText.isEmpty()) return;
    if (c.text == Qt::transparent || c.text.alpha() == 0) return;

    const QRectF content = contentRect(r);
    QFontMetrics fm(font());

    const qreal textW = text().isEmpty()     ? 0.0 : fm.horizontalAdvance(text());
    const qreal iconW = m_iconText.isEmpty() ? 0.0 : fm.horizontalAdvance(m_iconText);
    const qreal totalW = iconW + (m_iconText.isEmpty() ? 0.0 : 8.0) + textW;

    qreal x = content.center().x() - totalW / 2.0;
    const qreal y = content.center().y();

    p->save();
    p->setPen(c.text);
    p->setFont(font());

    if (!m_iconText.isEmpty()) {
        QRectF iconRect(x, y - fm.height() / 2.0, iconW, fm.height());
        p->drawText(iconRect, Qt::AlignCenter, m_iconText);
        x += iconW + 8.0;
    }
    if (!text().isEmpty()) {
        QRectF textRect(x, y - fm.height() / 2.0, textW, fm.height());
        p->drawText(textRect, Qt::AlignCenter, text());
    }

    p->restore();
}

QRectF Button::contentRect(const QRectF& r) const
{
    const qreal left   = r.left()   + m_horizontalPadding;
    const qreal right  = r.right()  - m_horizontalPadding;
    const qreal top    = r.top()    + m_verticalPadding;
    const qreal bottom = r.bottom() - m_verticalPadding;

    return QRectF(left, top,
                  qMax(0.0, right - left),
                  qMax(0.0, bottom - top));
}

// ----------------------------------------------------------------------
// 涟漪
// ----------------------------------------------------------------------
void Button::createRipple(const QPointF& pos)
{
    if (!m_rippleEnabled || !isEnabled()) return;

    cleanupFinishedRipples();

    auto* ripple = new Ripple(this);
    ripple->center = pos;
    ripple->radius = 0.0;
    ripple->opacity = rippleOpacity();
    ripple->isFadingOut = false;

    const QRectF r(0, 0, width(), height());
    const QPointF corners[] = { r.topLeft(), r.topRight(), r.bottomLeft(), r.bottomRight() };
    qreal maxDist = 0.0;
    for (const QPointF& corner : corners) {
        const qreal dx = corner.x() - pos.x();
        const qreal dy = corner.y() - pos.y();
        maxDist = qMax(maxDist, std::sqrt(dx * dx + dy * dy));
    }
    maxDist *= 1.2;

    auto* radiusAnim = new QVariantAnimation(ripple);
    radiusAnim->setDuration(global::Duration::medium4);
    radiusAnim->setEasingCurve(QEasingCurve::OutQuad);
    radiusAnim->setStartValue(0.0);
    radiusAnim->setEndValue(maxDist);

    QObject::connect(radiusAnim, &QVariantAnimation::valueChanged,
                     this, [this, ripple](const QVariant& v) {
                         ripple->radius = v.toReal();
                         update();
                     });

    auto* fadeTimer = new QTimer(ripple);
    fadeTimer->setSingleShot(true);
    QObject::connect(fadeTimer, &QTimer::timeout,
                     this, [this, ripple]() { startFadeOut(ripple); });

    ripple->radiusAnim = radiusAnim;
    ripple->fadeTimer  = fadeTimer;

    m_ripples.append(ripple);

    radiusAnim->start();
    update();
}

void Button::startFadeOut(Ripple* ripple)
{
    if (!ripple) return;
    if (ripple->isFadingOut) return;
    if (m_ripples.indexOf(ripple) < 0) return;

    ripple->isFadingOut = true;

    if (ripple->radiusAnim) ripple->radiusAnim->stop();

    auto* fadeAnim = new QVariantAnimation(ripple);
    fadeAnim->setDuration(global::Duration::medium2);
    fadeAnim->setEasingCurve(QEasingCurve::OutQuad);
    fadeAnim->setStartValue(ripple->opacity);
    fadeAnim->setEndValue(0.0);

    QObject::connect(fadeAnim, &QVariantAnimation::valueChanged,
                     this, [this, ripple](const QVariant& v) {
                         ripple->opacity = v.toReal();
                         update();
                     });

    QObject::connect(fadeAnim, &QVariantAnimation::finished,
                     this, [this, ripple]() {
                         ripple->opacity = 0.0;
                         removeRipple(ripple);
                         update();
                     });

    ripple->fadeAnim = fadeAnim;
    fadeAnim->start(QAbstractAnimation::DeleteWhenStopped);
}

void Button::removeRipple(Ripple* ripple)
{
    if (!ripple) return;
    const int idx = m_ripples.indexOf(ripple);
    if (idx < 0) return;
    m_ripples.removeAt(idx);
    ripple->deleteLater();
}

void Button::cleanupFinishedRipples()
{
    for (int i = m_ripples.size() - 1; i >= 0; --i) {
        Ripple* r = m_ripples[i];
        if (!r->isFadingOut && r->opacity <= 0.0) {
            m_ripples.removeAt(i);
            r->deleteLater();
        }
    }
}

// ----------------------------------------------------------------------
// 事件
// ----------------------------------------------------------------------
void Button::enterEvent(QEnterEvent* event)
{
    QPushButton::enterEvent(event);
    m_state.hovered = true;
    update();
}

void Button::leaveEvent(QEvent* event)
{
    QPushButton::leaveEvent(event);
    m_state.hovered = false;
    update();
}

void Button::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && isEnabled()) {
        m_state.pressed = true;
        createRipple(event->position());
        update();
    }
    QPushButton::mousePressEvent(event);
}

void Button::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && isEnabled()) {
        m_state.pressed = false;

        for (Ripple* r : m_ripples) {
            if (!r->isFadingOut && r->fadeTimer && !r->fadeTimer->isActive())
                r->fadeTimer->start(rippleFadeDelay());
        }
        update();
    }
    QPushButton::mouseReleaseEvent(event);
}

void Button::focusInEvent(QFocusEvent* event)
{
    QPushButton::focusInEvent(event);
    m_state.focused = true;
    update();   // ★ 让焦点环出现
}

void Button::focusOutEvent(QFocusEvent* event)
{
    QPushButton::focusOutEvent(event);
    m_state.focused = false;
    update();   // ★ 让焦点环消失
}

void Button::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::EnabledChange) {
        updateShadowEffect();
        update();
    }
    QPushButton::changeEvent(event);
}

// ----------------------------------------------------------------------
// MaterialElement 钩子
// ----------------------------------------------------------------------
void Button::onUpdateColor(global::Scheme mode)
{
    Q_UNUSED(mode);
    updateShadowEffect();
    update();
}

} // namespace material::input
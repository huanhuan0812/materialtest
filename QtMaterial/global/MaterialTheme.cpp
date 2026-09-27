#include "MaterialTheme.hpp"
#include <QFontDatabase>
#include <QtMath>

// Material Color Utilities
#include "cpp/cam/hct.h"
#include "cpp/palettes/tones.h"

using namespace material_color_utilities;

namespace material::global {

// ---------------------------------------------------------------------
// 工具
// ---------------------------------------------------------------------
static QColor argbToQColor(Argb argb)
{
    return QColor((argb >> 16) & 0xFF,
                  (argb >> 8)  & 0xFF,
                   argb        & 0xFF);
}

static Argb qColorToArgb(const QColor &c)
{
    return (255u << 24) | (quint32(c.red()) << 16)
                        | (quint32(c.green()) << 8)
                        |  quint32(c.blue());
}

// ---------------------------------------------------------------------
// 构造 / 析构
// ---------------------------------------------------------------------
MaterialTheme::MaterialTheme(QColor seedColor, QWidget *parent)
    : QObject(parent)
    , m_seedColor(seedColor.isValid() ? seedColor : SeedColors::Purple)
{
    regenerateColors();
}

MaterialTheme::~MaterialTheme() = default;

// ---------------------------------------------------------------------
// 主题切换
// ---------------------------------------------------------------------
void MaterialTheme::setScheme(Scheme mode)
{
    if (m_mode == mode)
        return;
    m_mode = mode;
    emit themeChanged(m_mode);
    emit colorsUpdated();
}

void MaterialTheme::setSeedColor(QColor seedColor)
{
    if (!seedColor.isValid() || seedColor == m_seedColor)
        return;
    m_seedColor = seedColor;
    regenerateColors();
    emit colorsUpdated();
}

// ---------------------------------------------------------------------
// 核心：一次性生成亮/暗两套色
// ---------------------------------------------------------------------
void MaterialTheme::regenerateColors()
{
    const Argb seedArgb = qColorToArgb(m_seedColor);
    Hct hct(seedArgb);

    double hue    = hct.get_hue();
    double chroma = hct.get_chroma();
    if (chroma < 10.0) chroma = 48.0;
    if (hue < 0.0 || hue >= 360.0) hue = 280.0;

    TonalPalette primaryPalette(hue, chroma);

    double secondaryHue = fmod(hue + 30.0, 360.0);
    if (secondaryHue < 0.0) secondaryHue += 360.0;
    TonalPalette secondaryPalette(secondaryHue, chroma * 0.7);

    double tertiaryHue = fmod(hue + 60.0, 360.0);
    if (tertiaryHue < 0.0) tertiaryHue += 360.0;
    TonalPalette tertiaryPalette(tertiaryHue, chroma * 0.6);

    TonalPalette neutralPalette(hue, qMin(6.0, chroma * 0.12));
    TonalPalette neutralVariantPalette(hue, qMin(12.0, chroma * 0.2));
    TonalPalette errorPalette(0.0, 84.0);

    auto tone = [](TonalPalette &p, int t) {
        return argbToQColor(p.get(static_cast<double>(t)));
    };

    // 每个角色给出 (lightTone, darkTone)
    struct TonePair { int light; int dark; };

    auto fill = [&](ColorPair &pair, TonalPalette &palette, TonePair tp) {
        pair.light = tone(palette, tp.light);
        pair.dark  = tone(palette, tp.dark);
    };

    // Primary
    fill(m_Primary,              primaryPalette, { 40, 80 });
    fill(m_OnPrimary,            primaryPalette, {100, 20 });
    fill(m_PrimaryContainer,     primaryPalette, { 90, 30 });
    fill(m_OnPrimaryContainer,   primaryPalette, { 10, 90 });

    // Secondary
    fill(m_Secondary,            secondaryPalette, { 45, 75 });
    fill(m_OnSecondary,          secondaryPalette, {100, 20 });
    fill(m_SecondaryContainer,   secondaryPalette, { 92, 25 });
    fill(m_OnSecondaryContainer, secondaryPalette, { 10, 90 });

    // Tertiary
    fill(m_Tertiary,             tertiaryPalette, { 40, 80 });
    fill(m_OnTertiary,           tertiaryPalette, {100, 20 });
    fill(m_TertiaryContainer,    tertiaryPalette, { 90, 30 });
    fill(m_OnTertiaryContainer,  tertiaryPalette, { 10, 90 });

    // Error
    fill(m_Error,                errorPalette, { 40, 80 });
    fill(m_OnError,              errorPalette, {100, 20 });
    fill(m_ErrorContainer,       errorPalette, { 90, 30 });
    fill(m_OnErrorContainer,     errorPalette, { 10, 90 });

    // Surface 家族（neutral）
    fill(m_Surface,                   neutralPalette, { 98,  6 });
    fill(m_SurfaceDim,                neutralPalette, { 87,  6 });
    fill(m_SurfaceBright,             neutralPalette, { 98, 24 });
    fill(m_SurfaceContainerLowest,    neutralPalette, {100,  4 });
    fill(m_SurfaceContainerLow,       neutralPalette, { 96, 10 });
    fill(m_SurfaceContainer,          neutralPalette, { 94, 12 });
    fill(m_SurfaceContainerHigh,      neutralPalette, { 92, 17 });
    fill(m_SurfaceContainerHighest,   neutralPalette, { 90, 22 });
    fill(m_OnSurface,                 neutralPalette, { 10, 87 });
    fill(m_OnSurfaceVariant,          neutralVariantPalette, { 30, 80 });

    // Background
    fill(m_Background,                neutralPalette, { 98,  6 });
    fill(m_OnBackGround,              neutralPalette, { 10, 87 });

    // Outline
    fill(m_Outline,                   neutralVariantPalette, { 50, 60 });
    fill(m_OutlineVariant,            neutralVariantPalette, { 80, 30 });

    // 其它
    fill(m_SurfaceTint,               primaryPalette, { 40, 80 });
    fill(m_InverseSurface,            neutralPalette, { 20, 90 });
    fill(m_InverseOnSurface,          neutralPalette, { 95, 20 });
    fill(m_InversePrimary,            primaryPalette, { 80, 40 });

    // Scrim / Shadow 用固定透明度
    m_Scrim.light  = QColor(0, 0, 0, 100);
    m_Scrim.dark   = QColor(0, 0, 0, 200);
    m_Shadow.light = QColor(0, 0, 0,  60);
    m_Shadow.dark  = QColor(0, 0, 0, 160);
}

// ---------------------------------------------------------------------
// 设计令牌（与 QtMaterial 版保持一致）
// ---------------------------------------------------------------------
QFont typography(TypographyType type)
{
    struct TypeSpec { qreal size; QFont::Weight weight; qreal ls; };
    static const TypeSpec kSpecs[] = {
        {57.0, QFont::Weight::Normal, 0.0 },  // DisplayLarge
        {45.0, QFont::Weight::Normal, 0.0 },  // DisplayMedium
        {36.0, QFont::Weight::Normal, 0.0 },  // DisplaySmall
        {32.0, QFont::Weight::Normal, 0.0 },  // HeadlineLarge
        {28.0, QFont::Weight::Normal, 0.0 },  // HeadlineMedium
        {24.0, QFont::Weight::Normal, 0.0 },  // HeadlineSmall
        {22.0, QFont::Weight::Normal, 0.0 },  // TitleLarge
        {16.0, QFont::Weight::Medium, 0.15},  // TitleMedium
        {14.0, QFont::Weight::Medium, 0.1 },  // TitleSmall
        {16.0, QFont::Weight::Normal, 0.5 },  // BodyLarge
        {14.0, QFont::Weight::Normal, 0.25},  // BodyMedium
        {12.0, QFont::Weight::Normal, 0.4 },  // BodySmall
        {14.0, QFont::Weight::Medium, 0.1 },  // LabelLarge
        {12.0, QFont::Weight::Medium, 0.5 },  // LabelMedium
        {11.0, QFont::Weight::Medium, 0.5 },  // LabelSmall
    };
    const int idx = qBound(0, static_cast<int>(type),
                           static_cast<int>(TypographyType::LabelSmall));
    const TypeSpec &spec = kSpecs[idx];

    static const QString kFamily = [] {
        const QStringList families = QFontDatabase::families();
        return families.contains(QStringLiteral("Roboto"))
            ? QStringLiteral("Roboto") : QString();
    }();

    QFont f;
    if (!kFamily.isEmpty()) f.setFamily(kFamily);
    f.setPixelSize(qRound(spec.size));
    f.setWeight(spec.weight);
    f.setLetterSpacing(QFont::AbsoluteSpacing, spec.ls);
    return f;
}

int typographyLineHeight(TypographyType type)
{
    static const int kLH[] = {
        64, 52, 44, 40, 36, 32, 28,
        24, 20, 24, 20, 16, 20, 16, 16
    };
    const int idx = qBound(0, static_cast<int>(type),
                           static_cast<int>(TypographyType::LabelSmall));
    return kLH[idx];
}

int cornerRadius(CornerSize size)
{
    switch (size) {
    case CornerSize::Small:      return 4;
    case CornerSize::Medium:     return 8;
    case CornerSize::Large:      return 12;
    case CornerSize::ExtraLarge: return 16;
    case CornerSize::Full:       return 9999;
    }
    return 8;
}

} // namespace material::global
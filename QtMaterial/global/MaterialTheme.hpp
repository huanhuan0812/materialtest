#pragma once
#include <QObject>
#include <QColor>
#include <QWidget>
#include "SeedColors.hpp"
#include "Constant.hpp"

namespace material::global{

    class MaterialTheme : public QObject
    {
        Q_OBJECT
    public:
        explicit MaterialTheme(QColor seedColor=SeedColors::Purple,QWidget *parent=nullptr);
        ~MaterialTheme();

        void setSeedColor(QColor seedColor);
        void setScheme(Scheme mode);

        Scheme getScheme() const {return m_mode;}
        QColor getSeedColor() const {return m_seedColor;}

        //static getColors;
        QColor Primary()              const { return getColor(m_Primary);};
        QColor OnPrimary()            const { return getColor(m_OnPrimary);}
        QColor PrimaryContainer()     const { return getColor(m_PrimaryContainer);}
        QColor OnPrimaryContainer()   const { return getColor(m_OnPrimaryContainer);}
        
        QColor Secondary()            const { return getColor(m_Secondary);}
        QColor OnSecondary()          const { return getColor(m_OnSecondary);}
        QColor SecondaryContainer()   const { return getColor(m_SecondaryContainer);}
        QColor OnSecondaryContainer() const { return getColor(m_OnSecondaryContainer);}

        QColor Tertiary()             const { return getColor(m_Tertiary);}
        QColor OnTertiary()           const { return getColor(m_OnTertiary);}
        QColor TertiaryContainer()    const { return getColor(m_TertiaryContainer);}
        QColor OnTertiaryContainer()  const { return getColor(m_OnTertiaryContainer);}

        QColor Error()                const { return getColor(m_Error);}
        QColor OnError()              const { return getColor(m_OnError);}
        QColor ErrorContainer()       const { return getColor(m_ErrorContainer);}
        QColor OnErrorContainer()     const { return getColor(m_OnErrorContainer);}

        QColor Surface()              const { return getColor(m_Surface);}
        QColor SurfaceDim()           const { return getColor(m_SurfaceDim);}
        QColor SurfaceBright()        const { return getColor(m_SurfaceBright);}
        QColor SurfaceContainerLowest() const { return getColor(m_SurfaceContainerLowest);}
        QColor SurfaceContainerLow()  const { return getColor(m_SurfaceContainerLow);}
        QColor SurfaceContainer()     const { return getColor(m_SurfaceContainer);}
        QColor SurfaceContainerHigh() const { return getColor(m_SurfaceContainerHigh);}
        QColor SurfaceContainerHighest() const { return getColor(m_SurfaceContainerHighest);}
        QColor OnSurface()            const { return getColor(m_OnSurface);}
        QColor OnSurfaceVariant()     const { return getColor(m_OnSurfaceVariant);}
        QColor Background()           const { return getColor(m_Background);}
        QColor OnBackGround()         const { return getColor(m_OnBackGround);}
        QColor Outline()              const { return getColor(m_Outline);}
        QColor OutlineVariant()       const { return getColor(m_OutlineVariant);}
        QColor SurfaceTint()          const { return getColor(m_SurfaceTint);}
        QColor InverseSurface()       const { return getColor(m_InverseSurface);}
        QColor InverseOnSurface()     const { return getColor(m_InverseOnSurface);}
        QColor InversePrimary()       const { return getColor(m_InversePrimary);}

        QColor Scrim()                const { return getColor(m_Scrim);}
        QColor Shadow()               const { return getColor(m_Shadow);}

    private:

        ColorPair m_Primary;
        ColorPair m_OnPrimary;
        ColorPair m_PrimaryContainer;
        ColorPair m_OnPrimaryContainer;

        ColorPair m_Secondary;
        ColorPair m_OnSecondary;
        ColorPair m_SecondaryContainer;
        ColorPair m_OnSecondaryContainer;

        ColorPair m_Tertiary;
        ColorPair m_OnTertiary;
        ColorPair m_TertiaryContainer;
        ColorPair m_OnTertiaryContainer;

        ColorPair m_Error;
        ColorPair m_OnError;
        ColorPair m_ErrorContainer;
        ColorPair m_OnErrorContainer;

        ColorPair m_Surface;
        ColorPair m_SurfaceDim;
        ColorPair m_SurfaceBright;
        ColorPair m_SurfaceContainerLowest;
        ColorPair m_SurfaceContainerLow;
        ColorPair m_SurfaceContainer;
        ColorPair m_SurfaceContainerHigh;
        ColorPair m_SurfaceContainerHighest;
        ColorPair m_OnSurface;
        ColorPair m_OnSurfaceVariant;
        
        ColorPair m_Background;
        ColorPair m_OnBackGround;

        ColorPair m_Outline;
        ColorPair m_OutlineVariant;
        ColorPair m_SurfaceTint;
        ColorPair m_InverseSurface;
        ColorPair m_InverseOnSurface;
        ColorPair m_InversePrimary;

        ColorPair m_Scrim;
        ColorPair m_Shadow;

        QColor m_seedColor;

        Scheme m_mode=Scheme::Light;

        QColor getColor(const ColorPair& color) const {
            return m_mode == Scheme::Light ? color.light : color.dark;
        }
        void regenerateColors();   // 重新计算所有 ColorPair
        
    signals:
        void themeChanged(Scheme mode);
        void colorsUpdated();

    public slots:
        void updateTheme(Scheme mode) { setScheme(mode); }
    };

    QFont typography(TypographyType type);
    int typographyLineHeight(TypographyType type);
    int cornerRadius(CornerSize size);
}
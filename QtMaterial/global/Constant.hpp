#pragma once
#include <QObject>
#include <QColor>

namespace material::global{
    enum class Scheme{
        Light,
        Dark
    };
    struct ColorPair{
        QColor light;
        QColor dark;
    };

    enum class TypographyType {
        DisplayLarge,    // 0
        DisplayMedium,   // 1
        DisplaySmall,    // 2
        HeadlineLarge,   // 3
        HeadlineMedium,  // 4
        HeadlineSmall,   // 5
        TitleLarge,      // 6
        TitleMedium,     // 7
        TitleSmall,      // 8
        BodyLarge,       // 9
        BodyMedium,      // 10
        BodySmall,       // 11
        LabelLarge,      // 12
        LabelMedium,     // 13
        LabelSmall       // 14
    };

    enum class CornerSize {
        Small,      /*!< 4 px    */
        Medium,     /*!< 8 px    */
        Large,      /*!< 12 px   */
        ExtraLarge, /*!< 16 px   */
        Full        /*!< 9999 px */
    };

    //动效时常
    class Duration{
    public:
        static inline int  short1 = 20;
        static inline int  short2 = 100;
        static inline int  short3 = 150;
        static inline int  short4 = 200;
        static inline int  medium1 = 250;
        static inline int  medium2 = 300;
        static inline int  medium3 = 350;
        static inline int  medium4 = 400;
        static inline int  long1 = 450;
        static inline int  long2 = 500;
        static inline int  long3 = 550;
        static inline int  long4 = 600;
    };
}
#pragma once

#include <QColor>
#include <QFont>
#include "SeedColors.hpp"

namespace material{

    inline QColor GlobalSeedColor=SeedColors::Purple;
    inline int fontId;
    inline QString MaterialFontFamily;
    inline QFont MaterialFont;
    /**
     * use this function to initialize the theme with a seed color and optionally read from local configuration
     * @param seedColor The default seed color to use if no local configuration is found
     * @param getLocal Whether to attempt to read the seed color from local configuration files
     * @note The local configuration files are searched in the following order:
     *       1. $HOME/.qmaterial
     *       2. ./qmaterial
     * @note Please call this function at the beginning of your application to ensure the theme is initialized properly.
     */
    void initTheme(QColor seedColor=SeedColors::Purple, bool getLocal=true);
}
#include "init.hpp"
#include <QFile>
#include <QtCore/qstandardpaths.h>
#include <QStandardPaths>
#include <QTextStream>
#include <QDebug>
#include <QFontDatabase>
#include <QDir>
#include "GlobalTheme.hpp"
#include "../utils/stringToColor.hpp"

namespace material {

void initTheme(QColor seedColor, bool getLocal) {
    bool hasLocal = false;
    QString colorStr;
    
    if (getLocal) {
        // 使用 Qt 方式构建路径
        QStringList searchPaths;
        searchPaths << QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.qmaterial"
                    << QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation) + "/.qmaterial";
        
        for (const QString& filePath : searchPaths) {
            QFile file(filePath);
            if (file.exists()) {
                // 读取文件内容
                if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QTextStream in(&file);
                    colorStr = in.readAll().trimmed();  // 去除首尾空白
                    file.close();
                    
                    if (!colorStr.isEmpty()) {
                        hasLocal = true;
                        break;  // 找到有效文件就退出
                    }
                }
            }
        }
    }

    // 处理颜色值
    if (hasLocal) {
        QColor parsedColor;
        if (material::chores::stringToColor(colorStr, parsedColor)) {
            GlobalSeedColor = parsedColor;
        } else {
            // 解析失败，使用默认颜色
            qDebug() << "Failed to parse color from local file. Using default seed color.";
            GlobalSeedColor = seedColor;
        }
    } else {
        GlobalSeedColor = seedColor;
    }

    qDebug() << "Global seed color set to:" << GlobalSeedColor.name();
    // qDebug() << QDir(":/").entryList(QDir::Files);   // 看根目录下有什么
    // qDebug() << QFile::exists(":/MaterialIconsRound-Regular.otf");
    // 加载字体
    material::fontId = QFontDatabase::addApplicationFont(":/MaterialIconsRound-Regular.otf");
    if (material::fontId == -1) {
        qDebug() << "Failed to load Material Icons font.";
    } else {
        material::MaterialFontFamily = QFontDatabase::applicationFontFamilies(material::fontId).first();
        material::MaterialFont = QFont(material::MaterialFontFamily);
        material::MaterialFont.setPixelSize(24);
        qDebug() << "Loaded Material Icons font family:" << material::MaterialFontFamily;
    }
    
    // 初始化主题实例
    material::global::MaterialTheme& theme = material::globalTheme();
}
}
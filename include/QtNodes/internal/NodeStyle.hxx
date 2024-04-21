///
/// @file NodeStyle.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef NODE_STYLE_HXX_59AA01ECAE2C
#define NODE_STYLE_HXX_59AA01ECAE2C

#include <QtGui/QColor>

#include "Export.hxx"
#include "Style.hxx"

namespace QtNodes {

///
/// @brief NodeStyle
///
class NODE_EDITOR_PUBLIC NodeStyle : public Style
{
public:
    NodeStyle();

    NodeStyle(QString jsonText);

    NodeStyle(const QJsonObject& json);

    virtual ~NodeStyle() = default;

public:
    static void setNodeStyle(QString jsonText);

public:
    void loadJson(const QJsonObject& json) override;

    QJsonObject toJson() const override;

public:
    QColor NormalBoundaryColor;
    QColor SelectedBoundaryColor;
    QColor GradientColor0;
    QColor GradientColor1;
    QColor GradientColor2;
    QColor GradientColor3;
    QColor ShadowColor;
    QColor FontColor;
    QColor FontColorFaded;
    QColor ConnectionPointColor;
    QColor FilledConnectionPointColor;
    QColor WarningColor;
    QColor ErrorColor;

    float PenWidth;
    float HoveredPenWidth;
    float ConnectionPointDiameter;
    float Opacity;
};

}  // namespace QtNodes

#endif  // NODE_STYLE_HXX_59AA01ECAE2C

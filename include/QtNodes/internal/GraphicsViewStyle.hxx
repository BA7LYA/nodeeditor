///
/// @file GraphicsViewStyle.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef GRAPHICS_VIEW_STYLE_HXX_24E685A06FB1
#define GRAPHICS_VIEW_STYLE_HXX_24E685A06FB1

#include <QtGui/QColor>

#include "Export.hxx"
#include "Style.hxx"

namespace QtNodes {

///
/// @brief GraphicsViewStyle
///
class NODE_EDITOR_PUBLIC GraphicsViewStyle : public Style
{
public:
    GraphicsViewStyle();

    GraphicsViewStyle(QString jsonText);

    ~GraphicsViewStyle() = default;

public:
    static void setStyle(QString jsonText);

private:
    void loadJson(const QJsonObject& json) override;

    QJsonObject toJson() const override;

public:
    QColor BackgroundColor;
    QColor FineGridColor;
    QColor CoarseGridColor;
};

}  // namespace QtNodes

#endif  // GRAPHICS_VIEW_STYLE_HXX_24E685A06FB1

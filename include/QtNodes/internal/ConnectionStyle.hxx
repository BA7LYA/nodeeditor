///
/// @file ConnectionStyle.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef CONNECTION_STYLE_HXX_84C9F1B15538
#define CONNECTION_STYLE_HXX_84C9F1B15538

#include <QtGui/QColor>

#include "Export.hxx"
#include "Style.hxx"

namespace QtNodes {

///
/// @brief ConnectionStyle
///
class NODE_EDITOR_PUBLIC ConnectionStyle : public Style
{
public:
    ConnectionStyle();

    ConnectionStyle(QString jsonText);

    ~ConnectionStyle() = default;

public:
    static void setConnectionStyle(QString jsonText);

    void loadJson(const QJsonObject& json) override;

    QJsonObject toJson() const override;

    QColor constructionColor() const;
    QColor normalColor() const;
    QColor normalColor(QString typeId) const;
    QColor selectedColor() const;
    QColor selectedHaloColor() const;
    QColor hoveredColor() const;

    float lineWidth() const;
    float constructionLineWidth() const;
    float pointDiameter() const;

    bool useDataDefinedColors() const;

private:
    QColor ConstructionColor;
    QColor NormalColor;
    QColor SelectedColor;
    QColor SelectedHaloColor;
    QColor HoveredColor;

    float LineWidth;
    float ConstructionLineWidth;
    float PointDiameter;

    bool UseDataDefinedColors;
};

}  // namespace QtNodes

#endif  // CONNECTION_STYLE_HXX_84C9F1B15538

#pragma once

#include <QtGui/QColor>

#include "QtNodes/Export.hxx"
#include "QtNodes/Style.hxx"

namespace QtNodes {

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

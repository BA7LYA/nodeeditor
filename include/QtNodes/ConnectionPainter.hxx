#pragma once

#include <QtGui/QPainter>
#include <QtGui/QPainterPath>

#include "QtNodes/Definitions.hxx"

namespace QtNodes {

class ConnectionGeometry;
class ConnectionGraphicsObject;

class ConnectionPainter
{
public:
    static void paint(QPainter* painter, const ConnectionGraphicsObject& cgo);

    static QPainterPath getPainterStroke(const ConnectionGraphicsObject& cgo);
};

}  // namespace QtNodes

///
/// @file ConnectionPainter.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef CONNECTION_PAINTER_HXX_48F3ACE3DAFF
#define CONNECTION_PAINTER_HXX_48F3ACE3DAFF

#include <QtGui/QPainter>
#include <QtGui/QPainterPath>

#include "Definitions.hxx"

namespace QtNodes {

class ConnectionGeometry;
class ConnectionGraphicsObject;

///
/// @brief ConnectionPainter
///
class ConnectionPainter
{
public:
    static void paint(QPainter* painter, const ConnectionGraphicsObject& cgo);

    static QPainterPath getPainterStroke(const ConnectionGraphicsObject& cgo);
};

}  // namespace QtNodes

#endif  // CONNECTION_PAINTER_HXX_48F3ACE3DAFF

///
/// @file locateNode.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef LOCATE_NODE_HXX_1F824627DD43
#define LOCATE_NODE_HXX_1F824627DD43

#include <QtCore/QPointF>
#include <QtGui/QTransform>

class QGraphicsScene;

namespace QtNodes {

class NodeGraphicsObject;

///
/// @brief
/// @param scenePoint
/// @param scene
/// @param viewTransform
/// @return
///
NodeGraphicsObject* locateNodeAt(
    QPointF           scenePoint,
    QGraphicsScene&   scene,
    const QTransform& viewTransform
);

}  // namespace QtNodes

#endif  // LOCATE_NODE_HXX_1F824627DD43

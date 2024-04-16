///
/// @file locateNode.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#include "locateNode.hxx"

#include <QtCore/QList>
#include <QtWidgets/QGraphicsScene>
#include <vector>

#include "NodeGraphicsObject.hxx"

namespace QtNodes {

NodeGraphicsObject* locateNodeAt(
    QPointF           scenePoint,
    QGraphicsScene&   scene,
    const QTransform& viewTransform
)
{
    // items under cursor
    QList<QGraphicsItem*> items = scene.items(
        scenePoint,
        Qt::IntersectsItemShape,
        Qt::DescendingOrder,
        viewTransform
    );

    // items convertable to NodeGraphicsObject
    std::vector<QGraphicsItem*> filteredItems;

    std::copy_if(
        items.begin(),
        items.end(),
        std::back_inserter(filteredItems),
        [](QGraphicsItem* item)
        { return (qgraphicsitem_cast<NodeGraphicsObject*>(item) != nullptr); }
    );

    NodeGraphicsObject* node = nullptr;

    if (!filteredItems.empty())
    {
        QGraphicsItem* graphicsItem = filteredItems.front();
        node = dynamic_cast<NodeGraphicsObject*>(graphicsItem);
    }

    return node;
}

}  // namespace QtNodes

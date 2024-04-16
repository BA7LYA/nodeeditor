///
/// @file AbstractNodePainter.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef ABSTRACT_NODE_PAINTER_HXX_9B6A1F38B0D6
#define ABSTRACT_NODE_PAINTER_HXX_9B6A1F38B0D6

#include <QPainter>

#include "Export.hxx"

class QPainter;

namespace QtNodes {

class NodeGraphicsObject;
class NodeDataModel;

///
/// @brief Class enables custom painting.
///
class NODE_EDITOR_PUBLIC AbstractNodePainter
{
public:
    virtual ~AbstractNodePainter() = default;

    /**
     * Reimplement this function in order to have a custom painting.
     *
     * Useful functions:
     * `NodeGraphicsObject::nodeScene()->nodeGeometry()`
     * `NodeGraphicsObject::graphModel()`
     */
    virtual void paint(QPainter* painter, NodeGraphicsObject& ngo) const = 0;
};

}  // namespace QtNodes

#endif  // ABSTRACT_NODE_PAINTER_HXX_9B6A1F38B0D6

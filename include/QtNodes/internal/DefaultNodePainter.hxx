///
/// @file DefaultNodePainter.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef DEFAULT_NODE_PAINTER_HXX_7DC3639B0482
#define DEFAULT_NODE_PAINTER_HXX_7DC3639B0482

#include <QtGui/QPainter>

#include "AbstractNodePainter.hxx"
#include "Definitions.hxx"

namespace QtNodes {

class BasicGraphicsScene;
class GraphModel;
class NodeGeometry;
class NodeGraphicsObject;
class NodeState;

///
/// @brief Lightweight class incapsulating paint code.
///
class NODE_EDITOR_PUBLIC DefaultNodePainter : public AbstractNodePainter
{
public:
    void paint(QPainter* painter, NodeGraphicsObject& ngo) const override;
    void drawNodeRect(QPainter* painter, NodeGraphicsObject& ngo) const;
    void drawConnectionPoints(QPainter* painter, NodeGraphicsObject& ngo) const;
    void drawFilledConnectionPoints(QPainter* painter, NodeGraphicsObject& ngo)
        const;
    void drawNodeCaption(QPainter* painter, NodeGraphicsObject& ngo) const;
    void drawEntryLabels(QPainter* painter, NodeGraphicsObject& ngo) const;
    void drawResizeRect(QPainter* painter, NodeGraphicsObject& ngo) const;
};

}  // namespace QtNodes

#endif  // DEFAULT_NODE_PAINTER_HXX_7DC3639B0482

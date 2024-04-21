///
/// @file DefaultVerticalNodeGeometry.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef DEFAULT_VERTICAL_NODE_GEOMETRY_HXX_19596ECF04B4
#define DEFAULT_VERTICAL_NODE_GEOMETRY_HXX_19596ECF04B4

#include <QtGui/QFontMetrics>

#include "AbstractNodeGeometry.hxx"

namespace QtNodes {

class AbstractGraphModel;
class BasicGraphicsScene;

///
/// @brief DefaultVerticalNodeGeometry
///
class NODE_EDITOR_PUBLIC DefaultVerticalNodeGeometry
    : public AbstractNodeGeometry
{
public:
    DefaultVerticalNodeGeometry(AbstractGraphModel& graphModel);

    QSize size(const NodeId nodeId) const override;

    void recomputeSize(const NodeId nodeId) const override;

    QPointF portPosition(
        const NodeId    nodeId,
        const PortType  portType,
        const PortIndex index
    ) const override;

    QPointF portTextPosition(
        const NodeId    nodeId,
        const PortType  portType,
        const PortIndex PortIndex
    ) const override;

    QPointF captionPosition(const NodeId nodeId) const override;
    QPointF widgetPosition(const NodeId nodeId) const override;

    QRectF captionRect(const NodeId nodeId) const override;

    QRect resizeHandleRect(const NodeId nodeId) const override;

private:
    QRectF portTextRect(
        const NodeId    nodeId,
        const PortType  portType,
        const PortIndex portIndex
    ) const;

    /// Finds
    unsigned int maxHorizontalPortsExtent(const NodeId nodeId) const;

    unsigned int maxPortsTextAdvance(
        const NodeId   nodeId,
        const PortType portType
    ) const;

    unsigned int portCaptionsHeight(
        const NodeId   nodeId,
        const PortType portType
    ) const;

private:
    // Some variables are mutable because we need to change drawing
    // metrics corresponding to fontMetrics but this doesn't change
    // constness of the Node.

    mutable unsigned int _portSize;
    unsigned int         _portSpasing;
    mutable QFontMetrics _fontMetrics;
    mutable QFontMetrics _boldFontMetrics;
};

}  // namespace QtNodes

#endif  // DEFAULT_VERTICAL_NODE_GEOMETRY_HXX_19596ECF04B4

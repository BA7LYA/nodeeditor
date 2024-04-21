///
/// @file DefaultHorizontalNodeGeometry.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef DEFAULT_HORIZONTAL_NODE_GEOMETRY_HXX_6241282C8AAA
#define DEFAULT_HORIZONTAL_NODE_GEOMETRY_HXX_6241282C8AAA

#include <QtGui/QFontMetrics>

#include "AbstractNodeGeometry.hxx"

namespace QtNodes {

class AbstractGraphModel;
class BasicGraphicsScene;

///
/// @brief DefaultHorizontalNodeGeometry
///
class NODE_EDITOR_PUBLIC DefaultHorizontalNodeGeometry
    : public AbstractNodeGeometry
{
public:
    DefaultHorizontalNodeGeometry(AbstractGraphModel& graphModel);

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

    ///
    /// @brief Finds max number of ports and multiplies by (a port height +
    /// interval).
    ///
    /// @param nodeId
    /// @return unsigned int
    ///
    unsigned int maxVerticalPortsExtent(const NodeId nodeId) const;

    unsigned int maxPortsTextAdvance(
        const NodeId   nodeId,
        const PortType portType
    ) const;

private:
    // Some variables are mutable because we need to change drawing metrics
    // corresponding to fontMetrics but this doesn't change constness of the
    // Node.

    mutable unsigned int _portSize;
    unsigned int         _portSpasing;
    mutable QFontMetrics _fontMetrics;
    mutable QFontMetrics _boldFontMetrics;
};

}  // namespace QtNodes

#endif  // DEFAULT_HORIZONTAL_NODE_GEOMETRY_HXX_6241282C8AAA

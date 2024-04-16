///
/// @file AbstractNodeGeometry.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#include "AbstractNodeGeometry.hxx"

#include <cmath>
#include <QMargins>

#include "AbstractGraphModel.hxx"
#include "StyleCollection.hxx"

namespace QtNodes {

AbstractNodeGeometry::AbstractNodeGeometry(AbstractGraphModel& graphModel)
    : _graphModel(graphModel)
{
    //
}

QRectF AbstractNodeGeometry::boundingRect(const NodeId nodeId) const
{
    QSize s = size(nodeId);

    double ratio = 0.20;

    int widthMargin  = s.width() * ratio;
    int heightMargin = s.height() * ratio;

    QMargins margins(widthMargin, heightMargin, widthMargin, heightMargin);

    QRectF r(QPointF(0, 0), s);

    return r.marginsAdded(margins);
}

QPointF AbstractNodeGeometry::portScenePosition(
    const NodeId      nodeId,
    const PortType    portType,
    const PortIndex   index,
    const QTransform& t
) const
{
    QPointF result = portPosition(nodeId, portType, index);

    return t.map(result);
}

PortIndex AbstractNodeGeometry::checkPortHit(
    const NodeId   nodeId,
    const PortType portType,
    const QPointF  nodePoint
) const
{
    const auto& nodeStyle = StyleCollection::nodeStyle();

    PortIndex result = InvalidPortIndex;

    if (portType == PortType::None)
    {
        return result;
    }

    const double tolerance = 2.0 * nodeStyle.ConnectionPointDiameter;

    const size_t n = _graphModel.nodeData<unsigned int>(
        nodeId,
        (portType == PortType::Out) ? NodeRole::OutPortCount
                                    : NodeRole::InPortCount
    );

    for (unsigned int portIndex = 0; portIndex < n; ++portIndex)
    {
        auto pp = portPosition(nodeId, portType, portIndex);

        QPointF p        = pp - nodePoint;
        auto    distance = std::sqrt(QPointF::dotProduct(p, p));

        if (distance < tolerance)
        {
            result = portIndex;
            break;
        }
    }

    return result;
}

}  // namespace QtNodes

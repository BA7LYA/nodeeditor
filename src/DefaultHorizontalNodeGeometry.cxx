///
/// @file DefaultHorizontalNodeGeometry.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#include "DefaultHorizontalNodeGeometry.hxx"

#include <QPoint>
#include <QRect>
#include <QWidget>

#include "AbstractGraphModel.hxx"
#include "NodeData.hxx"

namespace QtNodes {

DefaultHorizontalNodeGeometry::DefaultHorizontalNodeGeometry(
    AbstractGraphModel& graphModel
)
    : AbstractNodeGeometry(graphModel)
    , _portSize(20)
    , _portSpasing(10)
    , _fontMetrics(QFont())
    , _boldFontMetrics(QFont())
{
    QFont f;
    f.setBold(true);
    _boldFontMetrics = QFontMetrics(f);

    _portSize = _fontMetrics.height();
}

QSize DefaultHorizontalNodeGeometry::size(const NodeId nodeId) const
{
    return _graphModel.nodeData<QSize>(nodeId, NodeRole::Size);
}

void DefaultHorizontalNodeGeometry::recomputeSize(const NodeId nodeId) const
{
    unsigned int height = maxVerticalPortsExtent(nodeId);

    if (auto w = _graphModel.nodeData<QWidget*>(nodeId, NodeRole::Widget))
    {
        height = std::max(height, static_cast<unsigned int>(w->height()));
    }

    const QRectF capRect = captionRect(nodeId);

    height += capRect.height();

    height += _portSpasing;  // space above caption
    height += _portSpasing;  // space below caption

    unsigned int inPortWidth  = maxPortsTextAdvance(nodeId, PortType::In);
    unsigned int outPortWidth = maxPortsTextAdvance(nodeId, PortType::Out);

    unsigned int width = inPortWidth + outPortWidth + 4 * _portSpasing;

    if (auto w = _graphModel.nodeData<QWidget*>(nodeId, NodeRole::Widget))
    {
        width += w->width();
    }

    width = std::max(
        width,
        static_cast<unsigned int>(capRect.width()) + 2 * _portSpasing
    );

    QSize size(width, height);

    _graphModel.setNodeData(nodeId, NodeRole::Size, size);
}

QPointF DefaultHorizontalNodeGeometry::portPosition(
    const NodeId    nodeId,
    const PortType  portType,
    const PortIndex portIndex
) const
{
    const unsigned int step = _portSize + _portSpasing;

    QPointF result;

    double totalHeight = 0.0;

    totalHeight += captionRect(nodeId).height();
    totalHeight += _portSpasing;

    totalHeight += step * portIndex;
    totalHeight += step / 2.0;

    QSize size = _graphModel.nodeData<QSize>(nodeId, NodeRole::Size);

    switch (portType)
    {
    case PortType::In:
    {
        double x = 0.0;

        result = QPointF(x, totalHeight);
        break;
    }

    case PortType::Out:
    {
        double x = size.width();

        result = QPointF(x, totalHeight);
        break;
    }

    default: break;
    }

    return result;
}

QPointF DefaultHorizontalNodeGeometry::portTextPosition(
    const NodeId    nodeId,
    const PortType  portType,
    const PortIndex portIndex
) const
{
    QPointF p = portPosition(nodeId, portType, portIndex);

    QRectF rect = portTextRect(nodeId, portType, portIndex);

    p.setY(p.y() + rect.height() / 4.0);

    QSize size = _graphModel.nodeData<QSize>(nodeId, NodeRole::Size);

    switch (portType)
    {
    case PortType::In: p.setX(_portSpasing); break;

    case PortType::Out:
        p.setX(size.width() - _portSpasing - rect.width());
        break;

    default: break;
    }

    return p;
}

QRectF DefaultHorizontalNodeGeometry::captionRect(const NodeId nodeId) const
{
    if (!_graphModel.nodeData<bool>(nodeId, NodeRole::CaptionVisible))
    {
        return QRect();
    }

    QString name = _graphModel.nodeData<QString>(nodeId, NodeRole::Caption);

    return _boldFontMetrics.boundingRect(name);
}

QPointF DefaultHorizontalNodeGeometry::captionPosition(const NodeId nodeId
) const
{
    QSize size = _graphModel.nodeData<QSize>(nodeId, NodeRole::Size);
    return QPointF(
        0.5 * (size.width() - captionRect(nodeId).width()),
        0.5 * _portSpasing + captionRect(nodeId).height()
    );
}

QPointF DefaultHorizontalNodeGeometry::widgetPosition(const NodeId nodeId) const
{
    QSize size = _graphModel.nodeData<QSize>(nodeId, NodeRole::Size);

    unsigned int captionHeight = captionRect(nodeId).height();

    if (auto w = _graphModel.nodeData<QWidget*>(nodeId, NodeRole::Widget))
    {
        // If the widget wants to use as much vertical space as possible,
        // place it immediately after the caption.
        if (w->sizePolicy().verticalPolicy() & QSizePolicy::ExpandFlag)
        {
            return QPointF(
                2.0 * _portSpasing + maxPortsTextAdvance(nodeId, PortType::In),
                captionHeight
            );
        }
        else
        {
            return QPointF(
                2.0 * _portSpasing + maxPortsTextAdvance(nodeId, PortType::In),
                (captionHeight + size.height() - w->height()) / 2.0
            );
        }
    }
    return QPointF();
}

QRect DefaultHorizontalNodeGeometry::resizeHandleRect(const NodeId nodeId) const
{
    QSize size = _graphModel.nodeData<QSize>(nodeId, NodeRole::Size);

    unsigned int rectSize = 7;

    return QRect(
        size.width() - _portSpasing,
        size.height() - _portSpasing,
        rectSize,
        rectSize
    );
}

QRectF DefaultHorizontalNodeGeometry::portTextRect(
    const NodeId    nodeId,
    const PortType  portType,
    const PortIndex portIndex
) const
{
    QString s;
    if (_graphModel.portData<bool>(
            nodeId,
            portType,
            portIndex,
            PortRole::CaptionVisible
        ))
    {
        s = _graphModel.portData<QString>(
            nodeId,
            portType,
            portIndex,
            PortRole::Caption
        );
    }
    else
    {
        auto portData
            = _graphModel
                  .portData(nodeId, portType, portIndex, PortRole::DataType);

        s = portData.value<NodeDataType>().name;
    }

    return _fontMetrics.boundingRect(s);
}

unsigned int DefaultHorizontalNodeGeometry::maxVerticalPortsExtent(
    const NodeId nodeId
) const
{
    PortCount nInPorts
        = _graphModel.nodeData<PortCount>(nodeId, NodeRole::InPortCount);

    PortCount nOutPorts
        = _graphModel.nodeData<PortCount>(nodeId, NodeRole::OutPortCount);

    unsigned int maxNumOfEntries = std::max(nInPorts, nOutPorts);
    unsigned int step            = _portSize + _portSpasing;

    return step * maxNumOfEntries;
}

unsigned int DefaultHorizontalNodeGeometry::maxPortsTextAdvance(
    const NodeId   nodeId,
    const PortType portType
) const
{
    unsigned int width = 0;

    const size_t n
        = _graphModel
              .nodeData(
                  nodeId,
                  (portType == PortType::Out) ? NodeRole::OutPortCount
                                              : NodeRole::InPortCount
              )
              .toUInt();

    for (PortIndex portIndex = 0ul; portIndex < n; ++portIndex)
    {
        QString name;

        if (_graphModel.portData<bool>(
                nodeId,
                portType,
                portIndex,
                PortRole::CaptionVisible
            ))
        {
            name = _graphModel.portData<QString>(
                nodeId,
                portType,
                portIndex,
                PortRole::Caption
            );
        }
        else
        {
            NodeDataType portData = _graphModel.portData<NodeDataType>(
                nodeId,
                portType,
                portIndex,
                PortRole::DataType
            );

            name = portData.name;
        }

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        width = std::max(unsigned(_fontMetrics.horizontalAdvance(name)), width);
#else
        width = std::max(unsigned(_fontMetrics.width(name)), width);
#endif
    }

    return width;
}

}  // namespace QtNodes

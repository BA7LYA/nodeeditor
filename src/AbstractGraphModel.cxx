#include "QtNodes/AbstractGraphModel.hxx"

#include <QtNodes/ConnectionIdUtils.hxx>

namespace QtNodes {

void AbstractGraphModel::portsAboutToBeDeleted(
    const NodeId    nodeId,
    const PortType  portType,
    const PortIndex first,
    const PortIndex last
)
{
    _shiftedByDynamicPortsConnections.clear();

    auto portCountRole = portType == PortType::In ? NodeRole::InPortCount
                                                  : NodeRole::OutPortCount;

    unsigned int portCount = nodeData(nodeId, portCountRole).toUInt();

    if (first > portCount - 1)
    {
        return;
    }

    if (last < first)
    {
        return;
    }

    auto clampedLast = std::min(last, portCount - 1);

    for (PortIndex portIndex = first; portIndex <= clampedLast; ++portIndex)
    {
        std::unordered_set<ConnectionId> conns
            = connections(nodeId, portType, portIndex);

        for (auto connectionId : conns)
        {
            deleteConnection(connectionId);
        }
    }

    const std::size_t nRemovedPorts = clampedLast - first + 1;

    for (PortIndex portIndex = clampedLast + 1; portIndex < portCount;
         ++portIndex)
    {
        std::unordered_set<ConnectionId> conns
            = connections(nodeId, portType, portIndex);

        for (auto connectionId : conns)
        {
            // Erases the information about the port on one side;
            auto c = makeIncompleteConnectionId(connectionId, portType);

            c = makeCompleteConnectionId(c, nodeId, portIndex - nRemovedPorts);

            _shiftedByDynamicPortsConnections.push_back(c);

            deleteConnection(connectionId);
        }
    }
}

void AbstractGraphModel::portsDeleted()
{
    for (const auto connectionId : _shiftedByDynamicPortsConnections)
    {
        addConnection(connectionId);
    }

    _shiftedByDynamicPortsConnections.clear();
}

void AbstractGraphModel::portsAboutToBeInserted(
    const NodeId    nodeId,
    const PortType  portType,
    const PortIndex first,
    const PortIndex last
)
{
    _shiftedByDynamicPortsConnections.clear();

    auto portCountRole = portType == PortType::In ? NodeRole::InPortCount
                                                  : NodeRole::OutPortCount;

    unsigned int portCount = nodeData(nodeId, portCountRole).toUInt();

    if (first > portCount)
    {
        return;
    }

    if (last < first)
    {
        return;
    }

    const std::size_t nNewPorts = last - first + 1;

    for (PortIndex portIndex = first; portIndex < portCount; ++portIndex)
    {
        std::unordered_set<ConnectionId> conns
            = connections(nodeId, portType, portIndex);

        for (auto connectionId : conns)
        {
            // Erases the information about the port on one side;
            auto c = makeIncompleteConnectionId(connectionId, portType);

            c = makeCompleteConnectionId(c, nodeId, portIndex + nNewPorts);

            _shiftedByDynamicPortsConnections.push_back(c);

            deleteConnection(connectionId);
        }
    }
}

void AbstractGraphModel::portsInserted()
{
    for (const auto connectionId : _shiftedByDynamicPortsConnections)
    {
        addConnection(connectionId);
    }

    _shiftedByDynamicPortsConnections.clear();
}

}  // namespace QtNodes

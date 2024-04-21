///
/// @file ConnectionIdUtils.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef CONNECTION_ID_UTILS_HXX_8EC34A10D393
#define CONNECTION_ID_UTILS_HXX_8EC34A10D393

#include <iostream>
#include <QJsonObject>
#include <string>

#include "Definitions.hxx"

namespace QtNodes {

///
/// @brief
/// @param portType
/// @param connectionId
/// @return PortIndex
///
inline PortIndex getNodeId(PortType portType, ConnectionId connectionId)
{
    NodeId id = InvalidNodeId;

    if (portType == PortType::Out)
    {
        id = connectionId.outNodeId;
    }
    else if (portType == PortType::In)
    {
        id = connectionId.inNodeId;
    }

    return id;
}

///
/// @brief
/// @param portType
/// @param connectionId
/// @return PortIndex
///
inline PortIndex getPortIndex(PortType portType, ConnectionId connectionId)
{
    PortIndex index = InvalidPortIndex;

    if (portType == PortType::Out)
    {
        index = connectionId.outPortIndex;
    }
    else if (portType == PortType::In)
    {
        index = connectionId.inPortIndex;
    }

    return index;
}

///
/// @brief
/// @param port
/// @return
///
inline PortType oppositePort(PortType port)
{
    PortType result = PortType::None;

    switch (port)
    {
    case PortType::In: result = PortType::Out; break;

    case PortType::Out: result = PortType::In; break;

    case PortType::None: result = PortType::None; break;

    default: break;
    }
    return result;
}

///
/// @brief 
/// @param index 
/// @return 
///
inline bool isPortIndexValid(PortIndex index)
{
    return index != InvalidPortIndex;
}

///
/// @brief 
/// @param portType 
/// @return 
///
inline bool isPortTypeValid(PortType portType)
{
    return portType != PortType::None;
}

///
/// @brief Creates a connection Id instance filled just on one side.
///
/// @param connectedNodeId
/// @param connectedPort
/// @param connectedPortIndex
/// @return ConnectionId
///
inline ConnectionId makeIncompleteConnectionId(
    const NodeId    connectedNodeId,
    const PortType  connectedPort,
    const PortIndex connectedPortIndex
)
{
    return (connectedPort == PortType::In) ? ConnectionId { InvalidNodeId,
                                                            InvalidPortIndex,
                                                            connectedNodeId,
                                                            connectedPortIndex }
                                           : ConnectionId { connectedNodeId,
                                                            connectedPortIndex,
                                                            InvalidNodeId,
                                                            InvalidPortIndex };
}

///
/// @brief Turns a full connection Id into an incomplete one by removing the
/// data on the given side.
///
/// @param connectionId
/// @param portToDisconnect
/// @return ConnectionId
///
inline ConnectionId makeIncompleteConnectionId(
    ConnectionId   connectionId,
    const PortType portToDisconnect
)
{
    if (portToDisconnect == PortType::Out)
    {
        connectionId.outNodeId    = InvalidNodeId;
        connectionId.outPortIndex = InvalidPortIndex;
    }
    else
    {
        connectionId.inNodeId    = InvalidNodeId;
        connectionId.inPortIndex = InvalidPortIndex;
    }

    return connectionId;
}

///
/// @brief
/// @param incompleteConnectionId
/// @param nodeId
/// @param portIndex
/// @return
///
inline ConnectionId makeCompleteConnectionId(
    ConnectionId    incompleteConnectionId,
    const NodeId    nodeId,
    const PortIndex portIndex
)
{
    if (incompleteConnectionId.outNodeId == InvalidNodeId)
    {
        incompleteConnectionId.outNodeId    = nodeId;
        incompleteConnectionId.outPortIndex = portIndex;
    }
    else
    {
        incompleteConnectionId.inNodeId    = nodeId;
        incompleteConnectionId.inPortIndex = portIndex;
    }

    return incompleteConnectionId;
}

///
/// @brief
/// @param ostr
/// @param connectionId
/// @return
///
inline std::ostream& operator<<(
    std::ostream&      ostr,
    const ConnectionId connectionId
)
{
    ostr << "(" << connectionId.outNodeId << ", "
         << (isPortIndexValid(connectionId.outPortIndex)
                 ? std::to_string(connectionId.outPortIndex)
                 : "INVALID")
         << ", " << connectionId.inNodeId << ", "
         << (isPortIndexValid(connectionId.inPortIndex)
                 ? std::to_string(connectionId.inPortIndex)
                 : "INVALID")
         << ")" << std::endl;

    return ostr;
}

///
/// @brief
/// @param connId
/// @return
///
inline QJsonObject toJson(const ConnectionId& connId)
{
    QJsonObject connJson;

    connJson["outNodeId"]    = static_cast<qint64>(connId.outNodeId);
    connJson["outPortIndex"] = static_cast<qint64>(connId.outPortIndex);
    connJson["intNodeId"]    = static_cast<qint64>(connId.inNodeId);
    connJson["inPortIndex"]  = static_cast<qint64>(connId.inPortIndex);

    return connJson;
}

///
/// @brief
/// @param connJson
/// @return
///
inline ConnectionId fromJson(const QJsonObject& connJson)
{
    ConnectionId connId {
        static_cast<NodeId>(connJson["outNodeId"].toInt(InvalidNodeId)),
        static_cast<PortIndex>(connJson["outPortIndex"].toInt(InvalidPortIndex)
        ),
        static_cast<NodeId>(connJson["intNodeId"].toInt(InvalidNodeId)),
        static_cast<PortIndex>(connJson["inPortIndex"].toInt(InvalidPortIndex))
    };

    return connId;
}

}  // namespace QtNodes

#endif  // CONNECTION_ID_UTILS_HXX_8EC34A10D393

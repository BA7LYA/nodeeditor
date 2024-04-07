#pragma once

#include "QtNodes/ConnectionStyle.hxx"
#include "QtNodes/Export.hxx"
#include "QtNodes/GraphicsViewStyle.hxx"
#include "QtNodes/NodeStyle.hxx"

namespace QtNodes {

class NODE_EDITOR_PUBLIC StyleCollection
{
public:
    static const NodeStyle& nodeStyle();

    static const ConnectionStyle& connectionStyle();

    static const GraphicsViewStyle& flowViewStyle();

public:
    static void setNodeStyle(NodeStyle);

    static void setConnectionStyle(ConnectionStyle);

    static void setGraphicsViewStyle(GraphicsViewStyle);

private:
    StyleCollection() = default;

    StyleCollection(const StyleCollection&) = delete;

    StyleCollection& operator=(const StyleCollection&) = delete;

    static StyleCollection& instance();

private:
    NodeStyle _nodeStyle;

    ConnectionStyle _connectionStyle;

    GraphicsViewStyle _flowViewStyle;
};

}  // namespace QtNodes

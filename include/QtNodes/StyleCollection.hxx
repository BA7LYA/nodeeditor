#pragma once

#include "Export.hxx"

#include "ConnectionStyle.hxx"
#include "GraphicsViewStyle.hxx"
#include "NodeStyle.hxx"

namespace QtNodes {

class NODE_EDITOR_PUBLIC StyleCollection
{
public:
    static NodeStyle const &nodeStyle();

    static ConnectionStyle const &connectionStyle();

    static GraphicsViewStyle const &flowViewStyle();

public:
    static void setNodeStyle(NodeStyle);

    static void setConnectionStyle(ConnectionStyle);

    static void setGraphicsViewStyle(GraphicsViewStyle);

private:
    StyleCollection() = default;

    StyleCollection(StyleCollection const &) = delete;

    StyleCollection &operator=(StyleCollection const &) = delete;

    static StyleCollection &instance();

private:
    NodeStyle _nodeStyle;

    ConnectionStyle _connectionStyle;

    GraphicsViewStyle _flowViewStyle;
};

} // namespace QtNodes

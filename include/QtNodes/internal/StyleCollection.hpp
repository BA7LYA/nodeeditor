#pragma once

#include "ConnectionStyle.hpp"
#include "Export.hpp"
#include "GraphicsViewStyle.hpp"
#include "NodeStyle.hpp"

namespace QtNodes {

class NODE_EDITOR_PUBLIC StyleCollection
{
public:
    static void setNodeStyle(NodeStyle);
    static void setConnectionStyle(ConnectionStyle);
    static void setGraphicsViewStyle(GraphicsViewStyle);

    static const NodeStyle&         nodeStyle();
    static const ConnectionStyle&   connectionStyle();
    static const GraphicsViewStyle& flowViewStyle();

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

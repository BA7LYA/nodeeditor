///
/// @file StyleCollection.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-16
/// @copyright Copyright (c) 2024
///

#ifndef STYLE_COLLECTION_HXX_5BFCD63C0004
#define STYLE_COLLECTION_HXX_5BFCD63C0004

#include "ConnectionStyle.hxx"
#include "Export.hxx"
#include "GraphicsViewStyle.hxx"
#include "NodeStyle.hxx"

namespace QtNodes {

///
/// @brief Singleton
///
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
    NodeStyle         _nodeStyle;
    ConnectionStyle   _connectionStyle;
    GraphicsViewStyle _flowViewStyle;
};

}  // namespace QtNodes

#endif  // STYLE_COLLECTION_HXX_5BFCD63C0004

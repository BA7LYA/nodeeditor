///
/// @file StyleCollection.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#include "StyleCollection.hxx"

using QtNodes::ConnectionStyle;
using QtNodes::GraphicsViewStyle;
using QtNodes::NodeStyle;
using QtNodes::StyleCollection;

const NodeStyle& StyleCollection::nodeStyle()
{
    return instance()._nodeStyle;
}

const ConnectionStyle& StyleCollection::connectionStyle()
{
    return instance()._connectionStyle;
}

const GraphicsViewStyle& StyleCollection::flowViewStyle()
{
    return instance()._flowViewStyle;
}

void StyleCollection::setNodeStyle(NodeStyle nodeStyle)
{
    instance()._nodeStyle = nodeStyle;
}

void StyleCollection::setConnectionStyle(ConnectionStyle connectionStyle)
{
    instance()._connectionStyle = connectionStyle;
}

void StyleCollection::setGraphicsViewStyle(GraphicsViewStyle flowViewStyle)
{
    instance()._flowViewStyle = flowViewStyle;
}

StyleCollection& StyleCollection::instance()
{
    static StyleCollection collection;

    return collection;
}

#include "QtNodes/StyleCollection.hxx"

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

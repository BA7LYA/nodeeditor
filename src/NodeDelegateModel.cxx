///
/// @file NodeDelegateModel.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#include "NodeDelegateModel.hxx"

#include "StyleCollection.hxx"

namespace QtNodes {

NodeDelegateModel::NodeDelegateModel()
    : _nodeStyle(StyleCollection::nodeStyle())
{
    // Derived classes can initialize specific style here
}

QJsonObject NodeDelegateModel::save() const
{
    QJsonObject modelJson;
    modelJson["model-name"] = name();
    return modelJson;
}

void NodeDelegateModel::load(const QJsonObject&)
{
    //
}

ConnectionPolicy NodeDelegateModel::portConnectionPolicy(
    PortType portType,
    PortIndex
) const
{
    auto result = ConnectionPolicy::One;
    switch (portType)
    {
    case PortType::In: result = ConnectionPolicy::One; break;
    case PortType::Out: result = ConnectionPolicy::Many; break;
    case PortType::None: break;
    }

    return result;
}

const NodeStyle& NodeDelegateModel::nodeStyle() const
{
    return _nodeStyle;
}

void NodeDelegateModel::setNodeStyle(const NodeStyle& style)
{
    _nodeStyle = style;
}

}  // namespace QtNodes

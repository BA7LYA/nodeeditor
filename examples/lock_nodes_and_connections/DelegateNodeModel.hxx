///
/// @file DelegateNodeModel.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef DELEGATE_NODE_MODEL_HXX_3FB7FA3CAA79
#define DELEGATE_NODE_MODEL_HXX_3FB7FA3CAA79

#include <memory>
#include <QtCore/QObject>
#include <QtNodes/NodeData>
#include <QtNodes/NodeDelegateModel>

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

class SimpleNodeData : public NodeData
{
public:
    NodeDataType type() const override
    {
        return NodeDataType { "SimpleData", "Simple Data" };
    }
};

///
/// @brief The model dictates the number of inputs and outputs for the Node. In
/// this example it has no logic.
///
class SimpleDataModel : public NodeDelegateModel
{
    Q_OBJECT

public:
    QString caption() const override
    {
        return QString("Simple Data Model");
    }

    QString name() const override
    {
        return QString("SimpleDataModel");
    }

    unsigned int nPorts(const PortType portType) const override
    {
        return 2;
    }

    NodeDataType dataType(const PortType portType, const PortIndex portIndex)
        const override
    {
        return SimpleNodeData().type();
    }

    std::shared_ptr<NodeData> outData(const PortIndex port) override
    {
        return std::make_shared<SimpleNodeData>();
    }

    void setInData(std::shared_ptr<NodeData>, const PortIndex) override {}

    QWidget* embeddedWidget() override
    {
        return nullptr;
    }
};

#endif  // DELEGATE_NODE_MODEL_HXX_3FB7FA3CAA79

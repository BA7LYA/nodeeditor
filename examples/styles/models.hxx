///
/// @file models.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef MODELS_HXX_300099D22FB2
#define MODELS_HXX_300099D22FB2

#include <memory>
#include <QtCore/QObject>
#include <QtNodes/NodeData>
#include <QtNodes/NodeDelegateModel>

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

///
/// @brief The class can potentially incapsulate any user data which need to be
/// transferred within the Node Editor graph.
///
class MyNodeData : public NodeData
{
public:
    NodeDataType type() const override
    {
        return NodeDataType { "MyNodeData", "My Node Data" };
    }
};

///
/// @brief The model dictates the number of inputs and outputs for the Node. In
/// this example it has no logic.
///
class MyDataModel : public NodeDelegateModel
{
    Q_OBJECT

public:
    ~MyDataModel() = default;

    QString caption() const override
    {
        return QString("My Data Model");
    }

    QString name() const override
    {
        return QString("MyDataModel");
    }

    QJsonObject save() const override
    {
        QJsonObject modelJson;

        modelJson["name"] = name();

        return modelJson;
    }

    unsigned int nPorts(const PortType) const override
    {
        return 3;
    }

    NodeDataType dataType(const PortType, const PortIndex) const override
    {
        return MyNodeData().type();
    }

    std::shared_ptr<NodeData> outData(PortIndex) override
    {
        return std::make_shared<MyNodeData>();
    }

    void setInData(std::shared_ptr<NodeData>, const PortIndex) override {}

    QWidget* embeddedWidget() override
    {
        return nullptr;
    }
};

#endif  // MODELS_HXX_300099D22FB2

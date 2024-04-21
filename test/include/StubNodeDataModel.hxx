///
/// @file StubNodeDataModel.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-22
/// @copyright Copyright (c) 2024
///

#ifndef STUB_NODE_DATA_MODEL_HXX_8498C27C0F42
#define STUB_NODE_DATA_MODEL_HXX_8498C27C0F42

#include <QtNodes/NodeDataModel>
#include <utility>

class StubNodeDataModel : public QtNodes::NodeDataModel
{
public:
    QString name() const override
    {
        return _name;
    }

    QString caption() const override
    {
        return _caption;
    }

    unsigned int nPorts(QtNodes::PortType) const override
    {
        return 0;
    }

    QWidget* embeddedWidget() override
    {
        return nullptr;
    }

    QtNodes::NodeDataType dataType(QtNodes::PortType, QtNodes::PortIndex)
        const override
    {
        return QtNodes::NodeDataType();
    }

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex) override
    {
        return nullptr;
    }

    void setInData(std::shared_ptr<QtNodes::NodeData>, QtNodes::PortIndex)
        override
    {
    }

    void name(QString name)
    {
        _name = std::move(name);
    }

    void caption(QString caption)
    {
        _caption = std::move(caption);
    }

private:
    QString _name    = "name";
    QString _caption = "caption";
};

#endif  // STUB_NODE_DATA_MODEL_HXX_8498C27C0F42

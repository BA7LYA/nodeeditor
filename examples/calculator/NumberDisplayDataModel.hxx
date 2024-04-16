///
/// @file NumberDisplayDataModel.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef NUMBER_DISPLAY_DATA_MODEL_HXX_C5A8D8006D96
#define NUMBER_DISPLAY_DATA_MODEL_HXX_C5A8D8006D96

#include <iostream>
#include <QtCore/QObject>
#include <QtNodes/NodeDelegateModel>

#include "DecimalData.hxx"

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

class QLabel;

///
/// @brief The model dictates the number of inputs and outputs for the Node. In
/// this example it has no logic.
///
class NumberDisplayDataModel : public NodeDelegateModel
{
    Q_OBJECT

public:
    NumberDisplayDataModel();

    ~NumberDisplayDataModel() = default;

    QString caption() const override
    {
        return QStringLiteral("Result");
    }

    bool captionVisible() const override
    {
        return false;
    }

    QString name() const override
    {
        return QStringLiteral("Result");
    }

    unsigned int nPorts(PortType portType) const override;

    NodeDataType dataType(PortType portType, PortIndex portIndex)
        const override;

    std::shared_ptr<NodeData> outData(PortIndex port) override;

    void setInData(std::shared_ptr<NodeData> data, PortIndex portIndex)
        override;

    QWidget* embeddedWidget() override;

    double number() const;

private:
    std::shared_ptr<DecimalData> _numberData;

    QLabel* _label;
};

#endif  // NUMBER_DISPLAY_DATA_MODEL_HXX_C5A8D8006D96

///
/// @file TextDisplayDataModel.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef TEXT_DISPLAY_DATA_MODEL_HXX_E913F390AA36
#define TEXT_DISPLAY_DATA_MODEL_HXX_E913F390AA36

#include <iostream>
#include <QtCore/QObject>
#include <QtNodes/NodeDelegateModel>
#include <QtWidgets/QLabel>
#include <vector>

#include "TextData.hxx"

using QtNodes::ConnectionPolicy;
using QtNodes::NodeData;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

///
/// @brief The model dictates the number of inputs and outputs for the Node. In
/// this example it has no logic.
///
class TextDisplayDataModel : public NodeDelegateModel
{
    Q_OBJECT

public:
    TextDisplayDataModel();

    virtual ~TextDisplayDataModel() {}

    QString caption() const override
    {
        return QString("Text Display");
    }

    bool captionVisible() const override
    {
        return false;
    }

    static QString Name()
    {
        return QString("TextDisplayDataModel");
    }

    QString name() const override
    {
        return TextDisplayDataModel::Name();
    }

    unsigned int nPorts(PortType portType) const override;

    NodeDataType dataType(PortType portType, PortIndex portIndex)
        const override;

    std::shared_ptr<NodeData> outData(const PortIndex port) override;

    void setInData(std::shared_ptr<NodeData> data, const PortIndex portIndex)
        override;

    QWidget* embeddedWidget() override
    {
        return _label;
    }

private:
    QLabel* _label;
    QString _inputText;
};

#endif  // TEXT_DISPLAY_DATA_MODEL_HXX_E913F390AA36

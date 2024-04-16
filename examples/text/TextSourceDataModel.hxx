///
/// @file TextSourceDataModel.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef TEXT_SOURCE_DATA_MODEL_HXX_8D3EE4D1AFB2
#define TEXT_SOURCE_DATA_MODEL_HXX_8D3EE4D1AFB2

#include <iostream>
#include <QtCore/QObject>
#include <QtNodes/NodeDelegateModel>

#include "TextData.hxx"

using QtNodes::NodeData;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

class QLineEdit;

///
/// @brief The model dictates the number of inputs and outputs for the Node. In
/// this example it has no logic.
///
class TextSourceDataModel : public NodeDelegateModel
{
    Q_OBJECT

public:
    TextSourceDataModel();

    QString caption() const override
    {
        return QString("Text Source");
    }

    bool captionVisible() const override
    {
        return false;
    }

    static QString Name()
    {
        return QString("TextSourceDataModel");
    }

    QString name() const override
    {
        return TextSourceDataModel::Name();
    }

    unsigned int nPorts(PortType portType) const override;

    NodeDataType dataType(PortType portType, PortIndex portIndex)
        const override;

    std::shared_ptr<NodeData> outData(const PortIndex portIndex) override;

    void setInData(std::shared_ptr<NodeData>, const PortIndex) override {}

    QWidget* embeddedWidget() override;

private Q_SLOTS:

    void onTextEdited(const QString& string);

private:
    QLineEdit* _lineEdit;
};

#endif  // TEXT_SOURCE_DATA_MODEL_HXX_8D3EE4D1AFB2

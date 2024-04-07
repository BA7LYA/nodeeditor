#pragma once

#include <iostream>
#include <QtCore/QObject>
#include <QtNodes/NodeDelegateModel.hxx>

#include "TextData.hxx"

using QtNodes::NodeData;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

class QLineEdit;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class TextSourceDataModel : public NodeDelegateModel
{
    Q_OBJECT

public:
    TextSourceDataModel();

public:
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

public:
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

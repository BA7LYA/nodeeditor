///
/// @file ImageShowModel.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef IMAGE_SHOW_MODEL_HXX_E7663F8D1D3E
#define IMAGE_SHOW_MODEL_HXX_E7663F8D1D3E

#include <iostream>
#include <QtCore/QObject>
#include <QtNodes/NodeDelegateModel>
#include <QtNodes/NodeDelegateModelRegistry>
#include <QtWidgets/QLabel>

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

///
/// @brief The model dictates the number of inputs and outputs for the Node. In
/// this example it has no logic.
///
class ImageShowModel : public NodeDelegateModel
{
    Q_OBJECT

public:
    ImageShowModel();

    ~ImageShowModel() = default;

    QString caption() const override
    {
        return QString("Image Display");
    }

    QString name() const override
    {
        return QString("ImageShowModel");
    }

    virtual QString modelName() const
    {
        return QString("Resulting Image");
    }

    unsigned int nPorts(const PortType portType) const override;

    NodeDataType dataType(const PortType portType, const PortIndex portIndex)
        const override;

    std::shared_ptr<NodeData> outData(const PortIndex port) override;

    void setInData(std::shared_ptr<NodeData> nodeData, const PortIndex port)
        override;

    QWidget* embeddedWidget() override
    {
        return _label;
    }

    bool resizable() const override
    {
        return true;
    }

protected:
    bool eventFilter(QObject* object, QEvent* event) override;

private:
    QLabel* _label;

    std::shared_ptr<NodeData> _nodeData;
};

#endif  // IMAGE_SHOW_MODEL_HXX_E7663F8D1D3E

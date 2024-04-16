///
/// @file ImageLoaderModel.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef IMAGE_LOADER_MODEL_HXX_14CD7DCFCDEA
#define IMAGE_LOADER_MODEL_HXX_14CD7DCFCDEA

#include <iostream>
#include <QtCore/QObject>
#include <QtNodes/NodeDelegateModel>
#include <QtNodes/NodeDelegateModelRegistry>
#include <QtWidgets/QLabel>

#include "PixmapData.hxx"

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

///
/// @brief The model dictates the number of inputs and outputs for the Node. In
/// this example it has no logic.
///
class ImageLoaderModel : public NodeDelegateModel
{
    Q_OBJECT

public:
    ImageLoaderModel();

    ~ImageLoaderModel() = default;

    QString caption() const override
    {
        return QString("Image Source");
    }

    QString name() const override
    {
        return QString("ImageLoaderModel");
    }

    virtual QString modelName() const
    {
        return QString("Source Image");
    }

    unsigned int nPorts(const PortType portType) const override;

    NodeDataType dataType(const PortType portType, const PortIndex portIndex)
        const override;

    std::shared_ptr<NodeData> outData(const PortIndex port) override;

    void setInData(std::shared_ptr<NodeData>, const PortIndex portIndex)
        override
    {
    }

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
    QPixmap _pixmap;
};

#endif  // IMAGE_LOADER_MODEL_HXX_14CD7DCFCDEA

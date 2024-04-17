///
/// @file ImageShowModel.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#include "ImageShowModel.hxx"

#include <QtCore/QDir>
#include <QtCore/QEvent>
#include <QtNodes/NodeDelegateModelRegistry>
#include <QtWidgets/QFileDialog>

#include "PixmapData.hxx"

ImageShowModel::ImageShowModel()
    : _label(new QLabel("Image will appear here"))
{
    _label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    QFont f = _label->font();
    f.setBold(true);
    f.setItalic(true);

    _label->setFont(f);
    _label->setMinimumSize(200, 200);
    _label->installEventFilter(this);
}

unsigned int ImageShowModel::nPorts(PortType portType) const
{
    unsigned int result = 1;

    switch (portType)
    {
    case PortType::In: result = 1; break;

    case PortType::Out: result = 1;

    default: break;
    }

    return result;
}

bool ImageShowModel::eventFilter(QObject* object, QEvent* event)
{
    if (object == _label)
    {
        int w = _label->width();
        int h = _label->height();

        if (event->type() == QEvent::Resize)
        {
            auto d = std::dynamic_pointer_cast<PixmapData>(_nodeData);
            if (d)
            {
                _label->setPixmap(d->pixmap().scaled(w, h, Qt::KeepAspectRatio)
                );
            }
        }
    }

    return false;
}

NodeDataType ImageShowModel::dataType(const PortType, const PortIndex) const
{
    return PixmapData().type();
}

std::shared_ptr<NodeData> ImageShowModel::outData(PortIndex)
{
    return _nodeData;
}

void ImageShowModel::setInData(
    std::shared_ptr<NodeData> nodeData,
    const PortIndex
)
{
    _nodeData = nodeData;

    if (_nodeData)
    {
        auto d = std::dynamic_pointer_cast<PixmapData>(_nodeData);

        int w = _label->width();
        int h = _label->height();

        _label->setPixmap(d->pixmap().scaled(w, h, Qt::KeepAspectRatio));
    }
    else
    {
        _label->setPixmap(QPixmap());
    }

    Q_EMIT dataUpdated(0);
}
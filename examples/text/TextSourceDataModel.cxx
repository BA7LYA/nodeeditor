///
/// @file TextSourceDataModel.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#include "TextSourceDataModel.hxx"

#include <QtWidgets/QLineEdit>

TextSourceDataModel::TextSourceDataModel()
    : _lineEdit { nullptr }
{
    //
}

unsigned int TextSourceDataModel::nPorts(PortType portType) const
{
    unsigned int result = 1;

    switch (portType)
    {
    case PortType::In: result = 0; break;

    case PortType::Out: result = 1;

    default: break;
    }

    return result;
}

void TextSourceDataModel::onTextEdited(const QString& string)
{
    Q_UNUSED(string);

    Q_EMIT dataUpdated(0);
}

NodeDataType TextSourceDataModel::dataType(PortType, PortIndex) const
{
    return TextData().type();
}

std::shared_ptr<NodeData> TextSourceDataModel::outData(const PortIndex portIndex
)
{
    Q_UNUSED(portIndex);
    return std::make_shared<TextData>(_lineEdit->text());
}

QWidget* TextSourceDataModel::embeddedWidget()
{
    if (!_lineEdit)
    {
        _lineEdit = new QLineEdit("Default Text"),

        connect(
            _lineEdit,
            &QLineEdit::textEdited,
            this,
            &TextSourceDataModel::onTextEdited
        );
    }

    return _lineEdit;
}

///
/// @file TextDisplayDataModel.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#include "TextDisplayDataModel.hxx"

TextDisplayDataModel::TextDisplayDataModel()
    : _label(new QLabel("Resulting Text"))
{
    _label->setMargin(3);
}

unsigned int TextDisplayDataModel::nPorts(PortType portType) const
{
    unsigned int result = 1;

    switch (portType)
    {
    case PortType::In: result = 1; break;

    case PortType::Out: result = 0;

    default: break;
    }

    return result;
}

NodeDataType TextDisplayDataModel::dataType(PortType, PortIndex) const
{
    return TextData().type();
}

std::shared_ptr<NodeData> TextDisplayDataModel::outData(PortIndex)
{
    std::shared_ptr<NodeData> ptr;
    return ptr;
}

void TextDisplayDataModel::setInData(
    std::shared_ptr<NodeData> data,
    const PortIndex
)
{
    auto textData = std::dynamic_pointer_cast<TextData>(data);

    if (textData)
    {
        _inputText = textData->text();
    }
    else
    {
        _inputText = "";
    }

    _label->setText(_inputText);
    _label->adjustSize();
}

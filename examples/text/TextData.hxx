///
/// @file TextData.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef TEXT_DATA_HXX_EACCBD5416FF
#define TEXT_DATA_HXX_EACCBD5416FF

#include <QtNodes/NodeData>

using QtNodes::NodeData;
using QtNodes::NodeDataType;

///
/// @brief The class can potentially incapsulate any user data which need to be
/// transferred within the Node Editor graph.
///
class TextData : public NodeData
{
public:
    TextData() {}

    TextData(const QString& text)
        : _text(text)
    {
    }

    NodeDataType type() const override
    {
        return NodeDataType { "text", "Text" };
    }

    QString text() const
    {
        return _text;
    }

private:
    QString _text;
};

#endif  // TEXT_DATA_HXX_EACCBD5416FF

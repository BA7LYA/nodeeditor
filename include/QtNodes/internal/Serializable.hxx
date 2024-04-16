///
/// @file Serializable.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-16
/// @copyright Copyright (c) 2024
///

#ifndef SERIALIZABLE_HXX_5DED8A61CE62
#define SERIALIZABLE_HXX_5DED8A61CE62

#include <QtCore/QJsonObject>

namespace QtNodes {

class Serializable
{
public:
    virtual ~Serializable() = default;

    virtual QJsonObject save() const
    {
        return {};
    }

    virtual void load(const QJsonObject& /*p*/) {}
};

}  // namespace QtNodes

#endif  // SERIALIZABLE_HXX_5DED8A61CE62

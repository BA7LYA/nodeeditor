///
/// @file QUuidStdHash.hpp
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-16
/// @copyright Copyright (c) 2024
///

#ifndef Q_UUID_STD_HASH_HXX_B76C7F15E244
#define Q_UUID_STD_HASH_HXX_B76C7F15E244

#include <functional>
#include <QtCore/QUuid>
#include <QtCore/QVariant>

namespace std {

template<>
struct hash<QUuid>
{
    inline std::size_t operator()(const QUuid& uid) const
    {
        return qHash(uid);
    }
};

}  // namespace std

#endif  // Q_UUID_STD_HASH_HXX_B76C7F15E244

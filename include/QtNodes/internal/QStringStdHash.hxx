///
/// @file QStringStdHash.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef QSTRING_STD_HASH_HXX_6E6E145BD6E1
#define QSTRING_STD_HASH_HXX_6E6E145BD6E1

#include <QtGlobal>

#if (QT_VERSION < QT_VERSION_CHECK(5, 14, 0))

// As of 5.14 there is a specialization std::hash<QString>

#include <functional>
#include <QtCore/QString>
#include <QtCore/QVariant>

namespace std {

template<>
struct hash<QString>
{
    inline std::size_t operator()(const QString& s) const
    {
        return qHash(s);
    }
};

}  // namespace std

#endif

#endif  // QSTRING_STD_HASH_HXX_6E6E145BD6E1

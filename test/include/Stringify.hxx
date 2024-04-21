///
/// @file Stringify.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-22
/// @copyright Copyright (c) 2024
///

#pragma once

#include <catch2/catch.hpp>
#include <QtCore/QPoint>
#include <QtCore/QPointF>
#include <QtTest>

namespace Catch {

template<>
struct StringMaker<QPointF>
{
    static std::string convert(const QPointF& p)
    {
        return std::string(QTest::toString(p));
    }
};

template<>
struct StringMaker<QPoint>
{
    static std::string convert(const QPoint& p)
    {
        return std::string(QTest::toString(p));
    }
};

}  // namespace Catch

///
/// @file ApplicationSetup.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-22
/// @copyright Copyright (c) 2024
///

#pragma once

#include <memory>
#include <QApplication>

inline std::unique_ptr<QApplication> applicationSetup()
{
    static int    Argc       = 0;
    static char   ArgvVal    = '\0';
    static char*  ArgvValPtr = &ArgvVal;
    static char** Argv       = &ArgvValPtr;

    auto app = std::make_unique<QApplication>(Argc, Argv);
    app->setAttribute(Qt::AA_Use96Dpi, true);

    return app;
}

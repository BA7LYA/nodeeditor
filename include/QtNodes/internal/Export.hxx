///
/// @file Export.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-16
/// @copyright Copyright (c) 2024
///

#ifndef EXPORT_HXX_B68BF567CC26
#define EXPORT_HXX_B68BF567CC26

#include "Compiler.hxx"
#include "OperatingSystem.hxx"

#ifdef NODE_EDITOR_PLATFORM_WINDOWS
#define NODE_EDITOR_EXPORT __declspec(dllexport)
#define NODE_EDITOR_IMPORT __declspec(dllimport)
#define NODE_EDITOR_LOCAL
#elif NODE_EDITOR_COMPILER_GNU_VERSION_MAJOR >= 4 \
    || defined(NODE_EDITOR_COMPILER_CLANG)
#define NODE_EDITOR_EXPORT __attribute__((visibility("default")))
#define NODE_EDITOR_IMPORT __attribute__((visibility("default")))
#define NODE_EDITOR_LOCAL  __attribute__((visibility("hidden")))
#else
#define NODE_EDITOR_EXPORT
#define NODE_EDITOR_IMPORT
#define NODE_EDITOR_LOCAL
#endif

#ifdef __cplusplus
#define NODE_EDITOR_DEMANGLED extern "C"
#else
#define NODE_EDITOR_DEMANGLED
#endif

#if defined(NODE_EDITOR_SHARED) && !defined(NODE_EDITOR_STATIC)
#ifdef NODE_EDITOR_EXPORTS
#define NODE_EDITOR_PUBLIC NODE_EDITOR_EXPORT
#else
#define NODE_EDITOR_PUBLIC NODE_EDITOR_IMPORT
#endif
#define NODE_EDITOR_PRIVATE NODE_EDITOR_LOCAL
#elif !defined(NODE_EDITOR_SHARED) && defined(NODE_EDITOR_STATIC)
#define NODE_EDITOR_PUBLIC
#define NODE_EDITOR_PRIVATE
#elif defined(NODE_EDITOR_SHARED) && defined(NODE_EDITOR_STATIC)
#ifdef NODE_EDITOR_EXPORTS
#error "Cannot build as shared and static simultaneously."
#else
#error "Cannot link against shared and static simultaneously."
#endif
#else
#ifdef NODE_EDITOR_EXPORTS
#error "Choose whether to build as shared or static."
#else
#error "Choose whether to link against shared or static."
#endif
#endif

#endif  // EXPORT_HXX_B68BF567CC26

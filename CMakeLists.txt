cmake_minimum_required(VERSION 3.8)

# new in 3.11.
# The NEW behavior for this policy is to set OpenGL_GL_PREFERENCE to GLVND.
cmake_policy(SET CMP0072 NEW)

# new in 3.9.
# The NEW behavior of this policy is to ignore the RPATH settings for install_name on macOS.
cmake_policy(SET CMP0068 NEW)

project(QtNodesLibrary CXX)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)
set(OpenGL_GL_PREFERENCE LEGACY)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTING "Build tests" ON)
option(BUILD_EXAMPLES "Build Examples" ON)
option(BUILD_DOCS "Build Documentation" ON)
option(BUILD_SHARED_LIBS "Build as shared library" ON)
option(BUILD_DEBUG_POSTFIX_D "Append d suffix to debug libraries" OFF)
option(QT_NODES_FORCE_TEST_COLOR "Force colorized unit test output" OFF)
option(USE_QT6 "Build with Qt6 (Enabled by default)" ON)

enable_testing()

if(QT_NODES_DEVELOPER_DEFAULTS)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
endif()

if(BUILD_DEBUG_POSTFIX_D)
  set(CMAKE_DEBUG_POSTFIX "d")
  set(CMAKE_RELEASE_POSTFIX "")
  set(CMAKE_RELWITHDEBINFO_POSTFIX "rd")
  set(CMAKE_MINSIZEREL_POSTFIX "s")
endif()

add_subdirectory(external)

if(USE_QT6)
  find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
else()
  find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets)
endif()

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Gui OpenGL)
message(STATUS "QT_VERSION: ${QT_VERSION}, QT_DIR: ${QT_DIR}")

if (${QT_VERSION} VERSION_LESS 5.11.0)
  message(FATAL_ERROR "Requires qt version >= 5.11.0, Your current version is ${QT_VERSION}")
endif()

if (${QT_VERSION_MAJOR} EQUAL 6)
  qt_add_resources(RESOURCES ./resources/resources.qrc)
else()
  qt5_add_resources(RESOURCES ./resources/resources.qrc)
endif()

# Unfortunately, as we have a split include/src, AUTOMOC doesn't work.
# We'll have to manually specify some files
set(CMAKE_AUTOMOC ON)

set(CPP_SOURCE_FILES
  src/AbstractGraphModel.cxx
  src/AbstractNodeGeometry.cxx
  src/BasicGraphicsScene.cxx
  src/ConnectionGraphicsObject.cxx
  src/ConnectionPainter.cxx
  src/ConnectionState.cxx
  src/ConnectionStyle.cxx
  src/DataFlowGraphModel.cxx
  src/DataFlowGraphicsScene.cxx
  src/DefaultHorizontalNodeGeometry.cxx
  src/DefaultVerticalNodeGeometry.cxx
  src/Definitions.cxx
  src/GraphicsView.cxx
  src/GraphicsViewStyle.cxx
  src/NodeDelegateModelRegistry.cxx
  src/NodeConnectionInteraction.cxx
  src/NodeDelegateModel.cxx
  src/NodeGraphicsObject.cxx
  src/DefaultNodePainter.cxx
  src/NodeState.cxx
  src/NodeStyle.cxx
  src/StyleCollection.cxx
  src/UndoCommands.cxx
  src/locateNode.cxx
)

set(HPP_HEADER_FILES
  include/QtNodes/internal/AbstractGraphModel.hxx
  include/QtNodes/internal/AbstractNodeGeometry.hxx
  include/QtNodes/internal/AbstractNodePainter.hxx
  include/QtNodes/internal/BasicGraphicsScene.hxx
  include/QtNodes/internal/Compiler.hxx
  include/QtNodes/internal/ConnectionGraphicsObject.hxx
  include/QtNodes/internal/ConnectionIdHash.hxx
  include/QtNodes/internal/ConnectionIdUtils.hxx
  include/QtNodes/internal/ConnectionState.hxx
  include/QtNodes/internal/ConnectionStyle.hxx
  include/QtNodes/internal/DataFlowGraphicsScene.hxx
  include/QtNodes/internal/DataFlowGraphModel.hxx
  include/QtNodes/internal/DefaultNodePainter.hxx
  include/QtNodes/internal/Definitions.hxx
  include/QtNodes/internal/Export.hxx
  include/QtNodes/internal/GraphicsView.hxx
  include/QtNodes/internal/GraphicsViewStyle.hxx
  include/QtNodes/internal/locateNode.hxx
  include/QtNodes/internal/NodeData.hxx
  include/QtNodes/internal/NodeDelegateModel.hxx
  include/QtNodes/internal/NodeDelegateModelRegistry.hxx
  include/QtNodes/internal/NodeGraphicsObject.hxx
  include/QtNodes/internal/NodeState.hxx
  include/QtNodes/internal/NodeStyle.hxx
  include/QtNodes/internal/OperatingSystem.hxx
  include/QtNodes/internal/QStringStdHash.hxx
  include/QtNodes/internal/QUuidStdHash.hxx
  include/QtNodes/internal/Serializable.hxx
  include/QtNodes/internal/Style.hxx
  include/QtNodes/internal/StyleCollection.hxx
  src/ConnectionPainter.hxx
  src/DefaultHorizontalNodeGeometry.hxx
  src/DefaultVerticalNodeGeometry.hxx
  src/NodeConnectionInteraction.hxx
  src/UndoCommands.hxx
)

# If we want to give the option to build a static library,
# set BUILD_SHARED_LIBS option to OFF
add_library(QtNodes
  ${CPP_SOURCE_FILES}
  ${HPP_HEADER_FILES}
  ${RESOURCES}
)

add_library(QtNodes::QtNodes ALIAS QtNodes)


target_include_directories(QtNodes
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/QtNodes/internal>
)

target_link_libraries(QtNodes
  PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::OpenGL
)

target_compile_definitions(QtNodes
  PUBLIC
    NODE_EDITOR_SHARED
  PRIVATE
    NODE_EDITOR_EXPORTS
    #NODE_DEBUG_DRAWING
    QT_NO_KEYWORDS
)


target_compile_options(QtNodes
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /wd4127 /EHsc /utf-8>
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra>
    $<$<CXX_COMPILER_ID:AppleClang>:-Wall -Wextra -Werror>
)
if(NOT "${CMAKE_CXX_SIMULATE_ID}" STREQUAL "MSVC")
  # Clang-Cl on MSVC identifies as "Clang" but behaves more like MSVC:
  target_compile_options(QtNodes
    PRIVATE
      $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra>
  )
endif()

if(QT_NODES_DEVELOPER_DEFAULTS)
  target_compile_features(QtNodes PUBLIC cxx_std_14)
  set_target_properties(QtNodes PROPERTIES CXX_EXTENSIONS OFF)
endif()


set_target_properties(QtNodes
  PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

######
# Moc
##

file(GLOB_RECURSE HEADERS_TO_MOC include/QtNodes/internal/*.hxx)

if (${QT_VERSION_MAJOR} EQUAL 6)
  qt_wrap_cpp(nodes_moc
      ${HEADERS_TO_MOC}
      TARGET QtNodes
    OPTIONS --no-notes # Don't display a note for the headers which don't produce a moc_*.cpp
  )
else()
  qt5_wrap_cpp(nodes_moc
  ${HEADERS_TO_MOC}
  TARGET QtNodes
  OPTIONS --no-notes # Don't display a note for the headers which don't produce a moc_*.cpp
  )
endif()

target_sources(QtNodes PRIVATE ${nodes_moc})

###########
# Examples
##

if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(BUILD_DOCS)
  add_subdirectory(docs)
endif()

##################
# Automated Tests
##

if(BUILD_TESTING)
  #add_subdirectory(test)
endif()

###############
# Installation
##

include(GNUInstallDirs)

set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/QtNodes)

install(TARGETS QtNodes
  EXPORT QtNodesTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT QtNodesTargets
  FILE QtNodesTargets.cmake
  NAMESPACE QtNodes::
  DESTINATION ${INSTALL_CONFIGDIR}
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/QtNodesConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/QtNodesConfig.cmake
  INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/QtNodesConfig.cmake
  DESTINATION ${INSTALL_CONFIGDIR}
)

///
/// @file TestDragging.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-22
/// @copyright Copyright (c) 2024
///

#include <catch.hpp>
#include <QtNodes/Connection>
#include <QtNodes/FlowScene>
#include <QtNodes/FlowView>
#include <QtNodes/Node>
#include <QtTest>
#include <QtWidgets/QApplication>

#include "ApplicationSetup.hxx"
#include "Stringify.hxx"
#include "StubNodeDataModel.hxx"

using QtNodes::Connection;
using QtNodes::DataModelRegistry;
using QtNodes::FlowScene;
using QtNodes::FlowView;
using QtNodes::Node;
using QtNodes::NodeData;
using QtNodes::NodeDataModel;
using QtNodes::NodeDataType;
using QtNodes::PortIndex;
using QtNodes::PortType;

TEST_CASE("Dragging node changes position", "[gui]")
{
    auto app = applicationSetup();

    FlowScene scene;
    FlowView  view(&scene);

    view.show();
    REQUIRE(QTest::qWaitForWindowExposed(&view));

    SECTION("just one node")
    {
        auto& node = scene.createNode(std::make_unique<StubNodeDataModel>());

        auto& ngo = node.nodeGraphicsObject();

        QPointF scPosBefore = ngo.pos();

        QPointF scClickPos = ngo.boundingRect().center();
        scClickPos = QPointF(ngo.sceneTransform().map(scClickPos).toPoint());

        QPoint vwClickPos = view.mapFromScene(scClickPos);
        QPoint vwDestPos  = vwClickPos + QPoint(10, 20);

        QPointF scExpectedDelta = view.mapToScene(vwDestPos) - scClickPos;

        CAPTURE(scClickPos);
        CAPTURE(vwClickPos);
        CAPTURE(vwDestPos);
        CAPTURE(scExpectedDelta);

        QTest::mouseMove(view.windowHandle(), vwClickPos);
        QTest::mousePress(
            view.windowHandle(),
            Qt::LeftButton,
            Qt::NoModifier,
            vwClickPos
        );
        QTest::mouseMove(view.windowHandle(), vwDestPos);
        QTest::mouseRelease(
            view.windowHandle(),
            Qt::LeftButton,
            Qt::NoModifier,
            vwDestPos
        );

        QPointF scDelta            = ngo.pos() - scPosBefore;
        QPoint  roundDelta         = scDelta.toPoint();
        QPoint  roundExpectedDelta = scExpectedDelta.toPoint();

        CHECK(roundDelta == roundExpectedDelta);
    }
}
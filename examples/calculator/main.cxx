///
/// @file main.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#include <QtGui/QScreen>
#include <QtNodes/ConnectionStyle>
#include <QtNodes/DataFlowGraphicsScene>
#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/GraphicsView>
#include <QtNodes/NodeData>
#include <QtNodes/NodeDelegateModelRegistry>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QVBoxLayout>

#include "AdditionModel.hxx"
#include "DivisionModel.hxx"
#include "MultiplicationModel.hxx"
#include "NumberDisplayDataModel.hxx"
#include "NumberSourceDataModel.hxx"
#include "SubtractionModel.hxx"

using QtNodes::ConnectionStyle;
using QtNodes::DataFlowGraphicsScene;
using QtNodes::DataFlowGraphModel;
using QtNodes::GraphicsView;
using QtNodes::NodeDelegateModelRegistry;

static std::shared_ptr<NodeDelegateModelRegistry> registerDataModels()
{
    auto ret = std::make_shared<NodeDelegateModelRegistry>();

    ret->registerModel<NumberSourceDataModel>("Sources");
    ret->registerModel<NumberDisplayDataModel>("Displays");
    ret->registerModel<AdditionModel>("Operators");
    ret->registerModel<SubtractionModel>("Operators");
    ret->registerModel<MultiplicationModel>("Operators");
    ret->registerModel<DivisionModel>("Operators");

    return ret;
}

static void setStyle()
{
    ConnectionStyle::setConnectionStyle(
        R"(
  {
    "ConnectionStyle": {
      "ConstructionColor": "gray",
      "NormalColor": "black",
      "SelectedColor": "gray",
      "SelectedHaloColor": "deepskyblue",
      "HoveredColor": "deepskyblue",

      "LineWidth": 3.0,
      "ConstructionLineWidth": 2.0,
      "PointDiameter": 10.0,

      "UseDataDefinedColors": true
    }
  }
  )"
    );
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    setStyle();

    std::shared_ptr<NodeDelegateModelRegistry> registry = registerDataModels();

    QWidget mainWidget;

    auto   menuBar = new QMenuBar();
    QMenu* menu    = menuBar->addMenu("File");

    auto saveAction = menu->addAction("Save Scene");
    saveAction->setShortcut(QKeySequence::Save);

    auto loadAction = menu->addAction("Load Scene");
    loadAction->setShortcut(QKeySequence::Open);

    QVBoxLayout* l = new QVBoxLayout(&mainWidget);

    DataFlowGraphModel dataFlowGraphModel(registry);

    l->addWidget(menuBar);
    auto scene = new DataFlowGraphicsScene(dataFlowGraphModel, &mainWidget);

    auto view = new GraphicsView(scene);
    l->addWidget(view);
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);

    QObject::connect(
        saveAction,
        &QAction::triggered,
        scene,
        [scene, &mainWidget]()
        {
            if (scene->save())
            {
                mainWidget.setWindowModified(false);
            }
        }
    );

    QObject::connect(
        loadAction,
        &QAction::triggered,
        scene,
        &DataFlowGraphicsScene::load
    );

    QObject::connect(
        scene,
        &DataFlowGraphicsScene::sceneLoaded,
        view,
        &GraphicsView::centerScene
    );

    QObject::connect(
        scene,
        &DataFlowGraphicsScene::modified,
        &mainWidget,
        [&mainWidget]() { mainWidget.setWindowModified(true); }
    );

    mainWidget.setWindowTitle("[*]Data Flow: simplest calculator");
    mainWidget.resize(800, 600);
    // Center window.
    mainWidget.move(
        QApplication::primaryScreen()->availableGeometry().center()
        - mainWidget.rect().center()
    );
    mainWidget.showNormal();

    return app.exec();
}

///
/// @file main.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#include <QtNodes/DataFlowGraphicsScene>
#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/GraphicsView>
#include <QtNodes/NodeData>
#include <QtNodes/NodeDelegateModelRegistry>
#include <QtWidgets/QApplication>

#include "TextDisplayDataModel.hxx"
#include "TextSourceDataModel.hxx"

using QtNodes::DataFlowGraphicsScene;
using QtNodes::DataFlowGraphModel;
using QtNodes::GraphicsView;
using QtNodes::NodeDelegateModelRegistry;

static std::shared_ptr<NodeDelegateModelRegistry> registerDataModels()
{
    auto ret = std::make_shared<NodeDelegateModelRegistry>();

    ret->registerModel<TextSourceDataModel>();
    ret->registerModel<TextDisplayDataModel>();

    return ret;
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    std::shared_ptr<NodeDelegateModelRegistry> registry = registerDataModels();
    DataFlowGraphModel                         dataFlowGraphModel(registry);

    DataFlowGraphicsScene scene(dataFlowGraphModel);

    GraphicsView view(&scene);

    view.setWindowTitle("Node-based flow editor");
    view.resize(800, 600);
    view.show();

    return app.exec();
}

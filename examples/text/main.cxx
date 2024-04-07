#include <QtNodes/DataFlowGraphicsScene.hxx>
#include <QtNodes/DataFlowGraphModel.hxx>
#include <QtNodes/GraphicsView.hxx>
#include <QtNodes/NodeData.hxx>
#include <QtNodes/NodeDelegateModelRegistry.hxx>
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

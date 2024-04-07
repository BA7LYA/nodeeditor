#include <QtGui/QScreen>
#include <QtNodes/DataFlowGraphicsScene.hxx>
#include <QtNodes/DataFlowGraphModel.hxx>
#include <QtNodes/GraphicsView.hxx>
#include <QtNodes/NodeData.hxx>
#include <QtNodes/NodeDelegateModelRegistry.hxx>
#include <QtWidgets/QApplication>

#include "ImageLoaderModel.hxx"
#include "ImageShowModel.hxx"

using QtNodes::ConnectionStyle;
using QtNodes::DataFlowGraphicsScene;
using QtNodes::DataFlowGraphModel;
using QtNodes::GraphicsView;
using QtNodes::NodeDelegateModelRegistry;

static std::shared_ptr<NodeDelegateModelRegistry> registerDataModels()
{
    auto ret = std::make_shared<NodeDelegateModelRegistry>();
    ret->registerModel<ImageShowModel>();

    ret->registerModel<ImageLoaderModel>();

    return ret;
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    std::shared_ptr<NodeDelegateModelRegistry> registry = registerDataModels();

    DataFlowGraphModel dataFlowGraphModel(registry);

    DataFlowGraphicsScene scene(dataFlowGraphModel);

    GraphicsView view(&scene);

    view.setWindowTitle("Data Flow: Resizable Images");
    view.resize(800, 600);
    // Center window.
    view.move(
        QApplication::primaryScreen()->availableGeometry().center()
        - view.rect().center()
    );
    view.show();

    return app.exec();
}

///
/// @file DataFlowGraphicsScene.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef DATA_FLOW_GRAPHICS_SCENE_HXX_CD8E222F1D6B
#define DATA_FLOW_GRAPHICS_SCENE_HXX_CD8E222F1D6B

#include "BasicGraphicsScene.hxx"
#include "DataFlowGraphModel.hxx"
#include "Export.hxx"

namespace QtNodes {

///
/// @brief An advanced scene working with data-propagating graphs. The class
/// represents a scene that existed in v2.x but built wit the new model-view
/// approach in mind.
///
class NODE_EDITOR_PUBLIC DataFlowGraphicsScene : public BasicGraphicsScene
{
    Q_OBJECT

public:
    DataFlowGraphicsScene(
        DataFlowGraphModel& graphModel,
        QObject*            parent = nullptr
    );

    ~DataFlowGraphicsScene() = default;

public:
    std::vector<NodeId> selectedNodes() const;

    QMenu* createSceneMenu(const QPointF scenePos) override;

public Q_SLOTS:
    bool save() const;

    bool load();

Q_SIGNALS:
    void sceneLoaded();

private:
    DataFlowGraphModel& _graphModel;
};

}  // namespace QtNodes

#endif  // DATA_FLOW_GRAPHICS_SCENE_HXX_CD8E222F1D6B

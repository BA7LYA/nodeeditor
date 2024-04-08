#pragma once

#include <QtCore/QJsonObject>
#include <QtCore/QPointF>
#include <QUndoCommand>
#include <unordered_set>

#include "Definitions.hpp"

namespace QtNodes {

class BasicGraphicsScene;

class CreateCommand : public QUndoCommand
{
public:
    CreateCommand(
        BasicGraphicsScene* scene,
        const QString       name,
        const QPointF&      mouseScenePos
    );

    void undo() override;
    void redo() override;

private:
    BasicGraphicsScene* _scene;
    NodeId              _nodeId;
    QJsonObject         _sceneJson;
};

/**
 * Selected scene objects are serialized and then removed from the scene.
 * The deleted elements could be restored in `undo`.
 */
class DeleteCommand : public QUndoCommand
{
public:
    DeleteCommand(BasicGraphicsScene* scene);

    void undo() override;
    void redo() override;

private:
    BasicGraphicsScene* _scene;
    QJsonObject         _sceneJson;
};

class CopyCommand : public QUndoCommand
{
public:
    CopyCommand(BasicGraphicsScene* scene);
};

class PasteCommand : public QUndoCommand
{
public:
    PasteCommand(BasicGraphicsScene* scene, const QPointF& mouseScenePos);

    void undo() override;
    void redo() override;

private:
    QJsonObject takeSceneJsonFromClipboard();
    QJsonObject makeNewNodeIdsInScene(const QJsonObject& sceneJson);

private:
    BasicGraphicsScene* _scene;
    const QPointF&      _mouseScenePos;
    QJsonObject         _newSceneJson;
};

class DisconnectCommand : public QUndoCommand
{
public:
    DisconnectCommand(BasicGraphicsScene* scene, const ConnectionId);

    void undo() override;
    void redo() override;

private:
    BasicGraphicsScene* _scene;

    ConnectionId _connId;
};

class ConnectCommand : public QUndoCommand
{
public:
    ConnectCommand(BasicGraphicsScene* scene, const ConnectionId);

    void undo() override;
    void redo() override;

private:
    BasicGraphicsScene* _scene;

    ConnectionId _connId;
};

class MoveNodeCommand : public QUndoCommand
{
public:
    MoveNodeCommand(BasicGraphicsScene* scene, const QPointF& diff);

    void undo() override;
    void redo() override;

    /**
     * A command ID is used in command compression. It must be an integer unique
     * to this command's class, or -1 if the command doesn't support
     * compression.
     */
    int id() const override;

    /**
     * Several sequential movements could be merged into one command.
     */
    bool mergeWith(const QUndoCommand* c) override;

private:
    BasicGraphicsScene*        _scene;
    std::unordered_set<NodeId> _selectedNodes;
    QPointF                    _diff;
};

}  // namespace QtNodes

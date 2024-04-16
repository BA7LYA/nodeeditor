///
/// @file UndoCommands.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef UNDO_COMMANDS_HXX_A974F3034C19
#define UNDO_COMMANDS_HXX_A974F3034C19

#include <QtCore/QJsonObject>
#include <QtCore/QPointF>
#include <QUndoCommand>
#include <unordered_set>

#include "Definitions.hxx"

namespace QtNodes {

class BasicGraphicsScene;

///
/// @brief
///
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

///
/// @brief Selected scene objects are serialized and then removed from the
/// scene. The deleted elements could be restored in `undo`.
///
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

///
/// @brief
///
class CopyCommand : public QUndoCommand
{
public:
    CopyCommand(BasicGraphicsScene* scene);
};

///
/// @brief
///
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

///
/// @brief
///
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

///
/// @brief
///
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

///
/// @brief
///
class MoveNodeCommand : public QUndoCommand
{
public:
    MoveNodeCommand(BasicGraphicsScene* scene, const QPointF& diff);

    void undo() override;
    void redo() override;

    ///
    /// @brief A command ID is used in command compression. It must be an
    /// integer unique to this command's class, or -1 if the command doesn't
    /// support compression.
    /// @return
    ///
    int id() const override;

    ///
    /// @brief Several sequential movements could be merged into one command.
    /// @param c
    /// @return
    ///
    bool mergeWith(const QUndoCommand* c) override;

private:
    BasicGraphicsScene*        _scene;
    std::unordered_set<NodeId> _selectedNodes;
    QPointF                    _diff;
};

}  // namespace QtNodes

#endif  // UNDO_COMMANDS_HXX_A974F3034C19

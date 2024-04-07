#pragma once

#include <memory>
#include <QtWidgets/QWidget>

#include "QtNodes/Definitions.hxx"
#include "QtNodes/Export.hxx"
#include "QtNodes/NodeData.hxx"
#include "QtNodes/NodeStyle.hxx"
#include "QtNodes/Serializable.hxx"

namespace QtNodes {

class StyleCollection;

/**
 * The class wraps Node-specific data operations and propagates it to
 * the nesting DataFlowGraphModel which is a subclass of
 * AbstractGraphModel.
 * This class is the same what has been called NodeDataModel before v3.
 */
class NODE_EDITOR_PUBLIC NodeDelegateModel
    : public QObject
    , public Serializable
{
    Q_OBJECT

public:
    NodeDelegateModel();

    virtual ~NodeDelegateModel() = default;

    /// It is possible to hide caption in GUI
    virtual bool captionVisible() const
    {
        return true;
    }

    /// Caption is used in GUI
    virtual QString caption() const = 0;

    /// It is possible to hide port caption in GUI
    virtual bool portCaptionVisible(PortType, PortIndex) const
    {
        return false;
    }

    /// Port caption is used in GUI to label individual ports
    virtual QString portCaption(PortType, PortIndex) const
    {
        return QString();
    }

    /// Name makes this model unique
    virtual QString name() const = 0;

public:
    QJsonObject save() const override;

    void load(const QJsonObject&) override;

public:
    virtual unsigned int nPorts(PortType portType) const = 0;

    virtual NodeDataType dataType(PortType portType, PortIndex portIndex) const
        = 0;

public:
    virtual ConnectionPolicy portConnectionPolicy(PortType, PortIndex) const;

    const NodeStyle& nodeStyle() const;

    void setNodeStyle(const NodeStyle& style);

public:
    virtual void setInData(
        std::shared_ptr<NodeData> nodeData,
        const PortIndex           portIndex
    ) = 0;

    virtual std::shared_ptr<NodeData> outData(const PortIndex port) = 0;

    /**
     * It is recommented to preform a lazy initialization for the
     * embedded widget and create it inside this function, not in the
     * constructor of the current model.
     *
     * Our Model Registry is able to shortly instantiate models in order
     * to call the non-static `Model::name()`. If the embedded widget is
     * allocated in the constructor but not actually embedded into some
     * QGraphicsProxyWidget, we'll gonna have a dangling pointer.
     */
    virtual QWidget* embeddedWidget() = 0;

    virtual bool resizable() const
    {
        return false;
    }

public Q_SLOTS:

    virtual void inputConnectionCreated(const ConnectionId&) {}

    virtual void inputConnectionDeleted(const ConnectionId&) {}

    virtual void outputConnectionCreated(const ConnectionId&) {}

    virtual void outputConnectionDeleted(const ConnectionId&) {}

Q_SIGNALS:

    /// Triggers the updates in the nodes downstream.
    void dataUpdated(const PortIndex index);

    /// Triggers the propagation of the empty data downstream.
    void dataInvalidated(const PortIndex index);

    void computingStarted();

    void computingFinished();

    void embeddedWidgetSizeUpdated();

    /// Call this function before deleting the data associated with ports.
    /**
     * The function notifies the Graph Model and makes it remove and recompute
     * the affected connection addresses.
     */
    void portsAboutToBeDeleted(
        const PortType  portType,
        const PortIndex first,
        const PortIndex last
    );

    /// Call this function when data and port moditications are finished.
    void portsDeleted();

    /// Call this function before inserting the data associated with ports.
    /**
     * The function notifies the Graph Model and makes it recompute the affected
     * connection addresses.
     */
    void portsAboutToBeInserted(
        const PortType  portType,
        const PortIndex first,
        const PortIndex last
    );

    /// Call this function when data and port moditications are finished.
    void portsInserted();

private:
    NodeStyle _nodeStyle;
};

}  // namespace QtNodes

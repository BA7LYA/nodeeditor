///
/// @file NodeDelegateModel.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef NODE_DELEGATE_MODEL_HXX_3848C3FD7CDF
#define NODE_DELEGATE_MODEL_HXX_3848C3FD7CDF

#include <memory>
#include <QtWidgets/QWidget>

#include "Definitions.hxx"
#include "Export.hxx"
#include "NodeData.hxx"
#include "NodeStyle.hxx"
#include "Serializable.hxx"

namespace QtNodes {

class StyleCollection;

///
/// @brief The class wraps Node-specific data operations and propagates it to
/// the nesting DataFlowGraphModel which is a subclass of AbstractGraphModel.
/// This class is the same what has been called NodeDataModel before v3.
///
class NODE_EDITOR_PUBLIC NodeDelegateModel
    : public QObject
    , public Serializable
{
    Q_OBJECT

public:
    NodeDelegateModel();

    virtual ~NodeDelegateModel() = default;

    ///
    /// @brief It is possible to hide caption in GUI.
    ///
    /// @return true
    /// @return false
    ///
    virtual bool captionVisible() const
    {
        return true;
    }

    ///
    /// @brief Caption is used in GUI.
    ///
    /// @return QString
    ///
    virtual QString caption() const = 0;

    ///
    /// @brief It is possible to hide port caption in GUI.
    ///
    /// @return true
    /// @return false
    ///
    virtual bool portCaptionVisible(PortType, PortIndex) const
    {
        return false;
    }

    ///
    /// @brief Port caption is used in GUI to label individual ports.
    ///
    /// @return QString
    ///
    virtual QString portCaption(PortType, PortIndex) const
    {
        return QString();
    }

    ///
    /// @brief Name makes this model unique.
    ///
    /// @return QString
    ///
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

    ///
    /// @brief It is recommented to preform a lazy initialization for the
    /// embedded widget and create it inside this function, not in the
    /// constructor of the current model. Our Model Registry is able to shortly
    /// instantiate models in order to call the non-static `Model::name()`. If
    /// the embedded widget is allocated in the constructor but not actually
    /// embedded into some QGraphicsProxyWidget, we'll gonna have a dangling
    /// pointer.
    /// @return
    ///
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

    ///
    /// @brief Triggers the updates in the nodes downstream.
    ///
    /// @param index
    ///
    void dataUpdated(const PortIndex index);

    ///
    /// @brief Triggers the propagation of the empty data downstream.
    ///
    /// @param index
    ///
    void dataInvalidated(const PortIndex index);

    void computingStarted();

    void computingFinished();

    void embeddedWidgetSizeUpdated();

    ///
    /// @brief Call this function before deleting the data associated with
    /// ports. The function notifies the Graph Model and makes it remove and
    /// recompute the affected connection addresses.
    ///
    /// @param portType
    /// @param first
    /// @param last
    ///
    void portsAboutToBeDeleted(
        const PortType  portType,
        const PortIndex first,
        const PortIndex last
    );

    ///
    /// @brief Call this function when data and port moditications are finished.
    ///
    void portsDeleted();

    ///
    /// @brief Call this function before inserting the data associated with
    /// ports. The function notifies the Graph Model and makes it recompute the
    /// affected connection addresses.
    ///
    /// @param portType
    /// @param first
    /// @param last
    ///
    void portsAboutToBeInserted(
        const PortType  portType,
        const PortIndex first,
        const PortIndex last
    );

    ///
    /// @brief Call this function when data and port moditications are finished.
    ///
    void portsInserted();

private:
    NodeStyle _nodeStyle;
};

}  // namespace QtNodes

#endif  // NODE_DELEGATE_MODEL_HXX_3848C3FD7CDF

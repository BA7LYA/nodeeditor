///
/// @file PortAddRemoveWidget.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef PORT_ADD_REMOVE_WIDGET_HXX_FBDF438E9E29
#define PORT_ADD_REMOVE_WIDGET_HXX_FBDF438E9E29

#include <QHBoxLayout>
#include <QPushButton>
#include <QtNodes/Definitions>
#include <QVBoxLayout>
#include <QWidget>

using QtNodes::NodeId;
using QtNodes::PortIndex;
using QtNodes::PortType;

class DynamicPortsModel;

/**
 *                PortAddRemoveWidget
 *
 * ```
 *       _left                         _right
 *       layout                        layout
 *     ----------------------------------------
 *     |         |                  |         |
 *     | [+] [-] |                  | [+] [-] |
 *     |         |                  |         |
 *     | [+] [-] |                  | [+] [-] |
 *     |         |                  |         |
 *     | [+] [-] |                  | [+] [-] |
 *     |         |                  |         |
 *     | [+] [-] |                  |         |
 *     |         |                  |         |
 *     |_________|__________________|_________|
 * ```
 *
 * The widget has two main vertical layouts containing groups of buttons for
 * adding and removing ports. Each such a `[+] [-]` group is contained in a
 * dedicated QHVBoxLayout.
 *
 */
class PortAddRemoveWidget : public QWidget
{
    Q_OBJECT

public:
    PortAddRemoveWidget(
        unsigned int       nInPorts,
        unsigned int       nOutPorts,
        NodeId             nodeId,
        DynamicPortsModel& model,
        QWidget*           parent = nullptr
    );

    ~PortAddRemoveWidget();

    ///
    /// @brief Called from constructor, creates all button groups according to
    /// models'port counts.
    /// @param portType
    /// @param nPorts
    ///
    void populateButtons(PortType portType, unsigned int nPorts);

    ///
    /// @brief Adds a single `[+][-]` button group to a given layout.
    /// @param vbl
    /// @param portIndex
    /// @return
    ///
    QHBoxLayout* addButtonGroupToLayout(
        QVBoxLayout* vbl,
        unsigned int portIndex
    );

    ///
    /// @brief Removes a single `[+][-]` button group from a given layout.
    ///
    /// @param vbl
    /// @param portIndex
    ///
    void removeButtonGroupFromLayout(QVBoxLayout* vbl, unsigned int portIndex);

private Q_SLOTS:
    void onPlusClicked();
    void onMinusClicked();

private:
    ///
    /// @brief
    ///
    /// @param sender
    /// @param buttonIndex is the index of a button in the layout. Plus button
    /// has the index 0. Minus button has the index 1.
    /// @return `std::pair<PortType, PortIndex>`
    ///
    std::pair<PortType, PortIndex> findWhichPortWasClicked(
        QObject*  sender,
        const int buttonIndex
    );

private:
    const NodeId       _nodeId;
    DynamicPortsModel& _model;

    QVBoxLayout* _left;
    QVBoxLayout* _right;
};

#endif  // PORT_ADD_REMOVE_WIDGET_HXX_FBDF438E9E29

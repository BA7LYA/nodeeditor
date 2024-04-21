///
/// @file GraphicsView.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-16
/// @copyright Copyright (c) 2024
///

#ifndef GRAPHICS_VIEW_HXX_089BD984A91D
#define GRAPHICS_VIEW_HXX_089BD984A91D

#include <QtWidgets/QGraphicsView>

#include "Export.hxx"

namespace QtNodes {

class BasicGraphicsScene;

///
/// @brief A central view able to render objects from `BasicGraphicsScene`.
///
class NODE_EDITOR_PUBLIC GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    ///
    /// @brief ScaleRange
    ///
    struct ScaleRange
    {
        double minimum = 0;
        double maximum = 0;
    };

public:
    GraphicsView(QWidget* parent = Q_NULLPTR);
    GraphicsView(BasicGraphicsScene* scene, QWidget* parent = Q_NULLPTR);

    GraphicsView(const GraphicsView&)           = delete;
    GraphicsView operator=(const GraphicsView&) = delete;

    ///
    /// @brief
    ///
    /// @return QAction*
    ///
    QAction* clearSelectionAction() const;

    ///
    /// @brief
    /// @return
    ///
    QAction* deleteSelectionAction() const;

    ///
    /// @brief
    /// @param scene
    ///
    void setScene(BasicGraphicsScene* scene);

    ///
    /// @brief
    ///
    void centerScene();

    ///
    /// @brief max=0/min=0 indicates infinite zoom in/out
    ///
    /// @param minimum
    /// @param maximum
    ///
    void setScaleRange(double minimum = 0, double maximum = 0);

    ///
    /// @brief
    /// @param range
    ///
    void setScaleRange(ScaleRange range);

    ///
    /// @brief
    /// @return
    ///
    double getScale() const;

public Q_SLOTS:
    void scaleUp();
    void scaleDown();
    void setupScale(double scale);
    void onDeleteSelectedObjects();
    void onDuplicateSelectedObjects();
    void onCopySelectedObjects();
    void onPasteObjects();

Q_SIGNALS:
    void scaleChanged(double scale);

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void drawBackground(QPainter* painter, const QRectF& r) override;
    void showEvent(QShowEvent* event) override;

protected:
    ///
    /// @brief
    /// @return
    ///
    BasicGraphicsScene* nodeScene();

    ///
    /// @brief Computes scene position for pasting the copied/duplicated node
    /// groups.
    ///
    /// @return QPointF
    ///
    QPointF scenePastePosition();

private:
    QAction* _clearSelectionAction     = nullptr;
    QAction* _deleteSelectionAction    = nullptr;
    QAction* _duplicateSelectionAction = nullptr;
    QAction* _copySelectionAction      = nullptr;
    QAction* _pasteAction              = nullptr;

    QPointF    _clickPos;
    ScaleRange _scaleRange;
};

}  // namespace QtNodes

#endif  // GRAPHICS_VIEW_HXX_089BD984A91D

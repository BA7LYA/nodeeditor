///
/// @file PixmapData.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef PIXMAP_DATA_HXX_1996C274B13E
#define PIXMAP_DATA_HXX_1996C274B13E

#include <QtGui/QPixmap>
#include <QtNodes/NodeData>

using QtNodes::NodeData;
using QtNodes::NodeDataType;

///
/// @brief The class can potentially incapsulate any user data which need to be
/// transferred within the Node Editor graph.
///
class PixmapData : public NodeData
{
public:
    PixmapData() {}

    PixmapData(const QPixmap& pixmap)
        : _pixmap(pixmap)
    {
    }

    NodeDataType type() const override
    {
        // id      name
        return { "pixmap", "P" };
    }

    QPixmap pixmap() const
    {
        return _pixmap;
    }

private:
    QPixmap _pixmap;
};

#endif  // PIXMAP_DATA_HXX_1996C274B13E

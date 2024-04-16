///
/// @file NodeData.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef NODE_DATA_HXX_F6BB0207C6C2
#define NODE_DATA_HXX_F6BB0207C6C2

#include <memory>
#include <QtCore/QObject>
#include <QtCore/QString>

#include "Export.hxx"

namespace QtNodes {

///
/// @brief `id` represents an internal unique data type for the given port.
/// `name` is a normal text description.
///
struct NODE_EDITOR_PUBLIC NodeDataType
{
    QString id;
    QString name;
};

///
/// @brief Class represents data transferred between nodes. `type` is used for
/// comparing the types. The actual data is stored in subtypes.
///
class NODE_EDITOR_PUBLIC NodeData
{
public:
    virtual ~NodeData() = default;

    virtual bool sameType(const NodeData& nodeData) const
    {
        return (this->type().id == nodeData.type().id);
    }

    ///
    /// @brief Type for inner use.
    ///
    /// @return NodeDataType
    ///
    virtual NodeDataType type() const = 0;
};

}  // namespace QtNodes

Q_DECLARE_METATYPE(QtNodes::NodeDataType)
Q_DECLARE_METATYPE(std::shared_ptr<QtNodes::NodeData>)

#endif  // NODE_DATA_HXX_F6BB0207C6C2

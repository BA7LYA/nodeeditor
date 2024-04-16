///
/// @file NodeDelegateModelRegistry.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#include "NodeDelegateModelRegistry.hxx"

#include <QtCore/QFile>
#include <QtWidgets/QMessageBox>

using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::NodeDelegateModelRegistry;

std::unique_ptr<NodeDelegateModel> NodeDelegateModelRegistry::create(
    const QString& modelName
)
{
    auto it = _registeredItemCreators.find(modelName);

    if (it != _registeredItemCreators.end())
    {
        return it->second();
    }

    return nullptr;
}

const NodeDelegateModelRegistry::RegisteredModelCreatorsMap&
    NodeDelegateModelRegistry::registeredModelCreators() const
{
    return _registeredItemCreators;
}

const NodeDelegateModelRegistry::RegisteredModelsCategoryMap&
    NodeDelegateModelRegistry::registeredModelsCategoryAssociation() const
{
    return _registeredModelsCategory;
}

const NodeDelegateModelRegistry::CategoriesSet& NodeDelegateModelRegistry::
    categories() const
{
    return _categories;
}

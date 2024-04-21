///
/// @file NodeDelegateModelRegistry.hxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-17
/// @copyright Copyright (c) 2024
///

#ifndef NODE_DELEGATE_MODEL_REGISTRY_HXX_7DC3D117A46E
#define NODE_DELEGATE_MODEL_REGISTRY_HXX_7DC3D117A46E

#include <functional>
#include <memory>
#include <QtCore/QString>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Export.hxx"
#include "NodeData.hxx"
#include "NodeDelegateModel.hxx"
#include "QStringStdHash.hxx"

namespace QtNodes {

///
/// @brief Class uses map for storing models (name, model).
///
class NODE_EDITOR_PUBLIC NodeDelegateModelRegistry
{
public:
    using RegistryItemPtr     = std::unique_ptr<NodeDelegateModel>;
    using RegistryItemCreator = std::function<RegistryItemPtr()>;
    using RegisteredModelCreatorsMap
        = std::unordered_map<QString, RegistryItemCreator>;
    using RegisteredModelsCategoryMap = std::unordered_map<QString, QString>;
    using CategoriesSet               = std::set<QString>;

    // using RegisteredTypeConvertersMap = std::map<TypeConverterId,
    // TypeConverter>;

    NodeDelegateModelRegistry()  = default;
    ~NodeDelegateModelRegistry() = default;

    NodeDelegateModelRegistry(const NodeDelegateModelRegistry&) = delete;
    NodeDelegateModelRegistry(NodeDelegateModelRegistry&&)      = default;

    NodeDelegateModelRegistry& operator=(const NodeDelegateModelRegistry&)
        = delete;

    NodeDelegateModelRegistry& operator=(NodeDelegateModelRegistry&&) = default;

public:
    ///
    /// @brief
    /// @tparam ModelType
    /// @param creator
    /// @param category
    ///
    template<typename ModelType>
    void registerModel(
        RegistryItemCreator creator,
        const QString&      category = "Nodes"
    )
    {
        const QString name = computeName<ModelType>(
            HasStaticMethodName<ModelType> {},
            creator
        );
        if (!_registeredItemCreators.count(name))
        {
            _registeredItemCreators[name] = std::move(creator);
            _categories.insert(category);
            _registeredModelsCategory[name] = category;
        }
    }

    ///
    /// @brief
    /// @tparam ModelType
    /// @param category
    ///
    template<typename ModelType>
    void registerModel(const QString& category = "Nodes")
    {
        RegistryItemCreator creator
            = []() { return std::make_unique<ModelType>(); };
        registerModel<ModelType>(std::move(creator), category);
    }

#if 0
  template<typename ModelType>
  void
  registerModel(RegistryItemCreator creator,
                QString const&      category = "Nodes")
  {
    registerModel<ModelType>(std::move(creator), category);
  }


  template <typename ModelCreator>
  void
  registerModel(ModelCreator&& creator, QString const& category = "Nodes")
  {
    using ModelType = compute_model_type_t<decltype(creator())>;
    registerModel<ModelType>(std::forward<ModelCreator>(creator), category);
  }


  template <typename ModelCreator>
  void
  registerModel(QString const& category, ModelCreator&& creator)
  {
    registerModel(std::forward<ModelCreator>(creator), category);
  }


  void
  registerTypeConverter(TypeConverterId const& id,
                        TypeConverter          typeConverter)
  {
    _registeredTypeConverters[id] = std::move(typeConverter);
  }

#endif

    std::unique_ptr<NodeDelegateModel> create(const QString& modelName);

    const RegisteredModelCreatorsMap& registeredModelCreators() const;

    const RegisteredModelsCategoryMap& registeredModelsCategoryAssociation(
    ) const;

    const CategoriesSet& categories() const;

#if 0
  TypeConverter
  getTypeConverter(NodeDataType const& d1,
                   NodeDataType const& d2) const;
#endif

private:
    RegisteredModelsCategoryMap _registeredModelsCategory;

    CategoriesSet _categories;

    RegisteredModelCreatorsMap _registeredItemCreators;

#if 0
  RegisteredTypeConvertersMap _registeredTypeConverters;
#endif

private:
    ///
    /// @brief If the registered ModelType class has the static member method
    /// `static QString Name();`, use it. Otherwise use the non-static method:
    /// `virtual QString name() const;`.
    ///
    /// @tparam T
    /// @tparam typename
    ///
    template<typename T, typename = void>
    struct HasStaticMethodName : std::false_type
    {
    };

    ///
    /// @brief
    ///
    /// @tparam T
    ///
    template<typename T>
    struct HasStaticMethodName<
        T,
        typename std::enable_if<
            std::is_same<decltype(T::Name()), QString>::value>::type>
        : std::true_type
    {
    };

    ///
    /// @brief
    /// @tparam ModelType
    /// @param std::true_type
    /// @param const RegistryItemCreator&
    /// @return QString
    ///
    template<typename ModelType>
    static QString computeName(std::true_type, const RegistryItemCreator&)
    {
        return ModelType::Name();
    }

    ///
    /// @brief
    ///
    /// @tparam ModelType
    /// @param creator
    /// @return QString
    ///
    template<typename ModelType>
    static QString computeName(
        std::false_type,
        const RegistryItemCreator& creator
    )
    {
        return creator()->name();
    }

    ///
    /// @brief
    ///
    /// @tparam T
    ///
    template<typename T>
    struct UnwrapUniquePtr
    {
        // Assert always fires, but the compiler doesn't know this:
        static_assert(
            !std::is_same<T, T>::value,
            "The ModelCreator must return a std::unique_ptr<T>, where T "
            "inherits from NodeDelegateModel"
        );
    };

    ///
    /// @brief
    ///
    /// @tparam T
    ///
    template<typename T>
    struct UnwrapUniquePtr<std::unique_ptr<T>>
    {
        static_assert(
            std::is_base_of<NodeDelegateModel, T>::value,
            "The ModelCreator must return a std::unique_ptr<T>, where T "
            "inherits from NodeDelegateModel"
        );
        using type = T;
    };

    template<typename CreatorResult>
    using compute_model_type_t = typename UnwrapUniquePtr<CreatorResult>::type;
};

}  // namespace QtNodes

#endif  // NODE_DELEGATE_MODEL_REGISTRY_HXX_7DC3D117A46E

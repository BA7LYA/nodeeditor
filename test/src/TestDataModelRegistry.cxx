///
/// @file TestDataModelRegistry.cxx
/// @author BA7LYA (1042140025@qq.com)
/// @brief
/// @version 0.1
/// @date 2024-04-22
/// @copyright Copyright (c) 2024
///

#include <catch.hpp>
#include <QtNodes/DataModelRegistry>

#include "StubNodeDataModel.hxx"

using QtNodes::DataModelRegistry;
using QtNodes::NodeData;
using QtNodes::NodeDataModel;
using QtNodes::NodeDataType;
using QtNodes::PortIndex;
using QtNodes::PortType;

namespace {
class StubModelStaticName : public StubNodeDataModel
{
public:
    static QString Name()
    {
        return "Name";
    }
};
}  // namespace

TEST_CASE("DataModelRegistry::registerModel", "[interface]")
{
    DataModelRegistry registry;

    SECTION("stub model")
    {
        registry.registerModel<StubNodeDataModel>();
        auto model = registry.create("name");

        CHECK(model->name() == "name");
    }
    SECTION("stub model with static name")
    {
        registry.registerModel<StubModelStaticName>();
        auto model = registry.create("Name");

        CHECK(model->name() == "name");
    }
    SECTION("From model creator function")
    {
        SECTION("non-static name()")
        {
            registry.registerModel(
                [] { return std::make_unique<StubNodeDataModel>(); }
            );

            auto model = registry.create("name");

            REQUIRE(model != nullptr);
            CHECK(model->name() == "name");
            CHECK(dynamic_cast<StubNodeDataModel*>(model.get()));
        }
        SECTION("static Name()")
        {
            registry.registerModel(
                [] { return std::make_unique<StubModelStaticName>(); }
            );

            auto model = registry.create("Name");

            REQUIRE(model != nullptr);
            CHECK(model->name() == "name");
            CHECK(dynamic_cast<StubModelStaticName*>(model.get()));
        }
    }
}

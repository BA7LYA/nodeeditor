if (Qt6_FOUND)
    find_package(Qt6 COMPONENTS Test)
    set(Qt Qt)
else()
    find_package(Qt5 COMPONENTS Test)
    set(Qt Qt5)
endif()

find_package(Catch2 CONFIG REQUIRED)

add_executable(test_nodes
    # test_main.cxx
    src/TestDragging.cxx
    src/TestDataModelRegistry.cxx
    src/TestFlowScene.cxx
    src/TestNodeGraphicsObject.cxx
    include/ApplicationSetup.hxx
    include/Stringify.hxx
    include/StubNodeDataModel.hxx
)

target_include_directories(test_nodes
    PRIVATE
    ../src
    ../include/internal
    include
)

target_link_libraries(test_nodes
    PRIVATE
        QtNodes::QtNodes
        Catch2::Catch2
        Catch2::Catch2WithMain
        ${Qt}::Test
)

add_test(
    NAME test_nodes
    COMMAND
        $<TARGET_FILE:test_nodes>
        $<$<BOOL:${NE_FORCE_TEST_COLOR}>:--use-colour=yes>
)

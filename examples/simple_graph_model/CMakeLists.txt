file(GLOB_RECURSE CPPS  ./*.cxx )
file(GLOB_RECURSE HPPS  ./*.hxx )

add_executable(simple_graph_model ${CPPS} ${HPPS})

target_link_libraries(simple_graph_model QtNodes)

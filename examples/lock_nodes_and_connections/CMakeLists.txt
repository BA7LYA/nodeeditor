file(GLOB_RECURSE CPPS  ./*.cxx )

add_executable(lock_nodes_and_connections ${CPPS})

target_link_libraries(lock_nodes_and_connections QtNodes)

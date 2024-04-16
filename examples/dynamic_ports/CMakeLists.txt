file(GLOB_RECURSE CPPS  ./*.cxx )

add_executable(dynamic_ports ${CPPS})

target_link_libraries(dynamic_ports QtNodes)

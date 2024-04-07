file(GLOB_RECURSE CPPS  ./*.cxx )
file(GLOB_RECURSE HPPS  ./*.hxx )

add_executable(connection_colors ${CPPS} ${HPPS})

target_link_libraries(connection_colors QtNodes)

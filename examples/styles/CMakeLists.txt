file(GLOB_RECURSE CPPS  ./*.cxx )
file(GLOB_RECURSE HPPS  ./*.hxx )

add_executable(styles ${CPPS} ${HPPS})

target_link_libraries(styles QtNodes)

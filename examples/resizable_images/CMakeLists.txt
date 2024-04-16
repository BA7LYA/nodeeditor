file(GLOB_RECURSE CPPS  ./*.cxx )
file(GLOB_RECURSE HPPS  ./*.hxx )

add_executable(resizable_images ${CPPS} ${HPPS})

target_link_libraries(resizable_images QtNodes)

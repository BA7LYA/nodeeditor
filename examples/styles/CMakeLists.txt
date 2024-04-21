file(GLOB_RECURSE CPPS  ./*.cxx )
file(GLOB_RECURSE HPPS  ./*.hxx )

add_executable(styles ${CPPS} ${HPPS})

target_link_libraries(styles QtNodes)

# TODO(ba7lya)
# add_custom_command(
#     TARGET QtNodes
#     POST_BUILD
#     COMMAND ${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_DEFAULT_TRIPLET}/tools/Qt6/bin/
# )

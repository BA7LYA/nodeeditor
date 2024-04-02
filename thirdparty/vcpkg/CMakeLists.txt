# --- CMake Version ---
cmake_minimum_required(VERSION 3.21)

# --- vcpkg ---
SET(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")

# --- Project ---
project(vcpkg LANGUAGES CXX)

# --- Build Options ---
option(${PROJECT_NAME}_BUILD_BENCHMARK "Build the benchmark" OFF)
option(${PROJECT_NAME}_BUILD_DEMO "Build demo programs" OFF)
option(${PROJECT_NAME}_BUILD_DOC "Build Documentation" OFF)
option(${PROJECT_NAME}_BUILD_TEST "Build Test" OFF)

# --- Modules ---
include(FetchContent)
include(CTest)
include(GoogleTest)

# --- Dependency ---
find_package(spdlog CONFIG REQUIRED)
find_package(gtest CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# --- FetchContent ---
FetchContent_Declare(
	${PROJECT_NAME}
	GIT_REPOSITORY https://github.com/thedmd/imgui-node-editor.git
	GIT_TAG 2f99b2d613a400f6579762bd7e7c343a0d844158
)
FetchContent_MakeAvailable(${PROJECT_NAME})

# --- Taget ---
add_library(${PROJECT_NAME})

# --- Include Directory ---
target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	${${PROJECT_NAME}_SOURCE_DIR}/
)

# --- Source Directory ---
file(GLOB RPOJECT_SRC
	${${PROJECT_NAME}_SOURCE_DIR}/*.cpp
)
target_sources(
	${PROJECT_NAME}
	PRIVATE
	${RPOJECT_SRC}
)

# --- Linkage ---
target_link_libraries(
	${PROJECT_NAME}
	PUBLIC
	imgui::imgui
)

# --- Compile Feature ---
target_compile_features(
	${PROJECT_NAME}
	PRIVATE
	cxx_std_20
)

# --- Unit Tests ---
if(${PROJECT_NAME}_BUILD_TEST)
	enable_testing()
	add_subdirectory(test)
endif()

# --- Benchmark ---
if(${PROJECT_NAME}_BUILD_BENCHMARK)
	add_subdirectory(benchmark)
endif()

# --- Demo ---
if(${PROJECT_NAME}_BUILD_DEMO)
	add_subdirectory(demo)
endif()
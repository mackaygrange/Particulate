cmake_minimum_required(VERSION 3.30)
project(Particulate)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(LIBS)
file(GLOB SOURCE_FILES "src/*.cpp" "src/*.c")
file(GLOB HEADER_FILES "include/*.h" "include/*.hpp")
file(GLOB TEST_FILES "tests/*.cpp")

# CPM
file(
        DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
        EXPECTED_HASH SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

# SDL
CPMAddPackage("gh:libsdl-org/SDL#ac82534") # LAST UPDATED: 06/19/2025
list(APPEND LIBS SDL3::SDL3)

# SDL_image (removed for particle-only build)

# flecs
# CPMAddPackage("gh:SanderMertens/flecs#befc214")
# list(APPEND LIBS flecs::flecs)

# SQLite
# CPMAddPackage("gh:SRombauts/SQLiteCpp#643b153")
# list(APPEND LIBS SQLiteCpp)

# Executables
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})
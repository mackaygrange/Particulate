# CMake Standards:
cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SDL2_DIR "lib/SDL2/cmake")

# Compilation Information:
project(ferl)

file(GLOB SOURCE_FILES "src/*.cpp" "src/*.c")
file(GLOB HEADER_FILES "include/*.h")
file(GLOB TEST_FILES "tests/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/lib/SDL2/include ${CMAKE_SOURCE_DIR}/lib/nlohmann)

# Find SDL2 and link
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 SDL2::SDL2main SDL2_image::SDL2_image)

# Copy SDL2.dll to the output directory after build (Windows only)
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/lib/SDL2/lib/x64/SDL2.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    # Attempt copy of SDL2_image.dll variants if present
    foreach(dll SDL2_image.dll libpng16-16.dll zlib1.dll jpeg.dll)
        if(EXISTS "${CMAKE_SOURCE_DIR}/lib/SDL2/lib/x64/${dll}")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_SOURCE_DIR}/lib/SDL2/lib/x64/${dll}"
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
            )
        endif()
    endforeach()
endif()

# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   Specify the commit you depend on and update it regularly.
#   URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
#   DOWNLOAD_EXTRACT_TIMESTAMP True
# )
# For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# FetchContent_MakeAvailable(googletest)

# add_executable(run_tests ${SOURCE_FILES} ${TEST_FILES})

# Add test executable and link SDL2 for tests if needed
# target_include_directories(run_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
# target_link_libraries(run_tests gtest_main SDL2::SDL2 SDL2::SDL2main)
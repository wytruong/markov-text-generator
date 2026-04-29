cmake_minimum_required(VERSION 3.10)
project(TextGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Consistent output locations: put executables in build/bin and libraries in build/lib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# For multi-config generators
foreach(OUTPUT_CONFIG Debug Release RelWithDebInfo MinSizeRel)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_BINARY_DIR}/bin/${OUTPUT_CONFIG})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_BINARY_DIR}/lib/${OUTPUT_CONFIG})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${CMAKE_BINARY_DIR}/lib/${OUTPUT_CONFIG})
endforeach()

# Collect all .cpp files under src/
file(GLOB SRC_FILES CONFIGURE_DEPENDS src/*.cpp)

add_executable(text_generator ${SRC_FILES})

target_include_directories(text_generator PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    target_compile_options(text_generator PRIVATE -Wall -Wextra -Wpedantic)
elseif(MSVC)
    target_compile_options(text_generator PRIVATE /W4 /permissive-)
endif()

# Link stdc++fs on very old GCC (if using std::filesystem pre-GCC 9)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
    target_link_libraries(text_generator stdc++fs)
endif()

# -------------------- GoogleTest --------------------
include(CTest)
if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    file(GLOB GTEST_SRC CONFIGURE_DEPENDS gtest/*.cpp)
    if(GTEST_SRC)
        # Build tests as a separate executable; include project headers
        add_executable(text_generator_tests ${GTEST_SRC} src/FreqList.cpp src/WordStream.cpp)
        target_include_directories(text_generator_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
        target_link_libraries(text_generator_tests GTest::gtest_main)
        if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
            target_compile_options(text_generator_tests PRIVATE -Wall -Wextra -Wpedantic)
        elseif(MSVC)
            target_compile_options(text_generator_tests PRIVATE /W4 /permissive-)
        endif()
        set_target_properties(text_generator_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
        include(GoogleTest)
        gtest_discover_tests(text_generator_tests)
    endif()
endif()

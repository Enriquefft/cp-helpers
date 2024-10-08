cmake_minimum_required(VERSION 3.16)

project(
  CPP_TEMPLATE
  VERSION 1.0
  DESCRIPTION "A C++ template"
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

if(${CMAKE_VERSION} VERSION_LESS "3.25.2")
  message(STATUS "CMake version is less than 3.25.2,
			upgrade to silence unnecesary warnings")
endif()

# ##############################################################################
# Targets

add_executable(${PROJECT_NAME} src/main.cpp)

# ##############################################################################

add_library(common INTERFACE)
target_compile_features(common INTERFACE cxx_std_20)

# TODO: add iwty required - curently not working on windows
find_program(iwyu_path NAMES include-what-you-use iwyu)
set_target_properties(
  ${PROJECT_NAME} PROPERTIES CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path}
                             INTERPROCEDURAL_OPTIMIZATION TRUE)

target_compile_options(
  common
  INTERFACE # Prefered warnings
            $<$<CXX_COMPILER_ID:MSVC>:
            /Wall
            >
            $<$<CXX_COMPILER_ID:Clang>:-Weverything>
            $<$<CXX_COMPILER_ID:GCC>:-fconcepts-diagnostics-depth=3>
            $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
            -Werror
            -Wall
            -Wextra
            -Wpedantic
            -Wformat=2
            -Wshadow
            -Wwrite-strings
            -Wcast-qual
            -Wcast-align
            -Wno-switch
            -Wconversion
            -Wno-ignored-optimization-argument
            > # Disable some warnings when using clang's Weverything
            $<$<CXX_COMPILER_ID:Clang>:
            -Wno-c++98-compat
            -Wno-c++98-compat-pedantic
            -Wno-c++20-compat
            -Wno-c++20-extensions
            -Wno-c99-extensions
            -Wno-zero-as-null-pointer-constant
            -Wno-error=padded
            >
            $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize=address,undefined> #
            $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize-address-use-after-scope>
            $<$<CONFIG:DEBUG>:
            $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
            -ggdb3
            -O0
            >
            $<$<CXX_COMPILER_ID:MSVC>:
            /GL
            /LTCG
            /O3>
            >
            $<$<CONFIG:RELEASE>:
            $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
            -Ofast
            >
            >)

target_link_options(
  common INTERFACE
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize=address,undefined>)

# Threads needed in gcc
set(THREADS_HAVE_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads)

# config
target_link_libraries(${PROJECT_NAME} PRIVATE common)

# ##############################################################################

# Ctest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

if(${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
  option(PACKAGE_TESTS "Build the tests" ON)
endif()

if(${PACKAGE_TESTS})
  enable_testing()
  include(GoogleTest)
  add_subdirectory(tests)
endif()

cmake_minimum_required(VERSION 3.20)

project(BlazeScript LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find LLVM
set(LLVM_DIR "/opt/homebrew/opt/llvm/lib/cmake/llvm")
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")


include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(blazescript
    src/main.cpp
    src/ast_printer.cpp
    src/parse_expr.cpp
    src/parsestmt.cpp
    src/scanner.cpp
    src/scope.cpp
    src/semantic_analyzer.cpp
    src/token_type.cpp
    src/token.cpp
    src/type.cpp
    src/codegen_expr.cpp
    src/codegenstmt.cpp
    src/codegen_func.cpp
)

target_include_directories(blazescript PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)


llvm_map_components_to_libnames(LLVM_LIBS
    Core
    Support
    IRReader
)

target_link_libraries(blazescript PRIVATE
    ${LLVM_LIBS}
)
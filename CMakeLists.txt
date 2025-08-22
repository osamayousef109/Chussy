cmake_minimum_required(VERSION 3.30)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(ChessEngine main.cpp
)
# --- after add_executable(ChessEngine ... ) ---

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-flto" COMPILER_SUPPORTS_FLTO)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(ChessEngine PRIVATE
            -O3
            -march=native
    )
    if(COMPILER_SUPPORTS_FLTO)
        target_compile_options(ChessEngine PRIVATE -flto)
        # add to link step as well
        if (COMMAND target_link_options) # CMake 3.13+
            target_link_options(ChessEngine PRIVATE -flto)
        else()
            # fallback for very old CMake (rare)
            target_link_libraries(ChessEngine PRIVATE -flto)
        endif()
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(ChessEngine PRIVATE /O2 /Ob2 /GL)
endif()


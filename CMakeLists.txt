cmake_minimum_required(VERSION 3.16)
project(WavesLang LANGUAGES CXX)

# --- Compilation options ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Source files ---
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
)

add_executable(WavesLang ${SOURCES})

# --- Include directories ---
target_include_directories(WavesLang PRIVATE
    src
    src/lexer
    src/parser
    src/interpreter
    src/runtime
    src/common
)

if (MSVC)
    if (POLICY CMP0141)
        cmake_policy(SET CMP0141 NEW)
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT 
            "$<IF:$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue,ProgramDatabase>")
    endif()
endif()

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

add_executable(WavesLang ${SOURCES} "src/ast/AstPrinter.h")


# --- Fix working directory for Visual Studio ---
set_target_properties(WavesLang PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# --- Copy runtime folders (examples + vendor) ---
add_custom_command(TARGET WavesLang POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/src/examples"
        "$<TARGET_FILE_DIR:WavesLang>/src/examples"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/src/vendor"
        "$<TARGET_FILE_DIR:WavesLang>/src/vendor"
)

# --- Include directories ---
target_include_directories(WavesLang PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# --- MSVC-specific debugging format ---
if (MSVC)
    if (POLICY CMP0141)
        cmake_policy(SET CMP0141 NEW)
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT 
            "$<IF:$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue,ProgramDatabase>")
    endif()
endif()

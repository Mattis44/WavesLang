# CMakeList.txt : CMake project for WavesLang, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("WavesLang")

# Add source to this project's executable.
add_executable (WavesLang "src/main.cpp"  "src/lexer/TokenType.h" "src/lexer/Token.h" "src/lexer/Lexer.cpp" "src/lexer/Lexer.h" "src/lexer/Token.cpp" "src/parser/Parser.h" "src/parser/Stmt.h" "src/parser/Parser.cpp" "src/parser/Stmt.cpp" "src/interpreter/Interpreter.h" "src/interpreter/Interpreter.cpp" "src/runtime/ImportManager.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET WavesLang PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

# CMakeList.txt : CMake project for system-forge, include source and define
cmake_minimum_required (VERSION 3.12)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("system-forge" C CXX)

# Set C and C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)

# Enable optimizations in Release
set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

# Add source to this project's library.
add_subdirectory(libs/allocator/arena)
add_subdirectory(libs/allocator/pool)
add_subdirectory(libs/security/highwayhash)
add_subdirectory(libs/data/hashtable)

# Add benchmarks for libraries
add_subdirectory(benchmarks/allocator)

# Add source to this project's executable.
add_executable (system-forge "system-forge.cc" "system-forge.h")

# TODO: Add tests and install targets if needed.

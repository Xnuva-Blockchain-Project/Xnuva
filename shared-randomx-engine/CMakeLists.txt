cmake_minimum_required(VERSION 3.20)
project(XnuvaRandomXFoundation VERSION 0.1.0 LANGUAGES C CXX ASM)

# Standalone reusable RandomX v2 hashing library.
# This project NEVER chooses a fork height, changes blockchain consensus
# or creates/starts a mining daemon.
set(RANDOMX_FOUNDATION_SOURCE_DIR "" CACHE PATH
    "Optional local src/randomx checkout. Empty uses pinned XNUVA source.")
set(RANDOMX_FOUNDATION_PIN
    "853a3171ece198f60b5125a16dca83d68a8f53c3")

if(RANDOMX_FOUNDATION_SOURCE_DIR)
  if(NOT EXISTS "${RANDOMX_FOUNDATION_SOURCE_DIR}/src/randomx.h" OR
     NOT EXISTS "${RANDOMX_FOUNDATION_SOURCE_DIR}/CMakeLists.txt")
    message(FATAL_ERROR "Local RandomX directory must contain CMakeLists.txt and src/randomx.h")
  endif()
  set(RANDOMX_FOUNDATION_RX_ROOT "${RANDOMX_FOUNDATION_SOURCE_DIR}")
  message(WARNING
    "Using local RandomX source override: independently verify it matches "
    "the pinned version ${RANDOMX_FOUNDATION_PIN} before production integration")
  add_subdirectory("${RANDOMX_FOUNDATION_RX_ROOT}"
                   "${CMAKE_CURRENT_BINARY_DIR}/randomx-vendor" EXCLUDE_FROM_ALL)
else()
  include(FetchContent)
  # XNUVA's vendored RandomX exposes the nonstandard RANDOMX_FLAG_V2.
  # Vanilla upstream RandomX v1 is NOT interchangeable with this version.
  FetchContent_Declare(xnuva_randomx_pinned
    GIT_REPOSITORY https://github.com/Xnuva-Blockchain-Project/Xnuva.git
    GIT_TAG "${RANDOMX_FOUNDATION_PIN}"
    GIT_SHALLOW FALSE
    SOURCE_SUBDIR src/randomx
  )
  FetchContent_MakeAvailable(xnuva_randomx_pinned)
  set(RANDOMX_FOUNDATION_RX_ROOT
      "${xnuva_randomx_pinned_SOURCE_DIR}/src/randomx")
endif()

if(NOT TARGET randomx)
  message(FATAL_ERROR "Pinned RandomX source did not define 'randomx' target")
endif()
file(READ "${RANDOMX_FOUNDATION_RX_ROOT}/src/randomx.h" RANDOMX_FOUNDATION_API_HEADER)
if(NOT RANDOMX_FOUNDATION_API_HEADER MATCHES "RANDOMX_FLAG_V2")
  message(FATAL_ERROR "RandomX source lacks required XNUVA V2 consensus variant")
endif()

find_package(Threads REQUIRED)
add_library(xnuva_randomx_foundation STATIC
  src/engine.cpp
)
add_library(xnuva::randomx_foundation ALIAS xnuva_randomx_foundation)
target_compile_features(xnuva_randomx_foundation PUBLIC cxx_std_20)
target_include_directories(xnuva_randomx_foundation
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
  PRIVATE "${RANDOMX_FOUNDATION_RX_ROOT}/src"
)
target_link_libraries(xnuva_randomx_foundation
  PUBLIC randomx Threads::Threads
)

include(CTest)
if(BUILD_TESTING)
  add_executable(randomx-foundation-tests tests/engine_tests.cpp)
  target_compile_features(randomx-foundation-tests PRIVATE cxx_std_20)
  target_link_libraries(randomx-foundation-tests
    PRIVATE xnuva::randomx_foundation)
  add_test(NAME randomx-foundation-v2-vectors
           COMMAND randomx-foundation-tests)
endif()

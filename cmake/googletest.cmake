include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.15.2
  EXCLUDE_FROM_ALL
)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(INSTALL_GMOCK OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)
include(GoogleTest)
enable_testing()

add_custom_target(unittest)

macro(add_test_target _target _library)
    add_executable(${_target} ${ARGN})
    add_dependencies(unittest ${_target})

    target_link_libraries(${_target}
      PRIVATE ${_library}
      PRIVATE GTest::gtest_main GTest::gmock_main
    )
    gtest_discover_tests(${_target})
    target_include_directories(${_target}
      PRIVATE $<TARGET_PROPERTY:${_library},INCLUDE_DIRECTORIES>
    )
endmacro()   

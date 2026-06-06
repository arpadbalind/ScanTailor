cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project("ScanTailor Advanced")

# Set compiler flags.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

add_custom_target(
    quality
    COMMAND ${CMAKE_SOURCE_DIR}/tools/clang-tidy.sh ${CMAKE_BINARY_DIR}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "🔍 Running clang-tidy code quality checks"
)

include(cmake/googletest.cmake)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Werror=return-type -Wno-unused")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror=return-type -Wno-unused")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}"
    CACHE STRING "Common C flags for all build configurations." FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}"
    CACHE STRING "Common C++ flags for all build configurations." FORCE)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}"
    CACHE STRING "Common link flags for all build configurations." FORCE)

set(DEFAULT_COMPILER_FLAGS_IS_SET YES CACHE INTERNAL "" FORCE)


option(CMAKE_POSITION_INDEPENDENT_CODE "" ON)
option(CMAKE_INSTALL_RPATH_USE_LINK_PATH "" ON)


include(cmake/SetDefaultBuildType.cmake)
include(cmake/UpdateTranslations.cmake)
include(cmake/ListItemsPrepend.cmake)


st_set_default_build_type(Release)

if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
endif()
if (NOT CMAKE_INSTALL_BINDIR)
  set(CMAKE_INSTALL_BINDIR "bin")
endif()

#=================================== JPEG ===================================#
find_package(JPEG REQUIRED)

#=================================== ZLIB ===================================#
find_package(ZLIB REQUIRED)

#================================== PNG ==================================#
find_package(PNG REQUIRED)

#=================================== TIFF ===================================#

find_package(TIFF REQUIRED)

#================================= Boost ================================#
set(Boost_USE_MULTITHREADED ON)

find_package(Boost 1.60 COMPONENTS unit_test_framework prg_exec_monitor REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
add_definitions(-DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION)
if (NOT Boost_USE_STATIC_LIBS)
  add_definitions(-DBOOST_TEST_DYN_LINK)
endif()

#=================================== Qt ===================================#

set(qt_min_version 5.6)
find_package(
    Qt5 ${qt_min_version}
    COMPONENTS Core Gui Widgets Xml Network LinguistTools OpenGL Svg
    CONFIG REQUIRED)

#=================================== Threads ===================================#

if (UNIX)
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  link_libraries(Threads::Threads)
endif()

#=================================== System ===================================#

set(EXTRA_LIBS "")

#=================================== Main ===================================#

set(CMAKE_AUTOMOC ON)

# Prepare config.h
option(PORTABLE_VERSION "Whether to build the portable version." ON)
if (PORTABLE_VERSION)
  set(PORTABLE_CONFIG_DIR "config")
endif()
set(APPLICATION_NAME "scantailor-advanced")
set(TRANSLATION_DIRS ".:translations:../share/${APPLICATION_NAME}/translations")
set(PLUGIN_DIRS ".:../lib/${APPLICATION_NAME}")

configure_file(config.h.in "${CMAKE_BINARY_DIR}/config.h" @ONLY)
file(READ "${CMAKE_BINARY_DIR}/config.h" config_h_contents)
set(enable_developer_version_default OFF)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(enable_developer_version_default ON)
endif()
option(DEVELOPER_VERSION "Whether to enable the internal debug features." ${enable_developer_version_default})
if (NOT DEVELOPER_VERSION)
  string(REGEX REPLACE "(#define[ \\t]+ENABLE_DEBUG_FEATURES[^\n]*)" "//\\1" config_h_contents ${config_h_contents})
endif()
file(WRITE "${CMAKE_BINARY_DIR}/config.h" ${config_h_contents})
include_directories(BEFORE "${CMAKE_BINARY_DIR}")

# Extract VERSION and VERSION_QUAD from version.h
file(READ "${PROJECT_SOURCE_DIR}/version.h.in" version_h_contents)
string(
    REGEX REPLACE
    ".*#define[ \\t]+VERSION[ \\t]+\"([^\"]*)\".*"
    "\\1" VERSION "${version_h_contents}")
if ("${VERSION}" STREQUAL "${version_h_contents}")
  message(FATAL_ERROR "Failed to extract VERSION from version.h")
endif()
# VERSION_QUAD must be either empty or be in the form of X.Y.Z.Y
string(
    REGEX REPLACE
    ".*#define[ \\t]+VERSION_QUAD[ \\t]+\"(([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+)?)\".*"
    "\\1" VERSION_QUAD "${version_h_contents}")
if ("${VERSION_QUAD}" STREQUAL "${version_h_contents}")
  message(FATAL_ERROR "Failed to extract VERSION_QUAD from version.h")
endif()
configure_file(version.h.in "${CMAKE_BINARY_DIR}/version.h" COPYONLY)
string(REGEX REPLACE "([0-9]+)\\.[0-9]+\\.[0-9]+" "\\1" APPLICATION_VERSION_MAJOR "${VERSION}")
string(REGEX REPLACE "[0-9]+\\.([0-9]+)\\.[0-9]+" "\\1" APPLICATION_VERSION_MINOR "${VERSION}")
string(REGEX REPLACE "[0-9]+\\.[0-9]+\\.([0-9]+)" "\\1" APPLICATION_VERSION_PATCH "${VERSION}")

set(SCANTAILOR_RESOURCES_DIR "${CMAKE_SOURCE_DIR}/src/resources")
set(SCANTAILOR_TRANSLATIONS_DIR "${CMAKE_SOURCE_DIR}/src/translations")

add_subdirectory(src)

# Translations
file(GLOB translation_files "${SCANTAILOR_TRANSLATIONS_DIR}/scantailor_*.ts")

finalize_translations(scantailor ${translation_files})
update_translations_target(update_translations scantailor)

list(FILTER translation_files EXCLUDE REGEX ".*untranslated.ts")
qt5_add_translation(qm_files ${translation_files})
add_custom_target(compile_translations ALL DEPENDS ${qm_files})
install(FILES ${qm_files} DESTINATION "share/${APPLICATION_NAME}/translations")
install(FILES "${SCANTAILOR_RESOURCES_DIR}/unix/scantailor.desktop"
  DESTINATION "share/applications")
install(FILES "${SCANTAILOR_RESOURCES_DIR}/appicon.svg"
  DESTINATION "share/icons/hicolor/scalable/apps" RENAME "ScanTailor.svg")
install(FILES "${SCANTAILOR_RESOURCES_DIR}/unix/mime/scantailor-project.xml"
  DESTINATION "share/mime/packages")

set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION "${CMAKE_INSTALL_BINDIR}")
include(InstallRequiredSystemLibraries)

# Packaging
set(CPACK_PACKAGE_NAME "${APPLICATION_NAME}")
set(CPACK_PACKAGE_VERSION_MAJOR "${APPLICATION_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${APPLICATION_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${APPLICATION_VERSION_PATCH}")
set(CPACK_PACKAGE_VENDOR "4lex4 <4lex49@zoho.com>")
set(CPACK_PACKAGE_CONTACT "${CPACK_PACKAGE_VENDOR}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Interactive post-processing tool for scanned pages.")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_EXECUTABLES "scantailor;${CMAKE_PROJECT_NAME}")
set(CPACK_CREATE_DESKTOP_LINKS "scantailor")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${APPLICATION_NAME}-${VERSION}")
set(CPACK_SOURCE_IGNORE_FILES
    "/\\\\.svn/"
    "/\\\\.git/"
    "~$"
    "\\\\.pcs$"
    "TODO.txt"
    "CMakeLists.txt.user"
    "/doxygen/"
    "${CMAKE_BINARY_DIR}")
include(CPack)

# uninstall target
if (NOT TARGET uninstall)
  configure_file(
      "${CMAKE_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
      "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
      @ONLY)
  add_custom_target(
      uninstall
      COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")
endif()

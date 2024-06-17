# CMakeLists.txt for pulldog
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

# read version from ini file
file(READ "${CMAKE_CURRENT_LIST_DIR}/conf/VERSION" VERSION)

# Read UUID from file
file(READ "${CMAKE_CURRENT_LIST_DIR}/conf/APPUUID" UUID)

# set project name and version
project(pulldog VERSION ${VERSION} LANGUAGES CXX)

# set c++ standard 17
set(CMAKE_CXX_STANDARD 17)

# set c++ standard required
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include cmake modules
include(FetchContent)

# set the PULLDOG_ISSUEPAGE
set(PULLDOG_ISSUEPAGE "https://github.com/srilakshmikanthanp/pulldog/issues")

# Set the DONATE_URL
set(DONATE_URL "https://srilakshmikanthanp.github.io/donate")

# Set the PULLDOG_HOMEPAGE
set(PULLDOG_HOMEPAGE "https://github.com/srilakshmikanthanp/pulldog")

# Set the Version
set(VERSION ${PROJECT_VERSION})

# Set the UUID
set(UUID ${UUID})

# set organization name
set(PULLDOG_ORGANIZATION_NAME "srilakshmikanthanp")

# Set the logo.png path variable
set(PULLDOG_LOGO_PATH ":/images/logo.ico")

# set the main.qss path variable (Light)
set(PULLDOG_LIGHT_QSS_PATH ":/styles/light.qss")

# set the main.qss path variable (Dark)
set(PULLDOG_DARK_QSS_PATH ":/styles/dark.qss")

# Set Qt version
set(QT_MAJOR_VERSION 6)

# Append Qt cmake dir to CMAKE_PREFIX_PATH
list(APPEND CMAKE_PREFIX_PATH $ENV{QT_CMAKE_DIR})

# set CMP0071 to NEW
cmake_policy(SET CMP0071 NEW)

# --------------------------------- Main Project ---------------------------------#
# Set the QApplication class
set(QAPPLICATION_CLASS QApplication CACHE STRING "Inheritance class for SingleApplication")

# set QT_DEFAULT_MAJOR_VERSION to QT_MAJOR_VERSION
set(QT_DEFAULT_MAJOR_VERSION ${QT_MAJOR_VERSION})

# Fetch SingleApplication from github
FetchContent_Declare(SingleApplication
  GIT_REPOSITORY https://github.com/itay-grudev/SingleApplication.git
  GIT_TAG        v3.4.0
)

# Make Available SingleApplication
FetchContent_MakeAvailable(SingleApplication)

# glob pattern for main cpp files exclude
file(GLOB_RECURSE main_cpp *.cpp)

# remove tests folder from list
list(FILTER main_cpp EXCLUDE REGEX "tests/")

# remove build/**.cpp from list
list(FILTER main_cpp EXCLUDE REGEX "build/")

# Configure config.hpp
configure_file(
  ${PROJECT_SOURCE_DIR}/config/config.hpp.in
  ${PROJECT_BINARY_DIR}/config/config.hpp)

# Find Qt packages
find_package(Qt6 REQUIRED COMPONENTS
  Concurrent
  Widgets
  Network
  Sql)

# set up project using Qt
qt_standard_project_setup()

# add resources to project
qt_add_resources(RESOURCES
  ${PROJECT_SOURCE_DIR}/assets/resources.qrc)

# copy assets to build directory
file(
  COPY
  ${PROJECT_SOURCE_DIR}/assets
  DESTINATION
  ${CMAKE_CURRENT_BINARY_DIR}
)

# if windows
if(WIN32)
  set (RESOURCES ${RESOURCES} ${PROJECT_SOURCE_DIR}/assets/windows.rc)
endif()

# Add executable
qt_add_executable(pulldog
  ${main_cpp} ${RESOURCES})

# Include Directories for Root of project
target_include_directories(pulldog
  PRIVATE ${PROJECT_SOURCE_DIR}
  PRIVATE ${PROJECT_BINARY_DIR})

# Link libraries
target_link_libraries(pulldog
  PRIVATE SingleApplication::SingleApplication
  PRIVATE Qt6::Widgets
  PRIVATE Qt6::Network
  PRIVATE Qt6::Concurrent)

# set target properties
set_target_properties(pulldog PROPERTIES
  WIN32_EXECUTABLE TRUE
  MACOSX_BUNDLE TRUE
)

# add target compiler options GCC -std=c++17
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  target_compile_options(pulldog PRIVATE -std=c++17)
endif()

# add target compiler options MSVC /std:c++17
if(MSVC)
  target_compile_options(pulldog PRIVATE /std:c++17)
endif()

if(WIN32)
  add_definitions(-D_WIN32)
endif()

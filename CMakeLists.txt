cmake_minimum_required(VERSION 3.20)
project(TreeMaker VERSION 5.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Build configuration options
option(TM_BUILD_GUI "Build the wxWidgets GUI application" ON)
option(TM_BUILD_TESTS "Build test executables" ON)

# Build type configuration: map TreeMaker configs to CMake
# - Release: TMWX only, full optimization
# - Debug: TMWX + __WXDEBUG__, no optimization, debug symbols
# - Development: TMWX + TMDEBUG, no optimization, debug symbols
# - Profile: TMWX + TMPROFILE, full optimization, debug symbols
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
    STRINGS "Release" "Debug" "Development" "Profile")
endif()

# Treat Development and Profile as custom build types
if(CMAKE_BUILD_TYPE STREQUAL "Development")
  set(CMAKE_C_FLAGS_DEVELOPMENT "-g" CACHE STRING "" FORCE)
  set(CMAKE_CXX_FLAGS_DEVELOPMENT "-g" CACHE STRING "" FORCE)
elseif(CMAKE_BUILD_TYPE STREQUAL "Profile")
  set(CMAKE_C_FLAGS_PROFILE "-O3 -g" CACHE STRING "" FORCE)
  set(CMAKE_CXX_FLAGS_PROFILE "-O3 -g" CACHE STRING "" FORCE)
endif()

# Source root
set(TM_SOURCE_DIR "${CMAKE_SOURCE_DIR}/Source")

# --- wnlib (C library) ---
file(GLOB WNLIB_SOURCES
  ${TM_SOURCE_DIR}/tmModel/wnlib/cmp/*.c
  ${TM_SOURCE_DIR}/tmModel/wnlib/conjdir/*.c
  ${TM_SOURCE_DIR}/tmModel/wnlib/cpy/*.c
  ${TM_SOURCE_DIR}/tmModel/wnlib/list/*.c
  ${TM_SOURCE_DIR}/tmModel/wnlib/low/*.c
  ${TM_SOURCE_DIR}/tmModel/wnlib/mat/*.c
  ${TM_SOURCE_DIR}/tmModel/wnlib/mem/*.c
  ${TM_SOURCE_DIR}/tmModel/wnlib/random/*.c
  ${TM_SOURCE_DIR}/tmModel/wnlib/vect/*.c
)

add_library(wnlib STATIC ${WNLIB_SOURCES})
target_include_directories(wnlib PUBLIC
  ${TM_SOURCE_DIR}/tmModel/wnlib/cmp
  ${TM_SOURCE_DIR}/tmModel/wnlib/conjdir
  ${TM_SOURCE_DIR}/tmModel/wnlib/cpy
  ${TM_SOURCE_DIR}/tmModel/wnlib/list
  ${TM_SOURCE_DIR}/tmModel/wnlib/low
  ${TM_SOURCE_DIR}/tmModel/wnlib/mat
  ${TM_SOURCE_DIR}/tmModel/wnlib/mem
  ${TM_SOURCE_DIR}/tmModel/wnlib/random
  ${TM_SOURCE_DIR}/tmModel/wnlib/vect
)
# wnlib is old C code; suppress warnings we can't fix without rewriting it
target_compile_options(wnlib PRIVATE
  -Wno-register
  -Wno-deprecated-register
  -Wno-implicit-function-declaration
  -Wno-int-conversion
)

# --- tmModel (C++ library, no wx dependency) ---
file(GLOB TMMODEL_PTRCLASSES_SRC  ${TM_SOURCE_DIR}/tmModel/tmPtrClasses/*.cpp)
file(GLOB TMMODEL_NLCO_SRC        ${TM_SOURCE_DIR}/tmModel/tmNLCO/*.cpp)
file(GLOB TMMODEL_NLCO_C_SRC      ${TM_SOURCE_DIR}/tmModel/tmNLCO/*.c)
file(GLOB TMMODEL_SOLVERS_SRC     ${TM_SOURCE_DIR}/tmModel/tmSolvers/*.cpp)
file(GLOB TMMODEL_OPTIMIZERS_SRC  ${TM_SOURCE_DIR}/tmModel/tmOptimizers/*.cpp)
file(GLOB TMMODEL_TREECLASSES_SRC ${TM_SOURCE_DIR}/tmModel/tmTreeClasses/*.cpp)

set(TMMODEL_SOURCES
  ${TM_SOURCE_DIR}/tmHeader.cpp
  ${TMMODEL_PTRCLASSES_SRC}
  ${TMMODEL_NLCO_SRC}
  ${TMMODEL_NLCO_C_SRC}
  ${TMMODEL_SOLVERS_SRC}
  ${TMMODEL_OPTIMIZERS_SRC}
  ${TMMODEL_TREECLASSES_SRC}
)

add_library(tmModel STATIC ${TMMODEL_SOURCES})
target_include_directories(tmModel PUBLIC
  ${TM_SOURCE_DIR}
  ${TM_SOURCE_DIR}/tmModel
  ${TM_SOURCE_DIR}/tmModel/tmPtrClasses
  ${TM_SOURCE_DIR}/tmModel/tmNLCO
  ${TM_SOURCE_DIR}/tmModel/tmSolvers
  ${TM_SOURCE_DIR}/tmModel/tmOptimizers
  ${TM_SOURCE_DIR}/tmModel/tmTreeClasses
)
target_link_libraries(tmModel PUBLIC wnlib)

# --- Test executables (no wx) ---
if(TM_BUILD_TESTS)
  add_executable(tmArrayTester
    ${TM_SOURCE_DIR}/test/tmArrayTester.cpp
  )
  target_link_libraries(tmArrayTester PRIVATE tmModel)

  add_executable(tmDpptrTester
    ${TM_SOURCE_DIR}/test/tmDpptrTester.cpp
  )
  target_link_libraries(tmDpptrTester PRIVATE tmModel)

  add_executable(tmNewtonRaphsonTester
    ${TM_SOURCE_DIR}/test/tmNewtonRaphsonTester.cpp
  )
  target_link_libraries(tmNewtonRaphsonTester PRIVATE tmModel)

  add_executable(tmNLCOTester
    ${TM_SOURCE_DIR}/test/tmNLCOTester/tmNLCOTester.cpp
  )
  target_include_directories(tmNLCOTester PRIVATE
    ${TM_SOURCE_DIR}/test/tmNLCOTester
  )
  target_link_libraries(tmNLCOTester PRIVATE tmModel)

  add_executable(tmModelTester
    ${TM_SOURCE_DIR}/test/tmModelTester/tmModelTester.cpp
  )
  target_include_directories(tmModelTester PRIVATE
    ${TM_SOURCE_DIR}/test/tmModelTester
  )
  target_link_libraries(tmModelTester PRIVATE tmModel)

  # Copy test data files alongside the tmModelTester binary
  file(GLOB TM_TEST_DATA ${TM_SOURCE_DIR}/test/tmModelTester/*.tmd5)
  foreach(datafile ${TM_TEST_DATA})
    get_filename_component(fname ${datafile} NAME)
    configure_file(${datafile} ${CMAKE_BINARY_DIR}/${fname} COPYONLY)
  endforeach()
endif()

# --- GUI application (requires wxWidgets) ---
if(TM_BUILD_GUI)
  find_package(wxWidgets 3.2 REQUIRED
    COMPONENTS core base html adv)
  include(${wxWidgets_USE_FILE})

  file(GLOB TMWX_COMMON_SRC       ${TM_SOURCE_DIR}/tmwxGUI/tmwxCommon/*.cpp)
  file(GLOB TMWX_DOCVIEW_SRC      ${TM_SOURCE_DIR}/tmwxGUI/tmwxDocView/*.cpp)
  file(GLOB TMWX_INSPECTOR_SRC    ${TM_SOURCE_DIR}/tmwxGUI/tmwxInspector/*.cpp)
  file(GLOB TMWX_PALETTE_SRC      ${TM_SOURCE_DIR}/tmwxGUI/tmwxPalette/*.cpp)
  file(GLOB TMWX_HTMLHELP_SRC     ${TM_SOURCE_DIR}/tmwxGUI/tmwxHtmlHelp/*.cpp)
  file(GLOB TMWX_VIEWSETTINGS_SRC ${TM_SOURCE_DIR}/tmwxGUI/tmwxViewSettings/*.cpp)
  file(GLOB TMWX_OPTIMIZER_SRC    ${TM_SOURCE_DIR}/tmwxGUI/tmwxOptimizerDialog/*.cpp)
  file(GLOB TMWX_FOLDEDFORM_SRC   ${TM_SOURCE_DIR}/tmwxGUI/tmwxFoldedForm/*.cpp)
  file(GLOB TMWX_LOGFRAME_SRC     ${TM_SOURCE_DIR}/tmwxGUI/tmwxLogFrame/*.cpp)

  set(TMWX_SOURCES
    ${TM_SOURCE_DIR}/tmHeader.cpp
    ${TMWX_COMMON_SRC}
    ${TMWX_DOCVIEW_SRC}
    ${TMWX_INSPECTOR_SRC}
    ${TMWX_PALETTE_SRC}
    ${TMWX_HTMLHELP_SRC}
    ${TMWX_VIEWSETTINGS_SRC}
    ${TMWX_OPTIMIZER_SRC}
    ${TMWX_FOLDEDFORM_SRC}
    ${TMWX_LOGFRAME_SRC}
  )

  add_executable(TreeMaker ${TMWX_SOURCES})
  target_compile_definitions(TreeMaker PRIVATE TMWX)
  target_include_directories(TreeMaker PRIVATE
    ${TM_SOURCE_DIR}/tmwxGUI/tmwxCommon
    ${TM_SOURCE_DIR}/tmwxGUI/tmwxDocView
    ${TM_SOURCE_DIR}/tmwxGUI/tmwxInspector
    ${TM_SOURCE_DIR}/tmwxGUI/tmwxPalette
    ${TM_SOURCE_DIR}/tmwxGUI/tmwxHtmlHelp
    ${TM_SOURCE_DIR}/tmwxGUI/tmwxViewSettings
    ${TM_SOURCE_DIR}/tmwxGUI/tmwxOptimizerDialog
    ${TM_SOURCE_DIR}/tmwxGUI/tmwxFoldedForm
    ${TM_SOURCE_DIR}/tmwxGUI/tmwxLogFrame
  )
  target_link_libraries(TreeMaker PRIVATE tmModel ${wxWidgets_LIBRARIES})

  # Build type preprocessor symbols
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(TreeMaker PRIVATE __WXDEBUG__)
  elseif(CMAKE_BUILD_TYPE STREQUAL "Development")
    target_compile_definitions(TreeMaker PRIVATE TMDEBUG)
  elseif(CMAKE_BUILD_TYPE STREQUAL "Profile")
    target_compile_definitions(TreeMaker PRIVATE TMPROFILE)
  endif()

  # macOS application bundle
  if(APPLE)
    set_target_properties(TreeMaker PROPERTIES
      MACOSX_BUNDLE TRUE
      MACOSX_BUNDLE_BUNDLE_NAME "TreeMaker"
      MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
      MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
      MACOSX_BUNDLE_GUI_IDENTIFIER "com.langorigami.treemaker"
      MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/cmake/Info.plist.in"
    )
    # Build help.zip from Source/help and install into SharedSupport
    set(HELP_SRC_DIR "${CMAKE_SOURCE_DIR}/Source/help")
    set(HELP_ZIP "${CMAKE_BINARY_DIR}/help.zip")
    add_custom_command(
      OUTPUT ${HELP_ZIP}
      COMMAND ${CMAKE_COMMAND} -E rm -f ${HELP_ZIP}
      COMMAND zip -r ${HELP_ZIP} . -x ".*"
      WORKING_DIRECTORY ${HELP_SRC_DIR}
      COMMENT "Building help.zip"
    )
    add_custom_target(help_zip ALL DEPENDS ${HELP_ZIP})
    add_dependencies(TreeMaker help_zip)
    set(SHARED_SUPPORT "$<TARGET_BUNDLE_DIR:TreeMaker>/Contents/SharedSupport")
    add_custom_command(TARGET TreeMaker POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory ${SHARED_SUPPORT}
      COMMAND ${CMAKE_COMMAND} -E copy ${HELP_ZIP} ${SHARED_SUPPORT}/
      COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/Source/images/SplashScreen.png" ${SHARED_SUPPORT}/
      COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/Source/about/about.htm" ${SHARED_SUPPORT}/
      COMMENT "Bundling resources into SharedSupport"
    )
  endif()
endif()

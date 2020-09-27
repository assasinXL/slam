#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Glob" for configuration "Release"
set_property(TARGET Glob APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Glob PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/Glob-1.0/glob.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS Glob )
list(APPEND _IMPORT_CHECK_FILES_FOR_Glob "${_IMPORT_PREFIX}/lib/Glob-1.0/glob.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

# if already found via finder or simulated finder in osgQt
# CMakeLists.txt, skip the search
IF (NOT OSGQT_FOUND)
  SET (SEARCH_PATHS
    /usr/local/
    /usr/
    "${OSG_DIR}/"
    "${OSG_DIR}/build"
  )

  FIND_PATH (OSGQT_INCLUDE_DIR osgQOpenGL/GraphicsWindowEx
    PATHS ${SEARCH_PATHS}
    PATH_SUFFIXES include)

  FIND_LIBRARY(OSGQT_LIBRARY_RELEASE NAMES osgQOpenGL
    PATHS ${SEARCH_PATHS}
    PATH_SUFFIXES lib lib64)

  FIND_LIBRARY(OSGQT_LIBRARY_DEBUG NAMES osgQOpenGLd
    PATHS ${SEARCH_PATHS}
    PATH_SUFFIXES lib lib64)

#select configuration depending on platform (optimized... on windows)
  include(SelectLibraryConfigurations)
  select_library_configurations( OSGQT )

  set(OSGQT_LIBRARIES ${OSGQT_LIBRARY})
  set(OSGQT_INCLUDE_DIRS ${OSGQT_INCLUDE_DIR} )

#checks, if OSGQT was found and sets OSGQT_FOUND if so
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(osgQt  DEFAULT_MSG
                                    OSGQT_LIBRARY OSGQT_INCLUDE_DIR)

#sets the library dir
  if ( OSGQT_LIBRARY_RELEASE )
    get_filename_component(_OSGQT_LIBRARY_DIR ${OSGQT_LIBRARY_RELEASE} PATH)
  else( OSGQT_LIBRARY_RELEASE )
    get_filename_component(_OSGQT_LIBRARY_DIR ${OSGQT_LIBRARY_DEBUG} PATH)
  endif( OSGQT_LIBRARY_RELEASE )
  set (OSGQT_LIBRARY_DIR "${_OSGQT_LIBRARY_DIR}" CACHE PATH "The directory where the osgQt libraries can be found.")

  mark_as_advanced(OSGQT_INCLUDE_DIR OSGQT_LIBRARY_RELEASE OSGQT_LIBRARY_DEBUG OSGQT_LIBRARY_DIR)
endif()

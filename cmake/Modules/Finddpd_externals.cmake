if(NOT DPD_EXTERNALS_FOUND)
  find_path(DPD_EXTERNALS_INCLUDE_DIRS NAMES gen_GMPvector.h
    PATHS
    /usr/include/dpd_externals
    /usr/local/include/dpd_externals
  )

  find_library(DPD_EXTERNALS_LIBRARIES NAMES dpd_externals
    PATHS
    /usr/lib
    /usr/local/lib
  )

if(DPD_EXTERNALS_INCLUDE_DIRS AND DPD_EXTERNALS_LIBRARIES)
  set(DPD_EXTERNALS_FOUND TRUE CACHE INTERNAL "libdpd_externals found")
  message(STATUS "Found libdpd_externals: ${DPD_EXTERNALS_INCLUDE_DIRS}, ${DPD_EXTERNALS_LIBRARIES}")
else(DPD_EXTERNALS_INCLUDE_DIRS AND DPD_EXTERNALS_LIBRARIES)
  set(DPD_EXTERNALS_FOUND FALSE CACHE INTERNAL "libdpd_externals found")
  message(STATUS "libdpd_externals not found.")
endif(DPD_EXTERNALS_INCLUDE_DIRS AND DPD_EXTERNALS_LIBRARIES)

mark_as_advanced(DPD_EXTERNALS_LIBRARIES AND DPD_EXTERNALS_INCLUDE_DIRS)

endif(NOT DPD_EXTERNALS_FOUND)

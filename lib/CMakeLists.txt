# Copyright 2011,2012,2016 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR} ${LIBVXI11_INCLUDE_DIRS} ${DPD_EXTERNALS_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND dpd_sources
    peak_detect_impl.cc
    sts_blocker_impl.cc
    predistorter_impl.cc
    stream_to_zero_padded_vector_impl.cc
    add_cp_impl.cc
    Agilent_N1996A_impl.cc
    stream_to_gmp_vector_impl.cc
    signum_func_impl.cc
    gain_phase_calibrate_impl.cc
    postdistorter_impl.cc
    stream_to_message_impl.cc
    phase_calibrate_impl.cc )

set(dpd_sources "${dpd_sources}" PARENT_SCOPE)
if(NOT dpd_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT dpd_sources)

if (ARMADILLO_FOUND)
	LIST(APPEND dpd_sources)
else ()
	set(ARMADILLO_LIBRARIES)
endif ()

add_library(gnuradio-dpd SHARED ${dpd_sources})
target_link_libraries(gnuradio-dpd ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES} ${ARMADILLO_LIBRARIES} ${LIBVXI11_LIBRARIES} ${DPD_EXTERNALS_LIBRARIES})
set_target_properties(gnuradio-dpd PROPERTIES DEFINE_SYMBOL "gnuradio_dpd_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-dpd PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-dpd RUNTIME_COMPONENT "dpd_runtime" DEVEL_COMPONENT "dpd_devel")

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_dpd_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_dpd.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_dpd.cc
)

add_executable(test-dpd ${test_dpd_sources})

target_link_libraries(
  test-dpd
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  ${LIBVXI11_LIBRARIES}  
  ${DPD_EXTERNALS_LIBRARIES}
  gnuradio-dpd  
)

GR_ADD_TEST(test_dpd test-dpd)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")


# Copyright 2011,2012 Free Software Foundation, Inc.
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

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

list(APPEND Interfaces_sources
    queue_len_deframer_sink_b_impl.cc
    queue_len_framer_source_b_impl.cc
)

set(Interfaces_sources "${Interfaces_sources}" PARENT_SCOPE)
if(NOT Interfaces_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT Interfaces_sources)

add_library(gnuradio-Interfaces SHARED ${Interfaces_sources})
target_link_libraries(gnuradio-Interfaces ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-Interfaces PROPERTIES DEFINE_SYMBOL "gnuradio_Interfaces_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-Interfaces PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-Interfaces
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_Interfaces_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_Interfaces.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_Interfaces.cc
)

add_executable(test-Interfaces ${test_Interfaces_sources})

target_link_libraries(
  test-Interfaces
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-Interfaces
)

GR_ADD_TEST(test_Interfaces test-Interfaces)

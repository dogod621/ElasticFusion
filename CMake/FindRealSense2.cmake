#############################################################################
#
# ViSP, open source Visual Servoing Platform software.
# Copyright (C) 2005 - 2019 by Inria. All rights reserved.
#
# This software is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# See the file LICENSE.txt at the root directory of this source
# distribution for additional information about the GNU GPL.
#
# For using ViSP with software that can not be combined with the GNU
# GPL, please contact Inria about acquiring a ViSP Professional
# Edition License.
#
# See http://visp.inria.fr for more information.
#
# This software was developed at:
# Inria Rennes - Bretagne Atlantique
# Campus Universitaire de Beaulieu
# 35042 Rennes Cedex
# France
#
# If you have questions regarding the use of this file, please contact
# Inria at visp@inria.fr
#
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
# Description:
# Try to find Intel RealSense SDK to work with R200, F200 and SR300 devices.
# Once run this will define:
#
# REALSENSE2_FOUND
# REALSENSE2_INCLUDE_DIRS
# REALSENSE2_LIBRARIES
# REALSENSE2_VERSION
#
#############################################################################

set(REALSENSE2_INC_SEARCH_PATH /usr/local/include)
set(REALSENSE2_LIB_SEARCH_PATH /usr/local/lib)

if(MSVC)
	list(APPEND REALSENSE2_INC_SEARCH_PATH "C:/librealsense2/include")

	list(APPEND REALSENSE2_INC_SEARCH_PATH $ENV{REALSENSE2_HOME}/include)
	list(APPEND REALSENSE2_INC_SEARCH_PATH $ENV{REALSENSE2_DIR}/include)
	list(APPEND REALSENSE2_INC_SEARCH_PATH "C:/Program Files (x86)/Intel RealSense SDK 2.0/include")

	list(APPEND REALSENSE2_LIB_SEARCH_PATH $ENV{REALSENSE2_HOME}/lib)
	list(APPEND REALSENSE2_LIB_SEARCH_PATH $ENV{REALSENSE2_DIR}/lib)

	if(CMAKE_CL_64)
		list(APPEND REALSENSE2_LIB_SEARCH_PATH "C:/Program Files (x86)/Intel RealSense SDK 2.0/lib/x64")
	else()
		list(APPEND REALSENSE2_LIB_SEARCH_PATH "C:/Program Files (x86)/Intel RealSense SDK 2.0/lib/x86")
	endif()
else()
	list(APPEND REALSENSE2_INC_SEARCH_PATH /usr/include)
	list(APPEND REALSENSE2_LIB_SEARCH_PATH /usr/lib)

	list(APPEND REALSENSE2_INC_SEARCH_PATH $ENV{REALSENSE2_HOME}/include)
	list(APPEND REALSENSE2_LIB_SEARCH_PATH $ENV{REALSENSE2_HOME}/lib)
	list(APPEND REALSENSE2_INC_SEARCH_PATH $ENV{REALSENSE2_DIR}/include)
	list(APPEND REALSENSE2_LIB_SEARCH_PATH $ENV{REALSENSE2_DIR}/lib)
	list(APPEND REALSENSE2_INC_SEARCH_PATH ${REALSENSE2_DIR}/include)
	list(APPEND REALSENSE2_LIB_SEARCH_PATH ${REALSENSE2_DIR}/lib)
endif()

find_path(REALSENSE2_INCLUDE_DIRS librealsense2/rs.hpp
	PATHS
	${REALSENSE2_INC_SEARCH_PATH}
)

find_library(REALSENSE2_LIBRARIES
	NAMES realsense2
	PATHS
	${REALSENSE2_LIB_SEARCH_PATH}
)

# read set of version defines from the header file
macro(vp_parse_header FILENAME FILE_VAR)
  set(vars_regex "")
  set(__parnet_scope OFF)
  set(__add_cache OFF)
  foreach(name ${ARGN})
    if(${name} STREQUAL "PARENT_SCOPE")
      set(__parnet_scope ON)
    elseif(${name} STREQUAL "CACHE")
      set(__add_cache ON)
    elseif(vars_regex)
      set(vars_regex "${vars_regex}|${name}")
    else()
      set(vars_regex "${name}")
    endif()
  endforeach()
  if(EXISTS "${FILENAME}")
    file(STRINGS "${FILENAME}" ${FILE_VAR} REGEX "#define[ \t]+(${vars_regex})[ \t]+[0-9]+" )
  else()
    unset(${FILE_VAR})
  endif()
  foreach(name ${ARGN})
    if(NOT ${name} STREQUAL "PARENT_SCOPE" AND NOT ${name} STREQUAL "CACHE")
      if(${FILE_VAR})
        if(${FILE_VAR} MATCHES ".+[ \t]${name}[ \t]+([0-9]+).*")
          string(REGEX REPLACE ".+[ \t]${name}[ \t]+([0-9]+).*" "\\1" ${name} "${${FILE_VAR}}")
        else()
          set(${name} "")
        endif()
        if(__add_cache)
          set(${name} ${${name}} CACHE INTERNAL "${name} parsed from ${FILENAME}" FORCE)
        elseif(__parnet_scope)
          set(${name} "${${name}}" PARENT_SCOPE)
        endif()
      else()
        unset(${name} CACHE)
      endif()
    endif()
  endforeach()
endmacro()

if(REALSENSE2_LIBRARIES AND REALSENSE2_INCLUDE_DIRS)
	set(REALSENSE2_FOUND TRUE)
	vp_parse_header("${REALSENSE2_INCLUDE_DIRS}/librealsense2/rs.h" REALSENSE2_VERSION_LINES RS2_API_MAJOR_VERSION RS2_API_MINOR_VERSION RS2_API_PATCH_VERSION)
	set(REALSENSE2_VERSION "${RS2_API_MAJOR_VERSION}.${RS2_API_MINOR_VERSION}.${RS2_API_PATCH_VERSION}")
else()
	set(REALSENSE2_FOUND FALSE)
endif()


mark_as_advanced(
	REALSENSE2_INCLUDE_DIRS
	REALSENSE2_LIBRARIES
	REALSENSE2_INC_SEARCH_PATH
	REALSENSE2_LIB_SEARCH_PATH
)
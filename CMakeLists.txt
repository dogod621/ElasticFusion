cmake_minimum_required(VERSION 3.2.2)

# Setting
project(ElasticFusion)

IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/../install/" CACHE PATH "${CMAKE_CURRENT_SOURCE_DIR}/../install/" FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake/")

option(USE_REALSENSE "Use Intel RealSense (Default: OpenNI2)" ON)

set(CMAKE_CXX_FLAGS "-O3 -msse2 -msse3 -Wall -std=c++14")


# CUDA
find_package(CUDA REQUIRED)

# OpenNI2
find_package(OpenNI2 REQUIRED)

# RealSense2
if(${USE_REALSENSE})
	find_package(RealSense2 REQUIRED)
endif()

# Eigen
set(EIGEN_INCLUDE_DIRS
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/Eigen/install/include/eigen3/")

# Pangolin
set(PANGOLIN_INCLUDE_DIRS
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/Pangolin/install/include/")

set(PANGOLIN_LIBRARIES
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/Pangolin/install/lib/glew.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/Pangolin/install/lib/jpeg.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/Pangolin/install/lib/libpng16_static.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/Pangolin/install/lib/zlibstatic.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/Pangolin/install/lib/pangolin.lib")

# SuiteSparse
set(SUITESPARSE_INCLUDE_DIRS
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/include/")
	
set(SUITESPARSE_LIBRARIES
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib64/lapack_blas_windows/libblas.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib64/lapack_blas_windows/liblapack.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib/metis.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib/suitesparseconfig.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib/SuiteSparse_GPURuntime.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib/GPUQREngine.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib/libumfpack.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib/libspqr.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib/libcholmod.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib/libcamd.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib/libklu.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib/libcxsparse.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib/libldl.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib/libamd.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib/libbtf.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib/libccolamd.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib/libcolamd.lib")

set(SUITESPARSE_DLLS
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib64/lapack_blas_windows/libblas.dll"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib64/lapack_blas_windows/libgcc_s_sjlj-1.dll"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib64/lapack_blas_windows/libgfortran-3.dll"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib64/lapack_blas_windows/liblapack.dll"
	"${CMAKE_CURRENT_SOURCE_DIR}/../Dependency/SuiteSparse/install/lib64/lapack_blas_windows/libquadmath-0.dll")

# Core
set(CORE_INCLUDE_DIRS
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Utils"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Shaders"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Cuda"
	"${CMAKE_CURRENT_SOURCE_DIR}/Core/Cuda/containers")
	
set(CORE_LIBRARIES
	"${CMAKE_CURRENT_SOURCE_DIR}/../install/lib/Core/Core.lib")
	
#
add_subdirectory("Core")
add_subdirectory("GPUTest")
add_subdirectory("GUI")



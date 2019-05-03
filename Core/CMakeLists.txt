# Core
file(GLOB core_srcs					${CMAKE_CURRENT_SOURCE_DIR}/../Core/*.cpp)
file(GLOB core_utils_srcs			${CMAKE_CURRENT_SOURCE_DIR}/../Core/Utils/*.cpp)
file(GLOB core_shader_srcs			${CMAKE_CURRENT_SOURCE_DIR}/../Core/Shaders/*.cpp)
file(GLOB core_cuda					${CMAKE_CURRENT_SOURCE_DIR}/../Core/Cuda/*.cu)
file(GLOB core_cuda_containers		${CMAKE_CURRENT_SOURCE_DIR}/../Core/Cuda/containers/*.cpp)

file(GLOB core_hdrs					${CMAKE_CURRENT_SOURCE_DIR}/../Core/*.h)
file(GLOB core_utils_hdrs			${CMAKE_CURRENT_SOURCE_DIR}/../Core/Utils/*.h)
file(GLOB core_shader_hdrs			${CMAKE_CURRENT_SOURCE_DIR}/../Core/Shaders/*.h)
file(GLOB core_cuda_hdrs			${CMAKE_CURRENT_SOURCE_DIR}/../Core/Cuda/*.cuh)
file(GLOB core_cuda_containers_hdrs	${CMAKE_CURRENT_SOURCE_DIR}/../Core/Cuda/containers/*.hpp)

file(GLOB core_shader_glsls			${CMAKE_CURRENT_SOURCE_DIR}/../Core/Shaders/*.glsl)
file(GLOB core_shader_verts			${CMAKE_CURRENT_SOURCE_DIR}/../Core/Shaders/*.vert)
file(GLOB core_shader_geoms			${CMAKE_CURRENT_SOURCE_DIR}/../Core/Shaders/*.geom)
file(GLOB core_shader_frags			${CMAKE_CURRENT_SOURCE_DIR}/../Core/Shaders/*.frag)

#
# Compile CUDA 
# CUDA_ARCH_BIN is a space separated list of versions to include in output so-file. So you can set CUDA_ARCH_BIN = 10 11 12 13 20
# Also user can specify virtual arch in parenthesis to limit instructions set,
# for example CUDA_ARCH_BIN = 11(11) 12(11) 13(11) 20(11) 21(11) -> forces using only sm_11 instructions.
# The CMake scripts interpret XX as XX (XX). This allows user to omit parenthesis.
# Arch 21 is an exceptional case since it doesn't have own sm_21 instructions set.
# So 21 = 21(21) is an invalid configuration and user has to explicitly force previous sm_20 instruction set via 21(20).
# CUDA_ARCH_BIN adds support of only listed GPUs. As alternative CMake scripts also parse 'CUDA_ARCH_PTX' variable,
# which is a list of intermediate PTX codes to include in final so-file. The PTX code can/will be JIT compiled for any current or future GPU.
# To add support of older GPU for kinfu, I would embed PTX 11 and 12 into so-file. GPU with sm_13 will run PTX 12 code (no difference for kinfu)

# Find a complete list for CUDA compute capabilities at http://developer.nvidia.com/cuda-gpus
set(CUDA_ARCH_BIN_STR "30 35 50 52 61") # Original setting of Elastic fution

#if(NOT ${CUDA_VERSION_STRING} VERSION_LESS "10.0")
#	set(CUDA_ARCH_BIN_STR "3.0 3.5 5.0 5.2 5.3 6.0 6.1 7.0 7.2 7.5")
#elseif(NOT ${CUDA_VERSION_STRING} VERSION_LESS "9.1")
#	set(CUDA_ARCH_BIN_STR "3.0 3.5 5.0 5.2 5.3 6.0 6.1 7.0 7.2")
#elseif(NOT ${CUDA_VERSION_STRING} VERSION_LESS "9.0" )
#	set(CUDA_ARCH_BIN_STR "3.0 3.5 5.0 5.2 5.3 6.0 6.1 7.0")
#elseif(NOT ${CUDA_VERSION_STRING} VERSION_LESS "8.0")
#	set(CUDA_ARCH_BIN_STR "2.0 2.1(2.0) 3.0 3.5 5.0 5.2 5.3 6.0 6.1")
#else()
#	set(CUDA_ARCH_BIN_STR "2.0 2.1(2.0) 3.0 3.5 5.0 5.2")
#endif()

set(CUDA_ARCH_BIN ${CUDA_ARCH_BIN_STR} CACHE STRING "Specify 'real' GPU arch to build binaries for, BIN(PTX) format is supported. Example: 1.3 2.1(1.3) or 13 21(13)")
set(CUDA_ARCH_PTX "" CACHE STRING "Specify 'virtual' PTX arch to build PTX intermediate code for. Example: 1.0 1.2 or 10 12")

include("${CMAKE_CURRENT_SOURCE_DIR}/../CMake/CudaComputeTargetFlags.cmake")
APPEND_TARGET_ARCH_FLAGS()
set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS}	"-Xcompiler;-fPIC;-std=c++14")
set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} "--ftz=true;--prec-div=false;--prec-sqrt=false")
cuda_compile(core_cuda_objs ${core_cuda})

#
add_library(Core STATIC
			${core_srcs}
			${core_utils_srcs}
			${core_shader_srcs}
			${core_cuda}
			${core_cuda_objs}
			${core_cuda_containers}
			${core_hdrs}
			${core_utils_hdrs}
			${core_shader_hdrs}
			${core_cuda_hdrs}
			${core_cuda_containers_hdrs})

# WIN32
add_definitions(-DWIN32)
add_definitions(-DWIN32_LEAN_AND_MEAN)
add_definitions(-DNOMINMAX)
add_definitions(-Dlinux=0)
set (EXTRA_WINDOWS_LIBS ${EXTRA_WINDOWS_LIBS} 
	ws2_32
	opengl32.lib
	glu32.lib
	mf.lib
	mfplat.lib
	mfreadwrite.lib
	mfuuid.lib
	strmiids.lib)
target_link_libraries(Core ${EXTRA_WINDOWS_LIBS})

# CUDA
include_directories(${CUDA_INCLUDE_DIRS})
target_link_libraries(Core ${CUDA_LIBRARIES})

# Eigen
include_directories(${EIGEN_INCLUDE_DIRS})

# Pangolin
add_definitions(-DHAVE_EIGEN)
include_directories(${PANGOLIN_INCLUDE_DIRS})
target_link_libraries(Core ${PANGOLIN_LIBRARIES})

# SuiteSparse
include_directories(${SUITESPARSE_INCLUDE_DIRS})
target_link_libraries(Core ${SUITESPARSE_LIBRARIES})

# Core
include_directories(${CORE_INCLUDE_DIRS})

# Install
install(FILES ${core_hdrs} DESTINATION include/Core/)
install(FILES ${core_utils_hdrs} DESTINATION include/Core/Utils)
install(FILES ${core_shader_hdrs} DESTINATION include/Core/Shaders)
install(FILES ${core_cuda_hdrs} DESTINATION include/Core/Cuda)
install(FILES ${core_cuda_containers_hdrs} DESTINATION include/Core/Cuda/containers)

install(FILES ${core_shader_glsls} DESTINATION bin/GUI/Shaders)
install(FILES ${core_shader_verts} DESTINATION bin/GUI/Shaders)
install(FILES ${core_shader_geoms} DESTINATION bin/GUI/Shaders)
install(FILES ${core_shader_frags} DESTINATION bin/GUI/Shaders)

install(TARGETS Core
		RUNTIME DESTINATION bin/Core
		LIBRARY DESTINATION lib/Core
		ARCHIVE DESTINATION lib/Core)


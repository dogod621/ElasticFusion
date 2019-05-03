# GPUTest
file(GLOB gputest_srcs ${CMAKE_CURRENT_SOURCE_DIR}/../GPUTest/*.cpp)
file(GLOB gputest_testimages ${CMAKE_CURRENT_SOURCE_DIR}/../GPUTest/TestImages/*)

add_executable(GPUTest ${gputest_srcs})

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
target_link_libraries(GPUTest ${EXTRA_WINDOWS_LIBS})

# CUDA
include_directories(${CUDA_INCLUDE_DIRS})
target_link_libraries(GPUTest ${CUDA_LIBRARIES})

# Eigen
include_directories(${EIGEN_INCLUDE_DIRS})

# Pangolin
add_definitions(-DHAVE_EIGEN)
include_directories(${PANGOLIN_INCLUDE_DIRS})
target_link_libraries(GPUTest ${PANGOLIN_LIBRARIES})

# SuiteSparse
include_directories(${SUITESPARSE_INCLUDE_DIRS})
target_link_libraries(GPUTest ${SUITESPARSE_LIBRARIES})

# Core
include_directories(${CORE_INCLUDE_DIRS})
target_link_libraries(GPUTest Core)

# Install
install(FILES ${gputest_testimages} DESTINATION bin/GPUTest/TestImages)

install(TARGETS GPUTest
		RUNTIME DESTINATION bin/GPUTest)

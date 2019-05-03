# GUI
file(GLOB gui_srcs			${CMAKE_CURRENT_SOURCE_DIR}/../GUI/*.cpp)
file(GLOB gui_tools_srcs	${CMAKE_CURRENT_SOURCE_DIR}/../GUI/Tools/*.cpp)

file(GLOB gui_hdrs			${CMAKE_CURRENT_SOURCE_DIR}/../GUI/*.h)
file(GLOB gui_tools_hdrs	${CMAKE_CURRENT_SOURCE_DIR}/../GUI/Tools/*.h)

add_executable(GUI 
		${gui_srcs}
		${gui_tools_srcs}
		${gui_hdrs}
		${gui_tools_hdrs})
		
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
target_link_libraries(GUI ${EXTRA_WINDOWS_LIBS})

# CUDA
include_directories(${CUDA_INCLUDE_DIRS})
target_link_libraries(GUI ${CUDA_LIBRARIES})

# Eigen
include_directories(${EIGEN_INCLUDE_DIRS})

# Pangolin
add_definitions(-DHAVE_EIGEN)
include_directories(${PANGOLIN_INCLUDE_DIRS})
target_link_libraries(GUI ${PANGOLIN_LIBRARIES})

# SuiteSparse
include_directories(${SUITESPARSE_INCLUDE_DIRS})
target_link_libraries(GUI ${SUITESPARSE_LIBRARIES})

# OpenNI2
include_directories(${OPENNI2_INCLUDE_DIRS})
target_link_libraries(GUI ${OPENNI2_LIBRARIES})

# RealSense2
if(${USE_REALSENSE})
	add_definitions(-DWITH_REALSENSE)
	include_directories(${REALSENSE2_INCLUDE_DIRS})
	target_link_libraries(GUI ${REALSENSE2_LIBRARIES})
endif()

# Core
include_directories(${CORE_INCLUDE_DIRS})
target_link_libraries(GUI Core)

# Install
install(FILES ${SUITESPARSE_DLLS} DESTINATION bin/GUI)

install(TARGETS GUI
		RUNTIME DESTINATION bin/GUI)

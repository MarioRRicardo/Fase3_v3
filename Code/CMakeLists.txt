cmake_minimum_required(VERSION 2.8)

# Project Name
PROJECT(CGFase3)
set(GEN_HEADER_FILES generator.h tinyxml2.h xmlgen.h)
set(GEN_SOURCE_FILES generator.cpp tinyxml2.cpp xmlgen.cpp)

set(ENG_HEADER_FILES catmull-rom.h camera.h events.h fps.h engine.h objects.h vbo.h tinyxml2.h xmleng.h)
set(ENG_SOURCE_FILES catmull-rom.cpp camera.cpp events.cpp fps.cpp engine.cpp objects.cpp vbo.cpp tinyxml2.cpp xmleng.cpp)

# Version
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

if (NOT APPLE)
	if (WIN32)
		set(GLEW_ROOT_FOLDER "" CACHE PATH "Path to GLEW folder")
		include_directories(${GLEW_ROOT_FOLDER})
		link_directories(${GLEW_ROOT_FOLDER})
	else(WIN32)
		find_package(GLEW REQUIRED)
		include_directories(${GLEW_INCLUDE_DIRS})
		link_libraries(${GLEW_LIBRARIES})
	endif(WIN32)
endif(NOT APPLE)

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

# add the binary tree to the search path for include files
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

add_executable(Engine ${ENG_HEADER_FILES} ${ENG_SOURCE_FILES})
add_executable(Generator ${GEN_HEADER_FILES} ${GEN_SOURCE_FILES} )


if (WIN32)
	target_link_libraries(Engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} glew32)
	target_link_libraries(Generator ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} glew32)
else(WIN32)
	target_link_libraries(Engine ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
	target_link_libraries(Generator ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
endif (WIN32)



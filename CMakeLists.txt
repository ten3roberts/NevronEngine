cmake_minimum_required (VERSION 3.3)
project (NevronEngine)

add_subdirectory(Dependencies/GLFWx64)
add_subdirectory(Dependencies/GLEWx64)
include_directories(Dependencies/GLFWx64/include)


add_executable(PROJECT_NAME
	src/*
	Math/*
	Graphics/*
)


target_link_libraries(NevronEngine glfw ${GLFW_LIBRARIES})


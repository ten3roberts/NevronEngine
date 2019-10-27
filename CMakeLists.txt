cmake_minimum_required (VERSION 3.0)
project (NevronEngine)

find_package( OpenGL REQUIRED )

add_subdirectory(deps/glfw)
add_subdirectory(deps/glew/build/cmake)

include_directories(${PROJECT_NAME} deps/glfw/include)
include_directories(${PROJECT_NAME} deps/glew/include)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 1)

set (RELEASE false)

if(RELEASE)
  set(CMAKE_BUILD_TYPE Release)
else()
  set(CMAKE_BUILD_TYPE Debug)
endif()
set (CMAKE_CXX_STANDARD 17)

set (APPNAME ${PROJECT_NAME})


set(PLAT_LINUX false)
set(PLAT_UNIX false)
set(PLAT_APPLE false)
set(PLAT_WIN32 false)

if(UNIX)
  set(PLAT_UNIX true)
endif()

if(UNIX AND NOT APPLE)
  set(PLAT_LINUX true)
endif()

if(APPLE)
  set(PLAT_APPLE true)
endif()

if(WIN32)
  set(PLAT_WIN32 true)
endif()
#
#Used for configuring system specific macros
configure_file (
  "${PROJECT_SOURCE_DIR}/Config.h.in"
  "${PROJECT_SOURCE_DIR}/Config.h"
  )

add_executable(${PROJECT_NAME}
    ./Math/Vector3.cpp
    ./Math/Vector2.cpp
    ./Math/Vector4.cpp
    ./Math/Quaternion.cpp
    ./Math/Math.cpp
    ./Math/Vector.cpp
    ./Math/Matrix.cpp
    ./Scripts/Wave.cpp
    ./Graphics/Material.cpp
    ./Graphics/IndexBuffer.cpp
    ./Graphics/VertexBufferLayout.cpp
    ./Graphics/VertexBuffer.cpp
    ./Graphics/UniformBuffer.cpp
    ./Graphics/Renderer.cpp
    ./Graphics/Model.cpp
    ./Graphics/VertexArray.cpp
    ./Graphics/Texture.cpp
    ./Graphics/Shader.cpp
    ./src/GUID.cpp
    ./src/Utility.cpp
    ./src/Entity.cpp
    ./src/ResourceManager.cpp
    ./src/String.cpp
    ./src/Rigidbody.cpp
    ./src/Logger.cpp
    ./src/Main.cpp
    ./src/XMLParser.cpp
    ./src/Transform.cpp
    ./src/Camera.cpp
    ./src/Time.cpp
    #./src/vendor/stb_image/stb_image.cpp
    ./src/Timer.cpp
    ./src/Component.cpp
    ./src/Settings.cpp
    ./src/pch.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ./)

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} glew ${GLEW_LIBRARIES})

cmake_minimum_required (VERSION 3.0)
project (NevronEngine)

find_package( OpenGL REQUIRED )

add_subdirectory(deps/glfw)
add_subdirectory(deps/glew/build/cmake)

include_directories(NevronEngine deps/glfw/include)
include_directories(NevronEngine deos/glew/include)

set(CMAKE_BUILD_TYPE Debug)

set (CMAKE_CXX_STANDARD 17)


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
##./src/vendor/stb_image/stb_image.cpp
./src/Timer.cpp
./src/Component.cpp
./src/Settings.cpp
./src/pch.cpp

)

target_include_directories(${PROJECT_NAME} PRIVATE ./)

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})
target_link_libraries(${PROJECT_NAME} glew ${GLEW_LIBRARIES})
cmake_minimum_required(VERSION 3.22.1)
project(Chess_AI VERSION 0.1.0)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})

include_directories(src/extern/stb_image)
include_directories(src/extern/glm)
include_directories(src/utils)
include_directories(src/objects)

include_directories(res)

add_executable(chess_AI 
src/main.cpp
src/extern/stb_image/stb_image.cpp
src/viewer.cpp 
src/controller.cpp 
src/logic.cpp
src/utils/Object.cpp
src/utils/Renderer.cpp
src/utils/VertexBuffer.cpp
src/utils/IndexBuffer.cpp
src/utils/VertexBufferLayout.hpp
src/utils/VertexArray.cpp
src/utils/Shader.cpp
src/utils/Texture.cpp
src/utils/TextureManager.hpp
src/objects/Piece.cpp
src/objects/Square.cpp
src/objects/Board.cpp
src/objects/Move.hpp
)

target_compile_options(chess_AI PRIVATE -g)

target_link_libraries(chess_AI ${OPENGL_LIBRARIES} GLEW::GLEW glfw)
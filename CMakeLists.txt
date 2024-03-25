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
include_directories(src/extern/gltext)
include_directories(src/utils)
include_directories(src/objects)
include_directories(src/chess)
include_directories(res)

add_executable(chess_AI 
src/main.cpp
src/extern/stb_image/stb_image.cpp
src/viewer.cpp 
src/utils/Object.cpp
src/utils/Renderer.cpp
src/utils/VertexBuffer.cpp
src/utils/IndexBuffer.cpp
src/utils/VertexBufferLayout.hpp
src/utils/VertexArray.cpp
src/utils/Shader.cpp
src/utils/Texture.cpp
src/utils/TextureManager.hpp
src/utils/ShaderManager.hpp
src/utils/EventManager.cpp
src/utils/TextObject.cpp
src/objects/Piece.cpp
src/objects/Square.cpp
src/objects/Board.cpp
src/objects/InformationText.cpp
src/chess/Move.cpp
src/chess/PieceType.hpp
src/objects/PromotionSelector.hpp
src/chess/Zobrist.cpp
src/chess/TranspositionTable.cpp
src/chess/MoveGenerator.cpp
src/chess/Evaluator.cpp
src/chess/ChessAI.cpp
src/chess/Perft.cpp
)

target_compile_options(chess_AI PRIVATE -O3)

target_link_libraries(chess_AI ${OPENGL_LIBRARIES} GLEW::GLEW glfw)
# CMakeList.txt : CMake project for OpenGLEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("OpenGLEngine")

include_directories(core)
include_directories(includes)
include_directories(libs)
include_directories(libs/assimp/include)

# HeaderFiles
set (HEADER_FILES
core/Application.h "core/Window.h" "core/ShaderProgram.h" "core/Mesh.h" "core/Vertex.h" "core/Camera.h" "core/Transform.h" "core/MeshGenerator.h" "core/TriangleCollision.h" "core/Triangle.h" "core/DrawLineTemp.h" "core/Actor.h" "core/Component.h" "core/MeshComponent.h" "core/MeshActor.h" "core/Character.h" "core/CameraActor.h" "core/Enemy.h")

 # SourceFiles
set (SRC_FILES
includes/glad.c
core/Application.cpp "core/Window.cpp" "core/ShaderProgram.cpp" "core/Mesh.cpp" "core/Camera.cpp" "core/MeshGenerator.cpp" "core/TriangleCollision.cpp" "core/Actor.cpp" "core/Component.cpp" "core/MeshActor.cpp" "core/MeshComponent.cpp" "core/Character.cpp" "core/CameraActor.cpp" "core/Enemy.cpp")

add_subdirectory(libs/glfw-3.3.9)

#set(BUILD_SHARED_LIBS OFF)
add_subdirectory(libs/assimp)

# Add source to this project's executable.
add_executable (OpenGLEngine ${HEADER_FILES} ${SRC_FILES} "OpenGLEngine.cpp" "OpenGLEngine.h")

target_link_libraries(OpenGLEngine glfw)
target_link_libraries(OpenGLEngine assimp)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OpenGLEngine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

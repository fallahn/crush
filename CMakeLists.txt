project(CRUSH)
cmake_minimum_required(VERSION 2.8)

#prefer clang, comment out if using g++ (4.9+)
#SET (CMAKE_C_COMPILER             "/usr/bin/clang")
#SET (CMAKE_C_FLAGS                "-Wall -std=c99")
#SET (CMAKE_C_FLAGS_DEBUG          "-g -_DEBUG_")
#SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
#SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
#SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

#SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
#SET (CMAKE_CXX_FLAGS                "-Wall")
#SET (CMAKE_CXX_FLAGS_DEBUG          "-g -_DEBUG_")
#SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
#SET (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
#SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

#SET (CMAKE_AR      "/usr/bin/llvm-ar")
#SET (CMAKE_LINKER  "/usr/bin/llvm-ld")
#SET (CMAKE_NM      "/usr/bin/llvm-nm")
#SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
#SET (CMAKE_RANLIB  "/usr/bin/llvm-ranlib")
#---------------------------------------------

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

find_package(SFML 2 REQUIRED system window graphics)
#optional package manager
#include (FindPkgConfig)
#PKG_SEARCH_MODULE(SFML 2 REQUIRED system window graphics)


include_directories(
	${SFML_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/include)

link_libraries(
	${SFML_LIBRARIES})

add_executable(
	CRUSH src/main.cpp
	src/Affectors.cpp
	src/BlockController.cpp
	src/BlockStates.cpp
	src/BodyState.cpp
	src/Camera.cpp
	src/CollisionBody.cpp
	src/CollisionWorld.cpp
	src/CommandStack.cpp
	src/DebugShape.cpp
	src/FontResource.cpp
	src/Game.cpp
	src/GameState.cpp
	src/GameOverState.cpp
	src/Map.cpp
	src/MenuState.cpp
	src/Node.cpp
	src/NpcController.cpp
	src/NpcStates.cpp
	src/ParticleController.cpp
	src/Particles.cpp
	src/PauseState.cpp
	src/Player.cpp
	src/PlayerStates.cpp
	src/Scene.cpp
	src/ScoreBoard.cpp
	src/State.cpp
	src/StateStack.cpp
	src/TitleState.cpp)

#copy reources to output directory
add_custom_command(
	TARGET CRUSH POST_BUILD
	COMMAND {$CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/res
	$<TARGET_FILE_DIR:CRUSH>)
		


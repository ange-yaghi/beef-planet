cmake_minimum_required(VERSION 3.10)

option(DTV "Enable video output" OFF)

if (DTV)
    add_compile_definitions(BEEF_PLANET_ENABLE_VIDEO_CAPTURE)
endif (DTV)

# Enable group projects in folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

project(beef-planet)

set(CMAKE_CXX_STANDARD 11)

add_executable(beef-planet WIN32
    # Source files
    src/main.cpp
    src/beef_application.cpp
    src/game_object.cpp
    src/gravity_simulator.cpp
    src/mouse_controller.cpp
    src/physical_object.cpp
    src/physics_component.cpp
    src/planet_spawner.cpp
    src/player_object.cpp
    src/universe.cpp
    src/space_dust.cpp

    # Include files
    include/delta.h
    include/beef_application.h
    include/game_object.h
    include/gravity_simulator.h
    include/mouse_controller.h
    include/physical_object.h
    include/physics_component.h
    include/planet_spawner.h
    include/player_object.h
    include/universe.h
    include/space_dust.h
)

target_link_libraries(beef-planet
    delta-basic)

target_include_directories(beef-planet
    PUBLIC dependencies/submodules)

if (DTV)
    target_link_libraries(beef-planet
        direct-to-video)
endif (DTV)

add_subdirectory(dependencies)

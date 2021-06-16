cmake_minimum_required(VERSION 3.13)

project(incubus)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set (TARGET_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(OBJECTS_DIR "objects")
list(APPEND SRCS_OBJECTS
    ${OBJECTS_DIR}/object.cpp
    ${OBJECTS_DIR}/objectgameplay.cpp
    ${OBJECTS_DIR}/objectphysics.cpp
    ${OBJECTS_DIR}/physics_obj_classes/objectphysics_mob.cpp
    ${OBJECTS_DIR}/physics_obj_classes/objectphysics_projectile.cpp
    ${OBJECTS_DIR}/physics_obj_classes/objectphysics_construction.cpp
    ${OBJECTS_DIR}/physics_obj_classes/objectphysics_loot.cpp
    ${OBJECTS_DIR}/objectshow.cpp
    ${OBJECTS_DIR}/show_obj_classes/objectshow_mob.cpp
    ${OBJECTS_DIR}/show_obj_classes/objectshow_projectile.cpp
    ${OBJECTS_DIR}/show_obj_classes/objectshow_construction.cpp
    ${OBJECTS_DIR}/show_obj_classes/objectshow_loot.cpp
    ${OBJECTS_DIR}/objectbehavior.cpp
    ${OBJECTS_DIR}/gameplayobjectgenerator.cpp)

list(APPEND HDRS_OBJECTS
    ${OBJECTS_DIR}/object.h
    ${OBJECTS_DIR}/objectgameplay.h
    ${OBJECTS_DIR}/objectphysics.h
    ${OBJECTS_DIR}/physics_obj_classes/objectphysics_mob.h
    ${OBJECTS_DIR}/physics_obj_classes/objectphysics_projectile.h
    ${OBJECTS_DIR}/physics_obj_classes/objectphysics_construction.h
    ${OBJECTS_DIR}/physics_obj_classes/objectphysics_loot.h
    ${OBJECTS_DIR}/objectshow.h
    ${OBJECTS_DIR}/show_obj_classes/objectshow_mob.h
    ${OBJECTS_DIR}/show_obj_classes/objectshow_projectile.h
    ${OBJECTS_DIR}/show_obj_classes/objectshow_construction.h
    ${OBJECTS_DIR}/show_obj_classes/objectshow_loot.h
    ${OBJECTS_DIR}/objectbehavior.h
    ${OBJECTS_DIR}/common_obj.h
    ${OBJECTS_DIR}/i_object.h
    ${OBJECTS_DIR}/physics_obj_classes/iphobjclass.h
    ${OBJECTS_DIR}/show_obj_classes/ishobjclass.h
    ${OBJECTS_DIR}/behavior_obj_classes/ibhobjclass.h
    ${OBJECTS_DIR}/gameplayobjectgenerator.h)

set(CORES_DIR "cores")
list(APPEND SRCS_CORES
    ${CORES_DIR}/draw/drawopengl.cpp
    ${CORES_DIR}/draw/drawopengl3.cpp
    ${CORES_DIR}/draw/drawenginetextures.cpp
    ${CORES_DIR}/draw/drawshape.cpp
    ${CORES_DIR}/draw/drawfigure.cpp
    ${CORES_DIR}/draw/drawtrace.cpp
    ${CORES_DIR}/draw/drawtext.cpp
    ${CORES_DIR}/draw/drawprogressbar.cpp
    ${CORES_DIR}/gameplay.cpp
    ${CORES_DIR}/physics.cpp
    ${CORES_DIR}/behavior.cpp
    ${CORES_DIR}/show.cpp
    ${CORES_DIR}/show_gui/showgui.cpp
    ${CORES_DIR}/show_gui/guielementbutton.cpp
    ${CORES_DIR}/show_gui/guielementprogressbar.cpp)

list(APPEND HDRS_CORES
    ${CORES_DIR}/draw/idrawengine.h
    ${CORES_DIR}/draw/drawenginetextures.h
    ${CORES_DIR}/draw/drawopengl.h
    ${CORES_DIR}/draw/drawopengl3.h
    ${CORES_DIR}/draw/drawshape.h
    ${CORES_DIR}/draw/drawfigure.h
    ${CORES_DIR}/draw/drawtrace.h
    ${CORES_DIR}/draw/drawshaders.h
    ${CORES_DIR}/draw/drawbuffers.h
    ${CORES_DIR}/draw/drawtext.h
    ${CORES_DIR}/draw/drawprogressbar.h
    ${CORES_DIR}/gameplay.h
    ${CORES_DIR}/physics.h
    ${CORES_DIR}/behavior.h
    ${CORES_DIR}/show.h
    ${CORES_DIR}/common_show.h
    ${CORES_DIR}/show_gui/i_gui_element.h
    ${CORES_DIR}/show_gui/showgui.h
    ${CORES_DIR}/show_gui/guielementbutton.h
    ${CORES_DIR}/show_gui/guielementprogressbar.h
    ${CORES_DIR}/coresstatistic.h)

set(MESSAGES_DIR "cores/messages")
list(APPEND SRCS_MESSAGES
    ${MESSAGES_DIR}/messagecallbacks.cpp
    ${MESSAGES_DIR}/messagesender.cpp)

list(APPEND HDRS_MESSAGES
    ${MESSAGES_DIR}/messages.h
    ${MESSAGES_DIR}/messagecallbacks.h
    ${MESSAGES_DIR}/messagesender.h)

set(PARAM_DIR "param")
list(APPEND HDRS_PARAM
    ${PARAM_DIR}/object_param_common.h
    ${PARAM_DIR}/physics_param_common.h
    ${PARAM_DIR}/physics_param.h
    ${PARAM_DIR}/show_param.h
    ${PARAM_DIR}/gui_param.h)

set(SRCS
    "main.cpp"
    ${SRCS_CORES}
    ${SRCS_DISPATCHER}
    ${SRCS_OBJECTS}
    ${SRCS_MESSAGES}
    "media/mediagraphics.cpp"
    "media/mediasound.cpp"
    "test/unittest.cpp")
set(HDRS
    ${HDRS_CORES}
    "incubus_common.h"
    "description.h"
    ${HDRS_DISPATCHER}
    ${HDRS_OBJECTS}
    ${HDRS_MESSAGES}
    ${HDRS_PARAM}
    "media/sound.h"
    "media/mediagraphics.h"
    "media/mediasound.h"
    "test/unittest.h")
add_executable(${PROJECT_NAME} ${SRCS} ${HDRS})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
include_directories(${PROJECT_NAME})
find_package(Threads REQUIRED)
find_package(OpenAL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

if(NOT GLM_FOUND)
    message(Error "GLM not found")
endif(NOT GLM_FOUND)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/resources/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources/)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
find_package(GLEW REQUIRED)
target_link_libraries(${PROJECT_NAME} logger dispatcher math openal -lSOIL -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr)

if(OPENGL_FOUND)
    message('OPENGL_FOUND-is-true')
else()
    message('OPENGL_FOUND-is-false')
endif()
message(STATUS "OPENGL_INCLUDE_DIR:= ${OPENGL_INCLUDE_DIR}")
message(STATUS "OPENGL_LIBRARY:= ${OPENGL_LIBRARY}")
message(STATUS "OPENGL_LIBRARIES:= ${OPENGL_LIBRARIES}")

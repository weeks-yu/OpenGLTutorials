project (OpenGLTutorials)
cmake_minimum_required (VERSION 3.0)


set (CMAKE_ALLOW_LOOSE_CONSTRUCTS true)
if (NOT DEFINED MSVC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif ()

#### platform detection ####
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    message ("64 bits compiler detected")
    set (EX_PLATFORM 64)
    set (EX_PLATFORM_NAME "x64")
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4) 
    message ("32 bits compiler detected")
    set (EX_PLATFORM 32)
    set (EX_PLATFORM_NAME "x86")
endif ()



#### locate Qt ####
set (Qt_DIR "" CACHE PATH "Qt root directory here")
set (Qt_MODULES_REQUIRED Core Gui Widgets OpenGL OpenGLExtensions)

message (STATUS "Qt directory: ${Qt_DIR}")
list (APPEND CMAKE_PREFIX_PATH ${Qt_DIR})
set(CMAKE_INCLUDE_CURRENT_DIR on)
set (Qt_PATH ${Qt_DIR}/bin)

# set cmake library paths for windows, this is required by Qt OpenGL
set (Qt_INCLUDES "")
set (Qt_LIBS "")

foreach (_M ${Qt_MODULES_REQUIRED})
    find_package ("Qt5${_M}" REQUIRED)
    string(COMPARE EQUAL ${_M} OpenGL _IsOpenGL)
    if(${_IsOpenGL})
        #find_package (GLUT REQUIRED)
        find_package (OPENGL REQUIRED)
        #list(APPEND Qt_INCLUDES ${GLUT_INCLUDE_DIR})
        list(APPEND Qt_INCLUDES ${OPENGL_INCLUDE_DIR})
        list(APPEND Qt_LIBS ${OPENGL_LIBRARIES})
    endif()
    list(APPEND Qt_LIBS "Qt5::${_M}")
endforeach()

foreach (i ${Qt_INCLUDES})
    message (STATUS "qt includes: " ${i})
endforeach ()
foreach (i ${Qt_LIBS})
    message (STATUS "qt libs: " ${i})
endforeach ()


#### locate GLUT ####
if (WIN32 AND (EX_PLATFORM EQUAL 32))   
    set (GLUT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/include)
    set (GLUT_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/extern/lib/win32/glut32.lib)
    set (GLUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/extern/lib/win32)
else()
    find_package(GLUT)
endif()


### define data path ####
set (OPENGL_TUTORIALS_DATA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/data)
if (MSVC)
    add_definitions ( "/DOPENGL_TUTORIALS_DATA_PATH=\"${OPENGL_TUTORIALS_DATA_PATH}\"" )
else ()
    add_definitions ( "-DOPENGL_TUTORIALS_DATA_PATH=\"${OPENGL_TUTORIALS_DATA_PATH}\"" )
endif ()

### demo project ###
add_subdirectory (demo)
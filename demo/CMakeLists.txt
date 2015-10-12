file (GLOB SOURCES *.h  *.hpp *.cc *.cpp *.qrc *.ui)

set(CMAKE_AUTOMOC on)
set(CMAKE_AUTOUIC on)
set(CMAKE_AUTORCC on)

if (MSVC)
    message (WARNING "CMake cannot configure Visual Studio to modify the environment path during program execution, "
        "therefore you have to do this manually: add 'PATH=\$(PATH);${Qt_PATH};' to [Project Property]->[Debug]->[Environment] ")
endif ()


include_directories (${Qt_INCLUDES})
add_executable (Demo ${SOURCES})
target_link_libraries (Demo ${Qt_LIBS})
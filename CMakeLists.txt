cmake_minimum_required(VERSION 3.10)
project(RobotNavigation VERSION 1.0.0 LANGUAGES CXX)

# Inset path to wehere is your SFML cmkae conf file is located 
set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")

# find SFML library
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2 COMPONENTS graphics window system REQUIRED)

# collect all source files
file(GLOB sources src/*.cpp)
set(SOURCES ${sources})

# build exectutable
add_executable(${PROJECT_NAME} ${WIN32_GUI} ${SOURCES})
target_compile_options(${PROJECT_NAME} PUBLIC -lm -Wall)
target_include_directories(${PROJECT_NAME} PRIVATE SFML_INCLUDE_DIR)
target_include_directories(${PROJECT_NAME} PRIVATE SFML_INCLUDE_LIB)
target_include_directories(${PROJECT_NAME} PRIVATE "include")

# link SFML library with executable
if(SFML_FOUND)
  target_link_libraries(${PROJECT_NAME} 
        sfml-graphics 
        sfml-window 
        sfml-system
        opengl32
        winmm
        gdi32
        )
endif()


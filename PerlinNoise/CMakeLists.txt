#Минимальная версия сборщика
cmake_minimum_required (VERSION 3.8)

#название проекта сборки
project (PerlinNoiseProject)

#установка путей сборки
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/arc)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

#Включение conan
include(${CMAKE_SOURCE_DIR}/build/conanbuildinfo.cmake)
conan_basic_setup()

#find_package(GLEW REQUIRED)
#if (GLEW_FOUND)
#    include_directories(${GLEW_INCLUDE_DIRS})
#    link_libraries(${GLEW_LIBRARIES})
#endif()

#find_package(GLEW 2.0 REQUIRED)
#target_link_libraries(PerlinNoiseProject GLEW::GLEW)

include_directories(PerlinNoise)

add_executable (PerlinNoiseProject "main.cpp")

add_subdirectory(PerlinNoise)

#Настройка цели сборки
set_target_properties(PerlinNoiseProject PROPERTIES 
#Имя выходного файла
OUTPUT_NAME "Perlin noise"
)

target_link_libraries(PerlinNoiseProject ${CONAN_LIBS})

#target_link_libraries(PerlinNoiseProject PerlinNoiseLib)
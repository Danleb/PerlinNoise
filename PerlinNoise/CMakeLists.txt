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

#добавить папку 
include_directories(PerlinNoise)

#добавить к исполняемому коду этот файл
add_executable (PerlinNoiseProject "main.cpp" "PerlinNoise/PerlinNoise.h" "PerlinNoise/PerlinNoise.cpp")

#добавить папку к проекту
#add_subdirectory(PerlinNoise)

#Настройка цели сборки
set_target_properties(PerlinNoiseProject PROPERTIES 
#Имя выходного файла
OUTPUT_NAME "Perlin noise"
)

#прилинковать все библиотеки конана
target_link_libraries(PerlinNoiseProject ${CONAN_LIBS})

message("!!!")
message("'${CONAN_LIBS}'")

#find_package(GLEW REQUIRED)
#if (GLEW_FOUND)
#    include_directories(${GLEW_INCLUDE_DIRS})
#    link_libraries(${GLEW_LIBRARIES})
#endif()
#find_package(GLEW 2.0 REQUIRED)
#target_link_libraries(PerlinNoiseProject GLEW::GLEW)

#функция для включения шейдеров в исходники как строки
function(make_includable input_file output_file)
	#читам файл в переменную
    file(READ ${input_file} content)
	#устанавливаем разделитель
    set(delim "glsl")
	#создаём конечную строку
    set(content "R\"${delim}(\n${content})${delim}\"")
	#пишем в файл
    file(WRITE ${output_file} "${content}")
endfunction(make_includable)

make_includable(shaders/vertex.glsl generated/vertex.glsl)
make_includable(shaders/fragment.glsl generated/fragment.glsl)
cmake_minimum_required(VERSION 3.10)
project(MonteCarloIntegration)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")

# Указание относительного пути к Boost относительно корня проекта
set(BOOST_ROOT "${CMAKE_SOURCE_DIR}/boost_1_86_0")
set(Boost_INCLUDE_DIR "${BOOST_ROOT}")
set(Boost_LIBRARY_DIR "${BOOST_ROOT}/stage/lib")

# Поиск Boost
find_package(Boost REQUIRED COMPONENTS unit_test_framework)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/src) # Директория, где хранится exprtk.hpp

# Включение директорий с исходниками
include_directories(${CMAKE_SOURCE_DIR}/src)

# Основной исполняемый файл
add_executable(MonteCarloIntegration 
    src/main.cpp 
    src/integral_calculator.cpp 
    src/integral_calculator.h 
    src/functions.cpp 
    src/functions.h 
    src/gnuplot_i.hpp
    src/exprtk.hpp)

# Добавление тестов
add_executable(runTests 
    tests/test_monte_carlo.cpp
    src/integral_calculator.cpp  # Реализация monteCarloIntegral
    src/functions.cpp            # Реализация func1, func2, func3
    src/exprtk.hpp
)

# Линковка с Boost.Test
target_link_libraries(runTests ${Boost_LIBRARIES})

# Включение тестирования
enable_testing()
add_test(NAME runTests COMMAND runTests)

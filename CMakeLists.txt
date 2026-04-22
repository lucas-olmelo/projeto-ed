cmake_minimum_required(VERSION 3.10)

project(ProjetoED)

include_directories(include)

add_executable(projeto_ed src/main.cpp src/DataGenerator.cpp src/Sorter.cpp src/Searcher.cpp src/Metrics.cpp src/ExperimentRunner.cpp)
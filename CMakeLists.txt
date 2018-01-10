cmake_minimum_required(VERSION 3.9)
project(QuienEsQuien)

set(CMAKE_CXX_STANDARD 11)

add_executable(GuessWho src/main.cpp include/bintree.h include/bintree.hxx include/node.hxx include/pregunta.h include/quienesquien.h src/pregunta.cpp src/quienesquien.cpp include/funciones.h)
cmake_minimum_required(VERSION 3.16)

project(Desafio2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Desafio2 main.cpp
    selecciones.h selecciones.cpp
    partido.h partido.cpp
    torneo.h torneo.cpp
    jugadores.h jugadores.cpp
    statspartido.h statspartido.cpp
    statsteam.h statsteam.cpp

    grupos.h grupos.cpp
    gestorarchivos.h gestorarchivos.cpp
    fecha.h fecha.cpp
    medidor.h medidor.cpp)

include(GNUInstallDirs)
install(TARGETS Desafio2
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ------------------------------------------------------------------------------
# Makefile - trazador de rayos (CGyAV 2026, IUA).
#
#   make            compila bin/rrt
#   make run        compila y renderiza la escena por defecto
#   make clean
#
# Dependencias del sistema: libglm-dev. No usa GLFW ni glad: no abre ventana,
# escribe la imagen en un archivo PNG. Vendorizada en third_party/:
# stb_image_write (para escribir PNG).
# ------------------------------------------------------------------------------

# ------------------------------------------------------------------------------
# Configurar includes paths
INC_DIRS = \
    ./third_party \
    ./src/rt \
    ./src/escenas

# ------------------------------------------------------------------------------
# Fuentes que se recolectan solas (todos los .cpp de cada carpeta).
# Mantener DISJUNTO de SRC_DIR (si no, main.o se linkearía dos veces).
LIB_DIRS = \
    ./src/rt \
    ./src/escenas

# ------------------------------------------------------------------------------
# Dir del main de la aplicación y nombre del archivo de código fuente
SRC_DIR  = \
    ./src                     # main.cpp

MAIN_CXX     = main

# ------------------------------------------------------------------------------
# Configurar salida el proyecto
PROJECT_NAME = rrt

# ------------------------------------------------------------------------------
# Configurar linkeo de librerías: OpenMP reparte las filas de la imagen entre
# los núcleos del procesador
PROJECT_LDLIBS = -fopenmp

# ------------------------------------------------------------------------------
# Configurar opciones de compilación
# Para compilar con símbolos para debug usar: -g -Wall
# -MMD -MP: el compilador anota de qué headers depende cada .o (archivos .d en
# build/), para que tocar un .h recompile lo que lo incluye
USERCPPFLAGS = -O2 -Wall -Wextra -fopenmp -MMD -MP

# ------------------------------------------------------------------------------
# Makefile.master
ifeq ($(wildcard ./Makefile.master),)
    $(error Makefile.master no encontrado: se espera symlink a \
    makefile-unified/Makefile.master o una copia local junto a este Makefile)
endif
include ./Makefile.master

-include $(wildcard $(BUILD_DIR)/*.d)

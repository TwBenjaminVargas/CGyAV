#ifndef DEFINES_H
#define DEFINES_H

#include <limits>

/// Desplazamiento del origen de los rayos secundarios y de sombra, para que
/// el error de redondeo no los haga chocar con la superficie de la que salen
const float EPSILON = 1e-4f;

const float INF = std::numeric_limits<float>::infinity();

#endif // DEFINES_H

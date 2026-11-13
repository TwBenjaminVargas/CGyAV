#ifndef OBJECT_H
#define OBJECT_H

#include <memory>

#include <hit.h>
#include <material.h>
#include <ray.h>

/// Clase base de la geometría: para modelar todo lo que un rayo puede intersecar
class Object {
public:
    explicit Object(std::shared_ptr<Material> mat) : mat_(std::move(mat)) {}
    virtual ~Object() = default;

    /// Si el rayo toca el objeto en algún t > EPSILON, devuelve por referencia
    /// en el argumento out los datos de la intersección más cercana y devuelve
    /// true para indicar que el rayo efectivamente intersecta el objeto.
    virtual bool intersect(const Ray& r, Hit& out) const = 0;

protected:
    std::shared_ptr<Material> mat_;
};

#endif // OBJECT_H

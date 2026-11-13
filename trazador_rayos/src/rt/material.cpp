#include <material.h>
#include <scene.h>

glm::vec3 Material::local(const Scene& scene, const Ray& r, const Hit& h) const
{
    return scene.local_terms(r, h).total();
}

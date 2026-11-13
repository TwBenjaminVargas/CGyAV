#include <scene.h>

bool Scene::find_nearest(const Ray& r, Hit& closest) const
{
    bool any = false;
    Hit current;

    for( const auto& obj : objects_ ){
        if( obj->intersect(r, current) && current.t < closest.t ){
            closest = current;
            any = true;
        }
    }
    return any;
}

glm::vec3 Scene::background(const Ray& r) const
{
    const float up = glm::max(glm::normalize(r.d()).y, 0.0f);
    return glm::mix(bg_horizon_, bg_zenith_, up);
}

glm::vec3 Scene::normal_color(const Ray& r) const
{
    Hit h;
    if( !find_nearest(r, h) ){
        return background(r);
    }
    // De [-1,1] a [0,1], componente a componente
    return 0.5f * (h.n + glm::vec3(1.0f));
}

LocalTerms Scene::local_terms(const Ray& r, const Hit& h) const
{
    LocalTerms terms;
    const Material& m = *h.mat;

    const glm::vec3 v = glm::normalize(-r.d());     // hacia el observador
    const glm::vec3& n = h.n;                       // ya orientada hacia v

    // Ambiente: ka en el punto (un tablero lo hace variar)
    terms.ambient = m.ka_at(h.p);

    for( const auto& L : lights_ ){
        glm::vec3 s = L.pos() - h.p;                // hacia la luz
        const float dist = glm::length(s);
        s /= dist;

        // Sombra: ¿qué fracción de la luz llega hasta el punto? (paso 5)
        const float visibility = light_transmission(h.p, n, L.pos());
        if( visibility <= 0.0f ){
            continue;
        }

        // Difuso
        const float k_diff = glm::max(glm::dot(n, s), 0.0f);

        // Especular, Phong (sin brillo si la luz está detrás de la superficie)
        const glm::vec3 refl = glm::reflect(-s, n);
        const float k_spec = (k_diff > 0.0f)
                ? glm::pow(glm::max(glm::dot(refl, v), 0.0f), m.shininess)
                : 0.0f;

        // Intensidad que llega del foco
        const glm::vec3 Li = L.color() * (L.attenuation(dist) * visibility);

        terms.diffuse  += m.kd_at(h.p) * k_diff * Li;
        terms.specular += m.ks * k_spec * Li;
    }

    return terms;
}

float Scene::light_transmission(const glm::vec3& p, const glm::vec3& n,
                                const glm::vec3& light_pos) const
{
    constexpr int kMaxCrossings = 16;

    // El origen se corre un poco hacia afuera para que el rayo de sombra no
    // choque con la propia superficie por redondeo
    glm::vec3 origin = p + n * EPSILON;
    float transmission = 1.0f;

    // El rayo de sombra avanza hacia la luz; cada superficie que cruza deja
    // pasar shadow_transmission() de la luz (0 si es opaca)
    for( int i = 0; i < kMaxCrossings; ++i ){
        const glm::vec3 to_light = light_pos - origin;
        const float dist = glm::length(to_light);
        const glm::vec3 s = to_light / dist;

        // Termina si no hay ningún objeto entre el punto y la luz. Un objeto
        // que está más allá de la luz no cuenta
        Hit h;
        if( !find_nearest(Ray(origin, s), h) || h.t >= dist - EPSILON ){
            return transmission;
        }

        transmission *= h.mat->shadow_transmission();
        if( transmission <= 0.0f ){
            return 0.0f;
        }
        origin = h.p + s * EPSILON;
    }
    return transmission;
}

glm::vec3 Scene::trace(const Ray& r, unsigned int depth) const
{
    // Rama truncada: no aporta luz
    if( depth > max_depth_ ){
        return glm::vec3(0.0f);
    }

    Hit h;
    if( !find_nearest(r, h) ){
        return background(r);
    }
    const Material& m = *h.mat;

    // I = I_local + suma de (peso_i * I(rayo_i))
    //
    // La escena calcula los términos de Phong con sombras (local_terms) y el
    // material elige cuáles usa. La llamada es virtual: trace no necesita
    // saber qué material tiene el objeto
    glm::vec3 color = m.local(*this, r, h);

    Secondary sec[2];
    const int count = m.secondary(r, h, sec);
    for( int i = 0; i < count; ++i ){
        color += sec[i].weight * trace(sec[i].ray, depth + 1U);
    }

    // Sin recortar a [0,1]: el recorte va al final, en el píxel
    return color;
}

#ifndef NEXUS_SF_CONTEXTUAL_HPP
#define NEXUS_SF_CONTEXTUAL_HPP

#include "../../utils/nxContextual.hpp"

namespace nexus { namespace gfx {
    class Renderer;
}}

namespace nexus { namespace sr {

    // La struct gl::Container est un "template" permettant le stockage partagé de données GPU.
    // Il est associé de manière permanente à un gl::Context et sert de structure parente aux objets de chaque type de donnée GPU, comme gl::Texture, gl::Shader, gl::Mesh, etc.
    // En resumé, chaque class de resources GPU de l'API utilisera un container qui héritera de cette struct et chaque conateiner sera stocké sous forme de std::shared_ptr par les class tel que gl::Texture, gl::Shader, etc.
    // Cependant, la copie en grand nombre des resources GPU pourra donc demander des ressources supplémentaires en raison de la nature "thread-safe" du compteur de référence de std::shared_ptr,
    // il est donc recommender de favorisé le transfert des données contextuelle plutot par reference ou par déplacement, sauf lorsque la copie est absolument nécéssaire.

    using SoftContextual = utils::Contextual<class Context>;
    using HardContextual = utils::Contextual<gfx::Renderer>;
    using Contextual = utils::Contextual<class Context>;

    // Cette class gl::Contextual est la class parente de chaque type de resource GPU utilisable tel que gl::Texture, gl::Shader, etc.
    // C'est cette class Contextual qui contiendra le Container de resources partagée GPU. Cette class existe pour évité la duplication de code dans les
    // class type utilisable de l'API, tel que les fonctions GetContext, IsContext, etc...

    template <typename T_Contextual>
    using Container = utils::Container<T_Contextual>;

}}

#endif //NEXUS_SF_CONTEXTUAL_HPP

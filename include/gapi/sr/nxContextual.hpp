#ifndef NEXUS_SF_CONTEXTUAL_HPP
#define NEXUS_SF_CONTEXTUAL_HPP

#include "../../utils/nxContextual.hpp"

namespace nexus { namespace gfx {
    class Renderer;
}}

namespace nexus { namespace sr {

    using Contextual = utils::Contextual<class Context>;

    template <typename T_Contextual>
    using Container = utils::Container<T_Contextual>;

}}

#endif //NEXUS_SF_CONTEXTUAL_HPP

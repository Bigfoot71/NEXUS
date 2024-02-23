#ifndef NEXUS_GL_CONTEXTUAL_HPP
#define NEXUS_GL_CONTEXTUAL_HPP

#include "../../utils/nxContextual.hpp"

namespace nexus { namespace gl {

    using Contextual = utils::Contextual<class Context>;

    template <typename T_Contextual>
    using Container = utils::Container<T_Contextual>;

}}

#endif //NEXUS_GL_CONTEXTUAL_HPP

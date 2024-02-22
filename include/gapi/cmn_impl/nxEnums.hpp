#ifndef NEXUS_GAPI_IMPL_ENUMS_HPP
#define NEXUS_GAPI_IMPL_ENUMS_HPP

// Here are defined the enums common to all GAPI implementations

namespace nexus { namespace gapi {

    enum class MatrixMode
    {
        ModelView,
        Projection,
        Texture
    };

    enum class DrawMode
    {
        Lines       = 2,
        Triangles   = 3,
        Quads       = 4
    };

}}

#endif //NEXUS_GAPI_IMPL_ENUMS_HPP

#include "gapi/sr/sp_model/nxMesh.hpp"

using namespace nexus;

/* Private Mesh Implementation */

void _sr_impl::Mesh::Draw(sr::Material& material, const math::Mat4& transform)
{
    ctx.DrawVertexArray(*this, material, transform);
}

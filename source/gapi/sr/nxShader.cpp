#include "gapi/sr/nxShader.hpp"
#include "gapi/sr/nxContext.hpp"

using namespace nexus;

void sr::Shader::Begin()
{
    ctx->SetShader(this);
}

void sr::Shader::End()
{
    ctx->SetShader(nullptr);
}

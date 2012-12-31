//
// Created by joeld on 12/30/12.
//
// To change the template use AppCode | Preferences | File Templates.
//

#include "shader.h"
#include "render_device.h"

using namespace luddite;

Shader::Shader( const eastl::string &shaderKey) :
    m_shaderKey( shaderKey ),
    m_loaded(false)
{

}

eastl::string const & Shader::shaderKey()
{
    return m_shaderKey;
}

uint32_t Shader::shaderProgram()
{
    return m_program;
}


void Shader::load( RenderDevice *device )
{
    if (m_loaded) return;

    m_program = device->loadShader(m_shaderKey);
    printf("Shader::load %s result %d\n", m_shaderKey.c_str(), m_program );

    // TODO: if m_program is SHADER_FAIL then use
    // a common error shader instead...

    m_loaded = true;

}












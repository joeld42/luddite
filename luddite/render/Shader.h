//
// Created by joeld on 12/30/12.
//

#ifndef __Shader_H_
#define __Shader_H_

#include <stdint.h>
#include <EASTL/string.h>

namespace luddite {

class RenderDevice;

class Shader
{
  public:
    Shader( const eastl::string &shaderKey );

    const eastl::string &shaderKey();
    uint32_t shaderProgram();

    void load( RenderDevice *device ); // Load shader if needed

 protected:
    eastl::string m_shaderKey;
    int32_t m_program;
    bool m_loaded;
};

} // namespace luddite

#endif //__Shader_H_

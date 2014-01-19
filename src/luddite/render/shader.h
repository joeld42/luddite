//
// Created by joeld on 12/30/12.
//

#ifndef LUDDITE_RENDER_SHADER_H
#define LUDDITE_RENDER_SHADER_H

#include <string>
#include <stdint.h>

namespace luddite {

class RenderDevice;

class Shader
{
  public:
    Shader( const std::string &shaderKey );

    const std::string &shaderKey();
    uint32_t shaderProgram();

    void load( RenderDevice *device ); // Load shader if needed

 protected:
    std::string m_shaderKey;
    int32_t m_program;
    bool m_loaded;
};

} // namespace luddite

#endif //LUDDITE_RENDER_SHADER_H

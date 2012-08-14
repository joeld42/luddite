//
//  param.h
//  luddite_ios
//
//  Created by Joel Davis on 8/13/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#ifndef luddite_param_h
#define luddite_param_h

#include <EASTL/string.h>
#include <EASTL/vector.h>

namespace luddite
{

enum
{
    ParamType_SCALAR,
    ParamType_VEC2,
    ParamType_VEC3,
    ParamType_VEC4,
    ParamType_MATRIX3,
    ParamType_MATRIX4
};

struct Param
{
    Param();
    ~Param();
    
    Param( const eastl::string &name );
    
    Param( const Param &other );
    const Param &operator= (const Param &other );
    
    eastl::string m_name;
    int m_paramType;
    union {
        float scalar;
        float *data;
    } m_val;   
    
    // Use assignment to set type and value
    const Param &operator= (float value );
    const Param &operator= (const vec2f &value );
    const Param &operator= (const vec3f &value );
    const Param &operator= (const vec4f &value );
    //    const Param &operator= (const matrix3x3f &value );
    const Param &operator= (const matrix4x4f &value );
    
};

} // namespace luddite

#endif

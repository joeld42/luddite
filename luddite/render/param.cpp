//
//  param.cpp
//  luddite_ios
//
//  Created by Joel Davis on 8/13/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#include <prmath/prmath.hpp>

#include <luddite/common/debug.h>
#include <luddite/render/param.h>

using namespace luddite;

// Returns param size in number of elements
static size_t _paramDataSize( int paramType )
{
//    DBG::Assert( paramType != ParamType_SCALAR, 
//                "Shouldn't be calling this for scalar params" );
    
    switch( paramType )
    {
        case ParamType_VEC2: return 2; break;
        case ParamType_VEC3: return 3; break;
        case ParamType_VEC4: return 4; break;
            
        case ParamType_MATRIX3: return 9; break;
        case ParamType_MATRIX4: return 16; break;            
    }
    
//    DBG::Assert( false, "Unknown paramType" );
    return 0;
}

Param::Param()
{
    m_paramType = ParamType_SCALAR;
    m_val.scalar = 0.0;
}

Param::~Param()
{
    if (m_paramType != ParamType_SCALAR)
    {
        delete [] m_val.data;
    }
}

Param::Param( const eastl::string &name ) :
    m_name(name)
{
    m_paramType = ParamType_SCALAR;
    m_val.scalar = 0.0;   
}

// TODO: maybe make a bunch of constructors for all the different types,
// but perhaps not worth doing because we probably won't be creating these
// directly

Param::Param( const Param &other )
{
    m_paramType = ParamType_SCALAR;
    (*this) = other;
}

const Param &Param::operator= (const Param &other )
{
    // Is this the same type as "other"?
    if (m_paramType == other.m_paramType)
    {
        // yeah, skip the type conversion and just copy data
        size_t paramSz = _paramDataSize( other.m_paramType );
        memcpy( m_val.data, other.m_val.data, sizeof(float)*paramSz );
    }
    else
    {        
        // Convert type. 
        if (m_paramType != ParamType_SCALAR)
        {
            delete [] m_val.data;
        }
        
        // Copy param value
        if (other.m_paramType == ParamType_SCALAR)
        {
            // Copy scalar param value
            m_paramType = ParamType_SCALAR;
            m_val.scalar = other.m_val.scalar;
        }
        else
        {
            // Copy vec or matrix param value
            size_t paramSz = _paramDataSize( other.m_paramType );
            m_paramType = other.m_paramType;
            m_val.data = new float [paramSz];
            memcpy( m_val.data, other.m_val.data, sizeof(float)*paramSz );
        }
    }
    
    return *this;
}

const Param &Param::operator= (float value )
{
    if (m_paramType != ParamType_SCALAR)
    {
        delete [] m_val.data;
    }
    m_paramType = ParamType_SCALAR;
    m_val.scalar = 0.0;    
    
    return *this;
}

const Param &Param::operator= (const vec2f &value )
{
    if (m_paramType != ParamType_SCALAR)
    {
        delete [] m_val.data;
    }
    m_paramType = ParamType_VEC2;
    m_val.data = new float[2];
    
    memcpy( m_val.data, &value, sizeof(float)*2 );
    
    return *this;
}

const Param &Param::operator= (const vec3f &value )
{
    if (m_paramType != ParamType_SCALAR)
    {
        delete [] m_val.data;
    }
    m_paramType = ParamType_VEC3;
    m_val.data = new float[3];
    
    memcpy( m_val.data, &value, sizeof(float)*3 );    
    
    return *this;
}

const Param &Param::operator= (const vec4f &value )
{
    if (m_paramType != ParamType_SCALAR)
    {
        delete [] m_val.data;
    }
    m_paramType = ParamType_VEC4;
    m_val.data = new float[4];
    
    memcpy( m_val.data, &value, sizeof(float)*4 );

    return *this;
}

//    const Param &operator= (const matrix3x3f &value );

const Param &Param::operator= (const matrix4x4f &value )
{
    if (m_paramType != ParamType_SCALAR)
    {
        delete [] m_val.data;
    }
    m_paramType = ParamType_MATRIX4;
    m_val.data = new float[16];
    
    memcpy( m_val.data, &value, sizeof(float)*4 );   
    
    return *this;
}


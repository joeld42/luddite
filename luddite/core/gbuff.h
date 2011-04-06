#ifndef GBUFF_H
#define GBUFF_H

#include <luddite/core/debug.h>

#include <EASTL/vector.h>

// A GBuff is a single chuck of graphics data, i.e. 
// one drawarray or something. The GBuff class provides
// a growable container for that.
template <typename VertexT>
class GBuff
{
public:
    // Usage for glBufferData, should be GL_STATIC_DRAW or
    // GL_DYNAMIC_DRAW
    GBuff( GLuint usage=GL_DYNAMIC_DRAW );
    ~GBuff();

    // TODO: size, capacity, etc.. 

    // Adds some new vert data and returns a pointer so
    // you can initialize it. For example:
    //
    // MyVert *tri = buff->addTriangles( 1 );
    // tri[0].p = VEC3( 0.0, 0.0, 0.0 );
    // tri[1].p = VEC3( 1.0, 0.0, 0.0 );
    // tri[2].p = VEC3( 1.0, 1.0, 0.0 );
    VertexT *addTriangles( int n=1 );
    
    // for clarity, same as addTriangles(2);
    VertexT *addQuad();

    // returns the raw vert buffer
    VertexT *data();

    // Gets a vbo ready to render

protected:
    eastl::vector<VertexT> m_vertData;

    // vbo data
    size_t m_vboSize;
    GLuint m_vbo;
};

template <typename VertexT>
GBuff<VertexT>::GBuff( GLuint usage ) : m_vboSize( 0 ), 
    m_vbo( 0 )
{
}

template <typename VertexT>
GBuff<VertexT>::~GBuff()
{
    if (m_vboSize)
    {
        glDeleteBuffers( 1, &m_vbo );        
    }    
}

template <typename VertexT>
GBuff<VertexT>::addTriangles( int n )
{
    Assert( n > 0, "Can't add zero triangles" );
    
    size_t oldSize = m_vertData.size();    
    m_vertData.resize( oldSize + (n*3) );
    
    return &(m_vertData[oldSize]);    
    
}



#endif








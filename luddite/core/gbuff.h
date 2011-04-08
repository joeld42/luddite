#ifndef GBUFF_H
#define GBUFF_H

#include <luddite/platform/gl.h>

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

    // number of verts
    size_t numVerts() const;

    // Gets a vbo ready to render
    void update();
    
    // gets the vbo
    GLuint vbo();

    // Draw our contents. The VertexT must have a
    // "bind" method
    void draw();
    

protected:
    eastl::vector<VertexT> m_vertData;

    // gl usage enum
    GLuint m_usage;    

    // vbo data
    size_t m_vboSize;
    GLuint m_vbo;
};

template <typename VertexT>
GBuff<VertexT>::GBuff( GLuint usage ) : 
    m_vboSize( 0 ), 
    m_vbo( 0 ),
    m_usage( usage )
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
size_t GBuff<VertexT>::numVerts() const
{
    return m_vertData.size();    
}


template <typename VertexT>
VertexT* GBuff<VertexT>::addTriangles( int n )
{
    Assert( n > 0, "Can't add zero triangles" );
    
    size_t oldSize = m_vertData.size();    
    m_vertData.resize( oldSize + (n*3) );
    
    return &(m_vertData[oldSize]);    
    
}

template <typename VertexT>
GLuint GBuff<VertexT>::vbo()
{
    Assert( m_vertData.size(), "Tried to get vbo from empty gbuff (did you call update()?)" );
    return m_vbo;    
}

template <typename VertexT>
void GBuff<VertexT>::draw()
{
    glBindBuffer( GL_ARRAY_BUFFER, this->vbo() );    
    VertexT::bind();
    glDrawArrays( GL_TRIANGLES, 0, numVerts() );    
}


// call this after making any changes to vertex data to
// update vbo on the card,
template <typename VertexT>
void GBuff<VertexT>::update()
{
    Assert( m_vertData.size(), "Tried to update an empty gbuff" );    

    // size we need
    size_t vboSize = sizeof( VertexT ) * m_vertData.size();


    // have we not yet allocated the vbo?
    if (m_vboSize == 0)
    {
        glGenBuffers( 1, &m_vbo );
        glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
        CHECKGL( "a" );        
        
        // create the buffer
        glBufferData( GL_ARRAY_BUFFER, vboSize, &(m_vertData[0]), m_usage );        
        m_vboSize = vboSize;   
        CHECKGL( "b" );

    } 
    else if ( vboSize > m_vboSize )
    {
        // buffer has grown, realloc it
        glBufferData( GL_ARRAY_BUFFER, vboSize, &(m_vertData[0]), m_usage );
        m_vboSize = vboSize;
        CHECKGL( "c" );
        
    }    
    else // if ( vboSize <= m_vboSize )
    {
        // buffer has shrunk, just subdata part of it
        glBufferSubData( GL_ARRAY_BUFFER, 0, vboSize, &(m_vertData[0]) );
        m_vboSize = vboSize;
        CHECKGL( "d" );        
    }

    luddite::DBG::info("vbo updated (%d)\n", m_vboSize );    
}


#endif








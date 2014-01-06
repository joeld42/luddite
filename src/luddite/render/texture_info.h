//
// Created by joeld on 2/13/13.
//
// To change the template use AppCode | Preferences | File Templates.
//



#ifndef __TextureInfo_H_
#define __TextureInfo_H_

#include <string>

namespace luddite
{

class TextureInfo
{
public:
    std::string m_filename;
    uint32_t m_width, m_height;
    uint32_t m_texId;
};

}

#endif //__TextureInfo_H_

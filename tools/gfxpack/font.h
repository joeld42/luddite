#ifndef FONTPACK_FONT_H
#define FONTPACK_FONT_H
//
//  font.h
//  luddite
//
//  Created by Joel Davis on 4/9/11.
//  Copyright 2011 Joel Davis. All rights reserved.
//

#include <string>

namespace luddite
{
    
class Font
{
public:
    Font();
    
protected:
    std::string m_fontName;
    size_t m_pointSize;
    
};
    
    
}; // namespace luddite
    
#endif
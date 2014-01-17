//
//  Shader.fsh
//  CollectGame
//
//  Created by Joel Davis on 1/14/14.
//  Copyright (c) 2014 Tapnik. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}

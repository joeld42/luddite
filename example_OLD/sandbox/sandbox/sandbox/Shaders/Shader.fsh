//
//  Shader.fsh
//  shootymech
//
//  Created by Joel Davis on 6/7/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}

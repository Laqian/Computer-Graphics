//
//  CreatTexture.hpp
//  Texture
//
//  Created by 廖芊芊 on 2020/6/15.
//  Copyright © 2020年 廖芊芊. All rights reserved.
//

#ifndef CreatTexture_hpp
#define CreatTexture_hpp

#pragma once

#include <GLUT/GLUT.h>
#include <png.h>
#include <stdarg.h>
#include <stdlib.h>

void Reshape(GLsizei w,GLsizei h);
GLuint CreateTextureFromPng(const char* filename);

#endif /* CreatTexture_hpp */

#version 410 core

#define BIAS 0.0025

void main()
{             
    gl_FragDepth = gl_FragCoord.z;
    gl_FragDepth += gl_FrontFacing ? BIAS : 0.0;
} 
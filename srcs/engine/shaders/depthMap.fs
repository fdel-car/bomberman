#version 410 core

#define BIAS 0.0015

void main()
{             
    gl_FragDepth = gl_FragCoord.z;
    gl_FragDepth += gl_FrontFacing ? BIAS : 0.0;
} 
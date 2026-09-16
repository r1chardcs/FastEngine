//
// Created by dlllibstdntc on 12.09.2026.
//

#include "BufferBuilder.h"

#include <GL/gl.h>

void BufferBuilder::IfNotFirstCallInitialize() {
    if (isFirstCall == false) {
        glBegin(mode);
        isFirstCall = true;
    }
}

BufferBuilder::BufferBuilder(INT mode) : mode(mode) {

}

BufferBuilder & BufferBuilder::Vertex(FLOAT x, FLOAT y, FLOAT z, FLOAT r, FLOAT g, FLOAT b, FLOAT a) {
    IfNotFirstCallInitialize();
    glColor4f(r, g, b, a);
    glVertex3f(x, y, z);
    return *this;
}

BufferBuilder & BufferBuilder::Flush() {
    glEnd();
    return *this;
}

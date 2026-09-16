//
// Created by dlllibstdntc on 12.09.2026.
//

#ifndef FASTENGINE_BUFFERBUILDER_H
#define FASTENGINE_BUFFERBUILDER_H
#include "Platform.h"


class BufferBuilder {
    INT mode;
    BOOL isFirstCall = false;
    void IfNotFirstCallInitialize();
public:
    BufferBuilder(INT mode);

    BufferBuilder& Vertex(FLOAT x, FLOAT y, FLOAT z, FLOAT r, FLOAT g, FLOAT b, FLOAT a);
    BufferBuilder& Flush();
};


#endif //FASTENGINE_BUFFERBUILDER_H

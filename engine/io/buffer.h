// buffer.h
// 
// The global pixel buffer. The different IO methods will use whatever pixel information is in this buffer to print.

#ifndef BUFFER_H
#define BUFFER_H

#include "../col.h"

typedef struct Buffer
{
        int res_x, res_y;
        Col **pixels;
} Buffer;

Buffer buffer_init(int res_x, int res_y);
void buffer_destroy(Buffer *buffer);

#endif // BUFFER_H
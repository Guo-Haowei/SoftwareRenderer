#pragma once
#include <glm/glm.hpp>

namespace rs {

enum ClearFlag {
    ColorBufferBit = 1 << 0,
    DepthBufferBit = 1 << 1,
};

void initialize();
void finalize();

void createCanvas(int width, int height);
void destroyCanvas();
void* getCanvasRaw();

void clearColor(int r, int g, int b, int a);
void clear(int flag);

void setVertexArray(int slot, float* data, int comp);

void drawTriangles(int count);

}  // namespace rs

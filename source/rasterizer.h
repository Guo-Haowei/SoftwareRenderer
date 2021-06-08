#pragma once

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

}  // namespace rs

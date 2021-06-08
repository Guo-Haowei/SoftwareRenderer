#include "rasterizer.h"
#include <cassert>
#include <cstdlib>

namespace rs {

struct Rasterizer {
    struct ClearColor {
        char r, g, b, a;
    };

    ClearColor clearColor = {0, 0, 0, 0};
    int width = 0;
    int height = 0;
    char* colorBuffer = nullptr;
};

static Rasterizer g_rasterizer;

void initialize() {
}

void finalize() {
    destroyCanvas();
}

void createCanvas(int width, int height) {
    destroyCanvas();

    g_rasterizer.width = width;
    g_rasterizer.height = height;
    g_rasterizer.colorBuffer = reinterpret_cast<char*>(calloc(4, width * height));
}

void destroyCanvas() {
    if (g_rasterizer.colorBuffer != nullptr) {
        assert(g_rasterizer.width > 0);
        assert(g_rasterizer.height > 0);
        free(g_rasterizer.colorBuffer);
    }
}

void* getCanvasRaw() {
    assert(g_rasterizer.colorBuffer);
    return g_rasterizer.colorBuffer;
}

void clearColor(int r, int g, int b, int a) {
    g_rasterizer.clearColor.r = r;
    g_rasterizer.clearColor.g = g;
    g_rasterizer.clearColor.b = b;
    g_rasterizer.clearColor.a = a;
}

void clear(int flag) {
    char* colorBuffer = g_rasterizer.colorBuffer;
    const int width = g_rasterizer.width;
    const int height = g_rasterizer.height;
    for (int i = 0; i < 4 * width * height; i += 4) {
        colorBuffer[i + 0] = g_rasterizer.clearColor.r;
        colorBuffer[i + 1] = g_rasterizer.clearColor.g;
        colorBuffer[i + 2] = g_rasterizer.clearColor.b;
        colorBuffer[i + 3] = g_rasterizer.clearColor.a;
    }
}

}  // namespace rs

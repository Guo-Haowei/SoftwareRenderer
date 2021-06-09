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

using glm::vec2;
using glm::vec3;
/**
 * P = uA + vB + wC (where w = 1 - u - v)
 * P = uA + vB + C - uC - vC => (C - P) + u(A - C) + v(B - C) = 0
 * uCA + vCB + PC = 0
 * [u v 1] [CAx CBx PCx] = 0
 * [u v 1] [CAy CBy PCy] = 0
 * [u v 1] is the cross product
 */
static vec3 barycentric(const vec2& a, const vec2& b, const vec2& c, const vec2& p) {
    const vec2 PC = c - p, CA = a - c, CB = b - c;
    vec3 uvw = glm::cross(vec3(CA.x, CB.x, PC.x), vec3(CA.y, CB.y, PC.y));
    /**
     * what if uvw.z is 0?
     * CA.x * CB.y = CA.y * CB.x == 0 => CA.y / CA.x == CB.y / CB.x
     * CA // CB => A, B and C are on the same line
     */
    assert(uvw.z != 0.0f);
    uvw /= uvw.z;
    uvw.z -= (uvw.x + uvw.y);
    return uvw;
}

void drawTriangle(vec2 p1, vec2 p2, vec2 p3) {
    assert(g_rasterizer.width > 0 && g_rasterizer.height > 0);

    p1 = .5f * p1 + .5f;
    p2 = .5f * p2 + .5f;
    p3 = .5f * p3 + .5f;

    const int width = g_rasterizer.width, height = g_rasterizer.height;
    const int minX = 0, minY = 0;
    const int maxX = width, maxY = height;

    const vec2 a(p1.x * width, p1.y * height);
    const vec2 b(p2.x * width, p2.y * height);
    const vec2 c(p3.x * width, p3.y * height);

    for (int y = minY; y < maxY; ++y) {
        for (int x = minX; x < maxX; ++x) {
            vec3 bCoord = barycentric(a, b, c, vec2(x, y));
            float sum = bCoord.x + bCoord.y + bCoord.z;
            // TODO: refactor
            static const float epsilon = 0.00003f;
            bool test = (bCoord.x >= 0.0f && bCoord.y >= 0.0f && bCoord.z >= 0.0f && glm::abs(sum - 1.0f) <= epsilon);
            if (test == true) {
                // depth test
                const int index = y * g_rasterizer.width + x;
                g_rasterizer.colorBuffer[4 * index + 0] = static_cast<char>(255);
                g_rasterizer.colorBuffer[4 * index + 1] = static_cast<char>(255);
                g_rasterizer.colorBuffer[4 * index + 2] = static_cast<char>(255);
                g_rasterizer.colorBuffer[4 * index + 3] = static_cast<char>(255);
            }
        }
    }
}

}  // namespace rs

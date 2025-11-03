#include "perlin.h"
#include <math.h>
#include <stdlib.h>

static int p[512];

static const int grad2[8][2] = {
	{ 1, 1}, {-1, 1}, { 1,-1}, {-1,-1},
	{ 1, 0}, {-1, 0}, { 0, 1}, { 0,-1}
};

// Fade curve function
static float fade(float t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

// Linear interpolation
static float lerp(float t, float a, float b) {
	return a + t * (b - a);
}

// Dot product with gradient
static float grad(int hash, float x, float y) {
	int h = hash & 7;
	return grad2[h][0] * x + grad2[h][1] * y;
}

void perlin_init(int seed) {
	for (int i = 0; i < 256; i++)
		p[i] = i;

	srand(seed);
	for (int i = 255; i > 0; i--) {
		int j = rand() % (i + 1);
		int t = p[i];
		p[i] = p[j];
		p[j] = t;
	}

	for (int i = 0; i < 256; i++)
		p[i] = p[i + 256] = p[i];
}

// Perlin 2D noise using static permutation table
float perlin_2d(float x, float y) {
	int xi = (int)floorf(x) & 255;
	int yi = (int)floorf(y) & 255;

	float xf = x - floorf(x);
	float yf = y - floorf(y);

	float u = fade(xf);
	float v = fade(yf);

	int aa = p[p[xi] + yi];
	int ab = p[p[xi] + yi + 1];
	int ba = p[p[xi + 1] + yi];
	int bb = p[p[xi + 1] + yi + 1];

	float x1 = lerp(u, grad(aa, xf, yf), grad(ba, xf - 1, yf));
	float x2 = lerp(u, grad(ab, xf, yf - 1), grad(bb, xf - 1, yf - 1));

	return lerp(v, x1, x2);
}

int perlin_val_to_1_10(float value) {
	// Normalise from [-1,1] to [0,1]
	float normalised = (value + 1.0f) / 2.0f;
	// Scale to [1,10]
	return (int)(normalised * 9.0f) + 1;
}

#ifdef _PERLIN_TEST
#include <stdio.h>

int main() {
	perlin_init(42);

	float scale = 0.4f;

	for (int y = 0; y < 10; y++) {
		for (int x = 0; x < 10; x++) {
			int mapped = perlin_val_to_1_10(perlin_2d(x * scale, y * scale));
			printf("%d ", mapped);
		}
		printf("\n");
	}

	return 0;
}

#endif

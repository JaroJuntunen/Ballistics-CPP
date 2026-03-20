#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#include "Environment.hpp"

static constexpr float PERLIN_NOISE_FREAGUANCY = 0.05f;
static constexpr float PERLIN_NOISE_SCALE = 15.0f;

float Envirnoment::getWorldHeight(float worldX) {
	return ((stb_perlin_noise3(worldX * PERLIN_NOISE_FREAGUANCY, 0.0, 0.0,0,0,0) * PERLIN_NOISE_SCALE));
}
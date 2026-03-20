#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#include "Environment.hpp"

float Envirnoment::getWorldHeight(float worldX) {
	return ((stb_perlin_noise3(worldX * 0.05f, 0.0, 0.0,0,0,0) *30.0f));
}
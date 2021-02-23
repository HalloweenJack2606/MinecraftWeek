#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdint>

#define RENDERER_MAX_SPRITES 2048
#define RENDERER_SPRITE_SIZE (sizeof(float) * 6)
#define RENDERER_BUFFER_SIZE (RENDERER_SPRITE_SIZE * 16 * RENDERER_MAX_SPRITES)
#define RENDERER_INDICES_SIZE (RENDERER_MAX_SPRITES * 36 * sizeof(unsigned int))

#define SHADER_VERTEX_POSITION 0
#define SHADER_UV_POSITION 1
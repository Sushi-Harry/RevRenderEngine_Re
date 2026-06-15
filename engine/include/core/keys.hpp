#pragma once
#include <cstdint>


// These macros are for keyboar inputs and everything is mapped to glfw's key codes' integer values for convenience.
// Doing this right now so that I don't have to depend on GLFW when porting this code to a different API.
// #define  REV_KEY_SPACE 32

enum class Key : uint32_t {
    REV_KEY_SPACE = 32,
    REV_KEY_0 = 48,
    REV_KEY_1 = 49,
    REV_KEY_2 = 50,
    REV_KEY_3 = 51,
    REV_KEY_4 = 52,
    REV_KEY_5 = 53,
    REV_KEY_6 = 54,
    REV_KEY_7 = 55,
    REV_KEY_8 = 56,
    REV_KEY_9 = 57,
    REV_KEY_SEMICOLON = 59,
    REV_KEY_EQUAL = 61,
    REV_KEY_A = 65,
    REV_KEY_B = 66,
    REV_KEY_C = 67,
    REV_KEY_D = 68,
    REV_KEY_E = 69,
    REV_KEY_F = 70,
    REV_KEY_G = 71,
    REV_KEY_H = 72,
    REV_KEY_I = 73,
    REV_KEY_J = 74,
    REV_KEY_K = 75,
    REV_KEY_L = 76,
    REV_KEY_M = 77,
    REV_KEY_N = 78,
    REV_KEY_O = 79,
    REV_KEY_P = 80,
    REV_KEY_Q = 81,
    REV_KEY_R = 82,
    REV_KEY_S = 83,
    REV_KEY_T = 84,
    REV_KEY_U = 85,
    REV_KEY_V = 86,
    REV_KEY_W = 87,
    REV_KEY_X = 88,
    REV_KEY_Y = 89,
    REV_KEY_Z = 90
};

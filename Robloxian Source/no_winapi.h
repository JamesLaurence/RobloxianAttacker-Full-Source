#pragma once
#define M_PI 3.14159265358979323846264338327950288
#include <Windows.h>
#include <cstdint>
#include <cmath>

inline float cos_(float x) noexcept
{
	constexpr float tp = 1. / (2. * M_PI);
	x *= tp;
	x -= float(.25) + floor(x + float(.25));
	x *= float(16.) * (abs(x) - float(.5));
	return x;
}

inline float sqrtf_(float x)
{
	union { float f; uint32_t i; } z = { x };
	z.i = 0x5f3759df - (z.i >> 1);
	z.f *= (1.5f - (x * 0.5f * z.f * z.f));
	z.i = 0x7EEEEEEE - z.i;
	return z.f;
}

double powf_(double x, int y)
{
	double temp;
	if (y == 0)
		return 1;
	temp = powf_(x, y / 2);
	if ((y % 2) == 0) {
		return temp * temp;
	}
	else {
		if (y > 0)
			return x * temp * temp;
		else
			return (temp * temp) / x;
	}
}
#include "Animation.h"

#include <math.h>

namespace EasingFunction {
	f32 linear(f32 t, f32 d) {
		return t/d;
	}

	f32 inQuad(f32 t, f32 d) {
		return pow(t/d, 2.0f);
	}

	f32 outQuad(f32 t, f32 d) {
		t /= d;
		return -t * (t-2.0f);
	}

	f32 inOutQuad(f32 t, f32 d) {
		t = t/d*2;
		if (t < 1.0f) {
			return pow(t, 2.0f)/2.0f;
		} else {
			return -((t - 1.0f) * (t - 3.0f) - 1.0f)/2.0f;
		}
	}

	f32 outInQuad(f32 t, f32 d) {
		if (t < d/2) return outQuad(t*2.0f, d)/2.0f;
		return inQuad((t*2.0f)-d, d)/2.0f+0.5f;
	}

	EasingFunc LINEAR = &linear;

	EasingFunc IN_QUAD = &inQuad;
	EasingFunc OUT_QUAD = &outQuad;
	EasingFunc IN_OUT_QUAD = &inOutQuad;
	EasingFunc OUT_IN_QUAD = &outInQuad;
}

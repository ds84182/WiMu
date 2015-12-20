#pragma once

#include <gccore.h>

#include <util/Log.h>

class Animatable {
public:
	virtual ~Animatable() {};
	virtual void update(f32 progress) = 0;
};

class AnimatableFloat : public Animatable {
public:
	f32 *dest;
	f32 start;
	f32 end;

	AnimatableFloat(f32 *d, f32 s = 0.0f, f32 e = 1.0f) :
		dest(d), start(s), end(e) {};

	virtual ~AnimatableFloat();
	virtual void update(f32 progress) {
		*dest = progress*(end-start)+start;
	}
};

class AnimatableColor : public Animatable {
public:
	u32 *dest;
	u32 start;
	u32 end;

	AnimatableColor(u32 *d, u32 s = 0x000000FF, u32 e = 0xFFFFFFFF) :
		dest(d), start(s), end(e) {};

	virtual ~AnimatableColor();
	virtual void update(f32 progress) {
		u8 sr = start>>24, sg = (start>>16)&0xFF, sb = (start>>8)&0xFF, sa = start&0xFF;
		u8 er = end>>24, eg = (end>>16)&0xFF, eb = (end>>8)&0xFF, ea = end&0xFF;
		*dest = u8(progress*(ea-sa)+sa) |
			u8(progress*(eb-sb)+sb) << 8 |
			u8(progress*(eg-sg)+sg) << 16 |
			u8(progress*(er-sr)+sr) << 24;

		DCFlushRange(dest, sizeof(u32));
	}
};

// Adapted from kikito's tween.lua which was
// adapted from EmmanuelOga's easings...
// t -> time right now
// b -> starting value (0 when porting)
// c -> delta between begining and end (1 when porting)
// d -> duration
typedef f32 (*EasingFunc)(f32 t, f32 d);

namespace EasingFunction {
	extern EasingFunc LINEAR;

	extern EasingFunc IN_QUAD;
	extern EasingFunc OUT_QUAD;
	extern EasingFunc IN_OUT_QUAD;
	extern EasingFunc OUT_IN_QUAD;

	/*extern EasingFunc IN_CUBIC;
	extern EasingFunc OUT_CUBIC;
	extern EasingFunc IN_OUT_CUBIC;
	extern EasingFunc OUT_IN_CUBIC;

	extern EasingFunc IN_QUART;
	extern EasingFunc OUT_QUART;
	extern EasingFunc IN_OUT_QUART;
	extern EasingFunc OUT_IN_QUART;

	extern EasingFunc IN_QUINT;
	extern EasingFunc OUT_QUINT;
	extern EasingFunc IN_OUT_QUINT;
	extern EasingFunc OUT_IN_QUINT;

	extern EasingFunc IN_SINE;
	extern EasingFunc OUT_SINE;
	extern EasingFunc IN_OUT_SINE;
	extern EasingFunc OUT_IN_SINE;

	extern EasingFunc IN_EXPO;
	extern EasingFunc OUT_EXPO;
	extern EasingFunc IN_OUT_EXPO;
	extern EasingFunc OUT_IN_EXPO;

	extern EasingFunc IN_CIRC;
	extern EasingFunc OUT_CIRC;
	extern EasingFunc IN_OUT_CIRC;
	extern EasingFunc OUT_IN_CIRC;

	extern EasingFunc IN_ELASTIC;
	extern EasingFunc OUT_ELASTIC;
	extern EasingFunc IN_OUT_ELASTIC;
	extern EasingFunc OUT_IN_ELASTIC;

	extern EasingFunc IN_BACK;
	extern EasingFunc OUT_BACK;
	extern EasingFunc IN_OUT_BACK;
	extern EasingFunc OUT_IN_BACK;

	extern EasingFunc IN_BOUNCE;
	extern EasingFunc OUT_BOUNCE;
	extern EasingFunc IN_OUT_BOUNCE;
	extern EasingFunc OUT_IN_BOUNCE;*/
}

class EasingState {
public:
	f32 current;
	f32 duration;
	f32 delay;
	Animatable *subject;
	EasingFunc easing;

	EasingState(Animatable *s, f32 d, EasingFunc f, f32 dl = 0.0f) :
		current(0.0f), duration(d), delay(dl), subject(s), easing(f) {}

	bool update(f32 delta) {
		current += delta;

		f32 c = current-delay;

		if (c <= 0) {
			subject->update(0);
		} else if (c >= duration) {
			c = duration;
			subject->update(1);
		} else {
			subject->update(easing(c, duration));
		}

		return c >= duration;
	}
};

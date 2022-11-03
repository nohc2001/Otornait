#pragma once
#include <math.h>

class AnimClass {
public:
	AnimClass() {

	}

	virtual ~AnimClass() {

	}

	//slow->fast
	static float EaseIn(const float& trate, const float& power) {
		float f = powf(trate, power);
		return f;
		/*if (0 <= f && f <= 1) {
			return f;
		}
		else if (f < 0) {
			return 0;
		}
		else {
			return 1;
		}*/
	}

	//fast->slow
	static float EaseOut(const float& trate, const float& power) {
		return (1 - powf(1 - trate, power));
	}

	//slow->fast->slow
	static float EaseIO(const float& trate, const float& power) {
		if (trate < 0.5) {
			return 0.5f * EaseIn(trate * 2, power);
		}
		else {
			return 0.5f + 0.5f * EaseOut(2 * (trate - 0.5f), power);
		}
	}
};


#pragma once

class Light {

public:

	static const unsigned short DIRECTIONAL_LIGHT = 0;
	static const unsigned short POINT_LIGHT = 1;
	static const unsigned short SPOT_LIGHT = 2;

	struct LightData {
		
		struct {
			float x{};
			float y{};
			float z{};
			float w{};
		} pos;
		struct {
			float x{};
			float y{};
			float z{};
			float w{};
		} direction;
		struct {
			float r{};
			float g{};
			float b{};
			float a{};
		} color;

		float spotAngle{};
		float constAtten{};
		float linAtten{};
		float quadAtten{};

		int type{};
		int isEnabled{ false };
		int padding0{};
		int padding1{};

	};

};
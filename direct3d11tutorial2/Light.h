#pragma once

class Light {

public:

	static const unsigned short DIRECTIONAL_LIGHT = 0;
	static const unsigned short POINT_LIGHT = 1;
	static const unsigned short SPOT_LIGHT = 2;

	struct LightData {
		
		struct {
			float x{ 0.0f };
			float y{ 0.0f };
			float z{ 0.0f };
			float w{ 1.0f };
		} pos;
		struct {
			float x{ 1.0f };
			float y{ 1.0f };
			float z{ 1.0f };
			float w{ 1.0f };
		} direction;
		struct {
			float r{ 1.0f };
			float g{ 1.0f };
			float b{ 1.0f };
			float a{ 1.0f };
		} color;

		float spotAngle{ 15.0f };
		float constAtten{ 1.0f };
		float linAtten{ 0.08f };
		float quadAtten{ 0.0f };

		int type{ POINT_LIGHT };
		int isEnabled{ false };
		int padding0{};
		int padding1{};

	};

};
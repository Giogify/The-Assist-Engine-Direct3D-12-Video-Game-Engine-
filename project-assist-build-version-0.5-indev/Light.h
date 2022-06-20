#pragma once

class Light {

public:
	
	static const UINT8 MAX_LIGHTS = 8;
	static const UINT8 DIRECTIONAL_LIGHT = 0;
	static const UINT8 POINT_LIGHT = 1;
	static const UINT8 SPOT_LIGHT = 2;

	struct LightData {
		
		struct {
			double x{ 0.0f };
			double y{ 0.0f };
			double z{ 0.0f };
			double w{ 1.0f };
		} pos;
		struct {
			double x{ 1.0f };
			double y{ 1.0f };
			double z{ 1.0f };
			double w{ 1.0f };
		} direction;
		struct {
			double r{ 1.0f };
			double g{ 1.0f };
			double b{ 1.0f };
			double a{ 1.0f };
		} color;

		double spotAngle{ 15.0f };
		double constAtten{ 1.0f };
		double linAtten{ 0.08f };
		double quadAtten{ 0.0f };

		int type{ POINT_LIGHT };
		int isEnabled{ false };
		int padding0{};
		int padding1{};

	};

};
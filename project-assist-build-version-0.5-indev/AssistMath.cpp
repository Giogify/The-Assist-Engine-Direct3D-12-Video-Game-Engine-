#include "AssistMath.h"

using namespace AssistMath;

double AssistMath::AMConvertToRadians(double deg) noexcept { return deg * (AM_PI / 180.0); }
double AssistMath::AMConvertToDegrees(double rad) noexcept { return rad * (180.0 / AM_PI); }

// Load
AMVECTOR AssistMath::AMLoadDouble2(const AMDOUBLE2& src) noexcept {
	return { _mm256_set_pd(src.x, src.y, 0.0, 0.0) };
}
AMVECTOR AssistMath::AMLoadDouble3(const AMDOUBLE3& src) noexcept {
	return { _mm256_set_pd(src.x, src.y, src.z, 0.0) };
}
AMVECTOR AssistMath::AMLoadDouble4(const AMDOUBLE4& src) noexcept {
	return { _mm256_set_pd(src.x, src.y, src.z, src.w) };
}
AMMATRIX AssistMath::AMLoadDouble3x3(const AMDOUBLE3X3& src) noexcept {
	return { {
		{ _mm256_set_pd(src._11, src._12, src._13, 0.0) },
		{ _mm256_set_pd(src._21, src._22, src._23, 0.0) },
		{ _mm256_set_pd(src._31, src._32, src._33, 0.0) },
		{ _mm256_set_pd(0.0,     0.0,     0.0,     0.0) }
	} };
}
AMMATRIX AssistMath::AMLoadDouble4x3(const AMDOUBLE4X3& src) noexcept {
	return { {
		{ _mm256_set_pd(src._11, src._12, src._13, 0.0) },
		{ _mm256_set_pd(src._21, src._22, src._23, 0.0) },
		{ _mm256_set_pd(src._31, src._32, src._33, 0.0) },
		{ _mm256_set_pd(src._41, src._41, src._41, 0.0) }
	} };
}
AMMATRIX AssistMath::AMLoadDouble3x4(const AMDOUBLE3X4& src) noexcept {
	return { {
		{ _mm256_set_pd(src._11, src._12, src._13, src._14) },
		{ _mm256_set_pd(src._21, src._22, src._23, src._24) },
		{ _mm256_set_pd(src._31, src._32, src._33, src._34) },
		{ _mm256_set_pd(0.0,     0.0,     0.0,     0.0) }
	} };
}
AMMATRIX AssistMath::AMLoadDouble4x4(const AMDOUBLE4X4& src) noexcept {
	return { {
		{ _mm256_set_pd(src._14, src._13, src._12, src._11) },
		{ _mm256_set_pd(src._24, src._23, src._22, src._21) },
		{ _mm256_set_pd(src._34, src._33, src._32, src._31) },
		{ _mm256_set_pd(src._44, src._43, src._42, src._41) }
	} };
}

// Store (ref)
void AssistMath::AMStoreDouble2(AMDOUBLE2& dst, AMVECTOR& V) noexcept {
	dst.x = V.m256d_f64[0];
	dst.y = V.m256d_f64[1];
}
void AssistMath::AMStoreDouble3(AMDOUBLE3& dst, AMVECTOR& V) noexcept {
	dst.x = V.m256d_f64[0];
	dst.y = V.m256d_f64[1];
	dst.z = V.m256d_f64[2];
}
void AssistMath::AMStoreDouble4(AMDOUBLE4& dst, AMVECTOR& V) noexcept {
	/*dst.x = V.m256d_f64[0];
	dst.y = V.m256d_f64[1];
	dst.z = V.m256d_f64[2];
	dst.w = V.m256d_f64[3];*/
	_mm256_store_pd((double*)&dst, V);
}
void AssistMath::AMStoreDouble3x3(AMDOUBLE3X3& dst, AMMATRIX& M) noexcept {
	double m[4][4]{};
	_mm256_store_pd(m[0], M.m[0]);
	for (uint8_t i = 0; i < 3; i++) {
		for (uint8_t j = 0; j < 3; j++) {
			dst.m[i][j] = m[i][j];
		}
	}
}
void AssistMath::AMStoreDouble4x3(AMDOUBLE4X3& dst, AMMATRIX& M) noexcept {
	double m[4][4]{};
	_mm256_store_pd(m[0], M.m[0]);
	for (uint8_t i = 0; i < 4; i++) {
		for (uint8_t j = 0; j < 3; j++) {
			dst.m[i][j] = m[i][j];
		}
	}
}
void AssistMath::AMStoreDouble3x4(AMDOUBLE3X4& dst, AMMATRIX& M) noexcept {
	double m[4][4]{};
	_mm256_store_pd(m[0], M.m[0]);
	for (uint8_t i = 0; i < 3; i++) {
		for (uint8_t j = 0; j < 4; j++) {
			dst.m[i][j] = m[i][j];
		}
	}
}
void AssistMath::AMStoreDouble4x4(AMDOUBLE4X4& dst, AMMATRIX& M) noexcept {
	double m[4][4]{};
	_mm256_store_pd(m[0], M.m[0]);
	for (uint8_t i = 0; i < 4; i++) {
		for (uint8_t j = 0; j < 4; j++) {
			dst.m[i][j] = m[i][j];
		}
	}
}

// Store (new)
AMDOUBLE2 AssistMath::AMStoreDouble2(AMVECTOR& V) noexcept {
	return { V.m256d_f64[0], V.m256d_f64[1] };
}
AMDOUBLE3 AssistMath::AMStoreDouble3(AMVECTOR& V) noexcept {
	return { V.m256d_f64[0], V.m256d_f64[1], V.m256d_f64[2] };
}
AMDOUBLE4 AssistMath::AMStoreDouble4(AMVECTOR& V) noexcept {
	return { V.m256d_f64[0], V.m256d_f64[1], V.m256d_f64[2], V.m256d_f64[3] };
}
AMDOUBLE3X3 AssistMath::AMStoreDouble3x3(AMMATRIX& M) noexcept {
	double m[4][4]{};
	_mm256_store_pd(m[0], M.m[0]);
	return {
		m[0][0], m[0][1], m[0][2],
		m[1][0], m[1][1], m[1][2],
		m[2][0], m[2][1], m[2][2]
	};
}
AMDOUBLE4X3 AssistMath::AMStoreDouble4x3(AMMATRIX& M) noexcept {
	double m[4][4]{};
	_mm256_store_pd(m[0], M.m[0]);
	return {
		m[0][0], m[0][1], m[0][2],
		m[1][0], m[1][1], m[1][2],
		m[2][0], m[2][1], m[2][2],
		m[3][0], m[3][1], m[3][2]
	};
}
AMDOUBLE3X4 AssistMath::AMStoreDouble3x4(AMMATRIX& M) noexcept {
	double m[4][4]{};
	_mm256_store_pd(m[0], M.m[0]);
	return {
		m[0][0], m[0][1], m[0][2], m[0][3],
		m[1][0], m[1][1], m[1][2], m[1][3],
		m[2][0], m[2][1], m[2][2], m[2][3]
	};
}
AMDOUBLE4X4 AssistMath::AMStoreDouble4x4(AMMATRIX& M) noexcept {
	double m[4][4]{};
	_mm256_store_pd(m[0], M.m[0]);
	return {
		m[0][0], m[0][1], m[0][2], m[0][3],
		m[1][0], m[1][1], m[1][2], m[1][3],
		m[2][0], m[2][1], m[2][2], m[2][3],
		m[3][0], m[3][1], m[3][2], m[3][3]
	};
}

AMMATRIX AssistMath::AMMatrixIdentity() {
	return {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};
}
AMMATRIX AssistMath::AMMatrixRotationRollPitchYaw(double pitch, double yaw, double roll) {

	AMVECTOR vec{ AMLoadDouble3({ pitch, yaw, roll }) };

	AMVECTOR sinvec{ _mm256_sin_pd(vec) };
	AMVECTOR cosvec{ _mm256_cos_pd(vec) };

	AMMATRIX yRot{
		cosvec.m256d_f64[1],    0.0,    sinvec.m256d_f64[1],    0.0,
		0.0,                    1.0,    0.0,                    0.0,
		-sinvec.m256d_f64[1],   0.0,    cosvec.m256d_f64[1],    0.0,
		0.0,                    0.0,    0.0,                    1.0
	};
	AMMATRIX xRot{
		1.0,    0.0,                    0.0,                    0.0,
		0.0,    cosvec.m256d_f64[0],    -sinvec.m256d_f64[0],   0.0,
		0.0,    sinvec.m256d_f64[0],    cosvec.m256d_f64[0],    0.0,
		0.0,    0.0,                    0.0,					1.0
	};
	AMMATRIX zRot{
		cosvec.m256d_f64[2],	-sinvec.m256d_f64[2],	0.0,	0.0,
		sinvec.m256d_f64[2],    cosvec.m256d_f64[2],    0.0,    0.0,
		0.0,					0.0,					1.0,    0.0,
		0.0,					0.0,					0.0,    1.0
	};

	return { yRot * xRot * zRot };
}
AMMATRIX AssistMath::AMMatrixTranslation(double x, double y, double z) {
	return {
		1.0, 0.0, 0.0, x,
		0.0, 1.0, 0.0, y,
		0.0, 0.0, 1.0, z,
		0.0, 0.0, 0.0, 1.0
	};
}
AMVECTOR AssistMath::AMMatrixDeterminant(AMMATRIX& mx) {

	/*double a{ mx.m[0].m256d_f64[0] }; double b{ mx.m[0].m256d_f64[1] }; double c{ mx.m[0].m256d_f64[2] }; double d{ mx.m[0].m256d_f64[3] };
	double e{ mx.m[1].m256d_f64[0] }; double f{ mx.m[1].m256d_f64[1] }; double g{ mx.m[1].m256d_f64[2] }; double h{ mx.m[1].m256d_f64[3] };
	double i{ mx.m[2].m256d_f64[0] }; double j{ mx.m[2].m256d_f64[1] }; double k{ mx.m[2].m256d_f64[2] }; double l{ mx.m[2].m256d_f64[3] };
	double m{ mx.m[3].m256d_f64[0] }; double n{ mx.m[3].m256d_f64[1] }; double o{ mx.m[3].m256d_f64[2] }; double p{ mx.m[3].m256d_f64[3] };*/
	/*double _0 = a / a;
	double _1 = b / a;
	double _2 = c / a;
	double _3 = d / a;

	double _4 = _0 * (e / _0);
	double _5 = _1 * (e / _0);
	double _6 = _2 * (e / _0);
	double _7 = _3 * (e / _0);

	double _8 = e - _4;
	double _9 = f - _5;
	double _10 = g - _6;
	double _11 = h - _7;

	double _12 = a * (i / a);
	double _13 = b * (i / a);
	double _14 = c * (i / a);
	double _15 = d * (i / a);

	double _16 = i - _12;
	double _17 = j - _13;
	double _18 = k - _14;
	double _19 = l - _15;

	double _20 = a * (m / a);
	double _21 = b * (m / a);
	double _22 = c * (m / a);
	double _23 = d * (m / a);

	double _24 = m - _20;
	double _25 = n - _21;
	double _26 = o - _22;
	double _27 = p - _23;

	double _28 = _8 / _9;
	double _29 = _9 / _9;
	double _30 = _10 / _9;
	double _31 = _11 / _9;

	double _32 = _28 * _17;
	double _33 = _29 * _17;
	double _34 = _30 * _17;
	double _35 = _31 * _17;

	double _36 = _16 - _32;
	double _37 = _17 - _33;
	double _38 = _18 - _34;
	double _39 = _19 - _35;

	double _40 = _28 * _25;
	double _41 = _29 * _25;
	double _42 = _30 * _25;
	double _43 = _31 * _25;

	double _44 = _24 - _40;
	double _45 = _25 - _41;
	double _46 = _26 - _42;
	double _47 = _27 - _43;

	double _48 = _36 / _38;
	double _49 = _37 / _38;
	double _50 = _38 / _38;
	double _51 = _39 / _38;

	double _52 = _48 * _46;
	double _53 = _49 * _46;
	double _54 = _50 * _46;
	double _55 = _51 * _46;

	double _56 = _44 - _52;
	double _57 = _45 - _53;
	double _58 = _46 - _54;
	double _59 = _47 - _55;

	return {AMLoadDouble4({a, _9, _38, _59})};*/
	AMVECTOR _0{ _mm256_div_pd(mx.m[0], _mm256_set1_pd(mx.m[0].m256d_f64[0])) };
	AMVECTOR _1{ _mm256_mul_pd(_0, _mm256_set1_pd(mx.m[1].m256d_f64[0] / _0.m256d_f64[0])) };
	AMVECTOR _2{ _mm256_sub_pd(mx.m[1], _1) };
	AMVECTOR _3{ _mm256_mul_pd(mx.m[0], _mm256_set1_pd(mx.m[2].m256d_f64[0] / mx.m[0].m256d_f64[0])) };
	AMVECTOR _4{ _mm256_sub_pd(mx.m[2], _3) };
	AMVECTOR _5{ _mm256_mul_pd(mx.m[0], _mm256_set1_pd(mx.m[3].m256d_f64[0] / mx.m[0].m256d_f64[0])) };
	AMVECTOR _6{ _mm256_sub_pd(mx.m[3], _5) };
	AMVECTOR _7{ _mm256_div_pd(_2, _mm256_set1_pd(_2.m256d_f64[1])) };
	AMVECTOR _8{ _mm256_mul_pd(_7, _mm256_set1_pd(_4.m256d_f64[1])) };
	AMVECTOR _9{ _mm256_sub_pd(_4, _8) };
	AMVECTOR _10{ _mm256_mul_pd(_7, _mm256_set1_pd(_6.m256d_f64[1])) };
	AMVECTOR _11{ _mm256_sub_pd(_6, _10) };
	AMVECTOR _12{ _mm256_div_pd(_9, _mm256_set1_pd(_9.m256d_f64[2])) };
	AMVECTOR _13{ _mm256_mul_pd(_12, _mm256_set1_pd(_11.m256d_f64[2])) };
	AMVECTOR _14{ _mm256_sub_pd(_11, _13) };

	return { AMLoadDouble4({mx.m[0].m256d_f64[0], _2.m256d_f64[1], _9.m256d_f64[2], _14.m256d_f64[3]}) };

	/*f * (k * p - l * o) - g * (j * p - l * n) + h * (j * o - k * n);
	e * (k * p - l * o) - g * (i * p - l * m) + h * (i * o - k * m);
	e * (j * p - l * n) - f * (i * p - l * m) + h * (i * n - j * m);
	e * (j * o - k * n) - f * (i * o - k * m) + g * (i * n - j * m);*/
	/*auto _a = a * (f * (k * p - l * o) - g * (j * p - l * n) + h * (j * o - k * n));
	auto _b = b * (e * (k * p - l * o) - g * (i * p - l * m) + h * (i * o - k * m));
	auto _c = c * (e * (j * p - l * n) - f * (i * p - l * m) + h * (i * n - j * m));
	auto _d = d * (e * (j * o - k * n) - f * (i * o - k * m) + g * (i * n - j * m));

	return AMLoadDouble4({ _a, _b, _c, _d });*/
	/*AMVECTOR _0{ AMLoadDouble4({a, b, c, d}) };

	AMVECTOR _1{ AMLoadDouble4({f, e, e, e}) };
	AMVECTOR _2{ AMLoadDouble4({g, g, f, f}) };
	AMVECTOR _3{ AMLoadDouble4({h, h, h, g}) };

	AMVECTOR _4{ AMLoadDouble4({k, k, j, j}) };
	AMVECTOR _5{ AMLoadDouble4({p, p, p, o}) };
	AMVECTOR _6{ AMLoadDouble4({l, l, l, k}) };
	AMVECTOR _7{ AMLoadDouble4({o, o, n, n}) };
	AMVECTOR _8{ AMLoadDouble4({j, i, i, i}) };
	AMVECTOR _9{ AMLoadDouble4({n, m, m, m}) };

	AMVECTOR op1 = _mm256_mul_pd(_4, _5);
	AMVECTOR op2 = _mm256_mul_pd(_6, _7);
	AMVECTOR op3 = _mm256_mul_pd(_8, _5);
	AMVECTOR op4 = _mm256_mul_pd(_6, _9);
	AMVECTOR op5 = _mm256_mul_pd(_8, _7);
	AMVECTOR op6 = _mm256_mul_pd(_4, _9);

	AMVECTOR op7 = _mm256_sub_pd(op1, op2);
	AMVECTOR op8 = _mm256_sub_pd(op3, op4);
	AMVECTOR op9 = _mm256_sub_pd(op5, op6);

	AMVECTOR op10 = _mm256_mul_pd(_1, op7);
	AMVECTOR op11 = _mm256_mul_pd(_2, op8);
	AMVECTOR op12 = _mm256_mul_pd(_3, op9);

	AMVECTOR op13 = _mm256_sub_pd(op10, op11);
	AMVECTOR op14 = _mm256_add_pd(op13, op12);

	return _mm256_mul_pd(_0, op14);*/
	/*return _mm256_mul_pd(AMLoadDouble4({ mx.m[0].m256d_f64[0], mx.m[0].m256d_f64[1], mx.m[0].m256d_f64[2], mx.m[0].m256d_f64[3] }),
		_mm256_add_pd(_mm256_sub_pd(
			_mm256_mul_pd(AMLoadDouble4({ mx.m[1].m256d_f64[1], mx.m[1].m256d_f64[0], mx.m[1].m256d_f64[0], mx.m[1].m256d_f64[0] }),
			_mm256_sub_pd(
				_mm256_mul_pd(AMLoadDouble4({ mx.m[2].m256d_f64[2], mx.m[2].m256d_f64[2], mx.m[2].m256d_f64[1], mx.m[2].m256d_f64[1] }),
					AMLoadDouble4({ mx.m[3].m256d_f64[3], mx.m[3].m256d_f64[3], mx.m[3].m256d_f64[3], mx.m[3].m256d_f64[2] })),
			_mm256_mul_pd(AMLoadDouble4({ mx.m[2].m256d_f64[3], mx.m[2].m256d_f64[3], mx.m[2].m256d_f64[3], mx.m[2].m256d_f64[2] }),
				AMLoadDouble4({ mx.m[3].m256d_f64[2], mx.m[3].m256d_f64[2], mx.m[3].m256d_f64[1], mx.m[3].m256d_f64[1] })))),
			_mm256_mul_pd(AMLoadDouble4({ mx.m[1].m256d_f64[2], mx.m[1].m256d_f64[2], mx.m[1].m256d_f64[1], mx.m[1].m256d_f64[1] }),
				_mm256_sub_pd(_mm256_mul_pd(AMLoadDouble4({ mx.m[2].m256d_f64[1], mx.m[2].m256d_f64[0], mx.m[2].m256d_f64[0], mx.m[2].m256d_f64[0] }),
					AMLoadDouble4({ mx.m[3].m256d_f64[3], mx.m[3].m256d_f64[3], mx.m[3].m256d_f64[3], mx.m[3].m256d_f64[2] })),
					_mm256_mul_pd(AMLoadDouble4({ mx.m[2].m256d_f64[3], mx.m[2].m256d_f64[3], mx.m[2].m256d_f64[3], mx.m[2].m256d_f64[2] }),
						AMLoadDouble4({ mx.m[3].m256d_f64[1], mx.m[3].m256d_f64[0], mx.m[3].m256d_f64[0], mx.m[3].m256d_f64[0] }))))),
			_mm256_mul_pd(AMLoadDouble4({ mx.m[1].m256d_f64[3], mx.m[1].m256d_f64[3], mx.m[1].m256d_f64[3], mx.m[1].m256d_f64[2] }),
				_mm256_sub_pd(_mm256_mul_pd(AMLoadDouble4({ mx.m[2].m256d_f64[1], mx.m[2].m256d_f64[0], mx.m[2].m256d_f64[0], mx.m[2].m256d_f64[0] }),
					AMLoadDouble4({ mx.m[3].m256d_f64[2], mx.m[3].m256d_f64[2], mx.m[3].m256d_f64[1], mx.m[3].m256d_f64[1] })),
					_mm256_mul_pd(AMLoadDouble4({ mx.m[2].m256d_f64[2], mx.m[2].m256d_f64[2], mx.m[2].m256d_f64[1], mx.m[2].m256d_f64[1] }),
						AMLoadDouble4({ mx.m[3].m256d_f64[1], mx.m[3].m256d_f64[0], mx.m[3].m256d_f64[0], mx.m[3].m256d_f64[0] }))))));*/
}
AMMATRIX AssistMath::AMMatrixPerspectiveFovLH(double fov, double ar, double n, double f) {
	double yScale = 1.0 / std::tan(fov / 2);
	double xScale = yScale / ar;
	double dz = n - f;
	return {
		xScale,		0.0,		0.0,		0.0,
		0.0,		yScale,		0.0,		0.0,
		0.0,		0.0,		(f + n) / dz,	-1.0,
		0.0,		0.0,		2 * f * n / dz,	0.0
	};
}
AMMATRIX AssistMath::AMMatrixLookAtLH(AMVECTOR& eye, AMVECTOR& focus, AMVECTOR upDirection) {
	return { AMMatrixIdentity() };
}
AMMATRIX AssistMath::AMMatrixLookToLH(AMVECTOR& eye, AMVECTOR& direction, AMVECTOR upDirection) {
	return { AMMatrixIdentity() };
}
AMVECTOR AssistMath::AMVector4Transform(AMVECTOR& vec, AMMATRIX& mx) {
	AMVECTOR res{};

	AMMATRIX m2rot{
		mx.m[0].m256d_f64[0], mx.m[1].m256d_f64[0], mx.m[2].m256d_f64[0], mx.m[3].m256d_f64[0],
		mx.m[0].m256d_f64[1], mx.m[1].m256d_f64[1], mx.m[2].m256d_f64[1], mx.m[3].m256d_f64[1],
		mx.m[0].m256d_f64[2], mx.m[1].m256d_f64[2], mx.m[2].m256d_f64[2], mx.m[3].m256d_f64[2],
		mx.m[0].m256d_f64[3], mx.m[1].m256d_f64[3], mx.m[2].m256d_f64[3], mx.m[3].m256d_f64[3]
	};

	std::array<AMVECTOR, 4u> a{ m2rot.m[0], m2rot.m[1], m2rot.m[2], m2rot.m[3] };

	AMVECTOR mulRes{};

	for (int i = 0; i < 4; i++) {
		mulRes = _mm256_mul_pd(vec, a[i]);
		for (int j = 0; j < 4; j++) {
			res.m256d_f64[i] += mulRes.m256d_f64[j];
		}
	}
	return res;
}
AMMATRIX AssistMath::AMMatrixTranspose(AMMATRIX& mx) {
	return {
		mx.m[0].m256d_f64[0], mx.m[1].m256d_f64[0], mx.m[2].m256d_f64[0], mx.m[3].m256d_f64[0],
		mx.m[0].m256d_f64[1], mx.m[1].m256d_f64[1], mx.m[2].m256d_f64[1], mx.m[3].m256d_f64[1],
		mx.m[0].m256d_f64[2], mx.m[1].m256d_f64[2], mx.m[2].m256d_f64[2], mx.m[3].m256d_f64[2],
		mx.m[0].m256d_f64[3], mx.m[1].m256d_f64[3], mx.m[2].m256d_f64[3], mx.m[3].m256d_f64[3]
	};
}
AMMATRIX AssistMath::AMMatrixInverse(AMVECTOR& det, AMMATRIX& mx) {
	AMMATRIX mxI{ AMMatrixIdentity() };

	if (det.m256d_f64[0] * det.m256d_f64[1] * det.m256d_f64[2] * det.m256d_f64[3] == 0.0) {
		return AMMatrixIdentity();
	}

	double a1{ mx.m[0].m256d_f64[0] }; double b1{ mx.m[0].m256d_f64[1] }; double c1{ mx.m[0].m256d_f64[2] }; double d1{ mx.m[0].m256d_f64[3] };
	double e1{ mx.m[1].m256d_f64[0] }; double f1{ mx.m[1].m256d_f64[1] }; double g1{ mx.m[1].m256d_f64[2] }; double h1{ mx.m[1].m256d_f64[3] };
	double i1{ mx.m[2].m256d_f64[0] }; double j1{ mx.m[2].m256d_f64[1] }; double k1{ mx.m[2].m256d_f64[2] }; double l1{ mx.m[2].m256d_f64[3] };
	double m1{ mx.m[3].m256d_f64[0] }; double n1{ mx.m[3].m256d_f64[1] }; double o1{ mx.m[3].m256d_f64[2] }; double p1{ mx.m[3].m256d_f64[3] };

	AMVECTOR _0{ _mm256_div_pd(mx.m[0], _mm256_set1_pd(mx.m[0].m256d_f64[0])) };
	AMVECTOR _1{ _mm256_div_pd(mxI.m[0], _mm256_set1_pd(mx.m[0].m256d_f64[0])) };
	AMVECTOR _2{ _mm256_mul_pd(_0, _mm256_set1_pd(mx.m[1].m256d_f64[0])) };
	AMVECTOR _3{ _mm256_mul_pd(_1, _mm256_set1_pd(mx.m[1].m256d_f64[0])) };
	AMVECTOR _4{ _mm256_sub_pd(mx.m[1], _2) };
	AMVECTOR _5{ _mm256_sub_pd(mxI.m[1], _3) };
	AMVECTOR _6{ _mm256_mul_pd(_0, _mm256_set1_pd(mx.m[2].m256d_f64[0])) };
	AMVECTOR _7{ _mm256_mul_pd(_1, _mm256_set1_pd(mx.m[2].m256d_f64[0])) };
	AMVECTOR _8{ _mm256_sub_pd(mx.m[2], _6) };
	AMVECTOR _9{ _mm256_sub_pd(mxI.m[2], _7) };
	AMVECTOR _10{ _mm256_mul_pd(_0, _mm256_set1_pd(mx.m[3].m256d_f64[0])) };
	AMVECTOR _11{ _mm256_mul_pd(_1, _mm256_set1_pd(mx.m[3].m256d_f64[0])) };
	AMVECTOR _12{ _mm256_sub_pd(mx.m[3], _10) };
	AMVECTOR _13{ _mm256_sub_pd(mxI.m[3], _11) };
	AMVECTOR _14{ _mm256_div_pd(_4, _mm256_set1_pd(_4.m256d_f64[1])) };
	AMVECTOR _15{ _mm256_div_pd(_5, _mm256_set1_pd(_4.m256d_f64[1])) };
	AMVECTOR _16{ _mm256_mul_pd(_14, _mm256_set1_pd(_0.m256d_f64[1])) };
	AMVECTOR _17{ _mm256_mul_pd(_15, _mm256_set1_pd(_0.m256d_f64[1])) };
	AMVECTOR _18{ _mm256_sub_pd(_0, _16) };
	AMVECTOR _19{ _mm256_sub_pd(_1, _17) };
	AMVECTOR _20{ _mm256_mul_pd(_14, _mm256_set1_pd(_8.m256d_f64[1])) };
	AMVECTOR _21{ _mm256_mul_pd(_15, _mm256_set1_pd(_8.m256d_f64[1])) };
	AMVECTOR _22{ _mm256_sub_pd(_8, _20) };
	AMVECTOR _23{ _mm256_sub_pd(_9, _21) };
	AMVECTOR _24{ _mm256_mul_pd(_14, _mm256_set1_pd(_12.m256d_f64[1])) };
	AMVECTOR _25{ _mm256_mul_pd(_15, _mm256_set1_pd(_12.m256d_f64[1])) };
	AMVECTOR _26{ _mm256_sub_pd(_12, _24) };
	AMVECTOR _27{ _mm256_sub_pd(_13, _25) };
	AMVECTOR _28{ _mm256_div_pd(_22, _mm256_set1_pd(_22.m256d_f64[2])) };
	AMVECTOR _29{ _mm256_div_pd(_23, _mm256_set1_pd(_22.m256d_f64[2])) };
	AMVECTOR _30{ _mm256_mul_pd(_28, _mm256_set1_pd(_18.m256d_f64[2])) };
	AMVECTOR _31{ _mm256_mul_pd(_29, _mm256_set1_pd(_18.m256d_f64[2])) };
	AMVECTOR _32{ _mm256_sub_pd(_18, _30) };
	AMVECTOR _33{ _mm256_sub_pd(_19, _31) };
	AMVECTOR _34{ _mm256_mul_pd(_28, _mm256_set1_pd(_14.m256d_f64[2])) };
	AMVECTOR _35{ _mm256_mul_pd(_29, _mm256_set1_pd(_14.m256d_f64[2])) };
	AMVECTOR _36{ _mm256_sub_pd(_14, _34) };
	AMVECTOR _37{ _mm256_sub_pd(_15, _35) };
	AMVECTOR _38{ _mm256_mul_pd(_28, _mm256_set1_pd(_26.m256d_f64[2])) };
	AMVECTOR _39{ _mm256_mul_pd(_29, _mm256_set1_pd(_26.m256d_f64[2])) };
	AMVECTOR _40{ _mm256_sub_pd(_26, _38) };
	AMVECTOR _41{ _mm256_sub_pd(_27, _39) };
	AMVECTOR _42{ _mm256_div_pd(_40, _mm256_set1_pd(_40.m256d_f64[3])) };
	AMVECTOR _43{ _mm256_div_pd(_41, _mm256_set1_pd(_40.m256d_f64[3])) };
	AMVECTOR _44{ _mm256_mul_pd(_42, _mm256_set1_pd(_32.m256d_f64[3])) };
	AMVECTOR _45{ _mm256_mul_pd(_43, _mm256_set1_pd(_32.m256d_f64[3])) };
	AMVECTOR _46{ _mm256_sub_pd(_32, _44) };
	AMVECTOR _47{ _mm256_sub_pd(_33, _45) };
	AMVECTOR _48{ _mm256_mul_pd(_42, _mm256_set1_pd(_36.m256d_f64[3])) };
	AMVECTOR _49{ _mm256_mul_pd(_43, _mm256_set1_pd(_36.m256d_f64[3])) };
	AMVECTOR _50{ _mm256_sub_pd(_36, _48) };
	AMVECTOR _51{ _mm256_sub_pd(_37, _49) };
	AMVECTOR _52{ _mm256_mul_pd(_42, _mm256_set1_pd(_28.m256d_f64[3])) };
	AMVECTOR _53{ _mm256_mul_pd(_43, _mm256_set1_pd(_28.m256d_f64[3])) };
	AMVECTOR _54{ _mm256_sub_pd(_28, _52) };
	AMVECTOR _55{ _mm256_sub_pd(_29, _53) };

	return { _47, _51, _55, _43 };
	/*
	double _0 = a1 / a1;
	double _1 = b1 / a1;
	double _2 = c1 / a1;
	double _3 = d1 / a1;

	double _4 = 1.0 / a1;
	double _5 = 0.0 / a1;
	double _6 = 0.0 / a1;
	double _7 = 0.0 / a1;

	double _8 = _0 * e1;
	double _9 = _1 * e1;
	double _10 = _2 * e1;
	double _11 = _3 * e1;

	double _12 = _4 * e1;
	double _13 = _5 * e1;
	double _14 = _6 * e1;
	double _15 = _7 * e1;

	double _16 = e1 - _8;
	double _17 = f1 - _9;
	double _18 = g1 - _10;
	double _19 = h1 - _11;

	double _20 = 0.0 - _12;
	double _21 = 1.0 - _13;
	double _22 = 0.0 - _14;
	double _23 = 0.0 - _15;

	double _24 = _0 * i1;
	double _25 = _1 * i1;
	double _26 = _2 * i1;
	double _27 = _3 * i1;

	double _28 = _4 * i1;
	double _29 = _5 * i1;
	double _30 = _6 * i1;
	double _31 = _7 * i1;

	double _32 = i1 - _24;
	double _33 = j1 - _25;
	double _34 = k1 - _26;
	double _35 = l1 - _27;

	double _36 = 0.0 - _28;
	double _37 = 0.0 - _29;
	double _38 = 1.0 - _30;
	double _39 = 0.0 - _31;

	double _40 = _0 * m1;
	double _41 = _1 * m1;
	double _42 = _2 * m1;
	double _43 = _3 * m1;

	double _44 = _4 * m1;
	double _45 = _5 * m1;
	double _46 = _6 * m1;
	double _47 = _7 * m1;

	double _48 = m1 - _40;
	double _49 = n1 - _41;
	double _50 = o1 - _42;
	double _51 = p1 - _43;

	double _52 = 0.0 - _44;
	double _53 = 0.0 - _45;
	double _54 = 0.0 - _46;
	double _55 = 1.0 - _47;

	double _56 = _16 / _17;
	double _57 = _17 / _17;
	double _58 = _18 / _17;
	double _59 = _19 / _17;

	double _60 = _20 / _17;
	double _61 = _21 / _17;
	double _62 = _22 / _17;
	double _63 = _23 / _17;

	double _64 = _56 * _1;
	double _65 = _57 * _1;
	double _66 = _58 * _1;
	double _67 = _59 * _1;

	double _68 = _60 * _1;
	double _69 = _61 * _1;
	double _70 = _62 * _1;
	double _71 = _63 * _1;

	double _72 = _0 - _64;
	double _73 = _1 - _65;
	double _74 = _2 - _66;
	double _75 = _3 - _67;

	double _76 = _4 - _68;
	double _77 = _5 - _69;
	double _78 = _6 - _70;
	double _79 = _7 - _71;

	double _80 = _56 * _33;
	double _81 = _57 * _33;
	double _82 = _58 * _33;
	double _83 = _59 * _33;

	double _84 = _60 * _33;
	double _85 = _61 * _33;
	double _86 = _62 * _33;
	double _87 = _63 * _33;

	double _88 = _32 - _80;
	double _89 = _33 - _81;
	double _90 = _34 - _82;
	double _91 = _35 - _83;

	double _92 = _36 - _84;
	double _93 = _37 - _85;
	double _94 = _38 - _86;
	double _95 = _39 - _87;

	double _96 = _56 * _49;
	double _97 = _57 * _49;
	double _98 = _58 * _49;
	double _99 = _59 * _49;

	double _100 = _60 * _49;
	double _101 = _61 * _49;
	double _102 = _62 * _49;
	double _103 = _63 * _49;

	double _104 = _48 - _96;
	double _105 = _49 - _97;
	double _106 = _50 - _98;
	double _107 = _51 - _99;

	double _108 = _52 - _100;
	double _109 = _53 - _101;
	double _110 = _54 - _102;
	double _111 = _55 - _103;

	double _112 = _88 / _90;
	double _113 = _89 / _90;
	double _114 = _90 / _90;
	double _115 = _91 / _90;

	double _116 = _92 / _90;
	double _117 = _93 / _90;
	double _118 = _94 / _90;
	double _119 = _95 / _90;

	double _120 = _112 * _74;
	double _121 = _113 * _74;
	double _122 = _114 * _74;
	double _123 = _115 * _74;

	double _124 = _116 * _74;
	double _125 = _117 * _74;
	double _126 = _118 * _74;
	double _127 = _119 * _74;

	double _128 = _72 - _120;
	double _129 = _73 - _121;
	double _130 = _74 - _122;
	double _131 = _75 - _123;

	double _132 = _76 - _124;
	double _133 = _77 - _125;
	double _134 = _78 - _126;
	double _135 = _79 - _127;

	double _136 = _112 * _58;
	double _137 = _113 * _58;
	double _138 = _114 * _58;
	double _139 = _115 * _58;

	double _140 = _116 * _58;
	double _141 = _117 * _58;
	double _142 = _118 * _58;
	double _143 = _119 * _58;

	double _144 = _56 - _136;
	double _145 = _57 - _137;
	double _146 = _58 - _138;
	double _147 = _59 - _139;

	double _148 = _60 - _140;
	double _149 = _61 - _141;
	double _150 = _62 - _142;
	double _151 = _63 - _143;

	double _152 = _112 * _106;
	double _153 = _113 * _106;
	double _154 = _114 * _106;
	double _155 = _115 * _106;

	double _156 = _116 * _106;
	double _157 = _117 * _106;
	double _158 = _118 * _106;
	double _159 = _119 * _106;

	double _160 = _104 - _152;
	double _161 = _105 - _153;
	double _162 = _106 - _154;
	double _163 = _107 - _155;

	double _164 = _108 - _156;
	double _165 = _109 - _157;
	double _166 = _110 - _158;
	double _167 = _111 - _159;

	double _168 = _160 / _163;
	double _169 = _161 / _163;
	double _170 = _162 / _163;
	double _171 = _163 / _163;

	double _172 = _164 / _163;
	double _173 = _165 / _163;
	double _174 = _166 / _163;
	double _175 = _167 / _163;

	double _176 = _168 * _131;
	double _177 = _169 * _131;
	double _178 = _170 * _131;
	double _179 = _171 * _131;

	double _180 = _172 * _131;
	double _181 = _173 * _131;
	double _182 = _174 * _131;
	double _183 = _175 * _131;

	double _184 = _128 - _176;
	double _185 = _129 - _177;
	double _186 = _130 - _178;
	double _187 = _131 - _179;

	double _188 = _132 - _180;
	double _189 = _133 - _181;
	double _190 = _134 - _182;
	double _191 = _135 - _183;

	double _192 = _168 * _147;
	double _193 = _169 * _147;
	double _194 = _170 * _147;
	double _195 = _171 * _147;

	double _196 = _172 * _147;
	double _197 = _173 * _147;
	double _198 = _174 * _147;
	double _199 = _175 * _147;

	double _200 = _144 - _192;
	double _201 = _145 - _193;
	double _202 = _146 - _194;
	double _203 = _147 - _195;

	double _204 = _148 - _196;
	double _205 = _149 - _197;
	double _206 = _150 - _198;
	double _207 = _151 - _199;

	double _208 = _168 * _115;
	double _209 = _169 * _115;
	double _210 = _170 * _115;
	double _211 = _171 * _115;

	double _212 = _172 * _115;
	double _213 = _173 * _115;
	double _214 = _174 * _115;
	double _215 = _175 * _115;

	double _216 = _112 - _208;
	double _217 = _113 - _209;
	double _218 = _114 - _210;
	double _219 = _115 - _211;

	double _220 = _116 - _212;
	double _221 = _117 - _213;
	double _222 = _118 - _214;
	double _223 = _119 - _215;

	return {
		_188, _189, _190, _191,
		_204, _205, _206, _207,
		_220, _221, _222, _223,
		_172, _173, _174, _175
	};*/
}
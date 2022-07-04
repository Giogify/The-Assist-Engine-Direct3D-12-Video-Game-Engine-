//#pragma once
//#include <immintrin.h>
//#include <ammintrin.h>
//#include <cstdint>
//#include <compare>
//#include <array>
//#include <cmath>
//
//namespace AssistMath {
//
//	constexpr double AM_PI = 3.1415926535897932384626433832795;
//	constexpr double AM_2PI = 6.283185307179586476925286766559;
//	constexpr double AM_1DIVPI = 3.1415926535897932384626433832795;
//	constexpr double AM_1DIV2PI = 0.15915494309189533576888376337251;
//	constexpr double AM_PIDIV2 = 1.5707963267948966192313216916398;
//	constexpr double AM_PIDIV4 = 0.78539816339744830961566084581988;
//
//	constexpr uint32_t AM_SELECT_0 = 0x00000000;
//	constexpr uint32_t AM_SELECT_1 = 0xFFFFFFFF;
//
//	constexpr uint8_t AM_PERMUTE_0X = 0;
//	constexpr uint8_t AM_PERMUTE_0Y = 1;
//	constexpr uint8_t AM_PERMUTE_0Z = 2;
//	constexpr uint8_t AM_PERMUTE_0W = 3;
//	constexpr uint8_t AM_PERMUTE_1X = 4;
//	constexpr uint8_t AM_PERMUTE_1Y = 5;
//	constexpr uint8_t AM_PERMUTE_1Z = 6;
//	constexpr uint8_t AM_PERMUTE_1W = 7;
//
//	constexpr uint8_t AM_SWIZZLE_X = 0;
//	constexpr uint8_t AM_SWIZZLE_Y = 1;
//	constexpr uint8_t AM_SWIZZLE_Z = 2;
//	constexpr uint8_t AM_SWIZZLE_W = 3;
//
//	constexpr uint8_t AM_CRMASK_CR6 = 0x000000F0;
//	constexpr uint8_t AM_CRMASK_CR6TRUE = 0x00000080;
//	constexpr uint8_t AM_CRMASK_CR6FALSE = 0x00000020;
//	//constexpr uint32_t XM_CRMASK_CR6BOUNDS = XM_CRMASK_CR6FALSE;
//
//	// Unit conversion
//	double AMConvertToRadians(double deg) noexcept;
//	float AMConvertToRadians(float deg) noexcept;
//	double AMConvertToDegrees(double rad) noexcept;
//	float AMConvertToDegrees(float rad) noexcept;
//
//	// Types
//	using AMVECTOR = __m256d;
//	using FAMVECTOR = __m128;
//	struct AMMATRIX {
//		AMVECTOR m[4]{};
//
//		AMMATRIX operator*(const AMMATRIX& m2) {
//			AMMATRIX res{};
//
//			AMMATRIX m2rot{
//				m2.m[0].m256d_f64[0], m2.m[1].m256d_f64[0], m2.m[2].m256d_f64[0], m2.m[3].m256d_f64[0],
//				m2.m[0].m256d_f64[1], m2.m[1].m256d_f64[1], m2.m[2].m256d_f64[1], m2.m[3].m256d_f64[1],
//				m2.m[0].m256d_f64[2], m2.m[1].m256d_f64[2], m2.m[2].m256d_f64[2], m2.m[3].m256d_f64[2],
//				m2.m[0].m256d_f64[3], m2.m[1].m256d_f64[3], m2.m[2].m256d_f64[3], m2.m[3].m256d_f64[3]
//			};
//
//			std::array<AMVECTOR, 4u> a{ m[0], m[1], m[2], m[3] };
//			std::array<AMVECTOR, 4u> b{ m2rot.m[0], m2rot.m[1], m2rot.m[2], m2rot.m[3] };
//
//			AMVECTOR mulRes{};
//
//			for (int i = 0; i < 4; i++) {
//				for (int j = 0; j < 4; j++) {
//					mulRes = _mm256_mul_pd(a[i], b[j]);
//					for (int k = 0; k < 4; k++) {
//						res.m[i].m256d_f64[j] += mulRes.m256d_f64[k];
//					}
//				}
//			}
//			return res;
//		}
//	};
//	struct FAMMATRIX {
//		FAMVECTOR m[4]{};
//
//		FAMMATRIX operator*(const FAMMATRIX& m2) {
//			FAMMATRIX res{};
//
//			FAMMATRIX m2rot{
//				m2.m[0].m128_f32[0], m2.m[1].m128_f32[0], m2.m[2].m128_f32[0], m2.m[3].m128_f32[0],
//				m2.m[0].m128_f32[1], m2.m[1].m128_f32[1], m2.m[2].m128_f32[1], m2.m[3].m128_f32[1],
//				m2.m[0].m128_f32[2], m2.m[1].m128_f32[2], m2.m[2].m128_f32[2], m2.m[3].m128_f32[2],
//				m2.m[0].m128_f32[3], m2.m[1].m128_f32[3], m2.m[2].m128_f32[3], m2.m[3].m128_f32[3]
//			};
//
//			std::array<FAMVECTOR, 4u> a{ m[0], m[1], m[2], m[3] };
//			std::array<FAMVECTOR, 4u> b{ m2rot.m[0], m2rot.m[1], m2rot.m[2], m2rot.m[3] };
//
//			FAMVECTOR mulRes{};
//
//			for (int i = 0; i < 4; i++) {
//				for (int j = 0; j < 4; j++) {
//					mulRes = _mm_mul_ps(a[i], b[j]);
//					for (int k = 0; k < 4; k++) {
//						res.m[i].m128_f32[j] += mulRes.m128_f32[k];
//					}
//				}
//			}
//			return res;
//		}
//	};
//
//	struct AMFLOAT2 {
//		float x{};
//		float y{};
//
//		AMFLOAT2() = default;
//
//		AMFLOAT2(const AMFLOAT2&) = default;
//		AMFLOAT2& operator=(const AMFLOAT2&) = default;
//
//		AMFLOAT2(AMFLOAT2&&) = default;
//		AMFLOAT2& operator=(AMFLOAT2&&) = default;
//
//		constexpr AMFLOAT2(float _x, float _y) noexcept : x(_x), y(_y) {}
//		explicit AMFLOAT2(_In_reads_(2) const float* pArray)  noexcept : x(pArray[0]), y(pArray[1]) {}
//	};
//	struct AMFLOAT3 {
//		float x{};
//		float y{};
//		float z{};
//
//		AMFLOAT3() = default;
//
//		AMFLOAT3(const AMFLOAT3&) = default;
//		AMFLOAT3& operator=(const AMFLOAT3&) = default;
//
//		AMFLOAT3(AMFLOAT3&&) = default;
//		AMFLOAT3& operator=(AMFLOAT3&&) = default;
//
//		constexpr AMFLOAT3(float _x, float _y, float _z) noexcept : x(_x), y(_y), z(_z) {}
//		explicit AMFLOAT3(_In_reads_(3) const float* pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}
//	};
//	struct AMFLOAT4 {
//		float x{};
//		float y{};
//		float z{};
//		float w{};
//
//		AMFLOAT4() = default;
//
//		AMFLOAT4(const AMFLOAT4&) = default;
//		AMFLOAT4& operator=(const AMFLOAT4&) = default;
//
//		AMFLOAT4(AMFLOAT4&&) = default;
//		AMFLOAT4& operator=(AMFLOAT4&&) = default;
//
//		constexpr AMFLOAT4(float _x, float _y, float _z, float _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}
//		explicit AMFLOAT4(_In_reads_(4) const float* pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}
//
//		bool operator==(const AMFLOAT4&) const = default;
//		auto operator<=>(const AMFLOAT4&) const = default;
//	};
//	struct AMFLOAT3X3 {
//
//		union {
//			struct {
//				float _11, _12, _13;
//				float _21, _22, _23;
//				float _31, _32, _33;
//			};
//			float m[3][3];
//			float f[9];
//		};
//
//		AMFLOAT3X3() = default;
//
//		AMFLOAT3X3(const AMFLOAT3X3&) = default;
//		AMFLOAT3X3& operator=(const AMFLOAT3X3&) = default;
//
//		AMFLOAT3X3(AMFLOAT3X3&&) = default;
//		AMFLOAT3X3& operator=(AMFLOAT3X3&&) = default;
//
//		constexpr AMFLOAT3X3(
//			float m00, float m01, float m02,
//			float m10, float m11, float m12,
//			float m20, float m21, float m22) noexcept :
//			_11(m00), _12(m01), _13(m02),
//			_21(m10), _22(m11), _23(m12),
//			_31(m20), _32(m21), _33(m22) {}
//		explicit AMFLOAT3X3(_In_reads_(9) const float* pArray) noexcept :
//			_11(pArray[0]), _12(pArray[1]), _13(pArray[2]),
//			_21(pArray[3]), _22(pArray[4]), _23(pArray[5]),
//			_31(pArray[6]), _32(pArray[7]), _33(pArray[8]) {}
//
//		float operator() (size_t Row, size_t Column) const noexcept { return m[Row][Column]; }
//		float& operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }
//		bool operator==(const AMFLOAT3X3&) const = default;
//		auto operator<=>(const AMFLOAT3X3&) const = default;
//
//	};
//	struct AMFLOAT4X3 {
//
//		union {
//			struct {
//				float _11, _12, _13;
//				float _21, _22, _23;
//				float _31, _32, _33;
//				float _41, _42, _43;
//			};
//			float m[4][3];
//			float f[12];
//		};
//
//		AMFLOAT4X3() = default;
//
//		AMFLOAT4X3(const AMFLOAT4X3&) = default;
//		AMFLOAT4X3& operator=(const AMFLOAT4X3&) = default;
//
//		AMFLOAT4X3(AMFLOAT4X3&&) = default;
//		AMFLOAT4X3& operator=(AMFLOAT4X3&&) = default;
//
//		constexpr AMFLOAT4X3(
//			float m00, float m01, float m02,
//			float m10, float m11, float m12,
//			float m20, float m21, float m22,
//			float m30, float m31, float m32) noexcept :
//			_11(m00), _12(m01), _13(m02),
//			_21(m10), _22(m11), _23(m12),
//			_31(m20), _32(m21), _33(m22),
//			_41(m30), _42(m31), _43(m32) {}
//		explicit AMFLOAT4X3(_In_reads_(12) const float* pArray) noexcept :
//			_11(pArray[0]), _12(pArray[1]), _13(pArray[2]),
//			_21(pArray[3]), _22(pArray[4]), _23(pArray[5]),
//			_31(pArray[6]), _32(pArray[7]), _33(pArray[8]),
//			_41(pArray[9]), _42(pArray[10]), _43(pArray[11]) {}
//
//		float  operator() (size_t Row, size_t Column) const noexcept { return m[Row][Column]; }
//		float& operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }
//
//	};
//	struct AMFLOAT3X4 {
//
//		union {
//			struct {
//				float _11, _12, _13, _14;
//				float _21, _22, _23, _24;
//				float _31, _32, _33, _34;
//			};
//			float m[3][4];
//			float d[12];
//		};
//
//		AMFLOAT3X4() = default;
//
//		AMFLOAT3X4(const AMFLOAT3X4&) = default;
//		AMFLOAT3X4& operator=(const AMFLOAT3X4&) = default;
//
//		AMFLOAT3X4(AMFLOAT3X4&&) = default;
//		AMFLOAT3X4& operator=(AMFLOAT3X4&&) = default;
//
//		constexpr AMFLOAT3X4(
//			float m00, float m01, float m02, float m03,
//			float m10, float m11, float m12, float m13,
//			float m20, float m21, float m22, float m23) noexcept :
//			_11(m00), _12(m01), _13(m02), _14(m03),
//			_21(m10), _22(m11), _23(m12), _24(m13),
//			_31(m20), _32(m21), _33(m22), _34(m23) {}
//		explicit AMFLOAT3X4(_In_reads_(12) const float* pArray) noexcept :
//			_11(pArray[0]), _12(pArray[1]), _13(pArray[2]), _14(pArray[3]),
//			_21(pArray[4]), _22(pArray[5]), _23(pArray[6]), _24(pArray[7]),
//			_31(pArray[8]), _32(pArray[9]), _33(pArray[10]), _34(pArray[11]) {}
//
//		float  operator() (size_t Row, size_t Column) const  noexcept { return m[Row][Column]; }
//		float& operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }
//
//	};
//	struct AMFLOAT4X4 {
//
//		union {
//			struct {
//				float _11, _12, _13, _14;
//				float _21, _22, _23, _24;
//				float _31, _32, _33, _34;
//				float _41, _42, _43, _44;
//			};
//			float m[4][4];
//			float d[16];
//		};
//
//		AMFLOAT4X4() = default;
//
//		AMFLOAT4X4(const AMFLOAT4X4&) = default;
//		AMFLOAT4X4& operator=(const AMFLOAT4X4&) = default;
//
//		AMFLOAT4X4(AMFLOAT4X4&&) = default;
//		AMFLOAT4X4& operator=(AMFLOAT4X4&&) = default;
//
//		constexpr AMFLOAT4X4(
//			float m00, float m01, float m02, float m03,
//			float m10, float m11, float m12, float m13,
//			float m20, float m21, float m22, float m23,
//			float m30, float m31, float m32, float m33) noexcept :
//			_11(m00), _12(m01), _13(m02), _14(m03),
//			_21(m10), _22(m11), _23(m12), _24(m13),
//			_31(m20), _32(m21), _33(m22), _34(m23),
//			_41(m30), _42(m31), _43(m32), _44(m33) {}
//		explicit AMFLOAT4X4(_In_reads_(16) const float* pArray) noexcept :
//			_11(pArray[0]), _12(pArray[1]), _13(pArray[2]), _14(pArray[3]),
//			_21(pArray[4]), _22(pArray[5]), _23(pArray[6]), _24(pArray[7]),
//			_31(pArray[8]), _32(pArray[9]), _33(pArray[10]), _34(pArray[11]),
//			_41(pArray[12]), _42(pArray[13]), _43(pArray[14]), _44(pArray[15]) {}
//
//		float  operator() (size_t Row, size_t Column) const  noexcept { return m[Row][Column]; }
//		float& operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }
//
//	};
//
//	struct AMDOUBLE2 {
//		double x{};
//		double y{};
//
//		AMDOUBLE2() = default;
//
//		AMDOUBLE2(const AMDOUBLE2&) = default;
//		AMDOUBLE2& operator=(const AMDOUBLE2&) = default;
//
//		AMDOUBLE2(AMDOUBLE2&&) = default;
//		AMDOUBLE2& operator=(AMDOUBLE2&&) = default;
//
//		constexpr AMDOUBLE2(double _x, double _y) noexcept : x(_x), y(_y) {}
//		explicit AMDOUBLE2(_In_reads_(2) const double* pArray)  noexcept : x(pArray[0]), y(pArray[1]) {}
//	};
//	struct AMDOUBLE3 {
//		double x{};
//		double y{};
//		double z{};
//
//		AMDOUBLE3() = default;
//
//		AMDOUBLE3(const AMDOUBLE3&) = default;
//		AMDOUBLE3& operator=(const AMDOUBLE3&) = default;
//
//		AMDOUBLE3(AMDOUBLE3&&) = default;
//		AMDOUBLE3& operator=(AMDOUBLE3&&) = default;
//
//		constexpr AMDOUBLE3(double _x, double _y, double _z) noexcept : x(_x), y(_y), z(_z) {}
//		explicit AMDOUBLE3(_In_reads_(3) const double* pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}
//	};
//	struct AMDOUBLE4 {
//		double x{};
//		double y{};
//		double z{};
//		double w{};
//
//		AMDOUBLE4() = default;
//
//		AMDOUBLE4(const AMDOUBLE4&) = default;
//		AMDOUBLE4& operator=(const AMDOUBLE4&) = default;
//
//		AMDOUBLE4(AMDOUBLE4&&) = default;
//		AMDOUBLE4& operator=(AMDOUBLE4&&) = default;
//
//		constexpr AMDOUBLE4(double _x, double _y, double _z, double _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}
//		explicit AMDOUBLE4(_In_reads_(4) const double* pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}
//
//		bool operator==(const AMDOUBLE4&) const = default;
//		auto operator<=>(const AMDOUBLE4&) const = default;
//	};
//	struct AMDOUBLE3X3 {
//
//		union {
//			struct {
//				double _11, _12, _13;
//				double _21, _22, _23;
//				double _31, _32, _33;
//			};
//			double m[3][3];
//			double d[9];
//		};
//
//		AMDOUBLE3X3() = default;
//
//		AMDOUBLE3X3(const AMDOUBLE3X3&) = default;
//		AMDOUBLE3X3& operator=(const AMDOUBLE3X3&) = default;
//
//		AMDOUBLE3X3(AMDOUBLE3X3&&) = default;
//		AMDOUBLE3X3& operator=(AMDOUBLE3X3&&) = default;
//
//		constexpr AMDOUBLE3X3(
//			double m00, double m01, double m02,
//			double m10, double m11, double m12,
//			double m20, double m21, double m22) noexcept :
//			_11(m00), _12(m01), _13(m02),
//			_21(m10), _22(m11), _23(m12),
//			_31(m20), _32(m21), _33(m22) {}
//		explicit AMDOUBLE3X3(_In_reads_(9) const double* pArray) noexcept :
//			_11(pArray[0]), _12(pArray[1]), _13(pArray[2]),
//			_21(pArray[3]), _22(pArray[4]), _23(pArray[5]),
//			_31(pArray[6]), _32(pArray[7]), _33(pArray[8]) {}
//
//		double operator() (size_t Row, size_t Column) const noexcept { return m[Row][Column]; }
//		double& operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }
//		bool operator==(const AMDOUBLE3X3&) const = default;
//		auto operator<=>(const AMDOUBLE3X3&) const = default;
//
//	};
//	struct AMDOUBLE4X3 {
//
//		union {
//			struct {
//				double _11, _12, _13;
//				double _21, _22, _23;
//				double _31, _32, _33;
//				double _41, _42, _43;
//			};
//			double m[4][3];
//			double d[12];
//		};
//
//		AMDOUBLE4X3() = default;
//
//		AMDOUBLE4X3(const AMDOUBLE4X3&) = default;
//		AMDOUBLE4X3& operator=(const AMDOUBLE4X3&) = default;
//
//		AMDOUBLE4X3(AMDOUBLE4X3&&) = default;
//		AMDOUBLE4X3& operator=(AMDOUBLE4X3&&) = default;
//
//		constexpr AMDOUBLE4X3(
//			double m00, double m01, double m02,
//			double m10, double m11, double m12,
//			double m20, double m21, double m22,
//			double m30, double m31, double m32) noexcept :
//			_11(m00), _12(m01), _13(m02),
//			_21(m10), _22(m11), _23(m12),
//			_31(m20), _32(m21), _33(m22),
//			_41(m30), _42(m31), _43(m32) {}
//		explicit AMDOUBLE4X3(_In_reads_(12) const double* pArray) noexcept :
//			_11(pArray[0]), _12(pArray[1]), _13(pArray[2]),
//			_21(pArray[3]), _22(pArray[4]), _23(pArray[5]),
//			_31(pArray[6]), _32(pArray[7]), _33(pArray[8]),
//			_41(pArray[9]), _42(pArray[10]), _43(pArray[11]) {}
//
//		double  operator() (size_t Row, size_t Column) const  noexcept { return m[Row][Column]; }
//		double& operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }
//
//	};
//	struct AMDOUBLE3X4 {
//
//		union {
//			struct {
//				double _11, _12, _13, _14;
//				double _21, _22, _23, _24;
//				double _31, _32, _33, _34;
//			};
//			double m[3][4];
//			double d[12];
//		};
//
//		AMDOUBLE3X4() = default;
//
//		AMDOUBLE3X4(const AMDOUBLE3X4&) = default;
//		AMDOUBLE3X4& operator=(const AMDOUBLE3X4&) = default;
//
//		AMDOUBLE3X4(AMDOUBLE3X4&&) = default;
//		AMDOUBLE3X4& operator=(AMDOUBLE3X4&&) = default;
//
//		constexpr AMDOUBLE3X4(
//			double m00, double m01, double m02, double m03,
//			double m10, double m11, double m12, double m13,
//			double m20, double m21, double m22, double m23) noexcept :
//			_11(m00), _12(m01), _13(m02), _14(m03),
//			_21(m10), _22(m11), _23(m12), _24(m13),
//			_31(m20), _32(m21), _33(m22), _34(m23) {}
//		explicit AMDOUBLE3X4(_In_reads_(12) const double* pArray) noexcept :
//			_11(pArray[0]), _12(pArray[1]), _13(pArray[2]), _14(pArray[3]),
//			_21(pArray[4]), _22(pArray[5]), _23(pArray[6]), _24(pArray[7]),
//			_31(pArray[8]), _32(pArray[9]), _33(pArray[10]), _34(pArray[11]) {}
//
//		double  operator() (size_t Row, size_t Column) const  noexcept { return m[Row][Column]; }
//		double& operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }
//
//	};
//	struct AMDOUBLE4X4 {
//
//		union {
//			struct {
//				double _11, _12, _13, _14;
//				double _21, _22, _23, _24;
//				double _31, _32, _33, _34;
//				double _41, _42, _43, _44;
//			};
//			double m[4][4];
//			double d[16];
//		};
//
//		AMDOUBLE4X4() = default;
//
//		AMDOUBLE4X4(const AMDOUBLE4X4&) = default;
//		AMDOUBLE4X4& operator=(const AMDOUBLE4X4&) = default;
//
//		AMDOUBLE4X4(AMDOUBLE4X4&&) = default;
//		AMDOUBLE4X4& operator=(AMDOUBLE4X4&&) = default;
//
//		constexpr AMDOUBLE4X4(
//			double m00, double m01, double m02, double m03,
//			double m10, double m11, double m12, double m13,
//			double m20, double m21, double m22, double m23,
//			double m30, double m31, double m32, double m33) noexcept :
//			_11(m00), _12(m01), _13(m02), _14(m03),
//			_21(m10), _22(m11), _23(m12), _24(m13),
//			_31(m20), _32(m21), _33(m22), _34(m23),
//			_41(m30), _42(m31), _43(m32), _44(m33) {}
//		explicit AMDOUBLE4X4(_In_reads_(16) const double* pArray) noexcept :
//			_11(pArray[0]), _12(pArray[1]), _13(pArray[2]), _14(pArray[3]),
//			_21(pArray[4]), _22(pArray[5]), _23(pArray[6]), _24(pArray[7]),
//			_31(pArray[8]), _32(pArray[9]), _33(pArray[10]), _34(pArray[11]),
//			_41(pArray[12]), _42(pArray[13]), _43(pArray[14]), _44(pArray[15]) {}
//
//		double  operator() (size_t Row, size_t Column) const  noexcept { return m[Row][Column]; }
//		double& operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }
//
//	};
//
//	struct AMUSHORT3X3 {
//		
//		union {
//			struct {
//				uint16_t _11, _12, _13;
//				uint16_t _21, _22, _23;
//				uint16_t _31, _32, _33;
//			};
//			uint16_t m[3][3];
//			uint16_t d[9];
//		};
//
//		AMUSHORT3X3() = default;
//
//		AMUSHORT3X3(const AMUSHORT3X3&) = default;
//		AMUSHORT3X3& operator=(const AMUSHORT3X3&) = default;
//
//		AMUSHORT3X3(AMUSHORT3X3&&) = default;
//		AMUSHORT3X3& operator=(AMUSHORT3X3&&) = default;
//
//		constexpr AMUSHORT3X3(
//			uint16_t m00, uint16_t m01, uint16_t m02,
//			uint16_t m10, uint16_t m11, uint16_t m12,
//			uint16_t m20, uint16_t m21, uint16_t m22) noexcept :
//			_11(m00), _12(m01), _13(m02),
//			_21(m10), _22(m11), _23(m12),
//			_31(m20), _32(m21), _33(m22) {}
//		explicit AMUSHORT3X3(_In_reads_(9) const uint16_t* pArray) noexcept :
//			_11(pArray[0]), _12(pArray[1]), _13(pArray[2]),
//			_21(pArray[3]), _22(pArray[4]), _23(pArray[5]),
//			_31(pArray[6]), _32(pArray[7]), _33(pArray[8]) {}
//
//		uint16_t operator() (size_t Row, size_t Column) const noexcept { return m[Row][Column]; }
//		uint16_t& operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }
//		bool operator==(const AMUSHORT3X3&) const = default;
//		auto operator<=>(const AMUSHORT3X3&) const = default;
//	};
//
//	// Load
//	AMVECTOR AMLoadDouble2(const AMDOUBLE2& src) noexcept;
//	AMVECTOR AMLoadDouble3(const AMDOUBLE3& src) noexcept;
//	AMVECTOR AMLoadDouble4(const AMDOUBLE4& src) noexcept;
//	AMMATRIX AMLoadDouble3x3(const AMDOUBLE3X3& src) noexcept;
//	AMMATRIX AMLoadDouble4x3(const AMDOUBLE4X3& src) noexcept;
//	AMMATRIX AMLoadDouble3x4(const AMDOUBLE3X4& src) noexcept;
//	AMMATRIX AMLoadDouble4x4(const AMDOUBLE4X4& src) noexcept;
//
//	FAMVECTOR AMLoadFloat2(const AMFLOAT2& src) noexcept;
//	FAMVECTOR AMLoadFloat3(const AMFLOAT3& src) noexcept;
//	FAMVECTOR AMLoadFloat4(const AMFLOAT4& src) noexcept;
//	FAMMATRIX AMLoadFloat3x3(const AMFLOAT3X3& src) noexcept;
//	FAMMATRIX AMLoadFloat4x3(const AMFLOAT4X3& src) noexcept;
//	FAMMATRIX AMLoadFloat3x4(const AMFLOAT3X4& src) noexcept;
//	FAMMATRIX AMLoadFloat4x4(const AMFLOAT4X4& src) noexcept;
//
//	// Store (ref)
//	void AMStoreDouble2(AMDOUBLE2& dst, AMVECTOR& V) noexcept;
//	void AMStoreDouble3(AMDOUBLE3& dst, AMVECTOR& V) noexcept;
//	void AMStoreDouble4(AMDOUBLE4& dst, AMVECTOR& V) noexcept;
//	void AMStoreDouble3x3(AMDOUBLE3X3& dst, AMMATRIX& M) noexcept;
//	void AMStoreDouble4x3(AMDOUBLE4X3& dst, AMMATRIX& M) noexcept;
//	void AMStoreDouble3x4(AMDOUBLE3X4& dst, AMMATRIX& M) noexcept;
//	void AMStoreDouble4x4(AMDOUBLE4X4& dst, AMMATRIX& M) noexcept;
//
//	void AMStoreFloat2(AMFLOAT2& dst, FAMVECTOR& V) noexcept;
//	void AMStoreFloat3(AMFLOAT3& dst, FAMVECTOR& V) noexcept;
//	void AMStoreFloat4(AMFLOAT4& dst, FAMVECTOR& V) noexcept;
//	void AMStoreFloat3x3(AMFLOAT3X3& dst, FAMMATRIX& M) noexcept;
//	void AMStoreFloat4x3(AMFLOAT4X3& dst, FAMMATRIX& M) noexcept;
//	void AMStoreFloat3x4(AMFLOAT3X4& dst, FAMMATRIX& M) noexcept;
//	void AMStoreFloat4x4(AMFLOAT4X4& dst, FAMMATRIX& M) noexcept;
//
//	// Store (new)
//	AMDOUBLE2 AMStoreDouble2(AMVECTOR& V) noexcept;
//	AMDOUBLE3 AMStoreDouble3(AMVECTOR& V) noexcept;
//	AMDOUBLE4 AMStoreDouble4(AMVECTOR& V) noexcept;
//	AMDOUBLE3X3 AMStoreDouble3x3(AMMATRIX& M) noexcept;
//	AMDOUBLE4X3 AMStoreDouble4x3(AMMATRIX& M) noexcept;
//	AMDOUBLE3X4 AMStoreDouble3x4(AMMATRIX& M) noexcept;
//	AMDOUBLE4X4 AMStoreDouble4x4(AMMATRIX& M) noexcept;
//
//	FAMMATRIX FAMMatrixIdentity();
//	FAMMATRIX FAMMatrixRotationRollPitchYaw(FAMVECTOR& vec);
//	FAMMATRIX FAMMatrixTranslation(FAMVECTOR& vec);
//	FAMVECTOR FAMMatrixDeterminant(FAMMATRIX& mx);
//	FAMMATRIX FAMMatrixPerspectiveFovLH(float fov, float ar, float n, float f);
//	FAMMATRIX FAMMatrixLookAtLH(FAMVECTOR& eye, FAMVECTOR& focus, FAMVECTOR upDirection);
//	FAMMATRIX FAMMatrixLookToLH(FAMVECTOR& eye, FAMVECTOR& direction, FAMVECTOR upDirection);
//	FAMVECTOR FAMVector4Transform(FAMVECTOR& vec, FAMMATRIX& mx);
//	FAMMATRIX FAMMatrixTranspose(FAMMATRIX& mx);
//	FAMMATRIX FAMMatrixInverse(FAMVECTOR& det, FAMMATRIX& mx);
//	FAMVECTOR FAMVector4Normalize(const FAMVECTOR& vec);
//	FAMVECTOR FAMVector4DotProduct(const FAMVECTOR& vec0, const FAMVECTOR& vec1);
//	FAMVECTOR FAMVector4CrossProduct(const FAMVECTOR& vec0, const FAMVECTOR& vec1);
//	float FAMVector4SumVector(const FAMVECTOR& vec);
//	float FAMVector4Magnitude(const FAMVECTOR& vec);
//	FAMVECTOR FAMVector4Negate(const FAMVECTOR& vec);
//
//	AMMATRIX AMMatrixIdentity();
//	AMMATRIX AMMatrixRotationRollPitchYaw(AMVECTOR& vec);
//	AMMATRIX AMMatrixTranslation(AMVECTOR& vec);
//	AMVECTOR AMMatrixDeterminant(AMMATRIX& mx);
//	AMMATRIX AMMatrixPerspectiveFovLH(double fov, double ar, double n, double f);
//	AMMATRIX AMMatrixLookAtLH(AMVECTOR& eye, AMVECTOR& focus, AMVECTOR upDirection);
//	AMMATRIX AMMatrixLookToLH(AMVECTOR& eye, AMVECTOR& direction, AMVECTOR upDirection);
//	AMVECTOR AMVector4Transform(AMVECTOR& vec, AMMATRIX& mx);
//	AMMATRIX AMMatrixTranspose(AMMATRIX& mx);
//	AMMATRIX AMMatrixInverse(AMVECTOR& det, AMMATRIX& mx);
//	AMVECTOR AMVector4Normalize(const AMVECTOR& vec);
//	AMVECTOR AMVector4DotProduct(const AMVECTOR& vec0, const AMVECTOR& vec1);
//	AMVECTOR AMVector4CrossProduct(const AMVECTOR& vec0, const AMVECTOR& vec1);
//	double AMVector4SumVector(const AMVECTOR& vec);
//}
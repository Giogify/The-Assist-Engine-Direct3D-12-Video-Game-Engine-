#pragma once
// Includes below - Note: there shouldn't be any source file includes here.

#include "d3dx12.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <chrono>
#include <compare>
#include <cmath>
#include <queue>
#include <bitset>
#include <optional>
#include <vector>
#include <memory>
#include <array>
#include <iostream>
#include <thread>
#include <fstream>
#include <unordered_map>
#include <map>
#include <filesystem>
#include <future>
#include <regex>

#include <d3dcompiler.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <ammintrin.h>
#include <immintrin.h>
#include <Windows.h>

#include <d3d11_4.h>
#include <d3d11on12.h>

#include <d2d1_3.h>
#include <d2d1_3helper.h>
#include <d2dbasetypes.h>
#include <dwrite_3.h>

#include "inc/DirectXTex/DirectXTex/DirectXTex.h"
#include "inc/DirectXTex/DirectXTex/DirectXTex.inl"

//#include <fbxsdk.h>

#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxcompiler")
#pragma comment(lib, "d3d12")
#pragma comment(lib, "DXGI")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "dwrite")

#if defined(min)
#undef min
#endif

#if defined(max)
#undef max
#endif

//#if defined(_DEBUG)
//#pragma comment(lib, "C:\\Program Files\\Autodesk\\FBX\\FBX SDK\\2020.2.1\\lib\\vs2019\\x64\\debug\\libfbxsdk.lib")
//#endif
//#if defined(NDEBUG)
//#pragma comment(lib, "C:\\Program Files\\Autodesk\\FBX\\FBX SDK\\2020.2.1\\lib\\vs2019\\x64\\release\\libfbxsdk.lib")
//#endif

using namespace Microsoft::WRL;
using namespace DirectX;

// The Global Information Database. Contains data structures to be instantiated by the Global State Object, which is also contained herein.
// The Global State Object contains instances of data to track the current application state / game state.

// Entire AssistMath lib.
namespace GID::DSU::AssistMath {
	constexpr float AM_PI = 3.1415926535897932384626433832795f;
	constexpr float AM_2PI = 6.283185307179586476925286766559f;
	constexpr float AM_1DIVPI = 3.1415926535897932384626433832795f;
	constexpr float AM_1DIV2PI = 0.15915494309189533576888376337251f;
	constexpr float AM_PIDIV2 = 1.5707963267948966192313216916398f;
	constexpr float AM_PIDIV4 = 0.78539816339744830961566084581988f;
	
	constexpr float AM_SQRT2DIV2 = 0.70710678118654752440084436210485f;

	constexpr uint32_t AM_SELECT_0 = 0x00000000;
	constexpr uint32_t AM_SELECT_1 = 0xFFFFFFFF;

	constexpr uint8_t AM_PERMUTE_0X = 0;
	constexpr uint8_t AM_PERMUTE_0Y = 1;
	constexpr uint8_t AM_PERMUTE_0Z = 2;
	constexpr uint8_t AM_PERMUTE_0W = 3;
	constexpr uint8_t AM_PERMUTE_1X = 4;
	constexpr uint8_t AM_PERMUTE_1Y = 5;
	constexpr uint8_t AM_PERMUTE_1Z = 6;
	constexpr uint8_t AM_PERMUTE_1W = 7;

	constexpr uint8_t AM_SWIZZLE_X = 0;
	constexpr uint8_t AM_SWIZZLE_Y = 1;
	constexpr uint8_t AM_SWIZZLE_Z = 2;
	constexpr uint8_t AM_SWIZZLE_W = 3;

	constexpr uint8_t AM_CRMASK_CR6 = 0x000000F0;
	constexpr uint8_t AM_CRMASK_CR6TRUE = 0x00000080;
	constexpr uint8_t AM_CRMASK_CR6FALSE = 0x00000020;
	//constexpr uint32_t XM_CRMASK_CR6BOUNDS = XM_CRMASK_CR6FALSE;

	// Unit conversion
	inline double AMConvertToRadians(double deg) noexcept { return deg * (AM_PI / 180.0); }
	inline float AMConvertToRadians(float deg) noexcept { return deg * ((float)AM_PI / 180.0f); }
	inline double AMConvertToDegrees(double rad) noexcept { return rad * (180.0 / AM_PI); }
	inline float AMConvertToDegrees(float rad) noexcept { return rad * (180.0f / (float)AM_PI); }

	// Types
	using AMVECTOR = __m256d;
	using FAMVECTOR = __m128;
	struct AMMATRIX {
		AMVECTOR m[4]{};

		AMMATRIX operator*(const AMMATRIX& m2) {
			AMMATRIX res{};

			AMMATRIX m2rot{
				m2.m[0].m256d_f64[0], m2.m[1].m256d_f64[0], m2.m[2].m256d_f64[0], m2.m[3].m256d_f64[0],
				m2.m[0].m256d_f64[1], m2.m[1].m256d_f64[1], m2.m[2].m256d_f64[1], m2.m[3].m256d_f64[1],
				m2.m[0].m256d_f64[2], m2.m[1].m256d_f64[2], m2.m[2].m256d_f64[2], m2.m[3].m256d_f64[2],
				m2.m[0].m256d_f64[3], m2.m[1].m256d_f64[3], m2.m[2].m256d_f64[3], m2.m[3].m256d_f64[3]
			};

			std::array<AMVECTOR, 4u> a{ m[0], m[1], m[2], m[3] };
			std::array<AMVECTOR, 4u> b{ m2rot.m[0], m2rot.m[1], m2rot.m[2], m2rot.m[3] };

			AMVECTOR mulRes{};

			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					mulRes = _mm256_mul_pd(a[i], b[j]);
					for (int k = 0; k < 4; k++) {
						res.m[i].m256d_f64[j] += mulRes.m256d_f64[k];
					}
				}
			}
			return res;
		}
	};
	struct FAMMATRIX {
		FAMVECTOR m[4]{};

		FAMMATRIX operator*(const FAMMATRIX& m2) {
			FAMMATRIX res{};

			FAMMATRIX m2rot{
				m2.m[0].m128_f32[0], m2.m[1].m128_f32[0], m2.m[2].m128_f32[0], m2.m[3].m128_f32[0],
				m2.m[0].m128_f32[1], m2.m[1].m128_f32[1], m2.m[2].m128_f32[1], m2.m[3].m128_f32[1],
				m2.m[0].m128_f32[2], m2.m[1].m128_f32[2], m2.m[2].m128_f32[2], m2.m[3].m128_f32[2],
				m2.m[0].m128_f32[3], m2.m[1].m128_f32[3], m2.m[2].m128_f32[3], m2.m[3].m128_f32[3]
			};

			std::array<FAMVECTOR, 4u> a{ m[0], m[1], m[2], m[3] };
			std::array<FAMVECTOR, 4u> b{ m2rot.m[0], m2rot.m[1], m2rot.m[2], m2rot.m[3] };

			FAMVECTOR mulRes{};

			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					mulRes = _mm_mul_ps(a[i], b[j]);
					for (int k = 0; k < 4; k++) {
						res.m[i].m128_f32[j] += mulRes.m128_f32[k];
					}
				}
			}
			return res;
		}
	};

	struct AMFLOAT2 {
		float x{};
		float y{};

		AMFLOAT2() = default;

		AMFLOAT2(const AMFLOAT2&) = default;
		AMFLOAT2& operator=(const AMFLOAT2&) = default;

		AMFLOAT2(AMFLOAT2&&) = default;
		AMFLOAT2& operator=(AMFLOAT2&&) = default;

		constexpr AMFLOAT2(float _x, float _y) noexcept : x(_x), y(_y) {}
		explicit AMFLOAT2(_In_reads_(2) const float* pArray)  noexcept : x(pArray[0]), y(pArray[1]) {}
	};
	struct AMFLOAT3 {
		float x{};
		float y{};
		float z{};

		AMFLOAT3() = default;

		AMFLOAT3(const AMFLOAT3&) = default;
		AMFLOAT3& operator=(const AMFLOAT3&) = default;

		AMFLOAT3(AMFLOAT3&&) = default;
		AMFLOAT3& operator=(AMFLOAT3&&) = default;

		constexpr AMFLOAT3(float _x, float _y, float _z) noexcept : x(_x), y(_y), z(_z) {}
		explicit AMFLOAT3(_In_reads_(3) const float* pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}
	};
	struct AMFLOAT4 {
		float x{};
		float y{};
		float z{};
		float w{};

		AMFLOAT4() = default;

		AMFLOAT4(const AMFLOAT4&) = default;
		AMFLOAT4& operator=(const AMFLOAT4&) = default;

		AMFLOAT4(AMFLOAT4&&) = default;
		AMFLOAT4& operator=(AMFLOAT4&&) = default;

		constexpr AMFLOAT4(float _x, float _y, float _z, float _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}
		explicit AMFLOAT4(_In_reads_(4) const float* pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}

		bool operator==(const AMFLOAT4&) const = default;
		auto operator<=>(const AMFLOAT4&) const = default;
	};
	struct AMFLOAT3X3 {

		union {
			struct {
				float _11, _12, _13;
				float _21, _22, _23;
				float _31, _32, _33;
			};
			float m[3][3];
			float f[9];
		};

		AMFLOAT3X3() = default;

		AMFLOAT3X3(const AMFLOAT3X3&) = default;
		AMFLOAT3X3& operator=(const AMFLOAT3X3&) = default;

		AMFLOAT3X3(AMFLOAT3X3&&) = default;
		AMFLOAT3X3& operator=(AMFLOAT3X3&&) = default;

		constexpr AMFLOAT3X3(
			float m00, float m01, float m02,
			float m10, float m11, float m12,
			float m20, float m21, float m22) noexcept :
			_11(m00), _12(m01), _13(m02),
			_21(m10), _22(m11), _23(m12),
			_31(m20), _32(m21), _33(m22) {}
		explicit AMFLOAT3X3(_In_reads_(9) const float* pArray) noexcept :
			_11(pArray[0]), _12(pArray[1]), _13(pArray[2]),
			_21(pArray[3]), _22(pArray[4]), _23(pArray[5]),
			_31(pArray[6]), _32(pArray[7]), _33(pArray[8]) {}

		float operator() (size_t Row, size_t Column) const noexcept { return m[Row][Column]; }
		float& operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }
		bool operator==(const AMFLOAT3X3&) const = default;
		auto operator<=>(const AMFLOAT3X3&) const = default;

	};
	struct AMFLOAT4X3 {

		union {
			struct {
				float _11, _12, _13;
				float _21, _22, _23;
				float _31, _32, _33;
				float _41, _42, _43;
			};
			float m[4][3];
			float f[12];
		};

		AMFLOAT4X3() = default;

		AMFLOAT4X3(const AMFLOAT4X3&) = default;
		AMFLOAT4X3& operator=(const AMFLOAT4X3&) = default;

		AMFLOAT4X3(AMFLOAT4X3&&) = default;
		AMFLOAT4X3& operator=(AMFLOAT4X3&&) = default;

		constexpr AMFLOAT4X3(
			float m00, float m01, float m02,
			float m10, float m11, float m12,
			float m20, float m21, float m22,
			float m30, float m31, float m32) noexcept :
			_11(m00), _12(m01), _13(m02),
			_21(m10), _22(m11), _23(m12),
			_31(m20), _32(m21), _33(m22),
			_41(m30), _42(m31), _43(m32) {}
		explicit AMFLOAT4X3(_In_reads_(12) const float* pArray) noexcept :
			_11(pArray[0]), _12(pArray[1]), _13(pArray[2]),
			_21(pArray[3]), _22(pArray[4]), _23(pArray[5]),
			_31(pArray[6]), _32(pArray[7]), _33(pArray[8]),
			_41(pArray[9]), _42(pArray[10]), _43(pArray[11]) {}

		float  operator() (size_t Row, size_t Column) const noexcept { return m[Row][Column]; }
		float& operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }

	};
	struct AMFLOAT3X4 {

		union {
			struct {
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
			};
			float m[3][4];
			float d[12];
		};

		AMFLOAT3X4() = default;

		AMFLOAT3X4(const AMFLOAT3X4&) = default;
		AMFLOAT3X4& operator=(const AMFLOAT3X4&) = default;

		AMFLOAT3X4(AMFLOAT3X4&&) = default;
		AMFLOAT3X4& operator=(AMFLOAT3X4&&) = default;

		constexpr AMFLOAT3X4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23) noexcept :
			_11(m00), _12(m01), _13(m02), _14(m03),
			_21(m10), _22(m11), _23(m12), _24(m13),
			_31(m20), _32(m21), _33(m22), _34(m23) {}
		explicit AMFLOAT3X4(_In_reads_(12) const float* pArray) noexcept :
			_11(pArray[0]), _12(pArray[1]), _13(pArray[2]), _14(pArray[3]),
			_21(pArray[4]), _22(pArray[5]), _23(pArray[6]), _24(pArray[7]),
			_31(pArray[8]), _32(pArray[9]), _33(pArray[10]), _34(pArray[11]) {}

		float  operator() (size_t Row, size_t Column) const  noexcept { return m[Row][Column]; }
		float& operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }

	};
	struct AMFLOAT4X4 {

		union {
			struct {
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
			float d[16];
		};

		AMFLOAT4X4() = default;

		AMFLOAT4X4(const AMFLOAT4X4&) = default;
		AMFLOAT4X4& operator=(const AMFLOAT4X4&) = default;

		AMFLOAT4X4(AMFLOAT4X4&&) = default;
		AMFLOAT4X4& operator=(AMFLOAT4X4&&) = default;

		constexpr AMFLOAT4X4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33) noexcept :
			_11(m00), _12(m01), _13(m02), _14(m03),
			_21(m10), _22(m11), _23(m12), _24(m13),
			_31(m20), _32(m21), _33(m22), _34(m23),
			_41(m30), _42(m31), _43(m32), _44(m33) {}
		explicit AMFLOAT4X4(_In_reads_(16) const float* pArray) noexcept :
			_11(pArray[0]), _12(pArray[1]), _13(pArray[2]), _14(pArray[3]),
			_21(pArray[4]), _22(pArray[5]), _23(pArray[6]), _24(pArray[7]),
			_31(pArray[8]), _32(pArray[9]), _33(pArray[10]), _34(pArray[11]),
			_41(pArray[12]), _42(pArray[13]), _43(pArray[14]), _44(pArray[15]) {}

		float  operator() (size_t Row, size_t Column) const  noexcept { return m[Row][Column]; }
		float& operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }

	};

	struct AMDOUBLE2 {
		double x{};
		double y{};

		AMDOUBLE2() = default;

		AMDOUBLE2(const AMDOUBLE2&) = default;
		AMDOUBLE2& operator=(const AMDOUBLE2&) = default;

		AMDOUBLE2(AMDOUBLE2&&) = default;
		AMDOUBLE2& operator=(AMDOUBLE2&&) = default;

		constexpr AMDOUBLE2(double _x, double _y) noexcept : x(_x), y(_y) {}
		explicit AMDOUBLE2(_In_reads_(2) const double* pArray)  noexcept : x(pArray[0]), y(pArray[1]) {}
	};
	struct AMDOUBLE3 {
		double x{};
		double y{};
		double z{};

		AMDOUBLE3() = default;

		AMDOUBLE3(const AMDOUBLE3&) = default;
		AMDOUBLE3& operator=(const AMDOUBLE3&) = default;

		AMDOUBLE3(AMDOUBLE3&&) = default;
		AMDOUBLE3& operator=(AMDOUBLE3&&) = default;

		constexpr AMDOUBLE3(double _x, double _y, double _z) noexcept : x(_x), y(_y), z(_z) {}
		explicit AMDOUBLE3(_In_reads_(3) const double* pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}
	};
	struct AMDOUBLE4 {
		double x{};
		double y{};
		double z{};
		double w{};

		AMDOUBLE4() = default;

		AMDOUBLE4(const AMDOUBLE4&) = default;
		AMDOUBLE4& operator=(const AMDOUBLE4&) = default;

		AMDOUBLE4(AMDOUBLE4&&) = default;
		AMDOUBLE4& operator=(AMDOUBLE4&&) = default;

		constexpr AMDOUBLE4(double _x, double _y, double _z, double _w) noexcept : x(_x), y(_y), z(_z), w(_w) {}
		explicit AMDOUBLE4(_In_reads_(4) const double* pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}

		bool operator==(const AMDOUBLE4&) const = default;
		auto operator<=>(const AMDOUBLE4&) const = default;
	};
	struct AMDOUBLE3X3 {

		union {
			struct {
				double _11, _12, _13;
				double _21, _22, _23;
				double _31, _32, _33;
			};
			double m[3][3];
			double d[9];
		};

		AMDOUBLE3X3() = default;

		AMDOUBLE3X3(const AMDOUBLE3X3&) = default;
		AMDOUBLE3X3& operator=(const AMDOUBLE3X3&) = default;

		AMDOUBLE3X3(AMDOUBLE3X3&&) = default;
		AMDOUBLE3X3& operator=(AMDOUBLE3X3&&) = default;

		constexpr AMDOUBLE3X3(
			double m00, double m01, double m02,
			double m10, double m11, double m12,
			double m20, double m21, double m22) noexcept :
			_11(m00), _12(m01), _13(m02),
			_21(m10), _22(m11), _23(m12),
			_31(m20), _32(m21), _33(m22) {}
		explicit AMDOUBLE3X3(_In_reads_(9) const double* pArray) noexcept :
			_11(pArray[0]), _12(pArray[1]), _13(pArray[2]),
			_21(pArray[3]), _22(pArray[4]), _23(pArray[5]),
			_31(pArray[6]), _32(pArray[7]), _33(pArray[8]) {}

		double operator() (size_t Row, size_t Column) const noexcept { return m[Row][Column]; }
		double& operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }
		bool operator==(const AMDOUBLE3X3&) const = default;
		auto operator<=>(const AMDOUBLE3X3&) const = default;

	};
	struct AMDOUBLE4X3 {

		union {
			struct {
				double _11, _12, _13;
				double _21, _22, _23;
				double _31, _32, _33;
				double _41, _42, _43;
			};
			double m[4][3];
			double d[12];
		};

		AMDOUBLE4X3() = default;

		AMDOUBLE4X3(const AMDOUBLE4X3&) = default;
		AMDOUBLE4X3& operator=(const AMDOUBLE4X3&) = default;

		AMDOUBLE4X3(AMDOUBLE4X3&&) = default;
		AMDOUBLE4X3& operator=(AMDOUBLE4X3&&) = default;

		constexpr AMDOUBLE4X3(
			double m00, double m01, double m02,
			double m10, double m11, double m12,
			double m20, double m21, double m22,
			double m30, double m31, double m32) noexcept :
			_11(m00), _12(m01), _13(m02),
			_21(m10), _22(m11), _23(m12),
			_31(m20), _32(m21), _33(m22),
			_41(m30), _42(m31), _43(m32) {}
		explicit AMDOUBLE4X3(_In_reads_(12) const double* pArray) noexcept :
			_11(pArray[0]), _12(pArray[1]), _13(pArray[2]),
			_21(pArray[3]), _22(pArray[4]), _23(pArray[5]),
			_31(pArray[6]), _32(pArray[7]), _33(pArray[8]),
			_41(pArray[9]), _42(pArray[10]), _43(pArray[11]) {}

		double  operator() (size_t Row, size_t Column) const  noexcept { return m[Row][Column]; }
		double& operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }

	};
	struct AMDOUBLE3X4 {

		union {
			struct {
				double _11, _12, _13, _14;
				double _21, _22, _23, _24;
				double _31, _32, _33, _34;
			};
			double m[3][4];
			double d[12];
		};

		AMDOUBLE3X4() = default;

		AMDOUBLE3X4(const AMDOUBLE3X4&) = default;
		AMDOUBLE3X4& operator=(const AMDOUBLE3X4&) = default;

		AMDOUBLE3X4(AMDOUBLE3X4&&) = default;
		AMDOUBLE3X4& operator=(AMDOUBLE3X4&&) = default;

		constexpr AMDOUBLE3X4(
			double m00, double m01, double m02, double m03,
			double m10, double m11, double m12, double m13,
			double m20, double m21, double m22, double m23) noexcept :
			_11(m00), _12(m01), _13(m02), _14(m03),
			_21(m10), _22(m11), _23(m12), _24(m13),
			_31(m20), _32(m21), _33(m22), _34(m23) {}
		explicit AMDOUBLE3X4(_In_reads_(12) const double* pArray) noexcept :
			_11(pArray[0]), _12(pArray[1]), _13(pArray[2]), _14(pArray[3]),
			_21(pArray[4]), _22(pArray[5]), _23(pArray[6]), _24(pArray[7]),
			_31(pArray[8]), _32(pArray[9]), _33(pArray[10]), _34(pArray[11]) {}

		double  operator() (size_t Row, size_t Column) const  noexcept { return m[Row][Column]; }
		double& operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }

	};
	struct AMDOUBLE4X4 {

		union {
			struct {
				double _11, _12, _13, _14;
				double _21, _22, _23, _24;
				double _31, _32, _33, _34;
				double _41, _42, _43, _44;
			};
			double m[4][4];
			double d[16];
		};

		AMDOUBLE4X4() = default;

		AMDOUBLE4X4(const AMDOUBLE4X4&) = default;
		AMDOUBLE4X4& operator=(const AMDOUBLE4X4&) = default;

		AMDOUBLE4X4(AMDOUBLE4X4&&) = default;
		AMDOUBLE4X4& operator=(AMDOUBLE4X4&&) = default;

		constexpr AMDOUBLE4X4(
			double m00, double m01, double m02, double m03,
			double m10, double m11, double m12, double m13,
			double m20, double m21, double m22, double m23,
			double m30, double m31, double m32, double m33) noexcept :
			_11(m00), _12(m01), _13(m02), _14(m03),
			_21(m10), _22(m11), _23(m12), _24(m13),
			_31(m20), _32(m21), _33(m22), _34(m23),
			_41(m30), _42(m31), _43(m32), _44(m33) {}
		explicit AMDOUBLE4X4(_In_reads_(16) const double* pArray) noexcept :
			_11(pArray[0]), _12(pArray[1]), _13(pArray[2]), _14(pArray[3]),
			_21(pArray[4]), _22(pArray[5]), _23(pArray[6]), _24(pArray[7]),
			_31(pArray[8]), _32(pArray[9]), _33(pArray[10]), _34(pArray[11]),
			_41(pArray[12]), _42(pArray[13]), _43(pArray[14]), _44(pArray[15]) {}

		double  operator() (size_t Row, size_t Column) const  noexcept { return m[Row][Column]; }
		double& operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }

	};

	struct AMUSHORT3X3 {

		union {
			struct {
				uint16_t _11, _12, _13;
				uint16_t _21, _22, _23;
				uint16_t _31, _32, _33;
			};
			uint16_t m[3][3];
			uint16_t d[9];
		};

		AMUSHORT3X3() = default;

		AMUSHORT3X3(const AMUSHORT3X3&) = default;
		AMUSHORT3X3& operator=(const AMUSHORT3X3&) = default;

		AMUSHORT3X3(AMUSHORT3X3&&) = default;
		AMUSHORT3X3& operator=(AMUSHORT3X3&&) = default;

		constexpr AMUSHORT3X3(
			uint16_t m00, uint16_t m01, uint16_t m02,
			uint16_t m10, uint16_t m11, uint16_t m12,
			uint16_t m20, uint16_t m21, uint16_t m22) noexcept :
			_11(m00), _12(m01), _13(m02),
			_21(m10), _22(m11), _23(m12),
			_31(m20), _32(m21), _33(m22) {}
		explicit AMUSHORT3X3(_In_reads_(9) const uint16_t* pArray) noexcept :
			_11(pArray[0]), _12(pArray[1]), _13(pArray[2]),
			_21(pArray[3]), _22(pArray[4]), _23(pArray[5]),
			_31(pArray[6]), _32(pArray[7]), _33(pArray[8]) {}

		uint16_t operator() (size_t Row, size_t Column) const noexcept { return m[Row][Column]; }
		uint16_t& operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }
		bool operator==(const AMUSHORT3X3&) const = default;
		auto operator<=>(const AMUSHORT3X3&) const = default;
	};
	struct AMUINT3 {
		uint32_t x{};
		uint32_t y{};
		uint32_t z{};

		AMUINT3() = default;

		AMUINT3(const AMUINT3&) = default;
		AMUINT3& operator=(const AMUINT3&) = default;

		AMUINT3(AMUINT3&&) = default;
		AMUINT3& operator=(AMUINT3&&) = default;

		constexpr AMUINT3(uint32_t _x, uint32_t _y, uint32_t _z) noexcept : x(_x), y(_y), z(_z) {}
		explicit AMUINT3(_In_reads_(3) const uint32_t* pArray) noexcept : x(pArray[0]), y(pArray[1]), z(pArray[2]) {}
	};
	struct AMUINT3X3 {

		union {
			struct {
				uint32_t _11, _12, _13;
				uint32_t _21, _22, _23;
				uint32_t _31, _32, _33;
			};
			uint32_t m[3][3];
			uint32_t d[9];
		};

		AMUINT3X3() = default;

		AMUINT3X3(const AMUINT3X3&) = default;
		AMUINT3X3& operator=(const AMUINT3X3&) = default;

		AMUINT3X3(AMUINT3X3&&) = default;
		AMUINT3X3& operator=(AMUINT3X3&&) = default;

		constexpr AMUINT3X3(
			uint32_t m00, uint32_t m01, uint32_t m02,
			uint32_t m10, uint32_t m11, uint32_t m12,
			uint32_t m20, uint32_t m21, uint32_t m22) noexcept :
			_11(m00), _12(m01), _13(m02),
			_21(m10), _22(m11), _23(m12),
			_31(m20), _32(m21), _33(m22) {}
		explicit AMUINT3X3(_In_reads_(9) const uint32_t* pArray) noexcept :
			_11(pArray[0]), _12(pArray[1]), _13(pArray[2]),
			_21(pArray[3]), _22(pArray[4]), _23(pArray[5]),
			_31(pArray[6]), _32(pArray[7]), _33(pArray[8]) {}

		uint32_t operator() (size_t Row, size_t Column) const noexcept { return m[Row][Column]; }
		uint32_t& operator() (size_t Row, size_t Column) noexcept { return m[Row][Column]; }
		bool operator==(const AMUINT3X3&) const = default;
		auto operator<=>(const AMUINT3X3&) const = default;
	};

	// Load
	inline AMVECTOR AMLoadDouble2(const AMDOUBLE2& src) noexcept {
		return { _mm256_set_pd(src.x, src.y, 0.0, 0.0) };
	}
	inline AMVECTOR AMLoadDouble3(const AMDOUBLE3& src) noexcept {
		return { _mm256_set_pd(0.0, src.z, src.y, src.x) };
	}
	inline AMVECTOR AMLoadDouble4(const AMDOUBLE4& src) noexcept {
		return { _mm256_set_pd(src.x, src.y, src.z, src.w) };
	}
	inline AMMATRIX AMLoadDouble3x3(const AMDOUBLE3X3& src) noexcept {
		return {
			_mm256_set_pd(src._11, src._12, src._13, 0.0),
			_mm256_set_pd(src._21, src._22, src._23, 0.0),
			_mm256_set_pd(src._31, src._32, src._33, 0.0),
			_mm256_set_pd(0.0,     0.0,     0.0,     0.0)
		};
	}
	inline AMMATRIX AMLoadDouble4x3(const AMDOUBLE4X3& src) noexcept {
		return {
			_mm256_set_pd(src._11, src._12, src._13, 0.0),
			_mm256_set_pd(src._21, src._22, src._23, 0.0),
			_mm256_set_pd(src._31, src._32, src._33, 0.0),
			_mm256_set_pd(src._41, src._41, src._41, 0.0)
		};
	}
	inline AMMATRIX AMLoadDouble3x4(const AMDOUBLE3X4& src) noexcept {
		return {
			_mm256_set_pd(src._11, src._12, src._13, src._14),
			_mm256_set_pd(src._21, src._22, src._23, src._24),
			_mm256_set_pd(src._31, src._32, src._33, src._34),
			_mm256_set_pd(0.0,     0.0,     0.0,     0.0)
		};
	}
	inline AMMATRIX AMLoadDouble4x4(const AMDOUBLE4X4& src) noexcept {
		return {
			_mm256_set_pd(src._14, src._13, src._12, src._11),
			_mm256_set_pd(src._24, src._23, src._22, src._21),
			_mm256_set_pd(src._34, src._33, src._32, src._31),
			_mm256_set_pd(src._44, src._43, src._42, src._41)
		};
	}

	inline FAMVECTOR AMLoadFloat2(const AMFLOAT2& src) noexcept {
		return { _mm_set_ps(0.0f, 0.0f, src.y, src.x) };
	}
	inline FAMVECTOR AMLoadFloat3(const AMFLOAT3& src) noexcept {
		return { _mm_set_ps(0.0f, src.z, src.y, src.x) };
	}
	inline FAMVECTOR AMLoadFloat4(const AMFLOAT4& src) noexcept {
		return { _mm_set_ps(src.w, src.z, src.y, src.x) };
	}
	inline FAMMATRIX AMLoadFloat3x3(const AMFLOAT3X3& src) noexcept {
		return {
			_mm_set_ps(src._13,	src._12,	src._11,	0.0f),
			_mm_set_ps(src._23,	src._22,	src._21,	0.0f),
			_mm_set_ps(src._33,	src._32,	src._31,	0.0f),
			_mm_set_ps(0.0f,	0.0f,     0.0f,			0.0f)
		};
	}
	inline FAMMATRIX AMLoadFloat4x3(const AMFLOAT4X3& src) noexcept {
		return {
			_mm_set_ps(src._13,	src._12,	src._11,	0.0f),
			_mm_set_ps(src._23,	src._22,	src._21,	0.0f),
			_mm_set_ps(src._33,	src._32,	src._31,	0.0f),
			_mm_set_ps(src._43,	src._42,	src._41,	0.0f)
		};
	}
	inline FAMMATRIX AMLoadFloat3x4(const AMFLOAT3X4& src) noexcept {
		return {
			_mm_set_ps(src._14,	src._13,	src._12,	src._11),
			_mm_set_ps(src._24,	src._23,	src._22,	src._21),
			_mm_set_ps(src._34,	src._33,	src._32,	src._31),
			_mm_set_ps(0.0f,	0.0f,		0.0f,		0.0f)
		};
	}
	inline FAMMATRIX AMLoadFloat4x4(const AMFLOAT4X4& src) noexcept {
		return {
			_mm_set_ps(src._14,	src._13,	src._12,	src._11),
			_mm_set_ps(src._24,	src._23,	src._22,	src._21),
			_mm_set_ps(src._34,	src._33,	src._32,	src._31),
			_mm_set_ps(src._44,	src._43,	src._42,	src._41)
		};
	}

	// Store (ref)
	inline void AMStoreDouble2(AMDOUBLE2& dst, AMVECTOR& V) noexcept {
		dst.x = V.m256d_f64[0];
		dst.y = V.m256d_f64[1];
	}
	inline void AMStoreDouble3(AMDOUBLE3& dst, AMVECTOR& V) noexcept {
		dst.x = V.m256d_f64[0];
		dst.y = V.m256d_f64[1];
		dst.z = V.m256d_f64[2];
	}
	inline void AMStoreDouble4(AMDOUBLE4& dst, AMVECTOR& V) noexcept {
		/*dst.x = V.m256d_f64[0];
		dst.y = V.m256d_f64[1];
		dst.z = V.m256d_f64[2];
		dst.w = V.m256d_f64[3];*/
		_mm256_store_pd((double*)&dst, V);
	}
	inline void AMStoreDouble3x3(AMDOUBLE3X3& dst, AMMATRIX& M) noexcept {
		for (uint8_t i = 0; i < 3; i++)
			for (uint8_t j = 0; j < 3; j++)
				dst.m[i][j] = M.m[i].m256d_f64[j];
	}
	inline void AMStoreDouble4x3(AMDOUBLE4X3& dst, AMMATRIX& M) noexcept {
		double m[4][4]{};
		_mm256_store_pd(m[0], M.m[0]);
		for (uint8_t i = 0; i < 4; i++)
			for (uint8_t j = 0; j < 3; j++)
				dst.m[i][j] = m[i][j];
	}
	inline void AMStoreDouble3x4(AMDOUBLE3X4& dst, AMMATRIX& M) noexcept {
		double m[4][4]{};
		_mm256_store_pd(m[0], M.m[0]);
		for (uint8_t i = 0; i < 3; i++)
			for (uint8_t j = 0; j < 4; j++)
				dst.m[i][j] = m[i][j];
	}
	inline void AMStoreDouble4x4(AMDOUBLE4X4& dst, AMMATRIX& M) noexcept {
		double m[4][4]{};
		_mm256_store_pd(m[0], M.m[0]);
		for (uint8_t i = 0; i < 4; i++)
			for (uint8_t j = 0; j < 4; j++)
				dst.m[i][j] = m[i][j];
	}

	inline void AMStoreFloat2(AMFLOAT2& dst, FAMVECTOR& V) noexcept {
		dst.x = V.m128_f32[0];
		dst.y = V.m128_f32[1];
	}
	inline void AMStoreFloat3(AMFLOAT3& dst, FAMVECTOR& V) noexcept {
		dst.x = V.m128_f32[0];
		dst.y = V.m128_f32[1];
		dst.z = V.m128_f32[2];
	}
	inline void AMStoreFloat4(AMFLOAT4& dst, FAMVECTOR& V) noexcept {
		dst.x = V.m128_f32[0];
		dst.y = V.m128_f32[1];
		dst.z = V.m128_f32[2];
		dst.w = V.m128_f32[3];
	}
	inline void AMStoreFloat3x3(AMFLOAT3X3& dst, FAMMATRIX& M) noexcept {
		for (uint8_t i = 0; i < 3; i++)
			for (uint8_t j = 0; j < 3; j++)
				dst.m[i][j] = M.m[i].m128_f32[j];
	}
	inline void AMStoreFloat4x3(AMFLOAT4X3& dst, FAMMATRIX& M) noexcept {
		for (uint8_t i = 0; i < 4; i++)
			for (uint8_t j = 0; j < 3; j++)
				dst.m[i][j] = M.m[i].m128_f32[j];
	}
	inline void AMStoreFloat3x4(AMFLOAT3X4& dst, FAMMATRIX& M) noexcept {
		for (uint8_t i = 0; i < 3; i++)
			for (uint8_t j = 0; j < 4; j++)
				dst.m[i][j] = M.m[i].m128_f32[j];
	}
	inline void AMStoreFloat4x4(AMFLOAT4X4& dst, FAMMATRIX& M) noexcept {
		for (uint8_t i = 0; i < 4; i++)
			for (uint8_t j = 0; j < 4; j++)
				dst.m[i][j] = M.m[i].m128_f32[j];
	}

	// Store (new)
	inline AMDOUBLE2 AMStoreDouble2(AMVECTOR& V) noexcept {
		return { V.m256d_f64[0], V.m256d_f64[1] };
	}
	inline AMDOUBLE3 AMStoreDouble3(AMVECTOR& V) noexcept {
		return { V.m256d_f64[0], V.m256d_f64[1], V.m256d_f64[2] };
	}
	inline AMDOUBLE4 AMStoreDouble4(AMVECTOR& V) noexcept {
		return { V.m256d_f64[0], V.m256d_f64[1], V.m256d_f64[2], V.m256d_f64[3] };
	}
	inline AMDOUBLE3X3 AMStoreDouble3x3(AMMATRIX& M) noexcept {
		double m[4][4]{};
		_mm256_store_pd(m[0], M.m[0]);
		return {
			m[0][0], m[0][1], m[0][2],
			m[1][0], m[1][1], m[1][2],
			m[2][0], m[2][1], m[2][2]
		};
	}
	inline AMDOUBLE4X3 AMStoreDouble4x3(AMMATRIX& M) noexcept {
		double m[4][4]{};
		_mm256_store_pd(m[0], M.m[0]);
		return {
			m[0][0], m[0][1], m[0][2],
			m[1][0], m[1][1], m[1][2],
			m[2][0], m[2][1], m[2][2],
			m[3][0], m[3][1], m[3][2]
		};
	}
	inline AMDOUBLE3X4 AMStoreDouble3x4(AMMATRIX& M) noexcept {
		double m[4][4]{};
		_mm256_store_pd(m[0], M.m[0]);
		return {
			m[0][0], m[0][1], m[0][2], m[0][3],
			m[1][0], m[1][1], m[1][2], m[1][3],
			m[2][0], m[2][1], m[2][2], m[2][3]
		};
	}
	inline AMDOUBLE4X4 AMStoreDouble4x4(AMMATRIX& M) noexcept {
		double m[4][4]{};
		_mm256_store_pd(m[0], M.m[0]);
		return {
			m[0][0], m[0][1], m[0][2], m[0][3],
			m[1][0], m[1][1], m[1][2], m[1][3],
			m[2][0], m[2][1], m[2][2], m[2][3],
			m[3][0], m[3][1], m[3][2], m[3][3]
		};
	}

	// Forward decs
	inline FAMVECTOR FAMVector4Normalize(const FAMVECTOR& vec);
	inline FAMVECTOR FAMVector4DotProduct(const FAMVECTOR& vec0, const FAMVECTOR& vec1);
	inline FAMVECTOR FAMVector4CrossProduct(const FAMVECTOR& vec0, const FAMVECTOR& vec1);
	inline float FAMVector4SumVector(const FAMVECTOR& vec);
	inline float FAMVector4Magnitude(const FAMVECTOR& vec);
	inline FAMVECTOR FAMVector4Negate(const FAMVECTOR& vec);

	inline FAMMATRIX FAMMatrixIdentity() {
		return {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}
	inline FAMMATRIX FAMMatrixRotationRollPitchYaw(FAMVECTOR& vec) {
		FAMVECTOR cosvec{};
		FAMVECTOR sinvec{ _mm_sincos_ps(&cosvec, vec) };

		FAMMATRIX yRot{
			cosvec.m128_f32[1],    0.0,    sinvec.m128_f32[1],    0.0,
			0.0,                    1.0,    0.0,                    0.0,
			-sinvec.m128_f32[1],   0.0,    cosvec.m128_f32[1],    0.0,
			0.0,                    0.0,    0.0,                    1.0
		};
		FAMMATRIX xRot{
			1.0,	0.0,					0.0,                    0.0,
			0.0,	cosvec.m128_f32[0],    -sinvec.m128_f32[0],	0.0,
			0.0,	sinvec.m128_f32[0],	cosvec.m128_f32[0],	0.0,
			0.0,	0.0,					0.0,					1.0
		};
		FAMMATRIX zRot{
			cosvec.m128_f32[2],	-sinvec.m128_f32[2],	0.0,	0.0,
			sinvec.m128_f32[2],	cosvec.m128_f32[2],	0.0,	0.0,
			0.0,					0.0,					1.0,    0.0,
			0.0,					0.0,					0.0,	1.0
		};

		return { yRot * xRot * zRot };
	}
	inline FAMMATRIX FAMMatrixTranslation(FAMVECTOR& vec) {
		return {
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			vec.m128_f32[0], vec.m128_f32[1], vec.m128_f32[2], 1.0
		};
	}
	inline FAMVECTOR FAMMatrixDeterminant(FAMMATRIX& mx) {
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
		FAMVECTOR _0{ _mm_div_ps(mx.m[0], _mm_set_ps1(mx.m[0].m128_f32[0])) };
		FAMVECTOR _1{ _mm_mul_ps(_0, _mm_set_ps1(mx.m[1].m128_f32[0] / _0.m128_f32[0])) };
		FAMVECTOR _2{ _mm_sub_ps(mx.m[1], _1) };
		FAMVECTOR _3{ _mm_mul_ps(mx.m[0], _mm_set_ps1(mx.m[2].m128_f32[0] / mx.m[0].m128_f32[0])) };
		FAMVECTOR _4{ _mm_sub_ps(mx.m[2], _3) };
		FAMVECTOR _5{ _mm_mul_ps(mx.m[0], _mm_set_ps1(mx.m[3].m128_f32[0] / mx.m[0].m128_f32[0])) };
		FAMVECTOR _6{ _mm_sub_ps(mx.m[3], _5) };
		FAMVECTOR _7{ _mm_div_ps(_2, _mm_set_ps1(_2.m128_f32[1])) };
		FAMVECTOR _8{ _mm_mul_ps(_7, _mm_set_ps1(_4.m128_f32[1])) };
		FAMVECTOR _9{ _mm_sub_ps(_4, _8) };
		FAMVECTOR _10{ _mm_mul_ps(_7, _mm_set_ps1(_6.m128_f32[1])) };
		FAMVECTOR _11{ _mm_sub_ps(_6, _10) };
		FAMVECTOR _12{ _mm_div_ps(_9, _mm_set_ps1(_9.m128_f32[2])) };
		FAMVECTOR _13{ _mm_mul_ps(_12, _mm_set_ps1(_11.m128_f32[2])) };
		FAMVECTOR _14{ _mm_sub_ps(_11, _13) };
		FAMVECTOR result = AMLoadFloat4({ mx.m[0].m128_f32[0], _2.m128_f32[1], _9.m128_f32[2], _14.m128_f32[3] });
		return { AMLoadFloat4({mx.m[0].m128_f32[0], _2.m128_f32[1], _9.m128_f32[2], _14.m128_f32[3]}) };

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
	inline FAMMATRIX FAMMatrixPerspectiveFovLH(float fov, float ar, float n, float f) {
		float ySc = 1.f / std::tan(fov / 2.f);
		float dz = f - n;
		return {
			ySc / ar,		0.0,		0.0,		0.0,
			0.0,		ySc,		0.0,		0.0,
			0.0,		0.0,		f / dz,		1.0,
			0.0,		0.0,		-n * (f / dz),	0.0
		};
	}
	inline FAMMATRIX FAMMatrixLookAtLH(FAMVECTOR& eye, FAMVECTOR& focus, FAMVECTOR upDirection) {
		return { FAMMatrixIdentity() }; // implement this later
	}
	inline FAMMATRIX FAMMatrixLookToLH(FAMVECTOR& eye, FAMVECTOR& direction, FAMVECTOR upDirection) {
		FAMVECTOR F = direction;
		FAMVECTOR R = FAMVector4Normalize(FAMVector4CrossProduct(upDirection, F));
		FAMVECTOR U = FAMVector4CrossProduct(F, R);

		F = FAMVector4Normalize(F);
		R = FAMVector4Normalize(R);
		U = FAMVector4Normalize(U);

		float x{ -FAMVector4SumVector(FAMVector4DotProduct(R, eye)) };
		float y{ -FAMVector4SumVector(FAMVector4DotProduct(U, eye)) };
		float z{ -FAMVector4SumVector(FAMVector4DotProduct(F, eye)) };

		return {
			R.m128_f32[0],	U.m128_f32[0],	F.m128_f32[0],	0.0f,
			R.m128_f32[1],	U.m128_f32[1],	F.m128_f32[1],	0.0f,
			R.m128_f32[2],	U.m128_f32[2],	F.m128_f32[2],	0.0f,
			x,				y,				z,				1.0f,
		};
	}
	inline FAMVECTOR FAMVector4Transform(FAMVECTOR& vec, FAMMATRIX& mx) {
		FAMVECTOR res{};

		FAMMATRIX m2rot{
			mx.m[0].m128_f32[0], mx.m[1].m128_f32[0], mx.m[2].m128_f32[0], mx.m[3].m128_f32[0],
			mx.m[0].m128_f32[1], mx.m[1].m128_f32[1], mx.m[2].m128_f32[1], mx.m[3].m128_f32[1],
			mx.m[0].m128_f32[2], mx.m[1].m128_f32[2], mx.m[2].m128_f32[2], mx.m[3].m128_f32[2],
			mx.m[0].m128_f32[3], mx.m[1].m128_f32[3], mx.m[2].m128_f32[3], mx.m[3].m128_f32[3]
		};

		std::array<FAMVECTOR, 4u> a{ m2rot.m[0], m2rot.m[1], m2rot.m[2], m2rot.m[3] };

		FAMVECTOR mulRes{};

		for (int i = 0; i < 4; i++) {
			mulRes = _mm_mul_ps(vec, a[i]);
			for (int j = 0; j < 4; j++) {
				res.m128_f32[i] += mulRes.m128_f32[j];
			}
		}
		return res;
	}
	inline FAMMATRIX FAMMatrixTranspose(FAMMATRIX& mx) {
		return {
			mx.m[0].m128_f32[0], mx.m[1].m128_f32[0], mx.m[2].m128_f32[0], mx.m[3].m128_f32[0],
			mx.m[0].m128_f32[1], mx.m[1].m128_f32[1], mx.m[2].m128_f32[1], mx.m[3].m128_f32[1],
			mx.m[0].m128_f32[2], mx.m[1].m128_f32[2], mx.m[2].m128_f32[2], mx.m[3].m128_f32[2],
			mx.m[0].m128_f32[3], mx.m[1].m128_f32[3], mx.m[2].m128_f32[3], mx.m[3].m128_f32[3]
		};
	}
	inline FAMMATRIX FAMMatrixInverse(FAMVECTOR& det, FAMMATRIX& mx) {
		FAMMATRIX mxI{ FAMMatrixIdentity() };
		if (det.m128_f32[0] * det.m128_f32[1] * det.m128_f32[2] * det.m128_f32[3] == 0.0f)
			return mxI;
		//double a1{ mx.m[0].m256d_f64[0] }; double b1{ mx.m[0].m256d_f64[1] }; double c1{ mx.m[0].m256d_f64[2] }; double d1{ mx.m[0].m256d_f64[3] };
		//double e1{ mx.m[1].m256d_f64[0] }; double f1{ mx.m[1].m256d_f64[1] }; double g1{ mx.m[1].m256d_f64[2] }; double h1{ mx.m[1].m256d_f64[3] };
		//double i1{ mx.m[2].m256d_f64[0] }; double j1{ mx.m[2].m256d_f64[1] }; double k1{ mx.m[2].m256d_f64[2] }; double l1{ mx.m[2].m256d_f64[3] };
		//double m1{ mx.m[3].m256d_f64[0] }; double n1{ mx.m[3].m256d_f64[1] }; double o1{ mx.m[3].m256d_f64[2] }; double p1{ mx.m[3].m256d_f64[3] };
		FAMVECTOR _0{ _mm_div_ps(mx.m[0], _mm_set_ps1(mx.m[0].m128_f32[0])) };
		FAMVECTOR _1{ _mm_div_ps(mxI.m[0], _mm_set_ps1(mx.m[0].m128_f32[0])) };
		FAMVECTOR _2{ _mm_mul_ps(_0, _mm_set_ps1(mx.m[1].m128_f32[0])) };
		FAMVECTOR _3{ _mm_mul_ps(_1, _mm_set_ps1(mx.m[1].m128_f32[0])) };
		FAMVECTOR _4{ _mm_sub_ps(mx.m[1], _2) };
		FAMVECTOR _5{ _mm_sub_ps(mxI.m[1], _3) };
		FAMVECTOR _6{ _mm_mul_ps(_0, _mm_set_ps1(mx.m[2].m128_f32[0])) };
		FAMVECTOR _7{ _mm_mul_ps(_1, _mm_set_ps1(mx.m[2].m128_f32[0])) };
		FAMVECTOR _8{ _mm_sub_ps(mx.m[2], _6) };
		FAMVECTOR _9{ _mm_sub_ps(mxI.m[2], _7) };
		FAMVECTOR _10{ _mm_mul_ps(_0, _mm_set_ps1(mx.m[3].m128_f32[0])) };
		FAMVECTOR _11{ _mm_mul_ps(_1, _mm_set_ps1(mx.m[3].m128_f32[0])) };
		FAMVECTOR _12{ _mm_sub_ps(mx.m[3], _10) };
		FAMVECTOR _13{ _mm_sub_ps(mxI.m[3], _11) };
		FAMVECTOR _14{ _mm_div_ps(_4, _mm_set_ps1(_4.m128_f32[1])) };
		FAMVECTOR _15{ _mm_div_ps(_5, _mm_set_ps1(_4.m128_f32[1])) };
		FAMVECTOR _16{ _mm_mul_ps(_14, _mm_set_ps1(_0.m128_f32[1])) };
		FAMVECTOR _17{ _mm_mul_ps(_15, _mm_set_ps1(_0.m128_f32[1])) };
		FAMVECTOR _18{ _mm_sub_ps(_0, _16) };
		FAMVECTOR _19{ _mm_sub_ps(_1, _17) };
		FAMVECTOR _20{ _mm_mul_ps(_14, _mm_set_ps1(_8.m128_f32[1])) };
		FAMVECTOR _21{ _mm_mul_ps(_15, _mm_set_ps1(_8.m128_f32[1])) };
		FAMVECTOR _22{ _mm_sub_ps(_8, _20) };
		FAMVECTOR _23{ _mm_sub_ps(_9, _21) };
		FAMVECTOR _24{ _mm_mul_ps(_14, _mm_set_ps1(_12.m128_f32[1])) };
		FAMVECTOR _25{ _mm_mul_ps(_15, _mm_set_ps1(_12.m128_f32[1])) };
		FAMVECTOR _26{ _mm_sub_ps(_12, _24) };
		FAMVECTOR _27{ _mm_sub_ps(_13, _25) };
		FAMVECTOR _28{ _mm_div_ps(_22, _mm_set_ps1(_22.m128_f32[2])) };
		FAMVECTOR _29{ _mm_div_ps(_23, _mm_set_ps1(_22.m128_f32[2])) };
		FAMVECTOR _30{ _mm_mul_ps(_28, _mm_set_ps1(_18.m128_f32[2])) };
		FAMVECTOR _31{ _mm_mul_ps(_29, _mm_set_ps1(_18.m128_f32[2])) };
		FAMVECTOR _32{ _mm_sub_ps(_18, _30) };
		FAMVECTOR _33{ _mm_sub_ps(_19, _31) };
		FAMVECTOR _34{ _mm_mul_ps(_28, _mm_set_ps1(_14.m128_f32[2])) };
		FAMVECTOR _35{ _mm_mul_ps(_29, _mm_set_ps1(_14.m128_f32[2])) };
		FAMVECTOR _36{ _mm_sub_ps(_14, _34) };
		FAMVECTOR _37{ _mm_sub_ps(_15, _35) };
		FAMVECTOR _38{ _mm_mul_ps(_28, _mm_set_ps1(_26.m128_f32[2])) };
		FAMVECTOR _39{ _mm_mul_ps(_29, _mm_set_ps1(_26.m128_f32[2])) };
		FAMVECTOR _40{ _mm_sub_ps(_26, _38) };
		FAMVECTOR _41{ _mm_sub_ps(_27, _39) };
		FAMVECTOR _42{ _mm_div_ps(_40, _mm_set_ps1(_40.m128_f32[3])) };
		FAMVECTOR _43{ _mm_div_ps(_41, _mm_set_ps1(_40.m128_f32[3])) };
		FAMVECTOR _44{ _mm_mul_ps(_42, _mm_set_ps1(_32.m128_f32[3])) };
		FAMVECTOR _45{ _mm_mul_ps(_43, _mm_set_ps1(_32.m128_f32[3])) };
		FAMVECTOR _46{ _mm_sub_ps(_32, _44) };
		FAMVECTOR _47{ _mm_sub_ps(_33, _45) };
		FAMVECTOR _48{ _mm_mul_ps(_42, _mm_set_ps1(_36.m128_f32[3])) };
		FAMVECTOR _49{ _mm_mul_ps(_43, _mm_set_ps1(_36.m128_f32[3])) };
		FAMVECTOR _50{ _mm_sub_ps(_36, _48) };
		FAMVECTOR _51{ _mm_sub_ps(_37, _49) };
		FAMVECTOR _52{ _mm_mul_ps(_42, _mm_set_ps1(_28.m128_f32[3])) };
		FAMVECTOR _53{ _mm_mul_ps(_43, _mm_set_ps1(_28.m128_f32[3])) };
		FAMVECTOR _54{ _mm_sub_ps(_28, _52) };
		FAMVECTOR _55{ _mm_sub_ps(_29, _53) };

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
	inline FAMVECTOR FAMVector4Normalize(const FAMVECTOR& vec) {
		FAMVECTOR vecsqrd{ _mm_mul_ps(vec, vec) };
		float vecsqrt{ std::sqrtf(vecsqrd.m128_f32[0] + vecsqrd.m128_f32[1] + vecsqrd.m128_f32[2]) };
		return { _mm_div_ps(vec, _mm_set_ps1(vecsqrt)) };
	}
	inline FAMVECTOR FAMVector4DotProduct(const FAMVECTOR& vec0, const FAMVECTOR& vec1) {
		return { _mm_mul_ps(vec0, vec1) };
	}
	inline FAMVECTOR FAMVector4CrossProduct(const FAMVECTOR& vec0, const FAMVECTOR& vec1) {
		return { 
			_mm_sub_ps(
				_mm_mul_ps(AMLoadFloat3({ vec0.m128_f32[1], vec0.m128_f32[2], vec0.m128_f32[0] }),
					AMLoadFloat3({ vec1.m128_f32[2], vec1.m128_f32[0], vec1.m128_f32[1] })),
				_mm_mul_ps(AMLoadFloat3({ vec0.m128_f32[2], vec0.m128_f32[0], vec0.m128_f32[1] }),
					AMLoadFloat3({ vec1.m128_f32[1], vec1.m128_f32[2], vec1.m128_f32[0] }))
			)
		};
	}
	inline float FAMVector4SumVector(const FAMVECTOR& vec) {
		return { vec.m128_f32[0] + vec.m128_f32[1] + vec.m128_f32[2] + vec.m128_f32[3] };
	}
	inline float FAMVector4Magnitude(const FAMVECTOR& vec) {
		return { std::sqrtf(FAMVector4SumVector(_mm_mul_ps(vec, vec))) };
	}
	inline FAMVECTOR FAMVector4Negate(const FAMVECTOR& vec) {
		return { AMLoadFloat4({ -vec.m128_f32[0], -vec.m128_f32[1], -vec.m128_f32[2], -vec.m128_f32[3] }) };
	}

	/*AMMATRIX AMMatrixIdentity();
	AMMATRIX AMMatrixRotationRollPitchYaw(AMVECTOR& vec);
	AMMATRIX AMMatrixTranslation(AMVECTOR& vec);
	AMVECTOR AMMatrixDeterminant(AMMATRIX& mx);
	AMMATRIX AMMatrixPerspectiveFovLH(double fov, double ar, double n, double f);
	AMMATRIX AMMatrixLookAtLH(AMVECTOR& eye, AMVECTOR& focus, AMVECTOR upDirection);
	AMMATRIX AMMatrixLookToLH(AMVECTOR& eye, AMVECTOR& direction, AMVECTOR upDirection);
	AMVECTOR AMVector4Transform(AMVECTOR& vec, AMMATRIX& mx);
	AMMATRIX AMMatrixTranspose(AMMATRIX& mx);
	AMMATRIX AMMatrixInverse(AMVECTOR& det, AMMATRIX& mx);
	AMVECTOR AMVector4Normalize(const AMVECTOR& vec);
	AMVECTOR AMVector4DotProduct(const AMVECTOR& vec0, const AMVECTOR& vec1);
	AMVECTOR AMVector4CrossProduct(const AMVECTOR& vec0, const AMVECTOR& vec1);
	double AMVector4SumVector(const AMVECTOR& vec);*/
}

// Compute Shader CB struct
namespace GID::DSU {
	struct alignas(16) ComputeShaderTextureMetadataCB {
		uint32_t SrcMipLevel;
		uint32_t NumMipLevels;
		uint32_t SrcDimension;
		uint32_t isSRGB;
		AssistMath::AMFLOAT2 TexelSize;
	};
}

// Some basic DSU structs and enums (that don't depend on user-defined types)
namespace GID::DSU {
	enum class LightConst {
		MAX_LIGHTS = 8,
		DIRECTIONAL_LIGHT = 0,
		POINT_LIGHT = 1,
		SPOT_LIGHT = 2,
	};
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
		float constAtten{ 0.4f };
		float linAtten{ 0.08f };
		float quadAtten{ 0.0f };

		int32_t type{ (uint8_t)LightConst::POINT_LIGHT };
		int32_t isEnabled{ false };
		int32_t padding0{};
		int32_t padding1{};
	};
	struct VSInputData {
		uint32_t pos{};
		uint32_t tex{};
		uint32_t norm{};
	};
	struct MaterialData {
		struct {
			float r{};
			float g{};
			float b{};
			float a{};
		} colorEmissive, colorAmbient, colorDiffuse, colorSpecular;

		float specularPower{};
		int32_t isTextured{ false };
		int32_t padding0{};
		int32_t padding1{};
	};
	struct VertexConstantBufferData {
		AssistMath::FAMMATRIX transform{};
		AssistMath::FAMMATRIX camera{};
		AssistMath::FAMMATRIX projection{};
		AssistMath::FAMMATRIX invtpose{};
	};
	struct PixelConstantBufferData {
		MaterialData mtl{};
		AssistMath::AMFLOAT4 eyePos{};
		AssistMath::AMFLOAT4 globalAmbient{};
		std::array<LightData, (uint8_t)LightConst::MAX_LIGHTS> lights{};
	};
	struct Position {
		AssistMath::FAMVECTOR translation{};
		AssistMath::FAMVECTOR rotation{};
		AssistMath::FAMVECTOR center{};
	};
	struct Speed {
		AssistMath::FAMVECTOR deltaTranslation{};
		AssistMath::FAMVECTOR deltaRotation{};
	};
	enum class WindowType {
		MAINWINDOW,
		SUBWINDOW0,
		SUBWINDOW1,
		INVALID
	};
	struct GraphicsPipelineStateStream {
		CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
		CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
		CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopologyType;
		CD3DX12_PIPELINE_STATE_STREAM_VS VS;
		CD3DX12_PIPELINE_STATE_STREAM_PS PS;
		CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
		CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
		CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER RS;
	};
	struct ComputePipelineStateStream {
		CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
		CD3DX12_PIPELINE_STATE_STREAM_CS CS;
	};
	struct ObjectDrawStaticMatrices {
		AssistMath::FAMMATRIX camera;
		AssistMath::FAMMATRIX projection;
	};
	struct SphereCollisionCheckData {
		AssistMath::FAMVECTOR center{};
		float radius{};
	};
	enum class ActorGroundState {
		GROUND,
		AIR,
		WATER,
		INVALID
	};
	struct Timer {
		std::chrono::high_resolution_clock::time_point previous;
		Timer() { previous = std::chrono::steady_clock::now(); }
		float mark() {
			const auto before = previous;
			previous = std::chrono::steady_clock::now();
			const std::chrono::duration<float> frameDuration = previous - before;
			return frameDuration.count();
		}
		float peek() const { return std::chrono::duration<float>(std::chrono::steady_clock::now() - previous).count(); }
	};
}

//// Resource Structure
//namespace GID::DSU {
//	struct Resource {
//
//		ComPtr<ID3D12Resource2> mpResource{};
//		D3D12_FEATURE_DATA_FORMAT_SUPPORT mFormatSupport{};
//		std::unique_ptr<D3D12_CLEAR_VALUE> mpClearValue{};
//
//		Resource(ComPtr<ID3D12Device10>& pDevice, const D3D12_RESOURCE_DESC& desc, const std::unique_ptr<D3D12_CLEAR_VALUE> pClearValue = nullptr) {
//
//			// If a valid (not nullptr) clear value was passed through the parameters, then copy its values to the object's clear value
//			if (pClearValue) mpClearValue = std::make_unique<D3D12_CLEAR_VALUE>(*pClearValue);
//
//			// Create the committed resource
//			pDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COMMON, mpClearValue.get(), IID_PPV_ARGS(&mpResource));
//		}
//		Resource(ComPtr<ID3D12Device10>& pDevice, ComPtr<ID3D12Resource2>& pResource, const std::unique_ptr<D3D12_CLEAR_VALUE> pClearValue = nullptr) {
//			
//			// If a valid (not nullptr) clear value was passed through the parameters, then copy its values to the object's clear value
//			if (pClearValue) mpClearValue = std::make_unique<D3D12_CLEAR_VALUE>(*pClearValue);
//
//			mpResource = pResource;
//		}
//
//		D3D12_RESOURCE_DESC getDesc() {
//			[[likely]]
//			if (mpResource) return mpResource->GetDesc(); 
//			else {
//				std::wstringstream wss{};
//				wss << "[DEBUG WARNING] "
//					<< __FILE__ << "(" << __LINE__ << "): " 
//					<< "The descriptor of a resource was requested even though it was null.";
//				std::wcout << wss.str();
//				return D3D12_RESOURCE_DESC();
//			}
//		}
//	};
//}
//
//// Texture Structure
//namespace GID::DSU {
//	struct Texture {
//		Texture(ComPtr<ID3D12Device10>& pDevice, const D3D12_RESOURCE_DESC& desc, const std::unique_ptr<D3D12_CLEAR_VALUE> pClearValue = nullptr) {
//
//		}
//		Texture(ComPtr<ID3D12Device10>& pDevice, ComPtr<ID3D12Resource2>& pResource, const std::unique_ptr<D3D12_CLEAR_VALUE> pClearValue = nullptr) {
//
//		}
//
//		void resize(uint32_t width, uint32_t height, uint32_t depthOrArraySize = 1);
//		D3D12_CPU_DESCRIPTOR_HANDLE getRTV();
//		D3D12_CPU_DESCRIPTOR_HANDLE getDSV();
//		D3D12_CPU_DESCRIPTOR_HANDLE getSRV();
//		D3D12_CPU_DESCRIPTOR_HANDLE getUAV(uint32_t mip);
//		// eventual format support checking
//		/*bool checkSupportSRV() {
//		return CheckFormatSupport(D3D12_FORMAT_SUPPORT1_SHADER_SAMPLE);
//		}
//		bool checkSupportRTV() {
//		return CheckFormatSupport(D3D12_FORMAT_SUPPORT1_RENDER_TARGET);
//		}
//		bool checkSupportUAV() {
//		return CheckFormatSupport(D3D12_FORMAT_SUPPORT1_TYPED_UNORDERED_ACCESS_VIEW)
//		&& CheckFormatSupport(D3D12_FORMAT_SUPPORT2_UAV_TYPED_LOAD)
//		&& CheckFormatSupport(D3D12_FORMAT_SUPPORT2_UAV_TYPED_STORE);
//		}
//		bool checkSupportDSV() {
//		return CheckFormatSupport(D3D12_FORMAT_SUPPORT1_DEPTH_STENCIL);
//		}*/
//		bool hasAlpha();
//		size_t checkBBP();
//	};
//	inline bool isUAVCompatFormat(DXGI_FORMAT format);
//	inline bool isSRGBFormat(DXGI_FORMAT format);
//	inline bool isBGRFormat(DXGI_FORMAT format);
//	inline bool isDepthFormat(DXGI_FORMAT format);
//
//	inline DXGI_FORMAT getTypelessFormat(DXGI_FORMAT format);
//	inline DXGI_FORMAT getSRGBFormat(DXGI_FORMAT format);
//	inline DXGI_FORMAT getUAVCompatFormat(DXGI_FORMAT format);
//}

// Benchmark Function (only one per run)
namespace GID::Util::Benchmark {
	struct {
				DSU::Timer elapsedTime{};
				double total{};
				uint16_t runs{};
				bool init{ false };
			} gBenchmarkData;
	inline void doBenchmark(double dt, std::string type, double lengthInSec) {
				if (!gBenchmarkData.init) {
					gBenchmarkData.elapsedTime.mark();
					gBenchmarkData.init = true;
					return;
				}
				gBenchmarkData.total += dt * 1000.f;
				gBenchmarkData.runs++;
				if (gBenchmarkData.elapsedTime.peek() >= lengthInSec) {
					gBenchmarkData.elapsedTime.mark();
					std::ofstream file("benchmark.txt", std::ios::out | std::ios::app);
					file << '[' << type << "] " << gBenchmarkData.total / gBenchmarkData.runs << " ms\n";
					file.close();
				}
			}
}

// Benchmark Function (singleton ver.)
namespace GID::Util::Benchmark {
	inline void doBenchmark(double dt, std::string type) {
		std::ofstream file("benchmark.txt", std::ios::out | std::ios::app);
		file << '[' << type << "] " << dt * 1000.f << " ms\n";
		file.close();
	}
}

// Script IDs
namespace GID::DSU {
	enum class ScriptID {
		// Input Scripts
		BasicActorMove,

		// Update Scripts
		BasicCameraFollow,
		AdvancedCameraFollow,

		// Physics Scripts
		BasicGravity,
		BasicCollision,

		INVALID,
	};
}

// Scriptable Interface
namespace GID::DSU {
	struct Scriptable {
		std::vector<ScriptID> mScripts{};
		void addScript(ScriptID name) noexcept {
			std::vector<ScriptID>::iterator newend;
			newend = std::remove(mScripts.begin(), mScripts.end(), name);
			mScripts.push_back(name);
		}
		std::vector<ScriptID>& getScripts() noexcept { return mScripts; }
		void removeScript(ScriptID name) noexcept {
			std::vector<ScriptID>::iterator newend;
			newend = std::remove(mScripts.begin(), mScripts.end(), name);
		}
		void clearScripts() noexcept { mScripts.clear(); }
	};
}

// Inputtable Interface
namespace GID::DSU {
	struct Inputtable {
		virtual void input() noexcept = 0;
	};
}

// Keyboard
namespace GID::DSU {
	struct Keyboard {

		// Event Class
		struct Event {

			// Type of Event
			enum class Type {
				Press,
				Release,
				Invalid
			};

			// Fields
			Type			type = {};
			uint8_t			code = {};

			// Constructors
			Event() noexcept :
				type(Type::Invalid),
				code(0u) {}

			Event(Type type, uint8_t code) noexcept :
				type(type),
				code(code) {}

			// Getters/Setters/Is
			uint8_t getCode() const noexcept { return code; }
			bool isPress() const noexcept { return type == Type::Press; }
			bool isRelease() const noexcept { return type == Type::Release; }
			bool isValid() const noexcept { return type != Type::Invalid; }

		};

		// Fields
		static constexpr uint16_t		MKEYCOUNT{ 256u };
		static constexpr uint8_t		MBUFFERSIZE{ 16u };
		bool							mAutoRepeatEnabled{ false };
		std::bitset<MKEYCOUNT>			mKeyStates{};
		std::queue<Event>				mKeyBuffer{};
		std::queue<int8_t>				mCharBuffer{};

		static void trimBuffer(std::queue<Event>& buffer) noexcept {
			while (buffer.size() > MBUFFERSIZE) {
				buffer.pop();
			}
		}

		static void trimBuffer(std::queue<int8_t>& buffer) noexcept {
			while (buffer.size() > MBUFFERSIZE) {
				buffer.pop();
			}
		}

		// Part of the Window
		void onKeyPressed(uint8_t keycode) noexcept {
			mKeyStates[keycode] = true;
			mKeyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
			trimBuffer(mKeyBuffer);
		}
		void onKeyReleased(uint8_t keycode) noexcept {
			mKeyStates[keycode] = false;
			mKeyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
			trimBuffer(mKeyBuffer);
		}
		void onChar(int8_t character) noexcept {
			mCharBuffer.push(character);
			trimBuffer(mCharBuffer);
		}
		void clearState() noexcept {
			mKeyStates.reset();
		}

		// Constructors
		Keyboard() = default;

		// Key Event
		bool isKeyPressed(uint8_t keycode) const noexcept {
			return mKeyStates[keycode];
		}
		Event readKey() noexcept {
			if (mKeyBuffer.size() > 0u) {
				Keyboard::Event e = mKeyBuffer.front();
				mKeyBuffer.pop();
				return e;
			} else { return Keyboard::Event(); }
		}
		bool isKeyQueueEmpty() const noexcept {
			return mKeyBuffer.empty();
		}
		void clearKeyQueue() noexcept {
			mKeyBuffer = std::queue<Event>();
		}

		// Char Event
		int8_t readChar() noexcept {
			if (mCharBuffer.size() > 0u) {
				uint8_t charCode = mCharBuffer.front();
				mCharBuffer.pop();
				return charCode;
			} else return 0;
		}
		bool isCharQueueEmpty() const noexcept {
			return mCharBuffer.empty();
		}
		void clearCharQueue() noexcept {
			mCharBuffer = std::queue<int8_t>();
		}

		// Key and Char Event
		void clear() noexcept {
			clearKeyQueue();
			clearCharQueue();
		}

		// Auto-Repeat Controls
		void enableAutoRepeat() noexcept {
			mAutoRepeatEnabled = true;
		}
		void disableAutoRepeat() noexcept {
			mAutoRepeatEnabled = false;
		}
		bool isAutoRepeatEnabled() const noexcept {
			return mAutoRepeatEnabled;
		}

		// Operator Overloads
		//Keyboard& operator=(const Keyboard& kb) = delete;

	};
}

// Mouse
namespace GID::DSU {
	struct Mouse {

		// Event Class
		struct Event {

			enum class Type {

				LMBPress,
				LMBRelease,
				RMBPress,
				RMBRelease,
				WheelUp,
				WheelDown,
				Move,
				Enter,
				Exit,
				Invalid

			};

			Type	mType{};
			int16_t	mXPos{};
			int16_t	mYPos{};
			bool	mIsLMBPressed{};
			bool	mIsRMBPressed{};

			// Empty Constructor
			Event() noexcept :
				mType(Type::Invalid),
				mXPos(0),
				mYPos(0),
				mIsLMBPressed(false),
				mIsRMBPressed(false) {}

			// Expected Constructor
			Event(Type type, const Mouse& mouse) noexcept :
				mType(type),
				mXPos(mouse.mXPos),
				mYPos(mouse.mYPos),
				mIsLMBPressed(mouse.mIsLMBPressed),
				mIsRMBPressed(mouse.mIsRMBPressed) {}

			// Getters/Setters/Is
			Type getType() const noexcept { return mType; }
			bool isLMBPressed() const noexcept { return mIsLMBPressed; }
			bool isRMBPressed() const noexcept { return mIsRMBPressed; }
			int16_t getX() const noexcept { return mXPos; }
			int16_t getY() const noexcept { return mYPos; }
			bool isValid() const noexcept { return mType != Type::Invalid; }
			std::pair<int16_t, int16_t> getPos() const noexcept { return { mXPos, mYPos }; }

		};

		static constexpr uint16_t		MBUFFERSIZE{ USHRT_MAX };
		int16_t							mXPos{};
		int16_t							mYPos{};
		int16_t							mWheelDelta{ 0 };
		bool							mIsLMBPressed{ false };
		bool							mIsRMBPressed{ false };
		bool							mIsInsideWindow{ false };
		std::queue<Event>				mBuffer{};

		void onMouseMove(int16_t x, int16_t y) noexcept {
			mXPos = x;
			mYPos = y;

			mBuffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
			trimBuffer();
		}
		void onMouseEnter() noexcept {
			mIsInsideWindow = true;
			mBuffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
			trimBuffer();
		}
		void onMouseExit() noexcept {
			mIsInsideWindow = false;
			mBuffer.push(Mouse::Event(Mouse::Event::Type::Exit, *this));
			trimBuffer();
		}
		void onLMBPressed(int16_t x, int16_t y) noexcept {
			mIsLMBPressed = true;
			mBuffer.push(Mouse::Event(Mouse::Event::Type::LMBPress, *this));
			trimBuffer();
		}
		void onLMBReleased(int16_t x, int16_t y) noexcept {
			mIsLMBPressed = false;
			mBuffer.push(Mouse::Event(Mouse::Event::Type::LMBRelease, *this));
			trimBuffer();
		}
		void onRMBPressed(int16_t x, int16_t y) noexcept {
			mIsRMBPressed = true;
			mBuffer.push(Mouse::Event(Mouse::Event::Type::RMBPress, *this));
			trimBuffer();
		}
		void onRMBReleased(int16_t x, int16_t y) noexcept {
			mIsRMBPressed = false;
			mBuffer.push(Mouse::Event(Mouse::Event::Type::RMBRelease, *this));
			trimBuffer();
		}
		void onWheelUp(int16_t x, int16_t y) noexcept {
			mBuffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
			trimBuffer();
		}
		void onWheelDown(int16_t x, int16_t y) noexcept {
			mBuffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
			trimBuffer();
		}
		void onWheelDelta(int16_t x, int16_t y, int16_t delta) noexcept {
			mWheelDelta += delta;
			while (mWheelDelta >= WHEEL_DELTA) {
				mWheelDelta -= WHEEL_DELTA;
				onWheelUp(x, y);
			}
			while (mWheelDelta <= -(WHEEL_DELTA)) {
				mWheelDelta += WHEEL_DELTA;
				onWheelDown(x, y);
			}
		}
		void trimBuffer() noexcept {
			while (mBuffer.size() > MBUFFERSIZE) {
				mBuffer.pop();
			}
		}

		// Default Constructor
		Mouse() = default;

		// Getters/Setters/Is
		int16_t getX() const noexcept { return mXPos; }
		int16_t getY() const noexcept { return mYPos; }
		int16_t getWheelDelta() const noexcept { return mWheelDelta; }
		bool isLMBPressed() const noexcept { return mIsLMBPressed; }
		bool isRMBPressed() const noexcept { return mIsRMBPressed; }
		std::pair<int16_t, int16_t> getPos() const noexcept { return { mXPos,mYPos }; }
		bool isEmpty() const noexcept { return mBuffer.empty(); }

		// Read the Event
		Mouse::Event readEvent() noexcept {
			if (mBuffer.size() > 0u) {
				Mouse::Event e = mBuffer.front();
				mBuffer.pop();
				return e;
			} else { return Mouse::Event(); }
		}

		// Empty the Buffer
		void clear() noexcept { mBuffer = std::queue<Event>(); }

		// Is the Mouse in the Window
		bool isInsideWindow() const noexcept { return mIsInsideWindow; }

	};
}

// Packed Input
namespace GID::DSU {
	struct PackedInput {
		Keyboard kb{};
		std::vector<Keyboard::Event> keys{};
		std::vector<uint8_t> keysChar{};
		Mouse mouse{};
		std::vector<Mouse::Event> mouseEvents{};
	};
}

// General Global State (parsed from general cfg file)
namespace GID::GSO::General {
	struct {
		bool gIsWindowResized{};
		float gCurrentFrameDeltaTime{};
	} gGenVar;
	struct {
		float gMaxFPS{ 0.0f };
		float gFPSCap{ 60.0f };
		float gCameraAngle{ 70.0f };
		float gSpeedMultiplier{ 1.0f };
	} gCfgGen;
}

// Global Per-frame Packed Input Vector
namespace GID::GSO::Input {
	inline GID::DSU::PackedInput gInput{};
}

// Global Window Count
namespace GID::GSO::WindowNS {
	inline uint8_t windowCt{};
}

// Window Struct
namespace GID::DSU {
	struct Window {

		const std::wstring	wndClassName{ L"AssistEngine_ver0.6_indev" };
		HINSTANCE			hInst{};
		uint8_t				WindowID{};
		unsigned int		mWidth{};
		unsigned int		mHeight{};
		HWND				mhWnd{};
		//RAWINPUTDEVICE	mRID{};

		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			if (msg == WM_NCCREATE) {
				const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
				Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
				SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
				SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
				return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
			}
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
		}
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
			using namespace GID::GSO::Input;
			switch (msg) {

				// Close Window
				case WM_CLOSE:
					PostQuitMessage(0);
					return 0;

				// ---- KEYBOARD MESSAGES ---- //
					
				// When Key is Pressed
				case WM_KEYDOWN:
					if (!(lParam & 0x40000000) || gInput.kb.isAutoRepeatEnabled()) {
						gInput.kb.onKeyPressed((uint8_t)wParam);
					}
					break;

				// When System Key is Pressed
				case WM_SYSKEYDOWN:
					if (!(lParam & 0x40000000) || gInput.kb.isAutoRepeatEnabled()) {
						gInput.kb.onKeyPressed((uint8_t)wParam);
					}
					break;

				// When Key is Released
				case WM_KEYUP:
					gInput.kb.onKeyReleased((uint8_t)wParam);
					break;

				// When System Key is Released
				case WM_SYSKEYUP:
					gInput.kb.onKeyReleased((uint8_t)wParam);
					break;

				// The Value of the Pressed Key
				case WM_CHAR:
					gInput.kb.onChar((uint8_t)wParam);
					break;

				// ---- MOUSE MESSAGES ---- //

				/*case WM_INPUT: {
					UINT dwSize = sizeof(RAWINPUT);
					static BYTE lpb[sizeof(RAWINPUT)];

					GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

					RAWINPUT* raw = (RAWINPUT*)lpb;

					if (raw->header.dwType == RIM_TYPEMOUSE) {
						mouse.OnMouseMove(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
					}
					break;
				}*/

				// When the Mouse is Moved
				case WM_MOUSEMOVE: {
					const POINTS point = MAKEPOINTS(lParam);

					// If mouse is inside window
					if (point.x >= 0
						&& point.x < (int16_t)mWidth
						&& point.y >= 0
						&& point.y < (int16_t)mHeight) {
						gInput.mouse.onMouseMove(point.x, point.y);
						if (!gInput.mouse.isInsideWindow()) {
							SetCapture(hWnd);
							gInput.mouse.onMouseEnter();
						}
					}

					// Else (mouse is outside window)
					else {

						// If LMB or RMB is pressed
						if (gInput.mouse.isLMBPressed() || gInput.mouse.isRMBPressed()) {
							gInput.mouse.onMouseMove(point.x, point.y);
						}

						// Else (LMB and RMB not pressed)
						else {
							ReleaseCapture();
							gInput.mouse.onMouseExit();
						}
					}
					break;
				}

				// When the LeftMouseButton is Pressed
				case WM_LBUTTONDOWN: {
					const POINTS point = MAKEPOINTS(lParam);
					gInput.mouse.onLMBPressed(point.x, point.y);
					break;
				}

				// When the LeftMouseButton is Released
				case WM_LBUTTONUP: {
					const POINTS point = MAKEPOINTS(lParam);
					gInput.mouse.onLMBReleased(point.x, point.y);
					break;
				}

				// When the RightMouseButton is Pressed
				case WM_RBUTTONDOWN: {
					const POINTS point = MAKEPOINTS(lParam);
					gInput.mouse.onRMBPressed(point.x, point.y);
					break;
				}

				// When the RightMouseButton is Released
				case WM_RBUTTONUP: {
					const POINTS point = MAKEPOINTS(lParam);
					gInput.mouse.onRMBReleased(point.x, point.y);
					break;
				}

				// When the Mouse Wheel is Moved
				case WM_MOUSEWHEEL: {
					const POINTS point = MAKEPOINTS(lParam);
					const int16_t delta = GET_WHEEL_DELTA_WPARAM(wParam);
					gInput.mouse.onWheelDelta(point.x, point.y, delta);
					break;
				}

				// ---- MISCELLANEOUS MESSAGES ---- //

				// Losing Focus On Window
				case WM_KILLFOCUS:
					gInput.kb.clearState();
					break;
				case WM_SIZE: {
					WINDOWINFO wndInfo{ getWindowInfo() };
					mWidth = wndInfo.rcClient.right - wndInfo.rcClient.left;
					mHeight = wndInfo.rcClient.bottom - wndInfo.rcClient.top;
					GID::GSO::General::gGenVar.gIsWindowResized = true;
					break;
				}
			}
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		Window() = default;
		Window(uint16_t width, uint16_t height, const TCHAR* name) :
			mWidth(width),
			mHeight(height) {

			WindowID = GID::GSO::WindowNS::windowCt;

			// Window Class
			WNDCLASSEX wcex{};
			wcex.cbSize = sizeof(wcex);
			wcex.style = CS_OWNDC;
			wcex.lpfnWndProc = HandleMsgSetup;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = hInst;
			wcex.hIcon = nullptr;
			wcex.hCursor = nullptr;
			wcex.hbrBackground = nullptr;
			wcex.lpszMenuName = nullptr;
			wcex.lpszClassName = wndClassName.c_str();
			wcex.hIconSm = nullptr;

			// Register Window
			RegisterClassEx(&wcex);

			RECT rc = {};
			rc.left = 100;
			rc.right = width + rc.left;
			rc.top = 100;
			rc.bottom = height + rc.top;
			AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

			mhWnd = CreateWindow(
				wndClassName.c_str(),
				name,
				WS_OVERLAPPEDWINDOW,
				(1920 / 2) - ((rc.right - rc.left) / 2),
				(1080 / 2) - ((rc.bottom - rc.top) / 2),
				rc.right - rc.left,
				rc.bottom - rc.top,
				nullptr,
				nullptr,
				hInst,
				this
			);

			// Show the window, since it will be hidden on creation.
			ShowWindow(mhWnd, SW_SHOWDEFAULT);

			/*mRID.usUsagePage = HID_USAGE_PAGE_GENERIC;
			mRID.usUsage = HID_USAGE_GENERIC_MOUSE;
			mRID.dwFlags = RIDEV_INPUTSINK;
			mRID.hwndTarget = mhWnd;
			RegisterRawInputDevices(&mRID, 1u, sizeof(mRID));*/

		}
		//~Window() { DestroyWindow(mhWnd); }

		void setTitle(const std::wstring& title) { SetWindowText(mhWnd, title.c_str()); }
		static std::optional<WPARAM> handleMessages() {
			MSG msg;
			while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) return msg.wParam;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			return {}; // empty optional
		}
		WINDOWINFO getWindowInfo() noexcept {
			WINDOWINFO wndInfo = {};
			wndInfo.cbSize = sizeof(WINDOWINFO);
			GetWindowInfo(mhWnd, &wndInfo);
			return wndInfo;
		}
		HWND& getHandle() noexcept { return mhWnd; }
		float getAspectRatio() noexcept { return (float)mWidth / (float)mHeight; }
	};
}

// Window Global State
namespace GID::GSO::WindowNS {
	inline std::shared_ptr<GID::DSU::Window> gWnd{};
	inline void initWindow(UINT w, UINT h, const TCHAR* name) {
		gWnd = std::make_shared<GID::DSU::Window>(w, h, name);
	}
}

// Viewport Preset Enum
namespace GID::DSU {
	enum class ViewportPreset {
		VP1_DEFAULT,
		VP2_HORIZONTAL,
		VP2_VERTICAL,
		VP4_CORNER,
		INVALID
	};
}

// Viewport Preset Array
namespace GID::DSU {
	struct VIEWPORT_DESC {
		std::vector<D3D12_VIEWPORT> vps{};
	};
}

// Viewport Preset Container and Initialization
namespace GID::GSO::Render::Viewport {
	inline std::array<GID::DSU::VIEWPORT_DESC, 4u> ViewportPresets;
	inline void initViewportPresets() {
		RECT wnd{ WindowNS::gWnd.get()->getWindowInfo().rcClient };
		// VP1_DEFAULT
		ViewportPresets[0] = {
			{
				{
					CD3DX12_VIEWPORT(
						0.0f,
						0.0f,
						(FLOAT)wnd.right - (FLOAT)wnd.left,
						(FLOAT)wnd.bottom - (FLOAT)wnd.top
					)
				}
			}
		};
		// VP2_HORIZONTAL
		ViewportPresets[1] = {
			{
				{
					CD3DX12_VIEWPORT(
					0.0f,
					0.0f,
					((FLOAT)wnd.right - (FLOAT)wnd.left) / 2,
					(FLOAT)wnd.bottom - (FLOAT)wnd.top
					)
				},
				{
					CD3DX12_VIEWPORT(
						((FLOAT)wnd.right - (FLOAT)wnd.left) / 2,
						0.0f,
						((FLOAT)wnd.right - (FLOAT)wnd.left) / 2,
						(FLOAT)wnd.bottom - (FLOAT)wnd.top
					)
				}
			}
		};
		// VP2_VERTICAL
		ViewportPresets[2] = {
			{
				{
					CD3DX12_VIEWPORT(
					0.0f,
					0.0f,
					(FLOAT)wnd.right - (FLOAT)wnd.left,
					((FLOAT)wnd.bottom - (FLOAT)wnd.top) / 2
					)
				},
				{
					CD3DX12_VIEWPORT(
						0.0f,
						((FLOAT)wnd.bottom - (FLOAT)wnd.top) / 2,
						(FLOAT)wnd.right - (FLOAT)wnd.left,
						((FLOAT)wnd.bottom - (FLOAT)wnd.top) / 2
					)
				}
			}
		};
		// VP4_CORNER
		ViewportPresets[3] = {
			{
				{
					CD3DX12_VIEWPORT(
					0.0f,
					0.0f,
					((FLOAT)wnd.right - (FLOAT)wnd.left) / 2,
					((FLOAT)wnd.bottom - (FLOAT)wnd.top) / 2
					)
				},
				{
					CD3DX12_VIEWPORT(
						((FLOAT)wnd.right - (FLOAT)wnd.left) / 2,
						0.0f,
						((FLOAT)wnd.right - (FLOAT)wnd.left) / 2,
						((FLOAT)wnd.bottom - (FLOAT)wnd.top) / 2
					)
				},
				{
					CD3DX12_VIEWPORT(
						0.0f,
						((FLOAT)wnd.bottom - (FLOAT)wnd.top) / 2,
						((FLOAT)wnd.right - (FLOAT)wnd.left) / 2,
						((FLOAT)wnd.bottom - (FLOAT)wnd.top) / 2
					)
				},
				{
					CD3DX12_VIEWPORT(
						((FLOAT)wnd.right - (FLOAT)wnd.left) / 2,
						((FLOAT)wnd.bottom - (FLOAT)wnd.top) / 2,
						((FLOAT)wnd.right - (FLOAT)wnd.left) / 2,
						((FLOAT)wnd.bottom - (FLOAT)wnd.top) / 2
					)
				}
			}
		};
	}
}

// GFX Descriptor
namespace GID::DSU {
	struct GFX_DESC {
		VIEWPORT_DESC viewportDesc;
	};
}

// Global Timer
namespace GID::GSO::General {
	inline DSU::Timer gGlobalTimer{};
}

// Global Tick Timer
namespace GID::GSO::Update {
	inline DSU::Timer gTickTimer{};
}

// Global Tick
namespace GID::GSO::Update {
	inline float gTicks{};
}

// Initialize the update cycle (record ticks passed)
namespace GID::GSO::Update {
	inline void initUpdateCycle() {
		gTicks = gTickTimer.mark() * General::gCfgGen.gSpeedMultiplier;
	}
}

// Camera Container
namespace GID::DSU {
	struct Camera : Scriptable, Inputtable {

		enum Rotation { Pitch, Yaw, Roll };
		enum Translation { X, Y, Z };
		enum FollowMode { NO_FOLLOW, FOLLOW };

		AssistMath::FAMVECTOR Y_AXIS{ 0.0f, 1.0f, 0.0f, 0.0f };
		AssistMath::FAMVECTOR mEye{ 100.0f, 100.0f, 100.0f, 1.0f };
		AssistMath::FAMVECTOR mFocus{};
		AssistMath::FAMVECTOR mRotation{-30.0f, -135.0f, 0.0f, 0.0f };
		AssistMath::FAMVECTOR mDTranslation{};

		bool mbMouseControl{ false };
		bool mbFollow{ false };

		Camera() = default;

		AssistMath::FAMMATRIX getMatrix() noexcept {
			using namespace AssistMath;
			//AMVECTOR focus = DX::XMVectorSet(mFocus.x, mFocus.y, mFocus.z, 1.0f);
			FAMVECTOR rotationRad{ _mm_mul_ps(mRotation, _mm_set_ps1((float)AM_PI / 180.0f)) };
			FAMVECTOR cosvec{};
			FAMVECTOR sinvec{ _mm_sincos_ps(&cosvec, rotationRad) };
			FAMVECTOR look{
				sinvec.m128_f32[Rotation::Yaw] * cosvec.m128_f32[Rotation::Pitch],
				sinvec.m128_f32[Rotation::Pitch],
				cosvec.m128_f32[Rotation::Yaw] * cosvec.m128_f32[Rotation::Pitch],
				0.0f
			};
			return { FAMMatrixLookToLH(mEye, look, Y_AXIS) };
		}
		void addPitch(float delta) noexcept {
			mRotation.m128_f32[Rotation::Pitch] += delta;
			if (mRotation.m128_f32[Rotation::Pitch] > 90.0f) mRotation.m128_f32[Rotation::Pitch] = 89.99999f;
			if (mRotation.m128_f32[Rotation::Pitch] < -90.0f) mRotation.m128_f32[Rotation::Pitch] = -89.99999f;
		}
		void addYaw(float delta) noexcept {
			mRotation.m128_f32[Rotation::Yaw] -= delta;
			while (mRotation.m128_f32[Rotation::Yaw] > 360) mRotation.m128_f32[Rotation::Yaw] -= 360;
			while (mRotation.m128_f32[Rotation::Yaw] < -360) mRotation.m128_f32[Rotation::Yaw] += 360;
		}
		void translate(AssistMath::FAMVECTOR& translation) noexcept {
			mEye = _mm_add_ps(mEye, translation);
			mFocus = _mm_add_ps(mFocus, translation);
		}
		void input() noexcept override {

			using namespace GSO::Input; using namespace AssistMath;

			// If the camera is not set to follow an actor
			if (!mbFollow) {
				
				// Determine if shift is being pressed
				BOOL shift = gInput.kb.isKeyPressed(VK_SHIFT);

				// Increment shift (shift should now equal 2 if pressing shift, 1 if not)
				shift++;

				// Determine if control is being pressed
				BOOL control = gInput.kb.isKeyPressed(VK_CONTROL);

				// Increment control (control should now equal 2 if pressing control, 1 if not)
				control++;

				// If 'W' is pressed, move the camera forward along its facing vector (only in the X-Z plane)
				if (gInput.kb.isKeyPressed('W')) {
					
					// Calculate the movement's x-component based on the facing angle
					float xcom = std::sin(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw]));

					// Calculate the movement's z-component based on the facing angle
					float zcom = std::cos(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw]));

					// Update the camera's delta-x to the x-component of the movement
					mDTranslation.m128_f32[Translation::X] += 25.f * xcom * shift;

					// Update the camera's delta-z to the x-component of the movement
					mDTranslation.m128_f32[Translation::Z] += 25.f * zcom * shift;
				}
				
				// If 'S' is pressed, move the camera backward along its facing vector (only in the X-Z plane)
				if (gInput.kb.isKeyPressed('S')) {
					
					// Calculate the movement's x-component based on the facing angle
					float xcom = std::sin(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw]));
					
					// Calculate the movement's z-component based on the facing angle
					float zcom = std::cos(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw]));
					mDTranslation.m128_f32[Translation::X] += 25.f * -xcom * shift;
					mDTranslation.m128_f32[Translation::Z] += 25.f * -zcom * shift;
				}
				// If 'A' is pressed, move the camera left along its facing vector (only in the X-Z plane)
				if (gInput.kb.isKeyPressed('A')) {
					float xcom = std::sin(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw] - 90.0f));
					float zcom = std::cos(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw] - 90.0f));
					mDTranslation.m128_f32[Translation::X] += 25.f * xcom * shift;
					mDTranslation.m128_f32[Translation::Z] += 25.f * zcom * shift;
				}
				// If 'D' is pressed, move the camera right along its facing vector (only in the X-Z plane)
				if (gInput.kb.isKeyPressed('D')) {
					float xcom = std::sin(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw] + 90.0f));
					float zcom = std::cos(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw] + 90.0f));
					mDTranslation.m128_f32[Translation::X] += 25.f * xcom * shift;
					mDTranslation.m128_f32[Translation::Z] += 25.f * zcom * shift;
				}

				// If the spacebar is pressed, move the camera forward along the world's Y-axis vector
				if (gInput.kb.isKeyPressed(VK_SPACE)) mDTranslation.m128_f32[Translation::Y] += 25.f * shift;

				// If tab is pressed, move the camera backward along the world's Y-axis vector
				if (gInput.kb.isKeyPressed(VK_TAB)) mDTranslation.m128_f32[Translation::Y] -= 25.f * shift;

				// If any of the arrow keys are pressed, move the camera's facing angle accordingly
				if (gInput.kb.isKeyPressed(VK_LEFT)) addYaw(0.25f * control);
				if (gInput.kb.isKeyPressed(VK_RIGHT)) addYaw(-0.25f * control);
				if (gInput.kb.isKeyPressed(VK_UP)) addPitch(0.25f * control);
				if (gInput.kb.isKeyPressed(VK_DOWN)) addPitch(-0.25f * control);

				// control the camera with the mouse (debug only -- use arrow keys instead)
				
				[[likely]]
				if (true) {
					RECT rc{ GSO::WindowNS::gWnd->getWindowInfo().rcClient };
					if (mbMouseControl) {
						for (auto& e : gInput.mouseEvents) {
							if (e.getType() == Mouse::Event::Type::Move) {
								auto centerX = (rc.right - rc.left) / 2;
								auto centerY = (rc.bottom - rc.top) / 2;
								if (e.getX() != centerX || e.getY() != centerY) SetCursorPos(centerX + rc.left, centerY + rc.top);
								auto dx = centerX - e.getX();
								auto dy = centerY - e.getY();
								addYaw(0.05f * dx * control);
								addPitch(0.05f * dy * control);
							}
						}
					}
					for (auto& e : gInput.mouseEvents) if (e.getType() == Mouse::Event::Type::LMBPress) {
						mbMouseControl = !mbMouseControl;
						ShowCursor(!mbMouseControl);
						if (mbMouseControl) ClipCursor(&rc);
						else { ClipCursor(NULL); }
						auto centerX = (rc.right - rc.left) / 2;
						auto centerY = (rc.bottom - rc.top) / 2;
						SetCursorPos(centerX + rc.left, centerY + rc.top);
					}
				}
			}
		}
			
		void update() noexcept {
			using namespace GSO::Update;
			if (!mbFollow) {
				AssistMath::FAMVECTOR vel = _mm_mul_ps(mDTranslation, _mm_set_ps1(gTicks));
				translate(vel);
				//std::cout << "[X] " << mDTranslation.m128_f32[X] << " " << " [Y] " << mDTranslation.m128_f32[Y] << " " << " [Z] " << mDTranslation.m128_f32[Z] << '\n';
				/*if (mDTranslation.m128_f32[Translation::X] < 0.0001f
					&& mDTranslation.m128_f32[Translation::X] > -0.0001f)
					mDTranslation.m128_f32[Translation::X] = 0.0f;

				else mDTranslation.m128_f32[Translation::X] /= 1.02f;

				if (mDTranslation.m128_f32[Translation::Y] < 0.0001f
					&& mDTranslation.m128_f32[Translation::Y] > -0.0001f)
					mDTranslation.m128_f32[Translation::Y] = 0.0f;

				else mDTranslation.m128_f32[Translation::Y] /= 1.02f;

				if (mDTranslation.m128_f32[Translation::Z] < 0.0001f
					&& mDTranslation.m128_f32[Translation::Z] > -0.0001f)
					mDTranslation.m128_f32[Translation::Z] = 0.0f;

				else mDTranslation.m128_f32[Translation::Z] /= 1.02f;*/

				mDTranslation.m128_f32[0] = 0.0f;
				mDTranslation.m128_f32[1] = 0.0f;
				mDTranslation.m128_f32[2] = 0.0f;
				mDTranslation.m128_f32[3] = 0.0f;

			}
		}
		void setToFollow() {
			mbFollow = true;
		}
		void setToControl() {
			mbFollow = false;
		}
	};
}

// Graphics Pipeline Object (2D)
namespace GID::DSU {
	struct GFX2D {

		HRESULT hr{};

		ComPtr<ID3D11On12Device2> mpD11on12Device{};
		ComPtr<ID3D11DeviceContext4> mp3DDeviceContext{};

		ComPtr<ID2D1Factory7> mpFactory{};
		ComPtr<ID2D1Device6> mpDevice{};
		ComPtr<ID2D1DeviceContext6> mp2DDeviceContext{};
		
		std::vector<ComPtr<ID3D11Resource>> mpWrappedRenderTargets{};
		std::vector<ComPtr<ID2D1Bitmap1>> mpBitmaps{};

		ComPtr<IDWriteFactory7> mpWriteFactory{};
		std::vector<ComPtr<ID2D1SolidColorBrush>> mpBrushes{};
		std::vector<ComPtr<IDWriteTextFormat3>> mpFormats{};

		std::queue<std::wstring> mDebugQueue{};

		uint8_t mFrameIndex{};

		GFX2D() = default;
		GFX2D(ComPtr<ID3D12Device10>& pDevice, ComPtr<ID3D12CommandQueue>& pCommandQueue, std::vector<ComPtr<ID3D12Resource2>>& pRenderTargets) {
			createDeviceAndContext(pDevice, pCommandQueue);
			createWriteFactory();
			wrapRenderTargets(pRenderTargets, getDPI());
		}
		inline void createDeviceAndContext(ComPtr<ID3D12Device10>& pDevice, ComPtr<ID3D12CommandQueue>& pCommandQueue) {
			ComPtr<ID3D11Device5> pD11Device{};
			std::array<D3D_FEATURE_LEVEL, 1u> fl{};
			fl.at(0) = D3D_FEATURE_LEVEL_12_1;
			hr = D3D11On12CreateDevice(pDevice.Get(), D3D11_CREATE_DEVICE_BGRA_SUPPORT, fl.data(), (uint32_t)fl.size(), (IUnknown**)pCommandQueue.GetAddressOf(),
				1u, 0u, (ID3D11Device**)pD11Device.GetAddressOf(), (ID3D11DeviceContext**)mp3DDeviceContext.GetAddressOf(), nullptr);
			hr = pD11Device.As(&mpD11on12Device);
		}
		inline void createWriteFactory() {
			D2D1_FACTORY_OPTIONS d2dfo{};
			d2dfo.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
			hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory7), &d2dfo, &mpFactory);
			ComPtr<IDXGIDevice4> pDXGIDevice{};
			hr = mpD11on12Device.As(&pDXGIDevice);
			hr = mpFactory->CreateDevice(pDXGIDevice.Get(), &mpDevice);
			D2D1_DEVICE_CONTEXT_OPTIONS dco{ D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS };
			hr = mpDevice->CreateDeviceContext(dco, &mp2DDeviceContext);
			DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory7), &mpWriteFactory);
		}
		inline float getDPI() {
			return (float)GetDpiForWindow(GID::GSO::WindowNS::gWnd.get()->mhWnd);
		}
		inline void wrapRenderTargets(std::vector<ComPtr<ID3D12Resource2>>& pRenderTargets, float dpi) {
			D2D1_BITMAP_PROPERTIES1 bmprop{ D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_R16G16B16A16_FLOAT, D2D1_ALPHA_MODE_IGNORE), dpi, dpi) };
			D3D11_RESOURCE_FLAGS resFlags{ D3D11_BIND_RENDER_TARGET };

			mpWrappedRenderTargets.resize(pRenderTargets.size());
			mpBitmaps.resize(pRenderTargets.size());

			ComPtr<IDXGISurface> pSurface{};

			for (uint8_t iter = 0; iter < pRenderTargets.size(); iter++) {
				hr = mpD11on12Device->CreateWrappedResource(pRenderTargets.at(iter).Get(), &resFlags, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT,
					IID_PPV_ARGS(&mpWrappedRenderTargets.at(iter)));
				hr = mpWrappedRenderTargets.at(iter).As(&pSurface);
				hr = mp2DDeviceContext->CreateBitmapFromDxgiSurface(pSurface.Get(), &bmprop, &mpBitmaps.at(iter));
			}
		}
		void initDebugText() {
			ComPtr<ID2D1SolidColorBrush> pBrush1{};
			ComPtr<ID2D1SolidColorBrush> pBrush2{};
			ComPtr<IDWriteTextFormat3> pFormat{};
			
			hr = mp2DDeviceContext->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Green), 
				&pBrush1
			);
			hr = mp2DDeviceContext->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Black), 
				&pBrush2
			);
			ComPtr<IDWriteFontCollection3> pFontCollection{}; {
				hr = mpWriteFactory->GetSystemFontCollection((IDWriteFontCollection**)pFontCollection.GetAddressOf());
			}			
			hr = mpWriteFactory->CreateTextFormat(
				L"Cascadia Mono",
				pFontCollection.Get(),
				DWRITE_FONT_WEIGHT_BOLD,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				16,
				L"en-us",
				(IDWriteTextFormat**)pFormat.GetAddressOf()
			);
			mpBrushes.push_back(pBrush1);
			mpBrushes.push_back(pBrush2);
			mpFormats.push_back(pFormat);
		}
		void draw() noexcept {
			
			D2D1_SIZE_F rcSize = mpBitmaps.at(mFrameIndex)->GetSize();

			mpD11on12Device->AcquireWrappedResources(mpWrappedRenderTargets.at(mFrameIndex).GetAddressOf(), 1);
			mp2DDeviceContext->SetTarget(mpBitmaps.at(mFrameIndex).Get());
			mp2DDeviceContext->BeginDraw();
			mp2DDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

			[[likely]]
			if (!mDebugQueue.empty()) drawDebug(rcSize);

			hr = mp2DDeviceContext->EndDraw();
			mpD11on12Device->ReleaseWrappedResources(mpWrappedRenderTargets.at(mFrameIndex).GetAddressOf(), 1);
			mp3DDeviceContext->Flush();
		}
		void drawDebug(D2D1_SIZE_F& rcSize) noexcept {
			float indent{ 10.0f };
			float offset{ 3.0f };
			float spacing{ mpFormats.at(0).Get()->GetFontSize() };
			float additiveSpacing{ indent / 2.0f };
			D2D1_RECT_F textRect1{};
			D2D1_RECT_F textRect2{};
			std::wstring line{};

			while (!mDebugQueue.empty()) {
				line = mDebugQueue.front();
				mDebugQueue.pop();
				textRect1 = D2D1::RectF(indent, additiveSpacing, rcSize.width, rcSize.height);
				textRect2 = D2D1::RectF(indent + offset, additiveSpacing + offset, rcSize.width, rcSize.height);
				mp2DDeviceContext->DrawText(line.c_str(), (uint32_t)line.size(), mpFormats.at(0).Get(), &textRect2, mpBrushes.at(1).Get());
				mp2DDeviceContext->DrawText(line.c_str(), (uint32_t)line.size(), mpFormats.at(0).Get(), &textRect1, mpBrushes.at(0).Get());
				additiveSpacing += spacing;
			}
		}
		void addDebugQueue(std::wstring line) { 
			mDebugQueue.push(line);
		}
	};
}

// Graphics Pipeline Object (3D)
namespace GID::DSU {
	struct GFX3D {
		// Graphics Configuration
		bool											mError{ false };
		bool											mDebug{ false };
		bool											mUseWARPAdapter{ false };
		bool											mVSync{ false };
		BOOL											mTearingSupport{ false };
		bool											mFullscreen{ false };
		uint8_t											mBackBufferCount{ 3u };
		uint32_t										mShaderDebugCompileFlags{ D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION };
		uint32_t										mShaderReleaseCompileFlags{ 0u };
		D3D_FEATURE_LEVEL								mFeatureLevel{ D3D_FEATURE_LEVEL_12_1 };
		bool											mWireframe{ false };
		// Debug
		#if _DEBUG
		ComPtr<ID3D12Debug6>							mpDebugController{};
		ComPtr<ID3D12InfoQueue1>						mpInfoQueue{};
		#endif
		// Core
		ComPtr<IDXGIFactory7>							mpFactory{};
		ComPtr<ID3D12Device10>							mpDevice{};
		ComPtr<IDXGISwapChain4>							mpSwapChain{};
		// Adapters
		ComPtr<IDXGIAdapter4>							mpHardwareAdapter{};
		ComPtr<IDXGIAdapter4>							mpWARPAdapter{};
		// Graphics Commands
		std::vector<ComPtr<ID3D12CommandAllocator>>		mpGraphicsCommandAllocators{};
		ComPtr<ID3D12GraphicsCommandList7>				mpGraphicsCommandList{};
		ComPtr<ID3D12CommandQueue>						mpGraphicsCommandQueue{};
		// Graphics Pipeline state
		GraphicsPipelineStateStream						mGPSS{};
		ComPtr<ID3D12PipelineState>						mpGraphicsPipelineState{};
		// Graphics Root Signature
		ComPtr<ID3D12RootSignature>						mpGraphicsRootSignature{};
		ComPtr<ID3DBlob>								mpGraphicsSignatureRootBlob{};
		ComPtr<ID3DBlob>								mpGraphicsErrorBlob{};
		// Compute Commands
		std::vector<ComPtr<ID3D12CommandAllocator>>		mpComputeCommandAllocators{};
		ComPtr<ID3D12GraphicsCommandList7>				mpComputeCommandList{};
		ComPtr<ID3D12CommandQueue>						mpComputeCommandQueue{};
		// Compute Pipeline State
		ComputePipelineStateStream						mCPSS{};
		ComPtr<ID3D12PipelineState>						mpComputePipelineState{};
		// Compute Root Signature
		ComPtr<ID3D12RootSignature>						mpComputeRootSignature{};
		ComPtr<ID3DBlob>								mpComputeSignatureRootBlob{};
		ComPtr<ID3DBlob>								mpComputeErrorBlob{};
		// Render Target View(s)
		std::vector<ComPtr<ID3D12Resource2>>			mpRenderTargets{};
		ComPtr<ID3D12DescriptorHeap>					mpRTVHeap{};
		uint32_t										mRTVHeapSize{};
		std::vector<D3D12_VIEWPORT>						mViewport{};
		D3D12_RECT										mScissorRc{};
		// Depth Stencil
		ComPtr<ID3D12Resource2>							mpDepthStencilTexture{};
		ComPtr<ID3D12DescriptorHeap>					mpDSVHeap{};
		uint32_t										mDSVHeapSize{};
		// Fence
		ComPtr<ID3D12Fence1>							mpFence{};
		HANDLE											mhFenceEvent{};
		uint8_t											mFrameIndex{};
		uint64_t										mFenceValue{};
		// Shaders
		ComPtr<ID3DBlob>								mpVSBytecode{};
		ComPtr<ID3DBlob>								mpPSBytecode{};
		ComPtr<ID3DBlob>								mpCSBytecode{};
		// Projection Matrix
		AssistMath::FAMMATRIX							mProjection{};
		// Misc
		RECT											mWindowRc{};
		HRESULT											mHR{};
		std::string										mLastType{};
		Camera											mCamera{};
		GFX_DESC										mDesc{};
		// 2D GFX Pipeline
		GFX2D											m2D{};

		// Constructor
		GFX3D(HWND& hWnd, GFX_DESC& desc) {

			mDesc = desc;

			WINDOWINFO wndInfo{};
			wndInfo.cbSize = sizeof WINDOWINFO;
			GetWindowInfo(hWnd, &wndInfo);

			uint16_t width = (uint16_t)wndInfo.rcClient.right - (uint16_t)wndInfo.rcClient.left;
			uint16_t height = (uint16_t)wndInfo.rcClient.bottom - (uint16_t)wndInfo.rcClient.top;

			#if defined(_DEBUG)
			mDebug = true;
			#endif

			parseGraphicsConfig();

			if (mDebug) {
				std::cout << "[Graphics Pipeline | Initialization]\n";
				std::cout << "[Init Info] Using Debug Compile Flags for Shaders\n";
				std::cout << "[Init Info] Back Buffer Count: #" << mBackBufferCount << '\n';
				if (!mUseWARPAdapter) std::cout << "[Init Info] Expected Adapter Type: Hardware\n";
				if (mUseWARPAdapter) std::cout << "[Init Info] Expected Adapter Type: WARP\n";
				std::cout << "[Init Info] Minimum Feature Level: 12_1\n";
			}

			// Create the viewport
			mViewport = desc.viewportDesc.vps;
			//mViewport = { CD3DX12_VIEWPORT(0.0f, 0.0f, (FLOAT)width, (FLOAT)height) };
			mScissorRc = { CD3DX12_RECT(0u, 0u, LONG_MAX, LONG_MAX) };

			// Some multi-threaded bullshit for initializing graphics pipeline stuff that may or may not be faster than single-threaded
			std::thread thread0([this] { createFactory(); });
			#if _DEBUG
			std::thread thread1([this] { createDebugLayer(); });
			#endif
			thread0.join();
			std::thread thread2([this] { enumerateAdapters(); });
			thread2.join();
			std::thread thread3([this] { createDevice(); });
			thread3.join();
			std::thread thread4([this] { createCommandQueue(); });
			std::thread thread5([this] { checkTearingSupport(); });
			std::thread thread6([this] { createFence(); });
			#if _DEBUG
			std::thread thread7([this] { createInfoQueue(); });
			#endif
			std::thread thread8([this, width, height] { createDSV(width, height); });
			thread4.join();
			std::thread thread9([this, hWnd, width, height] { createSwapChain(hWnd, width, height); });
			std::thread thread10([this] { createCommandAllocatorAndList(); });
			thread9.join();
			std::thread thread11([this] { createRTV(); });
			#if _DEBUG
			thread1.join(); 
			#endif
			thread5.join(); thread6.join(); 
			#if _DEBUG
			thread7.join();
			#endif
			thread8.join(); thread10.join(); thread11.join();

			m2D = { mpDevice, mpGraphicsCommandQueue, mpRenderTargets };
			initGraphicsPipeline();
			initComputePipeline();
			flushGPU();
		}
		GFX3D() = default;

		// Destructor
		//~GFX3D() = default;

		void parseGraphicsConfig() noexcept {
			enum class GFXConfigDataType {
				UIBackBufferCount,
				BUseWARPAdapter,
				BVSync,
				BFullScreenOnStartup,
				BEnableWireframe,
				Invalid
			};
			if (mDebug) std::cout << "[CONFIG] Parsing Graphics Configuration File...\n";
			std::string strCurrentLine{}; std::ifstream file{};
			file.open("gfx_config.txt");
			if (file.is_open()) {
				while (file) {
					std::getline(file, strCurrentLine);
					if (strCurrentLine.empty()) continue;
					GFXConfigDataType currentData;
					if (strCurrentLine.starts_with("UIBackBufferCount")) currentData = GFXConfigDataType::UIBackBufferCount;
					else if (strCurrentLine.starts_with("BUseWARPAdapter")) currentData = GFXConfigDataType::BUseWARPAdapter;
					else if (strCurrentLine.starts_with("BVSync")) currentData = GFXConfigDataType::BVSync;
					else if (strCurrentLine.starts_with("BFullScreenOnStartup")) currentData = GFXConfigDataType::BFullScreenOnStartup;
					else if (strCurrentLine.starts_with("BEnableWireframe")) currentData = GFXConfigDataType::BEnableWireframe;
					else currentData = GFXConfigDataType::Invalid;
					switch (currentData) {
						case GFXConfigDataType::UIBackBufferCount:
							strCurrentLine.erase(0u, 19u);
							mBackBufferCount = std::min(std::max(uint8_t(2), (uint8_t)std::stoi(strCurrentLine)), uint8_t(DXGI_MAX_SWAP_CHAIN_BUFFERS));
							break;
						case GFXConfigDataType::BUseWARPAdapter:
							strCurrentLine.erase(0u, 17u);
							if (strCurrentLine == "TRUE") mUseWARPAdapter = true;
							break;
						case GFXConfigDataType::BVSync:
							strCurrentLine.erase(0u, 8u);
							if (strCurrentLine == "TRUE") mVSync = true;
							break;
						case GFXConfigDataType::BFullScreenOnStartup:
							strCurrentLine.erase(0u, 22u);
							if (strCurrentLine == "TRUE") mFullscreen = true;
							break;
						case GFXConfigDataType::BEnableWireframe:
							strCurrentLine.erase(0u, 18u);
							if (strCurrentLine == "TRUE") mWireframe = true;
							break;
						case GFXConfigDataType::Invalid:
							if (mDebug) std::cout << "[WARNING] Invalid line parsed from \"gfx_config.txt\"\n"
								<< "[LINE] " << strCurrentLine;
							break;
					}
					strCurrentLine.clear();
				}
			}
		}
		#if _DEBUG
		void createDebugLayer() noexcept {
			
			// Create the Debug Interface
			mHR = D3D12GetDebugInterface(__uuidof(ID3D12Debug5), (void**)&mpDebugController);
			mpDebugController->EnableDebugLayer();
			mpDebugController->SetEnableAutoName(TRUE);
		}
		void createInfoQueue() noexcept {
			mpDevice.As(&mpInfoQueue);
			mpInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
			mpInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
			mpInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_MESSAGE, TRUE);
		}
		#endif
		void createFactory() noexcept {
			// Create factory
			CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&mpFactory));
		}
		void enumerateAdapters() noexcept {
			// Obtain adapter
			if (mUseWARPAdapter) mpFactory->EnumWarpAdapter(IID_PPV_ARGS(&mpWARPAdapter)); // WARP
			else {
				SIZE_T maxVideoMem = 0;
				ComPtr<IDXGIAdapter1> refAdapter{};
				for (uint32_t i = 0; mpFactory->EnumAdapters1(i, refAdapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; i++) {
					DXGI_ADAPTER_DESC1 ad{};
					refAdapter->GetDesc1(&ad);
					if ((ad.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == (UINT)0 
						&& (UINT)D3D12CreateDevice(refAdapter.Get(), mFeatureLevel, __uuidof(ID3D12Device10), nullptr)
						&& ad.DedicatedVideoMemory > maxVideoMem) {
						maxVideoMem = ad.DedicatedVideoMemory; refAdapter.As(&mpHardwareAdapter);
					}
				}
			}
		}
		void createDevice() noexcept {
			// Create the Device
			if (mUseWARPAdapter) D3D12CreateDevice(mpWARPAdapter.Get(), mFeatureLevel, __uuidof(ID3D12Device9), (void**)&mpDevice); // WARP
			else D3D12CreateDevice(mpHardwareAdapter.Get(), mFeatureLevel, __uuidof(ID3D12Device9), (void**)&mpDevice); // Hardware
			mpDevice->SetName(L"mpDevice");
		}
		void createCommandQueue() noexcept {
			
			// Create the graphics command queue
			D3D12_COMMAND_QUEUE_DESC cqd = {}; {
				cqd.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
				cqd.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
				cqd.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
				mpDevice->CreateCommandQueue1(&cqd, __uuidof(ID3D12Device10), IID_PPV_ARGS(&mpGraphicsCommandQueue));
			}
			mpGraphicsCommandQueue->SetName(L"mpGraphicsCommandQueue");

			// Create the compute command queue
			{
				cqd.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
				cqd.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
				cqd.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
				mpDevice->CreateCommandQueue1(&cqd, __uuidof(ID3D12Device10), IID_PPV_ARGS(&mpComputeCommandQueue));
			}
			mpComputeCommandQueue->SetName(L"mpComputeCommandQueue");
		}
		void checkTearingSupport() noexcept {
			mpFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &mTearingSupport, sizeof(mTearingSupport));
		}
		void createSwapChain(const HWND& hWnd, const uint16_t& width, const uint16_t& height) noexcept {
			
			// Get window and monitor info
			HMONITOR hMonitor{ MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST) };
			MONITORINFOEX monitorInfo{};
			monitorInfo.cbSize = sizeof(MONITORINFOEX);
			GetMonitorInfo(hMonitor, &monitorInfo);
			DEVMODE devMode{};
			devMode.dmSize = sizeof(DEVMODE);
			EnumDisplaySettings(monitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &devMode);
			
			// Define swap chain descriptor
			DXGI_SWAP_CHAIN_DESC1 scd = {}; {
				scd.BufferCount = mBackBufferCount;
				scd.Width = width; scd.Height = height;
				scd.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
				scd.Stereo = FALSE;
				scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
				scd.SampleDesc.Count = 1u; scd.SampleDesc.Quality = 0u;
				scd.Scaling = DXGI_SCALING_STRETCH;
				scd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
				if (mTearingSupport) scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
			}
			
			// Define swap chain fullscreen descriptor
			DXGI_SWAP_CHAIN_FULLSCREEN_DESC scfd = {}; {
				scfd.RefreshRate.Numerator = devMode.dmDisplayFrequency;
				scfd.RefreshRate.Denominator = 1u;
				scfd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
				scfd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			}
			if (mFullscreen) scfd.Windowed = FALSE; else scfd.Windowed = TRUE;
			mpFactory->CreateSwapChainForHwnd(mpGraphicsCommandQueue.Get(), hWnd, &scd, &scfd, nullptr, (IDXGISwapChain1**)mpSwapChain.GetAddressOf());
			
			// Manual fullscreen support ( we use F11 around here :D )
			mpFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER); // DXGI_MWA_VALID
		}
		void createRTV() noexcept {
			
			// Create a Render Target View (RTV) heap with as many descriptors as there are buffers, so there is one heap per buffer
			// Typed as RTV and no flags
			D3D12_DESCRIPTOR_HEAP_DESC RTVHeapDesc = {}; {
				RTVHeapDesc.NumDescriptors = mBackBufferCount;
				RTVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				RTVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			}
			
			// Create the descriptor heap with the device
			mpDevice->CreateDescriptorHeap(&RTVHeapDesc, IID_PPV_ARGS(&mpRTVHeap));

			// Obtain and store the RTV Heap's incremental size 
			// The size of each pointer/handle to each individual descriptor/heap in the total RTV heap
			// (usually 152 bytes/pointer)
			mRTVHeapSize = mpDevice->GetDescriptorHandleIncrementSize(RTVHeapDesc.Type);
			mpRTVHeap->SetName(L"mpRTVHeap");
			
			// Get a handle (ptr) to the start of the Render Target View heap
			CD3DX12_CPU_DESCRIPTOR_HANDLE hRTV(mpRTVHeap->GetCPUDescriptorHandleForHeapStart());
			
			// Resize the RTV resources vector to fit all back buffers
			mpRenderTargets.resize(mBackBufferCount);
			for (UINT i = 0u; i < mBackBufferCount; i++) {
				
				// Obtain the indexed buffer and store it in the RTV (resource) vector
				mpSwapChain->GetBuffer(i, IID_PPV_ARGS(&mpRenderTargets.at(i)));

				// Have the hRTV (Render Target View handle/pointer) point to the instantiated RTV resource (creating a view on CPU side)
				mpDevice->CreateRenderTargetView(mpRenderTargets[i].Get(), nullptr, hRTV);
				
				// Offset the Render Target View handle/pointer by the size of a singleton heap pointer (for RTVs)
				// This essentially functions as indexing into an array of RTV descriptors
				hRTV.Offset(1u, mRTVHeapSize);
				
				// Object name (not important)
				std::wstringstream woss{};
				woss << "mpRenderTargets[" << i << ']';
				mpRenderTargets[i]->SetName(woss.str().c_str());
			}
		}
		void createDSV(const uint16_t& width, const uint16_t& height) noexcept {
			// Create Render Target View (RTV) heap
			D3D12_DESCRIPTOR_HEAP_DESC dsvhd{}; {
				dsvhd.NumDescriptors = 1u;
				dsvhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
				dsvhd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				mpDevice->CreateDescriptorHeap(&dsvhd, IID_PPV_ARGS(&mpDSVHeap));
				mDSVHeapSize = mpDevice->GetDescriptorHandleIncrementSize(dsvhd.Type);
			}
			D3D12_CLEAR_VALUE dscv{}; {
				dscv.Format = DXGI_FORMAT_D32_FLOAT;
				dscv.DepthStencil = { 1.0f, 0 };
			}
			// Configure Depth Stencil Texture
			CD3DX12_HEAP_PROPERTIES DSTexHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
			auto DSTexHeapDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, width, height);
			DSTexHeapDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			mHR = mpDevice->CreateCommittedResource(&DSTexHeapProperties, D3D12_HEAP_FLAG_NONE, &DSTexHeapDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &dscv,
				IID_PPV_ARGS(&mpDepthStencilTexture));
			mpDepthStencilTexture->SetName(L"mpDepthStencilTexture");
		}
		void createCommandAllocatorAndList() noexcept {
			// Setup Graphics Commands
			mpGraphicsCommandAllocators.resize(mBackBufferCount);
			for (uint8_t i = 0; i < mBackBufferCount; i++) {
				mpDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mpGraphicsCommandAllocators.at(i)));
				std::wstringstream woss{};
				woss << "mpGraphicsCommandAllocators[" << i << "]";
				mpGraphicsCommandAllocators.at(i)->SetName(woss.str().c_str());
			}
			mpDevice->CreateCommandList1(0u, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE, __uuidof(ID3D12GraphicsCommandList6), (void**)&mpGraphicsCommandList);
			mpGraphicsCommandList->SetName(L"mpGraphicsCommandList");

			mpGraphicsCommandAllocators.at(0)->Reset();
			mpGraphicsCommandList->Reset(mpGraphicsCommandAllocators.at(0).Get(), nullptr);

			// Set up Compute Commands
			mpComputeCommandAllocators.resize(mBackBufferCount);
			for (uint8_t i = 0; i < mBackBufferCount; i++) {
				mpDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE, IID_PPV_ARGS(&mpComputeCommandAllocators.at(i)));
				std::wstringstream woss{};
				woss << "mpGraphicsCommandAllocators[" << i << "]";
				mpComputeCommandAllocators.at(i)->SetName(woss.str().c_str());
			}
			mpDevice->CreateCommandList1(0u, D3D12_COMMAND_LIST_TYPE_COMPUTE, D3D12_COMMAND_LIST_FLAG_NONE, __uuidof(ID3D12GraphicsCommandList6), (void**)&mpComputeCommandList);
			mpComputeCommandList->SetName(L"mpComputeCommandList");

			mpComputeCommandAllocators.front()->Reset();
			mpComputeCommandList->Reset(mpComputeCommandAllocators.front().Get(), nullptr);
		}
		void createFence() noexcept {
			// Create the Fence for synchronization
			mpDevice->CreateFence(0u, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mpFence));
			mpFence->SetName(L"mpFence");
			mhFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		}
		void initGraphicsPipeline() noexcept {
			CD3DX12_DESCRIPTOR_RANGE1 texturePS{};
			texturePS.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1u, 0u, 0u, D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE);
			std::array<CD3DX12_ROOT_PARAMETER1, 6u> rp{}; {
				
				// ---Vertex Shader---

				// Constant Buffer (Matrices)
				rp.at(0).InitAsConstantBufferView(0u, 0u, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_VOLATILE, D3D12_SHADER_VISIBILITY_VERTEX);

				// Shader Resource (Positions)
				rp.at(1).InitAsShaderResourceView(0u, 0u, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_VOLATILE, D3D12_SHADER_VISIBILITY_VERTEX);
				
				// Shader Resource (Texture Coordinates)
				rp.at(2).InitAsShaderResourceView(1u, 0u, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_VOLATILE, D3D12_SHADER_VISIBILITY_VERTEX);
				
				// Shader Resource (Normals)
				rp.at(3).InitAsShaderResourceView(2u, 0u, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_VOLATILE, D3D12_SHADER_VISIBILITY_VERTEX);
				
				// ---Pixel Shader---

				// Constant Buffer (Material & Light Data)
				rp.at(4).InitAsConstantBufferView(0u, 0u, D3D12_ROOT_DESCRIPTOR_FLAG_DATA_VOLATILE, D3D12_SHADER_VISIBILITY_PIXEL);
				
				
				rp.at(5).InitAsDescriptorTable(1u, &texturePS, D3D12_SHADER_VISIBILITY_PIXEL);
			}
			std::array<CD3DX12_STATIC_SAMPLER_DESC, 1u> ssd{}; {
				ssd[0].Init(0u);
				ssd[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
			}
			D3D12_ROOT_SIGNATURE_FLAGS rsFlags{
				D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS
				| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS
			};
			D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData{}; {
				featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
			}
			CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rsd{}; {
				rsd.Init_1_1((uint32_t)rp.size(), rp.data(), (uint32_t)ssd.size(), ssd.data(), rsFlags);
				mHR = D3DX12SerializeVersionedRootSignature(&rsd, featureData.HighestVersion, &mpGraphicsSignatureRootBlob, &mpGraphicsErrorBlob);
				mHR = mpDevice->CreateRootSignature(0u, mpGraphicsSignatureRootBlob->GetBufferPointer(), mpGraphicsSignatureRootBlob->GetBufferSize(), IID_PPV_ARGS(&mpGraphicsRootSignature));
				mpGraphicsRootSignature->SetName(L"mpGraphicsRootSignature");
			}
			std::array<D3D12_INPUT_ELEMENT_DESC, 3u> ied{}; {
				ied[0] = { "POSITION", 0, DXGI_FORMAT_R32_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
				ied[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
				ied[2] = { "NORMAL", 0, DXGI_FORMAT_R32_UINT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			}
			mHR = D3DReadFileToBlob(L"VS_IndexedWithStructuredBuffers.cso", &mpVSBytecode);
			mHR = D3DReadFileToBlob(L"PixelShader.cso", &mpPSBytecode);
			D3D12_RT_FORMAT_ARRAY RTVFormatArray{}; {
				RTVFormatArray.NumRenderTargets = 1u;
				RTVFormatArray.RTFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
			}
			D3D12_PIPELINE_STATE_STREAM_DESC pssd{}; {
				mGPSS.pRootSignature = mpGraphicsRootSignature.Get();
				mGPSS.InputLayout = { ied.data(), (UINT)ied.size() };
				mGPSS.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
				mGPSS.VS = CD3DX12_SHADER_BYTECODE(mpVSBytecode.Get());
				mGPSS.PS = CD3DX12_SHADER_BYTECODE(mpPSBytecode.Get());
				mGPSS.DSVFormat = DXGI_FORMAT_D32_FLOAT;
				mGPSS.RTVFormats = RTVFormatArray;
				if (mDebug) {
					if (mWireframe) {
						mGPSS.RS = CD3DX12_RASTERIZER_DESC(D3D12_FILL_MODE_WIREFRAME, D3D12_CULL_MODE_NONE, FALSE, 0u, 0.0f, 0.0f, TRUE, FALSE, FALSE, 0u,
							D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF);
					} else {
						mGPSS.RS = CD3DX12_RASTERIZER_DESC(D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE, FALSE, 0u, 0.0f, 0.0f, TRUE, FALSE, FALSE, 0u,
							D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF);
					}
				} else if (mWireframe) {
					mGPSS.RS = CD3DX12_RASTERIZER_DESC(D3D12_FILL_MODE_WIREFRAME, D3D12_CULL_MODE_BACK, FALSE, 0u, 0.0f, 0.0f, TRUE, FALSE, FALSE, 0u,
						D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF);
				} else mGPSS.RS = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
				pssd.pPipelineStateSubobjectStream = &mGPSS;
				pssd.SizeInBytes = sizeof GID::DSU::GraphicsPipelineStateStream;
				mHR = mpDevice->CreatePipelineState(&pssd, IID_PPV_ARGS(&mpGraphicsPipelineState));
				mpGraphicsPipelineState->SetName(L"Graphics Pipeline State");
			} 
			D3D12_DEPTH_STENCIL_VIEW_DESC dsv{}; {
				dsv.Format = DXGI_FORMAT_D32_FLOAT;
				dsv.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
				dsv.Texture2D.MipSlice = 0u;
				dsv.Flags = D3D12_DSV_FLAG_NONE;
				CD3DX12_CPU_DESCRIPTOR_HANDLE hDSV(mpDSVHeap->GetCPUDescriptorHandleForHeapStart());
				mpDevice->CreateDepthStencilView(mpDepthStencilTexture.Get(), &dsv, hDSV);
			}
		}
		void initComputePipeline() noexcept {
			CD3DX12_DESCRIPTOR_RANGE1 srcMip{};
			CD3DX12_DESCRIPTOR_RANGE1 dstMip{};
			srcMip.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1u, 0u, 0u, D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE);
			dstMip.Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 4u, 0u, 0u, D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE);
			std::array<CD3DX12_ROOT_PARAMETER1, 3u> rp{}; {
				// ---Compute Shader---

				// Constant Buffer (Texture Metadata)
				rp.at(0).InitAsConstants(8u, 0u, 0u);

				// 2D Texture (Source Texture)
				rp.at(1).InitAsDescriptorTable(1u, &srcMip);

				// 2D Texture (Mipped Texture(s))
				rp.at(2).InitAsDescriptorTable(1u, &dstMip);
			}
			std::array<CD3DX12_STATIC_SAMPLER_DESC, 1u> ssd{}; {
				ssd[0].Init(0u);
			}
			D3D12_ROOT_SIGNATURE_FLAGS rsFlags{
				D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS 
				| D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_AMPLIFICATION_SHADER_ROOT_ACCESS 
				| D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS 
				| D3D12_ROOT_SIGNATURE_FLAG_DENY_MESH_SHADER_ROOT_ACCESS
			};
			CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rsd{}; {
				rsd.Init_1_1((uint32_t)rp.size(), rp.data(), (uint32_t)ssd.size(), ssd.data(), rsFlags);
				mHR = D3DX12SerializeVersionedRootSignature(&rsd, D3D_ROOT_SIGNATURE_VERSION_1_1, &mpComputeSignatureRootBlob, &mpComputeErrorBlob);
				mHR = mpDevice->CreateRootSignature(0u, mpComputeSignatureRootBlob->GetBufferPointer(), mpComputeSignatureRootBlob->GetBufferSize(), IID_PPV_ARGS(&mpComputeRootSignature));
				mpComputeRootSignature->SetName(L"[Shaders] [Compute Root Signature]");
			}
			mHR = D3DReadFileToBlob(L"MipmapComputeShader.cso", &mpCSBytecode);
			D3D12_PIPELINE_STATE_STREAM_DESC pssd{}; {
				mCPSS.pRootSignature = mpComputeRootSignature.Get();
				mCPSS.CS = CD3DX12_SHADER_BYTECODE(mpCSBytecode.Get());
				pssd.pPipelineStateSubobjectStream = &mCPSS;
				pssd.SizeInBytes = sizeof GID::DSU::ComputePipelineStateStream;
				mHR = mpDevice->CreatePipelineState(&pssd, IID_PPV_ARGS(&mpComputePipelineState));
				mpComputePipelineState->SetName(L"Compute Pipeline State");
			}

		}

		// Frame sync -- How this works:
		// 1. Place an 'increment the fence value' command at the end of the command queue, which will trigger when all of the GPU's work is finished and resources can be written to.
		// 2. Increment the independently-tracked, CPU-side fence value (mFenceValue).
		// 3. Check and see if the fence value of the fence (mpFence) has been incremented to the value it should be when the queue will increment it (mFenceValue).
		// 4. If the fence value is still less than its intended value, then the GPU still isn't done executing the command list. Set a fence event to trigger when it is.
		// 5. Wait for event to trigger.
		void signalFence() noexcept {
			
			// Signal the fence
			mHR = mpGraphicsCommandQueue->Signal(mpFence.Get(), ++mFenceValue);
		}
		int waitFence() noexcept {

			// Wait for the fence
			if (mpFence->GetCompletedValue() != mFenceValue) {

				// Set an internal event to return the completed fence value when complete
				mpFence->SetEventOnCompletion(mFenceValue, mhFenceEvent);

				// Wait for the internal event to complete
				WaitForSingleObject(mhFenceEvent, (DWORD)INFINITE);
			}
			return 0;
		}
		int flushGPU() noexcept {

			// Close the command list
			mpGraphicsCommandList->Close();

			// Demote the command list into a single-element array of command list pointers
			ID3D12CommandList* ppCommandLists[] = { mpGraphicsCommandList.Get() };

			// Place the command list on the command queue and submit the command list's work to the GPU
			mpGraphicsCommandQueue->ExecuteCommandLists(1u, ppCommandLists);

			// Frame sync
			signalFence();
			waitFence();
			
			// Reset the current frame's command allocator (for use when the frame loop rolls back around to this index)
			mpGraphicsCommandAllocators.at(mFrameIndex)->Reset();

			// Reset the command list
			mpGraphicsCommandList->Reset(mpGraphicsCommandAllocators.at(mFrameIndex).Get(), nullptr);
			return 0;
		}

		// Prepare
		void transitionRTVToWrite() noexcept {
			auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpRenderTargets[mFrameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
			mpGraphicsCommandList->ResourceBarrier(1u, &barrier);
		}
		void transitionRTVToRead() noexcept {
			auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpRenderTargets[mFrameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
			mpGraphicsCommandList->ResourceBarrier(1u, &barrier);
		}
		void clearRTV() noexcept {
			CD3DX12_CPU_DESCRIPTOR_HANDLE hRTV(mpRTVHeap->GetCPUDescriptorHandleForHeapStart(), mFrameIndex, mRTVHeapSize);
			//auto color = std::make_unique<float[]>(4); color[0] = 0.5294f; color[1] = 0.8078f; color[2] = 0.9216f; color[3] = 1.f;
			//auto color = std::make_unique<float[]>(4); color[0] = 0.0f; color[1] = 0.0f; color[2] = 0.0f; color[3] = 1.f;
			auto color = std::make_unique<float[]>(4); color[0] = 0.2f; color[1] = 0.2f; color[2] = 0.2f; color[3] = 1.f;
			mpGraphicsCommandList->ClearRenderTargetView(hRTV, color.get(), 0u, nullptr);
		}
		void clearDSV() noexcept {
			CD3DX12_CPU_DESCRIPTOR_HANDLE hDSV(mpDSVHeap->GetCPUDescriptorHandleForHeapStart());
			mpGraphicsCommandList->ClearDepthStencilView(hDSV, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0u, 0u, nullptr);
		}
		void setRenderTarget() noexcept {
			CD3DX12_CPU_DESCRIPTOR_HANDLE hRTV(mpRTVHeap->GetCPUDescriptorHandleForHeapStart(), mFrameIndex, mRTVHeapSize);
			CD3DX12_CPU_DESCRIPTOR_HANDLE hDSV(mpDSVHeap->GetCPUDescriptorHandleForHeapStart());
			mpGraphicsCommandList->OMSetRenderTargets(1u, &hRTV, FALSE, &hDSV);
		}

		// Render
		void startFrame() noexcept {
			[[unlikely]]
			if (GID::GSO::General::gGenVar.gIsWindowResized) resizeWindow();
			transitionRTVToWrite();
			clearRTV();
			clearDSV();
			setRenderTarget();
			mpGraphicsCommandList->SetGraphicsRootSignature(mpGraphicsRootSignature.Get());
			mpGraphicsCommandList->SetPipelineState(mpGraphicsPipelineState.Get());
			mpGraphicsCommandList->RSSetViewports((uint32_t)mViewport.size(), mViewport.data());
			mpGraphicsCommandList->RSSetScissorRects(1u, &mScissorRc);
		}
		void doFrame() noexcept {
			//const std::array<DSU::PositionData, 8u> vertices{
			//	DX::XMFLOAT3(-1.0f, -1.0f, -1.0f),
			//	DX::XMFLOAT3(-1.0f, 1.0f, -1.0f),
			//	DX::XMFLOAT3(1.0f, 1.0f, -1.0f),
			//	DX::XMFLOAT3(1.0f, -1.0f, -1.0f),
			//	DX::XMFLOAT3(-1.0f, -1.0f, 1.0f),
			//	DX::XMFLOAT3(-1.0f, 1.0f, 1.0f),
			//	DX::XMFLOAT3(1.0f, 1.0f, 1.0f),
			//	DX::XMFLOAT3(1.0f, -1.0f, 1.0f),
			//};
			//const std::array<WORD, 36u> indices{
			//	0, 1, 2, 0, 2, 3,
			//	4, 6, 5, 4, 7, 6,
			//	4, 5, 1, 4, 1, 0,
			//	3, 2, 6, 3, 6, 7,
			//	1, 5, 6, 1, 6, 2,
			//	4, 0, 3, 4, 3, 7
			//};
			//DX::XMMATRIX transformM{
			//	DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f)
			//	* DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f)
			//	* DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f)
			//	* DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f)
			//};
			//auto InvDeterminant = DirectX::XMMatrixDeterminant(transformM);
			//auto InvM = DirectX::XMMatrixTranspose(transformM);
			//DSU::VertexConstantBuffer mx{ transformM, mCamera.getMatrix(), DX::XMMatrixPerspectiveLH(1.f, 9.0f / 16.0f, 0.25f, 5000.f), 
			//	DirectX::XMMatrixInverse(&InvDeterminant, InvM) };
			//// Add resources
			//VertexBuffer vb(mpDevice, mpCommandList, vertices.data(), vertices.size());
			//IndexBuffer ib(mpDevice, mpCommandList, indices.data(), indices.size());
			//VertexConstantBuffer vcb(mpDevice, mpCommandList, mx);
			//vb.transitionToRead(mpCommandList);
			//ib.transitionToRead(mpCommandList);
			//vcb.transitionToRead(mpCommandList);
			//mpCommandList->SetGraphicsRootConstantBufferView(0u, vcb.getDestRes()->GetGPUVirtualAddress());
			//mpCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			//mpCommandList->IASetVertexBuffers(0u, 1u, &vb.getView());
			//mpCommandList->IASetIndexBuffer(&ib.getView());
			//mpCommandList->DrawIndexedInstanced(ib.getCount(), 1u, 0u, 0u, 0u);
		}
		void endFrame() noexcept {
			DXGI_PRESENT_PARAMETERS pp = {};
			pp.DirtyRectsCount = 0u;
			pp.pDirtyRects = 0u;
			pp.pScrollRect = 0u;
			pp.pScrollOffset = 0u;
			
			flushGPU();
			m2D.draw();
			flushGPU();
			//UINT syncInterval = mVSync ? 1u : 0u;
			//UINT presentFlags = mTearingSupport && !mVSync ? DXGI_PRESENT_ALLOW_TEARING : 0u;
			mpSwapChain->Present1((mVSync) ? 1u : 0u, (mTearingSupport && !mVSync) ? DXGI_PRESENT_ALLOW_TEARING : 0u, &pp);
			mFrameIndex = (uint8_t)mpSwapChain->GetCurrentBackBufferIndex();
			m2D.mFrameIndex = mFrameIndex;
		}

		// Util
		void resizeWindow() noexcept {
			WINDOWINFO windowInfo{};
			windowInfo.cbSize = sizeof WINDOWINFO;
			windowInfo = GID::GSO::WindowNS::gWnd.get()->getWindowInfo();

			int16_t clientWidth{ ((int16_t)windowInfo.rcClient.right - (int16_t)windowInfo.rcClient.left == 0)
				? 1 : (int16_t)windowInfo.rcClient.right - (int16_t)windowInfo.rcClient.left };
			
			int16_t clientHeight{ ((int16_t)windowInfo.rcClient.bottom - (int16_t)windowInfo.rcClient.top == 0)
				? 1 : (int16_t)windowInfo.rcClient.bottom - (int16_t)windowInfo.rcClient.top };

			flushGPU();

			for (auto& rt : mpRenderTargets) rt.Reset();
			for (auto& rt : mpRenderTargets) rt.Reset();
			mpRenderTargets.clear();
			mpDepthStencilTexture.Reset();

			for (auto& wrt : m2D.mpWrappedRenderTargets) wrt.Reset();
			for (auto& wrt : m2D.mpWrappedRenderTargets) wrt.Reset();
			m2D.mpWrappedRenderTargets.clear();

			for (auto& bm : m2D.mpBitmaps) bm.Reset();
			m2D.mpBitmaps.clear();
			
			flushGPU();

			DXGI_SWAP_CHAIN_DESC1 scd{};
			mpSwapChain->GetDesc1(&scd);
			mpSwapChain->ResizeBuffers(0, clientWidth, clientHeight, scd.Format, scd.Flags);
			
			createRTV();
			createDSV(clientWidth, clientHeight);
			D3D12_DEPTH_STENCIL_VIEW_DESC dsv{}; {
				dsv.Format = DXGI_FORMAT_D32_FLOAT;
				dsv.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
				dsv.Texture2D.MipSlice = 0u;
				dsv.Flags = D3D12_DSV_FLAG_NONE;
				CD3DX12_CPU_DESCRIPTOR_HANDLE hDSV(mpDSVHeap->GetCPUDescriptorHandleForHeapStart());
				mpDevice->CreateDepthStencilView(mpDepthStencilTexture.Get(), &dsv, hDSV);
			}
			m2D.wrapRenderTargets(mpRenderTargets, m2D.getDPI());
			mViewport.at(0).Width = clientWidth;
			mViewport.at(0).Height = clientHeight;
			GID::GSO::General::gGenVar.gIsWindowResized = false;
		}
		int setFullscreen() noexcept {

			HWND hWnd{};
			mpSwapChain->GetHwnd(&hWnd);
			WINDOWINFO wndInfo{};
			wndInfo.cbSize = sizeof(WINDOWINFO);
			GetWindowInfo(hWnd, &wndInfo);

			if (!mFullscreen) {
				GetWindowRect(hWnd, &mWindowRc);
				UINT windowStyle{ WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX) };
				SetWindowLong(hWnd, GWL_STYLE, windowStyle);

				HMONITOR hMonitor{ MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST) };
				MONITORINFOEX monitorInfo{};
				monitorInfo.cbSize = sizeof(MONITORINFOEX);
				GetMonitorInfo(hMonitor, &monitorInfo);
				SetWindowPos(hWnd, HWND_TOP, monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top, monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
					monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top, SWP_FRAMECHANGED | SWP_NOACTIVATE);
				ShowWindow(hWnd, SW_MAXIMIZE);
			} else {
				SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
				SetWindowPos(hWnd, HWND_NOTOPMOST, mWindowRc.left, mWindowRc.top, mWindowRc.right - mWindowRc.left, mWindowRc.bottom - mWindowRc.top,
					SWP_FRAMECHANGED | SWP_NOACTIVATE);
				ShowWindow(hWnd, SW_NORMAL);
			}

			mFullscreen = !mFullscreen;

			return 0;
		}
		Camera& getCamera() noexcept { return mCamera; }

		void setProjection(const AssistMath::FAMMATRIX& projection) noexcept { mProjection = projection; }
		AssistMath::FAMMATRIX& getProjection() noexcept { return mProjection; }
		ComPtr<ID3D12GraphicsCommandList7>& getCommandList() noexcept { return mpGraphicsCommandList; }
		ComPtr<ID3D12Device10>& getDevice() noexcept { return mpDevice; }

		void doDebug(HRESULT& hr, const char* type, const char* element, const char* verb) {
			if (hr == S_OK) {
				std::cout << '[' << type << "] " << element << ' ' << verb << " Successfully!\n";
			}
			else {
				std::cout << '[' << type << "] " << element << ' ' << verb << " Unsuccessfully!" << " [ERROR CODE] " << hr << '\n';
			}
			mLastType = type;
		}
		void doDebug(HRESULT& hr, const char* element, const char* verb) {
			if (hr == S_OK) {
				std::cout << '[' << mLastType << "] " << element << ' ' << verb << " Successfully!\n";
			}
			else {
				std::cout << '[' << mLastType << "] " << element << ' ' << verb << " Unsuccessfully!" << " [ERROR CODE] " << hr << '\n';
			}
		}
		void doDebug(HRESULT& hr, const char* type, const char* element, const char* verb, int amount) {
			if (hr == S_OK) {
				std::cout << '[' << type << "] " << element << " #" << amount << ' ' << verb << " Successfully!\n";
			}
			else {
				std::cout << '[' << type << "] " << element << " #" << amount << ' ' << verb << " Unsuccessfully!" << " [ERROR CODE] " << hr << '\n';
			}
			mLastType = type;
		}
		void doDebug(HRESULT& hr, const char* element, const char* verb, int amount) {
			if (hr == S_OK) {
				std::cout << '[' << mLastType << "] " << element << " #" << amount << ' ' << verb << " Successfully!\n";
			}
			else {
				std::cout << '[' << mLastType << "] " << element << " #" << amount << ' ' << verb << " Unsuccessfully!" << " [ERROR CODE] " << hr << '\n';
			}
		}
	};
}

// Global GFX Vector and External GFX Functions
namespace GID::GSO::Render {
	inline GID::DSU::GFX3D gGFX{};
	inline void initGFX(GID::DSU::GFX_DESC desc) { gGFX = { GID::GSO::WindowNS::gWnd.get()->mhWnd, desc }; }
	inline void setGFXProjection(GID::DSU::AssistMath::FAMMATRIX& projection) { gGFX.setProjection(projection); }
}

// Vertex Buffer Structure
namespace GID::DSU {
	struct VertexBuffer {

		ComPtr<ID3D12Resource2>		mpDestRes{};
		ComPtr<ID3D12Resource2>		mpIntermedRes{};
		D3D12_VERTEX_BUFFER_VIEW	mVBView{};

		VertexBuffer() = default;
		VertexBuffer(ComPtr<ID3D12Device10>& pDevice, ComPtr<ID3D12GraphicsCommandList7>& pCommandList, const VSInputData* vertexIndices, const UINT64& size) {
			createDestinationResource(pDevice, size);
			createIntermediateResource(pDevice, size);
			createVertexBufferView(pCommandList, vertexIndices, size);
		}
		void createDestinationResource(ComPtr<ID3D12Device10>& pDevice, const UINT64& size) noexcept {
			CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_DEFAULT);
			CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(size * sizeof VSInputData) };
			pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&mpDestRes));
			mpDestRes->SetName(L"[ID3D12Resource] [Destination Resource - VertexBuffer]");
		}
		void createIntermediateResource(ComPtr<ID3D12Device10>& pDevice, const UINT64& size) noexcept {
			CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_UPLOAD);
			CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(size * sizeof VSInputData) };
			pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mpIntermedRes));
			mpIntermedRes->SetName(L"[ID3D12Resource] [Intermediate Resource - VertexBuffer]");
		}
		void createVertexBufferView(ComPtr<ID3D12GraphicsCommandList7>& pCommandList, const VSInputData* vertexIndices, const UINT64& size) noexcept {
			D3D12_SUBRESOURCE_DATA srd{}; {
				srd.pData = vertexIndices;
				srd.RowPitch = size * sizeof VSInputData;
				srd.SlicePitch = srd.RowPitch;
			}
			UpdateSubresources(pCommandList.Get(), mpDestRes.Get(), mpIntermedRes.Get(), 0u, 0u, 1u, &srd);
			mVBView.BufferLocation = mpDestRes->GetGPUVirtualAddress();
			mVBView.SizeInBytes = (uint32_t)(sizeof VSInputData * size);
			mVBView.StrideInBytes = sizeof VSInputData;
		}

		ComPtr<ID3D12Resource2>& getDestinationResource() noexcept { return mpDestRes; }
		ComPtr<ID3D12Resource2>& getIntermediateResource() noexcept { return mpIntermedRes; }
		D3D12_VERTEX_BUFFER_VIEW& getView() noexcept { return mVBView; }
		UINT getCount() const noexcept {
			return mVBView.SizeInBytes / sizeof VSInputData;
		}
		void transitionToRead(ComPtr<ID3D12GraphicsCommandList7>& pCommandList) noexcept {
			auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
			pCommandList->ResourceBarrier(1u, &barrier);
		}
		void transitionToWrite(ComPtr<ID3D12GraphicsCommandList7>& pCommandList) noexcept {
			auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
				D3D12_RESOURCE_STATE_COPY_DEST);
			pCommandList->ResourceBarrier(1u, &barrier);
		}
	};
}

// Vertex SRV Structures
namespace GID::DSU {
	struct VSSRVPosSBData {

		ComPtr<ID3D12Resource2>			mpDestRes{};
		ComPtr<ID3D12Resource2>			mpIntermedRes{};
		ComPtr<ID3D12DescriptorHeap>	mpHeap{};
		uint32_t						mpHeapSize{};
		D3D12_SUBRESOURCE_DATA			mSRD{};

		VSSRVPosSBData() = default;
		VSSRVPosSBData(ComPtr<ID3D12Device10>& pDevice, ComPtr<ID3D12GraphicsCommandList7>& pCommandList, const std::vector<AssistMath::AMFLOAT3>& data) {
			createDestinationResource(pDevice, data);
			createIntermediateResource(pDevice, data);
			createDescriptorHeap(pDevice);
			createShaderResourceView(pDevice, pCommandList, data);
		}
		void createDestinationResource(ComPtr<ID3D12Device10>& pDevice, const std::vector<AssistMath::AMFLOAT3>& data) noexcept {
			CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_DEFAULT);
			CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(sizeof(AssistMath::AMFLOAT3) * data.size()) };
			pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&mpDestRes));
		}
		void createIntermediateResource(ComPtr<ID3D12Device10>& pDevice, const std::vector<AssistMath::AMFLOAT3>& data) noexcept {
			CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_UPLOAD);
			CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(sizeof(AssistMath::AMFLOAT3) * data.size()) };
			pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mpIntermedRes));
		}
		void createDescriptorHeap(ComPtr<ID3D12Device10>& pDevice) noexcept {
			D3D12_DESCRIPTOR_HEAP_DESC srvhd{};
			srvhd.NumDescriptors = 1u;
			srvhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			srvhd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			pDevice->CreateDescriptorHeap(&srvhd, IID_PPV_ARGS(&mpHeap));
			mpHeapSize = pDevice->GetDescriptorHandleIncrementSize(srvhd.Type);
		}
		void createShaderResourceView(ComPtr<ID3D12Device10>& pDevice, ComPtr<ID3D12GraphicsCommandList7>& pCommandList, const std::vector<AssistMath::AMFLOAT3>& data) noexcept {
			D3D12_SUBRESOURCE_DATA srd{}; {
				srd.pData = data.data();
				srd.RowPitch = (sizeof(AssistMath::AMFLOAT3) * data.size());
				srd.SlicePitch = srd.RowPitch;
			}
			UpdateSubresources(pCommandList.Get(), mpDestRes.Get(), mpIntermedRes.Get(), 0u, 0u, 1u, &srd);
			D3D12_BUFFER_SRV bsrv{}; {
				bsrv.FirstElement = 0;
				bsrv.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
				bsrv.NumElements = (uint32_t)data.size();
				bsrv.StructureByteStride = sizeof AssistMath::AMFLOAT3;
			}
			D3D12_SHADER_RESOURCE_VIEW_DESC srvd{}; {
				srvd.Format = DXGI_FORMAT_UNKNOWN;
				srvd.Buffer = bsrv;
				srvd.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
				srvd.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			}
			pDevice->CreateShaderResourceView(mpDestRes.Get(), &srvd, mpHeap->GetCPUDescriptorHandleForHeapStart());
		}
		ComPtr<ID3D12Resource2>& getDestRes() noexcept { return mpDestRes; }
		ComPtr<ID3D12Resource2>& getIntermedRes() noexcept { return mpIntermedRes; }
		uint32_t alignBytes(uint32_t _size) noexcept { return _size % 256 == 0 ? _size : 256 - _size % 256 + _size; }
		void transitionToRead(ComPtr<ID3D12GraphicsCommandList7>& pCommandList) noexcept {
			auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
			pCommandList->ResourceBarrier(1u, &barrier);
		}
		void transitionToWrite(ComPtr<ID3D12GraphicsCommandList7>& pCommandList) noexcept {
			auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_COPY_DEST);
			pCommandList->ResourceBarrier(1u, &barrier);
		}
		void updateResource(ComPtr<ID3D12Device10>& pDevice, ComPtr<ID3D12GraphicsCommandList7>& pCommandList, const std::vector<AssistMath::AMFLOAT3>& data) {
			mSRD.pData = data.data();
			UpdateSubresources(pCommandList.Get(), mpDestRes.Get(), mpIntermedRes.Get(), 0u, 0u, 1u, &mSRD);
			D3D12_BUFFER_SRV bsrv{}; {
				bsrv.FirstElement = 0;
				bsrv.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
				bsrv.NumElements = (uint32_t)data.size();
				bsrv.StructureByteStride = sizeof AssistMath::AMFLOAT3;
			}
			D3D12_SHADER_RESOURCE_VIEW_DESC srvd{}; {
				srvd.Format = DXGI_FORMAT_UNKNOWN;
				srvd.Buffer = bsrv;
				srvd.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
				srvd.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			}
			pDevice->CreateShaderResourceView(mpDestRes.Get(), &srvd, mpHeap->GetCPUDescriptorHandleForHeapStart());
		}
	};
	struct VSSRVTexSBData {

		using FLOAT2 = AssistMath::AMFLOAT2;

		ComPtr<ID3D12Resource2>			mpDestRes{};
		ComPtr<ID3D12Resource2>			mpIntermedRes{};
		ComPtr<ID3D12DescriptorHeap>	mpHeap{};
		uint32_t						mpHeapSize{};
		D3D12_SUBRESOURCE_DATA			mSRD{};

		VSSRVTexSBData() = default;
		VSSRVTexSBData(ComPtr<ID3D12Device10>& pDevice, ComPtr<ID3D12GraphicsCommandList7>& pCommandList, const std::vector<FLOAT2>& data) {
			createDestinationResource(pDevice, data);
			createIntermediateResource(pDevice, data);
			createDescriptorHeap(pDevice);
			createShaderResourceView(pDevice, pCommandList, data);
		}
		void createDestinationResource(ComPtr<ID3D12Device10>& pDevice, const std::vector<FLOAT2>& data) noexcept {
			CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_DEFAULT);
			CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(sizeof(FLOAT2) * data.size()) };
			pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&mpDestRes));
		}
		void createIntermediateResource(ComPtr<ID3D12Device10>& pDevice, const std::vector<FLOAT2>& data) noexcept {
			CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_UPLOAD);
			CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(sizeof(FLOAT2) * data.size()) };
			pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mpIntermedRes));
		}
		void createDescriptorHeap(ComPtr<ID3D12Device10>& pDevice) noexcept {
			D3D12_DESCRIPTOR_HEAP_DESC srvhd{};
			srvhd.NumDescriptors = 1u;
			srvhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			srvhd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			pDevice->CreateDescriptorHeap(&srvhd, IID_PPV_ARGS(&mpHeap));
			mpHeapSize = pDevice->GetDescriptorHandleIncrementSize(srvhd.Type);
		}
		void createShaderResourceView(ComPtr<ID3D12Device10>& pDevice, ComPtr<ID3D12GraphicsCommandList7>& pCommandList, const std::vector<FLOAT2>& data) noexcept {
			D3D12_SUBRESOURCE_DATA srd{}; {
				srd.pData = data.data();
				srd.RowPitch = (sizeof(FLOAT2) * data.size());
				srd.SlicePitch = srd.RowPitch;
			}
			UpdateSubresources(pCommandList.Get(), mpDestRes.Get(), mpIntermedRes.Get(), 0u, 0u, 1u, &srd);
			D3D12_BUFFER_SRV bsrv{}; {
				bsrv.FirstElement = 0;
				bsrv.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
				bsrv.NumElements = (uint32_t)data.size();
				bsrv.StructureByteStride = sizeof AssistMath::AMFLOAT2;
			}
			D3D12_SHADER_RESOURCE_VIEW_DESC srvd{}; {
				srvd.Format = DXGI_FORMAT_UNKNOWN;
				srvd.Buffer = bsrv;
				srvd.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
				srvd.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			}
			pDevice->CreateShaderResourceView(mpDestRes.Get(), &srvd, mpHeap->GetCPUDescriptorHandleForHeapStart());
		}
		ComPtr<ID3D12Resource2>& getDestRes() noexcept { return mpDestRes; }
		ComPtr<ID3D12Resource2>& getIntermedRes() noexcept { return mpIntermedRes; }
		uint32_t alignBytes(uint32_t _size) noexcept { return _size % 256 == 0 ? _size : 256 - _size % 256 + _size; }
		void transitionToRead(ComPtr<ID3D12GraphicsCommandList7>& pCommandList) noexcept {
			auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
			pCommandList->ResourceBarrier(1u, &barrier);
		}
		void transitionToWrite(ComPtr<ID3D12GraphicsCommandList7>& pCommandList) noexcept {
			auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_COPY_DEST);
			pCommandList->ResourceBarrier(1u, &barrier);
		}
		void updateResource(ComPtr<ID3D12Device10>& pDevice, ComPtr<ID3D12GraphicsCommandList7>& pCommandList, const std::vector<FLOAT2>& data) {
			mSRD.pData = data.data();
			UpdateSubresources(pCommandList.Get(), mpDestRes.Get(), mpIntermedRes.Get(), 0u, 0u, 1u, &mSRD);
			D3D12_BUFFER_SRV bsrv{}; {
				bsrv.FirstElement = 0;
				bsrv.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
				bsrv.NumElements = (uint32_t)data.size();
				bsrv.StructureByteStride = sizeof AssistMath::AMFLOAT2;
			}
			D3D12_SHADER_RESOURCE_VIEW_DESC srvd{}; {
				srvd.Format = DXGI_FORMAT_UNKNOWN;
				srvd.Buffer = bsrv;
				srvd.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
				srvd.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			}
			pDevice->CreateShaderResourceView(mpDestRes.Get(), &srvd, mpHeap->GetCPUDescriptorHandleForHeapStart());
		}
	};
	struct VSSRVNormSBData {

		using FLOAT3 = AssistMath::AMFLOAT3;

		ComPtr<ID3D12Resource2>			mpDestRes{};
		ComPtr<ID3D12Resource2>			mpIntermedRes{};
		ComPtr<ID3D12DescriptorHeap>	mpHeap{};
		uint32_t						mpHeapSize{};
		D3D12_SUBRESOURCE_DATA			mSRD{};

		VSSRVNormSBData() = default;
		VSSRVNormSBData(ComPtr<ID3D12Device10>& pDevice, ComPtr<ID3D12GraphicsCommandList7>& pCommandList, const std::vector<FLOAT3>& data) {
			createDestinationResource(pDevice, data);
			createIntermediateResource(pDevice, data);
			createDescriptorHeap(pDevice);
			createShaderResourceView(pDevice, pCommandList, data);
		}
		void createDestinationResource(ComPtr<ID3D12Device10>& pDevice, const std::vector<FLOAT3>& data) noexcept {
			CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_DEFAULT);
			CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(sizeof(FLOAT3) * data.size()) };
			pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&mpDestRes));
		}
		void createIntermediateResource(ComPtr<ID3D12Device10>& pDevice, const std::vector<FLOAT3>& data) noexcept {
			CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_UPLOAD);
			CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(sizeof(FLOAT3) * data.size()) };
			pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mpIntermedRes));
		}
		void createDescriptorHeap(ComPtr<ID3D12Device10>& pDevice) noexcept {
			D3D12_DESCRIPTOR_HEAP_DESC srvhd{};
			srvhd.NumDescriptors = 1u;
			srvhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			srvhd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			pDevice->CreateDescriptorHeap(&srvhd, IID_PPV_ARGS(&mpHeap));
			mpHeapSize = pDevice->GetDescriptorHandleIncrementSize(srvhd.Type);
		}
		void createShaderResourceView(ComPtr<ID3D12Device10>& pDevice, ComPtr<ID3D12GraphicsCommandList7>& pCommandList, const std::vector<FLOAT3>& data) noexcept {
			D3D12_SUBRESOURCE_DATA srd{}; {
				srd.pData = data.data();
				srd.RowPitch = (sizeof(FLOAT3) * data.size());
				srd.SlicePitch = srd.RowPitch;
			}
			UpdateSubresources(pCommandList.Get(), mpDestRes.Get(), mpIntermedRes.Get(), 0u, 0u, 1u, &srd);
			D3D12_BUFFER_SRV bsrv{}; {
				bsrv.FirstElement = 0;
				bsrv.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
				bsrv.NumElements = (uint32_t)data.size();
				bsrv.StructureByteStride = sizeof AssistMath::AMFLOAT3;
			}
			D3D12_SHADER_RESOURCE_VIEW_DESC srvd{}; {
				srvd.Format = DXGI_FORMAT_UNKNOWN;
				srvd.Buffer = bsrv;
				srvd.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
				srvd.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			}
			pDevice->CreateShaderResourceView(mpDestRes.Get(), &srvd, mpHeap->GetCPUDescriptorHandleForHeapStart());
		}
		ComPtr<ID3D12Resource2>& getDestRes() noexcept { return mpDestRes; }
		ComPtr<ID3D12Resource2>& getIntermedRes() noexcept { return mpIntermedRes; }
		uint32_t alignBytes(uint32_t _size) noexcept { return _size % 256 == 0 ? _size : 256 - _size % 256 + _size; }
		void transitionToRead(ComPtr<ID3D12GraphicsCommandList7>& pCommandList) noexcept {
			auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
			pCommandList->ResourceBarrier(1u, &barrier);
		}
		void transitionToWrite(ComPtr<ID3D12GraphicsCommandList7>& pCommandList) noexcept {
			auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_COPY_DEST);
			pCommandList->ResourceBarrier(1u, &barrier);
		}
		void updateResource(ComPtr<ID3D12Device10>& pDevice, ComPtr<ID3D12GraphicsCommandList7>& pCommandList, const std::vector<FLOAT3>& data) {
			mSRD.pData = data.data();
			UpdateSubresources(pCommandList.Get(), mpDestRes.Get(), mpIntermedRes.Get(), 0u, 0u, 1u, &mSRD);
			D3D12_BUFFER_SRV bsrv{}; {
				bsrv.FirstElement = 0;
				bsrv.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
				bsrv.NumElements = (uint32_t)data.size();
				bsrv.StructureByteStride = sizeof AssistMath::AMFLOAT3;
			}
			D3D12_SHADER_RESOURCE_VIEW_DESC srvd{}; {
				srvd.Format = DXGI_FORMAT_UNKNOWN;
				srvd.Buffer = bsrv;
				srvd.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
				srvd.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			}
			pDevice->CreateShaderResourceView(mpDestRes.Get(), &srvd, mpHeap->GetCPUDescriptorHandleForHeapStart());
		}
	};
}

// Vertex Constant Buffer Structure
namespace GID::DSU {
	struct VertexConstantBuffer {

		ComPtr<ID3D12Resource2>			mpDestRes{};
		ComPtr<ID3D12Resource2>			mpIntermedRes{};
		ComPtr<ID3D12DescriptorHeap>	mpCBVHeap{};
		uint8_t							mpCBVHeapSize{};
		D3D12_SUBRESOURCE_DATA			mSRD{};

		VertexConstantBuffer() = default;
		VertexConstantBuffer(ComPtr<ID3D12Device10>& pDevice, ComPtr<ID3D12GraphicsCommandList7>& pCommandList, const VertexConstantBufferData& data) {
			createDestinationResource(pDevice);
			createIntermediateResource(pDevice);
			createDescriptorHeap(pDevice);
			createConstantBufferView(pDevice, pCommandList, data);
		}

		void createDestinationResource(ComPtr<ID3D12Device10>& pDevice) noexcept {
			CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_DEFAULT);
			CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(alignBytes()) };
			pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&mpDestRes));
		}
		void createIntermediateResource(ComPtr<ID3D12Device10>& pDevice) noexcept {
			CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_UPLOAD);
			CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(alignBytes()) };
			pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mpIntermedRes));
		}
		void createDescriptorHeap(ComPtr<ID3D12Device10>& pDevice) noexcept {
			D3D12_DESCRIPTOR_HEAP_DESC cbvhd{};
			cbvhd.NumDescriptors = 1u;
			cbvhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			cbvhd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			pDevice->CreateDescriptorHeap(&cbvhd, IID_PPV_ARGS(&mpCBVHeap));
			mpCBVHeapSize = (uint8_t)pDevice->GetDescriptorHandleIncrementSize(cbvhd.Type);
		}
		void createConstantBufferView(ComPtr<ID3D12Device10>& pDevice, ComPtr<ID3D12GraphicsCommandList7>& pCommandList, const GID::DSU::VertexConstantBufferData& data) noexcept {
			D3D12_SUBRESOURCE_DATA srd{}; {
				srd.pData = &data;
				srd.RowPitch = alignBytes();
				srd.SlicePitch = srd.RowPitch;
			}
			UpdateSubresources(pCommandList.Get(), mpDestRes.Get(), mpIntermedRes.Get(), 0u, 0u, 1u, &srd);
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvd{}; {
				cbvd.BufferLocation = mpDestRes->GetGPUVirtualAddress();
				cbvd.SizeInBytes = alignBytes();
			}
			pDevice->CreateConstantBufferView(&cbvd, mpCBVHeap->GetCPUDescriptorHandleForHeapStart());
		}

		ComPtr<ID3D12Resource2>& getDestRes() noexcept { return mpDestRes; }
		ComPtr<ID3D12Resource2>& getIntermedRes() noexcept { return mpIntermedRes; }
		UINT alignBytes() const noexcept {
			return (UINT)(sizeof(VertexConstantBufferData) % 256 == 0
				? sizeof(VertexConstantBufferData)
				: 256 - sizeof(VertexConstantBufferData) % 256 + sizeof(VertexConstantBufferData));
		}
		void transitionToRead(ComPtr<ID3D12GraphicsCommandList7>& pCommandList) noexcept {
			auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
			pCommandList->ResourceBarrier(1u, &barrier);
		}
		void transitionToWrite(ComPtr<ID3D12GraphicsCommandList7>& pCommandList) noexcept {
			auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
				D3D12_RESOURCE_STATE_COPY_DEST);
			pCommandList->ResourceBarrier(1u, &barrier);
		}
		void updateResource(ComPtr<ID3D12Device10>& pDevice, ComPtr<ID3D12GraphicsCommandList7>& pCommandList, const VertexConstantBufferData& data) {
			mSRD.pData = &data;
			UpdateSubresources(pCommandList.Get(), mpDestRes.Get(), mpIntermedRes.Get(), 0u, 0u, 1u, &mSRD);
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvd{}; {
				cbvd.BufferLocation = mpDestRes->GetGPUVirtualAddress();
				cbvd.SizeInBytes = alignBytes();
			}
			pDevice->CreateConstantBufferView(&cbvd, mpCBVHeap->GetCPUDescriptorHandleForHeapStart());
		}
	};
}

// Pixel Constant Buffer Structure
namespace GID::DSU {
	struct PixelConstantBuffer {

		ComPtr<ID3D12Resource2>			mpDestRes{};
		ComPtr<ID3D12Resource2>			mpIntermedRes{};
		ComPtr<ID3D12DescriptorHeap>	mpCBVHeap{};
		uint8_t							mpCBVHeapSize{};
		D3D12_SUBRESOURCE_DATA			mSRD{};

		PixelConstantBuffer() = default;
		PixelConstantBuffer(ComPtr<ID3D12Device10>& pDevice, ComPtr<ID3D12GraphicsCommandList7>& pCommandList, const PixelConstantBufferData& data) {
			createDestinationResource(pDevice);
			createIntermediateResource(pDevice);
			createDescriptorHeap(pDevice);
			createConstantBufferView(pDevice, pCommandList, data);
		}
		void createDestinationResource(ComPtr<ID3D12Device10>& pDevice) noexcept {
			CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_DEFAULT);
			CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(alignBytes()) };
			pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&mpDestRes));
		}
		void createIntermediateResource(ComPtr<ID3D12Device10>& pDevice) noexcept {
			CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_UPLOAD);
			CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(alignBytes()) };
			pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mpIntermedRes));
		}
		void createDescriptorHeap(ComPtr<ID3D12Device10>& pDevice) noexcept {
			D3D12_DESCRIPTOR_HEAP_DESC cbvhd{};
			cbvhd.NumDescriptors = 1u;
			cbvhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			cbvhd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			pDevice->CreateDescriptorHeap(&cbvhd, IID_PPV_ARGS(&mpCBVHeap));
			mpCBVHeapSize = (uint8_t)pDevice->GetDescriptorHandleIncrementSize(cbvhd.Type);
		}
		void createConstantBufferView(ComPtr<ID3D12Device10>& pDevice, ComPtr<ID3D12GraphicsCommandList7>& pCommandList, const PixelConstantBufferData& data) noexcept {
			mSRD.pData = &data;
			mSRD.RowPitch = alignBytes();
			mSRD.SlicePitch = mSRD.RowPitch;
			UpdateSubresources(pCommandList.Get(), mpDestRes.Get(), mpIntermedRes.Get(), 0u, 0u, 1u, &mSRD);
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvd{}; {
				cbvd.BufferLocation = mpDestRes->GetGPUVirtualAddress();
				cbvd.SizeInBytes = alignBytes();
			}
			pDevice->CreateConstantBufferView(&cbvd, mpCBVHeap->GetCPUDescriptorHandleForHeapStart());
		}
		inline ComPtr<ID3D12Resource2>& getDestRes() noexcept { return mpDestRes; }
		inline ComPtr<ID3D12Resource2>& getIntermedRes() noexcept { return mpIntermedRes; }
		inline UINT alignBytes() const noexcept {
			return (UINT)(sizeof(PixelConstantBufferData) % 256 == 0
				? sizeof(PixelConstantBufferData)
				: 256 - sizeof(PixelConstantBufferData) % 256 + sizeof(PixelConstantBufferData));
		}
		void transitionToRead(ComPtr<ID3D12GraphicsCommandList7>& pCommandList) noexcept {
			auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
			pCommandList->ResourceBarrier(1u, &barrier);
		}
		void transitionToWrite(ComPtr<ID3D12GraphicsCommandList7>& pCommandList) noexcept {
			auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
				D3D12_RESOURCE_STATE_COPY_DEST);
			pCommandList->ResourceBarrier(1u, &barrier);
		}
		void updateResource(ComPtr<ID3D12Device10>& pDevice, ComPtr<ID3D12GraphicsCommandList7>& pCommandList, const PixelConstantBufferData& data) {
			mSRD.pData = &data;
			UpdateSubresources(pCommandList.Get(), mpDestRes.Get(), mpIntermedRes.Get(), 0u, 0u, 1u, &mSRD);
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvd{}; {
				cbvd.BufferLocation = mpDestRes->GetGPUVirtualAddress();
				cbvd.SizeInBytes = alignBytes();
			}
			pDevice->CreateConstantBufferView(&cbvd, mpCBVHeap->GetCPUDescriptorHandleForHeapStart());
		}
	};
}

// Model File Data Structures
namespace GID::DSU {
	enum class MaterialFileDataType {
		NEWMTL,
		KA, KD, KS, TF,
		IF, D, NS, SHARP, NI,
		INVALID
	};
	enum class ObjectFileDataType {
		USEMTL,
		O, V, VT, VN, F,
		INVALID
	};
	struct MaterialFileData {
		std::string name{};
		struct {
			bool isRGB{ false }, isSpectral{ false }, isSpectralFactor{ false }, isXYZ{ false };
			AssistMath::AMFLOAT3 RGB{ 0.0f, 0.0f, 0.0f }, XYZ{ 0.0f, 0.0f, 0.0f };
			std::string file{}; float factor{};
		} KA, KD, KS, TF;
		std::array<bool, 11u> IF{ false, false, false, false, false, false, false, false, false, false, false };
		float D{}; bool isDHalo{ false };
		// Material Textures /* Implement later */
		float NS{}, sharpness{}, NI{};
	};
	struct ObjectFileData {
		std::string name{};
		std::vector<AssistMath::AMFLOAT2> tex{};
		std::vector<AssistMath::AMFLOAT3> pos{}, norm{};
		std::vector<AssistMath::AMUINT3X3> ind{};
		MaterialFileData mtl{};
	};
	struct ModelFileData {
		std::vector<ObjectFileData> ofd{};
	};
	struct AnimationPackageData {
		std::vector<ModelFileData> apd{};
	};
	struct ActorData {
		std::string name{};
		std::vector<AnimationPackageData> ad{};
	};
}

inline ComPtr<ID3D12Resource2> gTexture0{};
inline ComPtr<ID3D12Resource2> gTexture1{};
inline ComPtr<ID3D12DescriptorHeap> gTexture2{};
inline std::vector<D3D12_SUBRESOURCE_DATA> gTexture3{};

// Model File Parsing Functions
namespace GID::Util::FileParsing {
	inline GID::DSU::ActorData parse(std::string dir) {

		using FLOAT3 = GID::DSU::AssistMath::AMFLOAT3;
		using FLOAT2 = GID::DSU::AssistMath::AMFLOAT2;
		using UINT3X3 = GID::DSU::AssistMath::AMUINT3X3;
		using namespace GID::DSU;
		using namespace GID::GSO::Render;

		HRESULT hr{};

		ActorData data{};
		std::ifstream file("res\\actor\\" + dir + ".bin", std::ios::binary); uint32_t _size{}; std::string _str{};

		// Actor name
		file.read((char*)&_size, sizeof uint32_t);
		_str.resize(_size);
		file.read(_str.data(), _size);
		data.name = _str;
		
		// Amount of animations
		file.read((char*)&_size, sizeof uint32_t);
		data.ad.resize(_size);
		for (auto& ad : data.ad) {

			// Amount of frames (models)
			file.read((char*)&_size, sizeof uint32_t);
			ad.apd.resize(_size);
			for (auto& apd : ad.apd) {
				
				// Amount of objects
				file.read((char*)&_size, sizeof uint32_t);
				apd.ofd.resize(_size);
				for (auto& ofd : apd.ofd) {
					
					// Object name
					file.read((char*)&_size, sizeof uint32_t);
					_str.resize(_size);
					file.read(_str.data(), _size);
					ofd.name = _str;
					
					// Position Data
					file.read((char*)&_size, sizeof uint32_t);
					ofd.pos.resize(_size);
					for (auto& pos : ofd.pos) file.read((char*)&pos, sizeof FLOAT3);
					
					// Texture Data
					file.read((char*)&_size, sizeof uint32_t);
					ofd.tex.resize(_size);
					for (auto& tex : ofd.tex) file.read((char*)&tex, sizeof FLOAT2);
					
					// Normal Data
					file.read((char*)&_size, sizeof uint32_t);
					ofd.norm.resize(_size);
					for (auto& norm : ofd.norm) file.read((char*)&norm, sizeof FLOAT3);
					
					// Indices Data
					file.read((char*)&_size, sizeof uint32_t);
					ofd.ind.resize(_size);
					for (auto& ind : ofd.ind) file.read((char*)&ind, sizeof UINT3X3);

					// Material name
					file.read((char*)&_size, sizeof uint32_t);
					_str.resize(_size);
					file.read(_str.data(), _size);
					ofd.mtl.name = _str;
					
					// Material Data
					file.read((char*)&ofd.mtl.KA.isRGB, sizeof(bool));
					file.read((char*)&ofd.mtl.KA.isSpectral, sizeof(bool));
					file.read((char*)&ofd.mtl.KA.isSpectralFactor, sizeof(bool));
					file.read((char*)&ofd.mtl.KA.isXYZ, sizeof(bool));
					file.read((char*)&ofd.mtl.KA.RGB, sizeof(FLOAT3));
					file.read((char*)&ofd.mtl.KA.XYZ, sizeof(FLOAT3));
					file.read((char*)&_size, sizeof uint32_t);
					_str.resize(_size);
					file.read(_str.data(), _size);
					ofd.mtl.KA.file = _str;
					file.read((char*)&ofd.mtl.KA.factor, sizeof(float));
					
					
					file.read((char*)&ofd.mtl.KD.isRGB, sizeof(bool));
					file.read((char*)&ofd.mtl.KD.isSpectral, sizeof(bool));
					file.read((char*)&ofd.mtl.KD.isSpectralFactor, sizeof(bool));
					file.read((char*)&ofd.mtl.KD.isXYZ, sizeof(bool));
					file.read((char*)&ofd.mtl.KD.RGB, sizeof(FLOAT3));
					file.read((char*)&ofd.mtl.KD.XYZ, sizeof(FLOAT3));
					file.read((char*)&_size, sizeof uint32_t);
					_str.resize(_size);
					file.read(_str.data(), _size);
					ofd.mtl.KD.file = _str;
					file.read((char*)&ofd.mtl.KD.factor, sizeof(float));
					
					
					file.read((char*)&ofd.mtl.KS.isRGB, sizeof(bool));
					file.read((char*)&ofd.mtl.KS.isSpectral, sizeof(bool));
					file.read((char*)&ofd.mtl.KS.isSpectralFactor, sizeof(bool));
					file.read((char*)&ofd.mtl.KS.isXYZ, sizeof(bool));
					file.read((char*)&ofd.mtl.KS.RGB, sizeof(FLOAT3));
					file.read((char*)&ofd.mtl.KS.XYZ, sizeof(FLOAT3));
					file.read((char*)&_size, sizeof uint32_t);
					_str.resize(_size);
					file.read(_str.data(), _size);
					ofd.mtl.KS.file = _str;
					file.read((char*)&ofd.mtl.KS.factor, sizeof(float));
					
					
					file.read((char*)&ofd.mtl.TF.isRGB, sizeof(bool));
					file.read((char*)&ofd.mtl.TF.isSpectral, sizeof(bool));
					file.read((char*)&ofd.mtl.TF.isSpectralFactor, sizeof(bool));
					file.read((char*)&ofd.mtl.TF.isXYZ, sizeof(bool));
					file.read((char*)&ofd.mtl.TF.RGB, sizeof(FLOAT3));
					file.read((char*)&ofd.mtl.TF.XYZ, sizeof(FLOAT3));
					file.read((char*)&_size, sizeof uint32_t);
					_str.resize(_size);
					file.read(_str.data(), _size);
					ofd.mtl.TF.file = _str;
					file.read((char*)&ofd.mtl.TF.factor, sizeof(float));
					for (auto& IF : ofd.mtl.IF) file.read((char*)&IF, sizeof(bool));
					file.read((char*)&ofd.mtl.D, sizeof(float));
					file.read((char*)&ofd.mtl.isDHalo, sizeof(bool));
					file.read((char*)&ofd.mtl.NS, sizeof(float));
					file.read((char*)&ofd.mtl.sharpness, sizeof(float));
					file.read((char*)&ofd.mtl.NI, sizeof(float));
				}
			}
		}

		// Get texture file
		TexMetadata metadata{}; ScratchImage scratchImage{};
		if (LoadFromWICFile(L"Textures\\Dragon_Bump_Col2.jpg", WIC_FLAGS_FORCE_RGB, &metadata, scratchImage) != S_OK) throw std::exception("Cannot load texture file.");
		ScratchImage mipChain{};

		uint32_t _0 = 1;
		uint32_t h{ (uint32_t)metadata.height };
		uint32_t w{ (uint32_t)metadata.width };
		while (h > 1 || w > 1) {
			if (h > 1) h >>= 1;
			if (w > 1) w >>= 1;
			++_0;
		}

		GenerateMipMaps(scratchImage.GetImages(), scratchImage.GetImageCount(), metadata, TEX_FILTER_LINEAR, (size_t)_0, mipChain);

		D3D12_RESOURCE_DESC _1{}; {
			DXGI_FORMAT format{ mipChain.GetMetadata().format };
			uint64_t x{ (uint64_t)mipChain.GetMetadata().width };
			uint32_t y{ (uint32_t)mipChain.GetMetadata().height };
			switch (metadata.dimension) {
				case TEX_DIMENSION_TEXTURE1D: _1 = CD3DX12_RESOURCE_DESC::Tex1D(format, x, (uint16_t)mipChain.GetMetadata().arraySize, (uint16_t)mipChain.GetMetadata().mipLevels); break;
				case TEX_DIMENSION_TEXTURE2D: _1 = CD3DX12_RESOURCE_DESC::Tex2D(format, x, y, (uint16_t)mipChain.GetMetadata().arraySize, (uint16_t)mipChain.GetMetadata().mipLevels); break;
				case TEX_DIMENSION_TEXTURE3D: _1 = CD3DX12_RESOURCE_DESC::Tex3D(format, x, y, (uint16_t)mipChain.GetMetadata().depth, (uint16_t)mipChain.GetMetadata().mipLevels); break;
				default: throw std::exception("Invalid texture dimension."); break;
			}
		}
		{
			CD3DX12_HEAP_PROPERTIES x{CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT)};
			D3D12_HEAP_FLAGS y{ D3D12_HEAP_FLAG_NONE };
			D3D12_RESOURCE_STATES z{ D3D12_RESOURCE_STATE_COPY_DEST };
			gGFX.mpDevice->CreateCommittedResource(&x, y, &_1, z, nullptr, IID_PPV_ARGS(&gTexture0));
		}
		{
			gTexture3.resize(mipChain.GetImageCount());
			const Image* ptr = mipChain.GetImages();
			for (uint32_t i = 0; i < mipChain.GetImageCount(); i++) {
				auto& sr = gTexture3[i];
				sr.RowPitch = ptr[i].rowPitch;
				sr.SlicePitch = ptr[i].slicePitch;
				sr.pData = ptr[i].pixels;
			}
		}
		{
			uint64_t sz = GetRequiredIntermediateSize(gTexture0.Get(), 0u, (uint32_t)gTexture3.size());
			CD3DX12_HEAP_PROPERTIES x{CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD)};
			D3D12_HEAP_FLAGS y{ D3D12_HEAP_FLAG_NONE };
			CD3DX12_RESOURCE_DESC z = CD3DX12_RESOURCE_DESC::Buffer(sz);
			D3D12_RESOURCE_STATES w{ D3D12_RESOURCE_STATE_GENERIC_READ };
			gGFX.mpDevice->CreateCommittedResource(&x, y, &z, w, nullptr, IID_PPV_ARGS(&gTexture1));
			UpdateSubresources(gGFX.mpGraphicsCommandList.Get(), gTexture0.Get(), gTexture1.Get(), 0, 0, (uint32_t)gTexture3.size(), gTexture3.data());
		}
		auto barrier{ CD3DX12_RESOURCE_BARRIER::Transition(gTexture0.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE) };
		gGFX.mpGraphicsCommandList->ResourceBarrier(1u, &barrier);
		gGFX.flushGPU();
		{
			D3D12_DESCRIPTOR_HEAP_DESC x{}; {
				x.NumDescriptors = 1u;
				x.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				x.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			}
			gGFX.mpDevice->CreateDescriptorHeap(&x, IID_PPV_ARGS(&gTexture2));
		}
		D3D12_SHADER_RESOURCE_VIEW_DESC _6{}; {
			_6.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			_6.Format = mipChain.GetMetadata().format;
			_6.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			_6.Texture2D.MipLevels = (uint32_t)mipChain.GetMetadata().mipLevels;
			gGFX.mpDevice->CreateShaderResourceView(gTexture0.Get(), &_6, gTexture2->GetCPUDescriptorHandleForHeapStart());
		}
		
		return data;
	}
}

// Scene Data - Lights
namespace GID::GSO::Scene {
		inline std::vector<GID::DSU::LightData> gLights{};
		inline void addLight(GID::DSU::LightData& light) {
			gLights.push_back(light);
		}
}

// Object Structure
namespace GID::DSU {
	struct Object {

		Position mPos{};
		Speed mSpeed{};
		SphereCollisionCheckData mCollision{};

		VSSRVPosSBData mVSPosSB{};
		VSSRVTexSBData mVSTexSB{};
		VSSRVNormSBData mVSNormSB{};
		VertexBuffer mVertexBuffer{};
		VertexConstantBuffer mVCB{};
		PixelConstantBuffer mPCB{};

		PixelConstantBufferData pcbData{};

		// Object Data
		//ObjectFileData mObjectFileData{};
		MaterialData mMaterialData{};
		

		Object() = default;
		Object(ObjectFileData& data) {
			
			using namespace AssistMath;

			// Process data
			std::vector<DSU::VSInputData> ind{}; ind.resize(data.ind.size() * 3u);
			std::vector<AssistMath::AMFLOAT3> pos{}; pos.resize(data.pos.size());
			std::vector<AssistMath::AMFLOAT2> tex{}; tex.resize(data.tex.size());
			std::vector<AssistMath::AMFLOAT3> norm{}; norm.resize(data.norm.size());
			uint32_t iter{ 0 };
			for (auto& p : data.pos) { pos.at(iter) = p; iter++; }
			iter = 0;
			for (auto& t : data.tex) { tex.at(iter) = t; iter++; }
			iter = 0;
			for (auto& n : data.norm) { norm.at(iter) = n; iter++; }
			iter = 0;
			for (auto& i : data.ind) {
				ind.at(iter) = { i.d[0], i.d[1], i.d[2] };
				iter++;
				ind.at(iter) = { i.d[3], i.d[4], i.d[5] };
				iter++;
				ind.at(iter) = { i.d[6], i.d[7], i.d[8] };
				iter++;
			}

			// Material Data
			{
				// Emissive Color
				{
					mMaterialData.colorEmissive.r = 0.0f;
					mMaterialData.colorEmissive.g = 0.0f;
					mMaterialData.colorEmissive.b = 0.0f;
					mMaterialData.colorEmissive.a = 1.0f;
				}
				// Ambient Color
				{
					mMaterialData.colorAmbient.r = data.mtl.KA.RGB.x;
					mMaterialData.colorAmbient.g = data.mtl.KA.RGB.y;
					mMaterialData.colorAmbient.b = data.mtl.KA.RGB.z;
					mMaterialData.colorAmbient.a = 1.0f;
				}
				// Diffuse Color
				{
					mMaterialData.colorDiffuse.r = data.mtl.KD.RGB.x;
					mMaterialData.colorDiffuse.g = data.mtl.KD.RGB.y;
					mMaterialData.colorDiffuse.b = data.mtl.KD.RGB.z;
					mMaterialData.colorDiffuse.a = 1.0f;
				}
				// Specular Color
				{
					mMaterialData.colorSpecular.r = data.mtl.KS.RGB.x;
					mMaterialData.colorSpecular.g = data.mtl.KS.RGB.y;
					mMaterialData.colorSpecular.b = data.mtl.KS.RGB.z;
					mMaterialData.colorSpecular.a = 1.0f;
				}
				// Misc
				{
					mMaterialData.specularPower = data.mtl.NS;
					mMaterialData.isTextured = false;
				}
			}

			mVSPosSB = { GSO::Render::gGFX.getDevice(), GSO::Render::gGFX.getCommandList(), pos };
			mVSTexSB = { GSO::Render::gGFX.getDevice(), GSO::Render::gGFX.getCommandList(), tex };
			mVSNormSB = { GSO::Render::gGFX.getDevice(), GSO::Render::gGFX.getCommandList(), norm };
			mVertexBuffer = { GSO::Render::gGFX.getDevice(), GSO::Render::gGFX.getCommandList(), ind.data(), ind.size() };

			using namespace GSO::Render;
			AssistMath::FAMMATRIX transformM{ getTransformMx() };
			AssistMath::FAMVECTOR det{ FAMMatrixDeterminant(transformM) };
			AssistMath::FAMMATRIX tpose{ FAMMatrixTranspose(transformM) };
			VertexConstantBufferData matrices{ transformM, gGFX.getCamera().getMatrix(), gGFX.getProjection(), FAMMatrixInverse(det, tpose) };

			mVCB = { GSO::Render::gGFX.getDevice(), gGFX.getCommandList(), matrices };

			pcbData.mtl = mMaterialData;
			mPCB = { GSO::Render::gGFX.getDevice(), gGFX.getCommandList(), pcbData };
		}

		Position& getPos() noexcept { return mPos; }
		Speed& getSpeed() noexcept { return mSpeed; }
		MaterialData& getMaterialData() noexcept { return mMaterialData; }

		void update() noexcept {
			AssistMath::FAMVECTOR dt = _mm_set_ps1(GSO::Update::gTicks);
			mPos.translation = _mm_add_ps(mPos.translation, _mm_mul_ps(mSpeed.deltaTranslation, dt));
			mPos.rotation = _mm_add_ps(mPos.rotation, _mm_mul_ps(mSpeed.deltaRotation, dt));
			//mPos.center = _mm_add_ps(mPos.center, _mm_mul_ps(mSpeed.deltaTranslation, dt));
		}
		void draw() noexcept {
				
			//mVertexBuffer.transitionToRead(mainGFX().getCommandList());
			//mainGFX().getCommandList()->IASetVertexBuffers(0u, 1u, &mVertexBuffer.getView());
			//mainGFX().getCommandList()->DrawInstanced(mVertexBuffer.getCount(), 1u, 0u, 0u);
			//mVertexBuffer.transitionToWrite(mainGFX().getCommandList());
			
		}
		void updateResources() noexcept {
			using namespace GSO::Render;
			AssistMath::FAMMATRIX transformM{ getTransformMx() };
			AssistMath::FAMVECTOR det{ FAMMatrixDeterminant(transformM) };
			AssistMath::FAMMATRIX tpose{ FAMMatrixTranspose(transformM) };
			VertexConstantBufferData matrices{
				transformM,
				gGFX.getCamera().getMatrix(),
				gGFX.getProjection(),
				FAMMatrixInverse(det, tpose)
			};
			pcbData.eyePos = { gGFX.getCamera().mEye.m128_f32[0], gGFX.getCamera().mEye.m128_f32[1], gGFX.getCamera().mEye.m128_f32[2], 1.0f };
			pcbData.globalAmbient = { 0.f, 0.f, 0.f, 1.f };
			for (int i = 0; i < GSO::Scene::gLights.size(); i++)
				pcbData.lights[i] = GSO::Scene::gLights[i];

			mVCB.updateResource(GSO::Render::gGFX.getDevice(), gGFX.getCommandList(), matrices);
			mPCB.updateResource(GSO::Render::gGFX.getDevice(), gGFX.getCommandList(), pcbData);
		}
		void transitionResourcesToRead() noexcept {
			using namespace GSO::Render;
			mVertexBuffer.transitionToRead(gGFX.getCommandList());
			mVCB.transitionToRead(gGFX.getCommandList());
			mVSPosSB.transitionToRead(gGFX.getCommandList());
			mVSTexSB.transitionToRead(gGFX.getCommandList());
			mVSNormSB.transitionToRead(gGFX.getCommandList());
			mPCB.transitionToRead(gGFX.getCommandList());
		}
		void doCommandList() noexcept {
			using namespace GSO::Render;
			
			gGFX.getCommandList()->SetGraphicsRootConstantBufferView(0u, mVCB.getDestRes()->GetGPUVirtualAddress());
			gGFX.getCommandList()->SetGraphicsRootShaderResourceView(1u, mVSPosSB.getDestRes()->GetGPUVirtualAddress());
			gGFX.getCommandList()->SetGraphicsRootShaderResourceView(2u, mVSTexSB.getDestRes()->GetGPUVirtualAddress());
			gGFX.getCommandList()->SetGraphicsRootShaderResourceView(3u, mVSNormSB.getDestRes()->GetGPUVirtualAddress());
			gGFX.getCommandList()->SetGraphicsRootConstantBufferView(4u, mPCB.getDestRes()->GetGPUVirtualAddress());
			gGFX.getCommandList()->SetDescriptorHeaps(1u, gTexture2.GetAddressOf());
			gGFX.getCommandList()->SetGraphicsRootDescriptorTable(5u, gTexture2->GetGPUDescriptorHandleForHeapStart());
			gGFX.getCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			gGFX.getCommandList()->IASetVertexBuffers(0u, 1u, &mVertexBuffer.getView());
			gGFX.getCommandList()->DrawInstanced(mVertexBuffer.getCount(), 1u, 0u, 0u);
		}
		void transitionResourcesToWrite() noexcept {
			using namespace GSO::Render;
			mVertexBuffer.transitionToWrite(gGFX.getCommandList());
			mVCB.transitionToWrite(gGFX.getCommandList());
			mVSPosSB.transitionToWrite(gGFX.getCommandList());
			mVSTexSB.transitionToWrite(gGFX.getCommandList());
			mVSNormSB.transitionToWrite(gGFX.getCommandList());
			mPCB.transitionToWrite(gGFX.getCommandList());
		}
		AssistMath::FAMMATRIX getTransformMx() noexcept {
			return {
			AssistMath::FAMMatrixRotationRollPitchYaw(mPos.rotation)
			* AssistMath::FAMMatrixTranslation(mPos.translation)
			};
		}
	};
}

// Actor File Data Collection
namespace GID::GSO::Collections::ActorData {
	inline std::unordered_map<std::string, GID::DSU::ActorData> mCollection{};
	inline auto& get() noexcept { return mCollection; }
	inline void cAdd(std::string& name, GID::DSU::ActorData ad) {
		mCollection.insert(std::pair<std::string, GID::DSU::ActorData>(name, ad));
	}
	inline void rAdd(std::string& name, GID::DSU::ActorData& ad) {
		mCollection.insert(std::pair<std::string, GID::DSU::ActorData>(name, ad));
	}
	inline bool bSearch(std::string& name) {
		for (auto& pair : mCollection) {
			if (pair.first == name) {
				return true;
			}
		}
		return false;
	}
	inline GID::DSU::ActorData& rSearch(std::string& name) {
		for (auto& pair : mCollection) {
			if (pair.first == name) {
				return pair.second;
			}
		}
	}
	inline GID::DSU::ActorData& quickSearchAndAdd(std::string& name) {
		if (bSearch(name)) return rSearch(name);
		else cAdd(name, GID::Util::FileParsing::parse(name));
		return rSearch(name);
	}
}

// Model Structure
namespace GID::DSU {
	struct Model {

		//ModelFileData mModelFileData{};
		std::vector<Object> mObjects{};

		Model() = default;
		Model(GID::DSU::ModelFileData& mfd) {
			mObjects.resize(mfd.ofd.size());
			uint32_t iter = 0;
			ModelFileData mModelFileData = mfd;
			for (auto& objectdata : mModelFileData.ofd) {
				mObjects.at(iter) = { objectdata };
				iter++;
			}
		}

		void update() noexcept { for (auto& o : mObjects) o.update(); }

		void draw() noexcept { for (auto& o : mObjects) o.draw(); }

		//GID::DSU::ModelFileData& getModelData() noexcept { return mModelFileData; }
		std::vector<Object>& getObjects() noexcept { return mObjects; }
	};
}

// ActorID Collection
namespace GID::GSO::Collections::ActorID {
	inline std::vector<std::pair<std::string, UINT>> mCollection{};
	inline auto& get() noexcept { return mCollection; }
	inline UINT add(std::string& actorID) {
		for (auto& str : mCollection) {
			if (actorID == str.first) {
				str.second++;
				return str.second;
			}
		}
		std::pair<std::string, UINT> newActorID{ actorID, 1u };
		mCollection.push_back(newActorID);
		return 1u;
	}
}

// A Data Structure to hold a single animation for an object
namespace GID::DSU {
	struct AnimationStruct {
		std::string mAnimationID{};
		std::vector<Model> mFrames{};
	};
	struct AnimationPackage {
		std::string mAnimationID{};
		std::vector<AnimationStruct> mAnimations{};

		/*AnimationPackage(std::string& path) {
			GID::Util::FileParsing::parse(path);
		}*/
	};
}

// Actor Structure
namespace GID::DSU {
	struct Actor : Inputtable, Scriptable {

		std::pair<std::string, UINT> mActorID{};
		AnimationPackage mAnimationPackage{};

		Timer mInitTimer{};
		Timer mCurrentStateTimer{};
			
		ActorGroundState mGroundState{ ActorGroundState::GROUND };

		Actor() = default;
		Actor(std::string& objPath) {
			using namespace GSO::Render;
			//std::vector<std::vector<VSInputData>> mIndices{};
			
			mActorID.first = objPath;
			mActorID.second = GID::GSO::Collections::ActorID::add(objPath);
			//mActorData = GID::GSO::Collections::ActorData::quickSearchAndAdd(objPath);
			ActorData mActorData = GID::Util::FileParsing::parse(objPath);
			
			mAnimationPackage.mAnimations.resize(mActorData.ad.size());
			
			mAnimationPackage.mAnimationID = mActorData.name;
			
			for (uint16_t iter{}; iter < mActorData.ad.at(0).apd.size(); iter++) {
				mAnimationPackage.mAnimations.at(0).mFrames.push_back({ mActorData.ad.at(0).apd.at(iter) });
			}
			mCurrentStateTimer.mark();
			mInitTimer.mark();
		}
		void input() noexcept override {

		}
		void update() noexcept {
			for (auto& a : mAnimationPackage.mAnimations)
				for (auto& m : a.mFrames)
					m.update();
		}
		void draw() noexcept {
			using namespace GSO::Render;
			uint32_t currFrameIndex{ (uint32_t)std::truncf(mInitTimer.peek() * 30.f * GSO::General::gCfgGen.gSpeedMultiplier) };
			currFrameIndex %= mAnimationPackage.mAnimations.at(0).mFrames.size();
			//currFrameIndex = 0;
			for (auto& o : mAnimationPackage.mAnimations.at(0).mFrames.at(currFrameIndex).getObjects()) {
				o.updateResources();
				o.transitionResourcesToRead();
				o.doCommandList();
				o.transitionResourcesToWrite();
			}
		}
	};
}

// Scene Data - Actor
namespace GID::GSO::Scene {
	inline std::vector<GID::DSU::Actor> gActors{};
	inline void addActor(std::string actorName) {
		gActors.push_back({ actorName });
	}
}

// Initialize the GSO
namespace GID::GSO::Util {
		inline void initGSO() {
			GSO::Render::Viewport::initViewportPresets();
		}
}

// General quickstart/preset initialization of the GSO
namespace GID::GSO::Util {
	inline void initQuickStart() {
		using namespace GSO; using namespace GSO::Render::Viewport;
		WindowNS::initWindow(1600, 900, L"Window");
		Util::initGSO();
		Render::initGFX({ ViewportPresets[(uint8_t)GID::DSU::ViewportPreset::VP1_DEFAULT] });
		Render::gGFX.m2D.initDebugText();

	}
}

// Debug Input Processing
namespace GID::GSO::Input {
	inline int doDebugInput() {
		if (IsWindowEnabled(WindowNS::gWnd->getHandle())) {
			if (gInput.kb.isKeyPressed(VK_ESCAPE)) return 1;
		}
		return 0;
	}
}

// Per-frame Process Input Function
namespace GID::GSO::Input {
	inline int doInput() {
		using namespace DSU;
		//auto ind = (uint8_t)WindowType::MAINWINDOW;
		bool inputDebug{ false };

		while (!gInput.kb.isKeyQueueEmpty()) {
			gInput.keys.push_back(gInput.kb.readKey());
			if (inputDebug) std::cout << "[Key Pressed (Code)] (" << gInput.keys.back().getCode() << ")" << '\n';
		}
		while (!gInput.kb.isCharQueueEmpty()) {
			gInput.keysChar.push_back(gInput.kb.readChar());
			if (inputDebug) std::cout << "[Character Inputted] (" << gInput.keysChar.back() << ")" << '\n';
		}
		while (!gInput.mouse.isEmpty()) {
			gInput.mouseEvents.push_back(gInput.mouse.readEvent());
			if (inputDebug) {
				std::cout << "[";
				switch (gInput.mouseEvents.back().getType()) {
				case Mouse::Event::Type::Enter:
					std::cout << "Enter]";
					break;
				case Mouse::Event::Type::Exit:
					std::cout << "Exit]";
					break;
				case Mouse::Event::Type::LMBPress:
					std::cout << "LMBPress]";
					break;
				case Mouse::Event::Type::LMBRelease:
					std::cout << "LMBRelease]";
					break;
				case Mouse::Event::Type::Move:
					std::cout << "Move] (" << gInput.mouseEvents.back().getX() << ", " << gInput.mouseEvents.back().getY() << ")";
					break;
				case Mouse::Event::Type::RMBPress:
					std::cout << "RMBPress]";
					break;
				case Mouse::Event::Type::RMBRelease:
					std::cout << "RMBRelease]";
					break;
				case Mouse::Event::Type::WheelDown:
					std::cout << "WheelDown]";
					break;
				case Mouse::Event::Type::WheelUp:
					std::cout << "WheelUp]";
					break;
				}
				std::cout << '\n';
			}
		}

		if (doDebugInput() != 0) return 1;
		Render::gGFX.mCamera.input();

		gInput.keys.clear();
		gInput.keysChar.clear();
		gInput.mouseEvents.clear();

		return 0;
	}
}

// Basic Gravity Script for Actors
namespace GID::GSO::Scripts::Update {
	//inline void doBasicGravity(GID::DSU::Actor& actor) {
	//	using namespace GID::GSO::Update;
	//	if (actor.mGroundState == DSU::ActorGroundState::AIR) {
	//		for (auto& o : actor.mModel.getObjects()) {
	//			o.getSpeed().deltaTranslation.m128_f32[1] += -19.6133f * (gTicks);
	//		}
	//		/*if (actor.getModel().getObjects().at(0).getSpeed().deltaTranslation.m128_f32[0] > -53.6448f / 1000.f) {
	//			for (auto& o : actor.getModel().getObjects()) {
	//				o.getSpeed().deltaTranslation.m128_f32[0] += -9.81f * 0.001f * 0.001f;
	//			}
	//		}*/
	//	}
	//}
}

// Realistic intersection-reaction collision script (will probably be replaced, but its code has useful physics math)
namespace GID::GSO::Scripts::Update {
	//inline void doBasicCollision(GID::DSU::Actor& actor, std::vector<GID::DSU::Actor>& actors) {
	//	using namespace DSU::AssistMath;
	//	struct Triangle {
	//		DSU::AssistMath::FAMVECTOR p1{};
	//		DSU::AssistMath::FAMVECTOR p2{};
	//		DSU::AssistMath::FAMVECTOR p3{};
	//	};
	//	struct Line {
	//		DSU::AssistMath::FAMVECTOR p1{};
	//		DSU::AssistMath::FAMVECTOR p2{};
	//	};
	//	// Gather all tris from the first actor
	//	std::vector<Triangle> actor0Tris{};
	//	{
	//		std::vector<Triangle> tris{};
	//					
	//		UINT iter = 0u;
	//		FAMVECTOR v0{};
	//		FAMVECTOR v1{};
	//		FAMVECTOR v2{};
	//					
	//		for (auto& o : actor.mModel.mObjects) {
	//			FAMMATRIX transformMx = o.getTransformMx();
	//			for (auto& v : o.mObjectFileData.pos) {

	//				FAMVECTOR vector{ AMLoadFloat4({ v.x, v.y, v.z, 1.0f }) };
	//				vector = FAMVector4Transform(vector, transformMx);

	//				if (iter == 0) v0 = { AMLoadFloat3({ vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2] }) };
	//				if (iter == 1) v1 = { AMLoadFloat3({ vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2] }) };
	//				if (iter == 2) {
	//					v2 = { vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2] };
	//					iter = 0;
	//					actor0Tris.push_back({ v0, v1, v2 });
	//					continue;
	//				}
	//				iter++;
	//			}
	//		}
	//	}

	//	// Cycle through all of the actors to check for collisions
	//	for (auto& a : actors) {

	//		// Don't want to check collisions with the same actor
	//		if (a.mActorID != actor.mActorID) {

	//			// Gather all tris from the second actor
	//			std::vector<Triangle> actor1Tris{};
	//			{
	//				std::vector<Triangle> tris{};

	//				UINT iter = 0u;
	//				FAMVECTOR v0{};
	//				FAMVECTOR v1{};
	//				FAMVECTOR v2{};

	//				for (auto& o : a.mModel.mObjects) {
	//					FAMMATRIX transformMx = o.getTransformMx();
	//					for (auto& v : o.mObjectFileData.pos) {

	//						FAMVECTOR vector{ AMLoadFloat4({ v.x, v.y, v.z, 1.0f }) };
	//						vector = FAMVector4Transform(vector, transformMx);

	//						if (iter == 0) v0 = { AMLoadFloat3({ vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2] }) };
	//						if (iter == 1) v1 = { AMLoadFloat3({ vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2] }) };
	//						if (iter == 2) {
	//							v2 = { vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2] };
	//							iter = 0;
	//							actor1Tris.push_back({ v0, v1, v2 });
	//							continue;
	//						}
	//						iter++;
	//					}
	//				}
	//			}

	//			// Determine if a triangle from actor 1 collides with a triangle of actor 2 (O(n) = n^2, make faster)
	//			float result{};
	//			FAMVECTOR triNorm{};
	//			Triangle collidedTri{};
	//			{
	//				float intersectingDist{};

	//				for (auto& tri0 : actor0Tris) {
	//					for (auto& tri1 : actor1Tris) {

	//						std::array<Line, 3u> lines = {
	//							{
	//								{ tri0.p1, tri0.p2 },
	//								{ tri0.p1, tri0.p3 },
	//								{ tri0.p2, tri0.p3 }
	//							}
	//						};

	//						for (UINT i = 0; i < lines.size(); i++) {

	//							enum Point {
	//								x,
	//								y,
	//								z
	//							};

	//							AMFLOAT3 V{
	//								tri1.p2.m128_f32[x] - tri1.p1.m128_f32[x],
	//								tri1.p2.m128_f32[y] - tri1.p1.m128_f32[y],
	//								tri1.p2.m128_f32[z] - tri1.p1.m128_f32[z]
	//							};

	//							AMFLOAT3 W{
	//								tri1.p3.m128_f32[x] - tri1.p1.m128_f32[x],
	//								tri1.p3.m128_f32[y] - tri1.p1.m128_f32[y],
	//								tri1.p3.m128_f32[z] - tri1.p1.m128_f32[z]
	//							};

	//							float normX = (V.y * W.z) - (V.z * W.y);
	//							float normY = (V.z * W.x) - (V.x * W.z);
	//							float normZ = (V.x * W.y) - (V.y * W.x);

	//							float normConst = std::sqrtf(normX * normX + normY * normY + normZ * normZ);

	//							float Ax = normX / normConst;
	//							float Ay = normY / normConst;
	//							float Az = normZ / normConst;

	//							float test = std::sqrtf(Ax * Ax + Ay * Ay + Az * Az);

	//							float A = Ax;
	//							float B = Ay;
	//							float C = Az;
	//							float a = tri1.p1.m128_f32[x];
	//							float b = tri1.p1.m128_f32[y];
	//							float c = tri1.p1.m128_f32[z];
	//							float D = -(A * a + B * b + C * c);

	//							float triA = A;
	//							float triB = B;
	//							float triC = C;

	//							float result0 = (A * lines[i].p1.m128_f32[x] + B * lines[i].p1.m128_f32[y] + C * lines[i].p1.m128_f32[z] + D)
	//								/ std::sqrtf(A * A + B * B + C * C);

	//							float result1 = (A * lines[i].p2.m128_f32[x] + B * lines[i].p2.m128_f32[y] + C * lines[i].p2.m128_f32[z] + D)
	//								/ std::sqrtf(A * A + B * B + C * C);

	//							bool isDifferentSign{ (signbit(result0) == signbit(result1)) ? false : true };

	//							if (isDifferentSign) {

	//								FAMVECTOR outsidePoint{ (result1 > 0.0f) ? lines[i].p2 : lines[i].p1 };
	//								FAMVECTOR insidePoint{ (result1 > 0.0f) ? lines[i].p1 : lines[i].p2 };

	//								Triangle plane0tri{};
	//								plane0tri.p1 = outsidePoint;
	//								plane0tri.p2 = tri1.p2;
	//								plane0tri.p3 = tri1.p1;
	//								Triangle plane1tri{};
	//								plane1tri.p1 = outsidePoint;
	//								plane1tri.p2 = tri1.p3;
	//								plane1tri.p3 = tri1.p2;
	//								Triangle plane2tri{};
	//								plane2tri.p1 = outsidePoint;
	//								plane2tri.p2 = tri1.p1;
	//								plane2tri.p3 = tri1.p3;

	//								AMFLOAT3 vector0{
	//									plane0tri.p2.m128_f32[x] - plane0tri.p1.m128_f32[x],
	//									plane0tri.p2.m128_f32[y] - plane0tri.p1.m128_f32[y],
	//									plane0tri.p2.m128_f32[z] - plane0tri.p1.m128_f32[z] };
	//								AMFLOAT3 vector1{
	//									plane0tri.p3.m128_f32[x] - plane0tri.p1.m128_f32[x],
	//									plane0tri.p3.m128_f32[y] - plane0tri.p1.m128_f32[y],
	//									plane0tri.p3.m128_f32[z] - plane0tri.p1.m128_f32[z] };

	//								float normX = (vector0.y * vector1.z) - (vector0.z * vector1.y);
	//								float normY = (vector0.z * vector1.x) - (vector0.x * vector1.z);
	//								float normZ = (vector0.x * vector1.y) - (vector0.y * vector1.x);

	//								float normConst = std::sqrtf(normX * normX + normY * normY + normZ * normZ);

	//								float Ax = normX / normConst;
	//								float Ay = normY / normConst;
	//								float Az = normZ / normConst;

	//								float A = Ax;
	//								float B = Ay;
	//								float C = Az;
	//								float a = outsidePoint.m128_f32[x];
	//								float b = outsidePoint.m128_f32[y];
	//								float c = outsidePoint.m128_f32[z];
	//								float D = -(A * a + B * b + C * c);

	//								float result = (A * insidePoint.m128_f32[x] + B * insidePoint.m128_f32[y] + C * insidePoint.m128_f32[z] + D)
	//									/ std::sqrt(A * A + B * B + C * C);

	//								if (result < 0.0f) continue;

	//								vector0 = {
	//									plane1tri.p2.m128_f32[x] - plane1tri.p1.m128_f32[x],
	//									plane1tri.p2.m128_f32[y] - plane1tri.p1.m128_f32[y],
	//									plane1tri.p2.m128_f32[z] - plane1tri.p1.m128_f32[z]
	//								};

	//								vector1 = {
	//									plane1tri.p3.m128_f32[x] - plane1tri.p1.m128_f32[x],
	//									plane1tri.p3.m128_f32[y] - plane1tri.p1.m128_f32[y],
	//									plane1tri.p3.m128_f32[z] - plane1tri.p1.m128_f32[z]
	//								};

	//								normX = (vector0.y * vector1.z) - (vector0.z * vector1.y);
	//								normY = (vector0.z * vector1.x) - (vector0.x * vector1.z);
	//								normZ = (vector0.x * vector1.y) - (vector0.y * vector1.x);

	//								normConst = std::sqrtf(normX * normX + normY * normY + normZ * normZ);

	//								Ax = normX / normConst;
	//								Ay = normY / normConst;
	//								Az = normZ / normConst;

	//								A = Ax;
	//								B = Ay;
	//								C = Az;
	//								a = outsidePoint.m128_f32[x];
	//								b = outsidePoint.m128_f32[y];
	//								c = outsidePoint.m128_f32[z];
	//								D = -(A * a + B * b + C * c);

	//								result = (A * insidePoint.m128_f32[x] + B * insidePoint.m128_f32[y] + C * insidePoint.m128_f32[z] + D)
	//									/ std::sqrtf(A * A + B * B + C * C);

	//								if (result < 0.0f) continue;

	//								vector0 = {
	//									plane2tri.p2.m128_f32[x] - plane2tri.p1.m128_f32[x],
	//									plane2tri.p2.m128_f32[y] - plane2tri.p1.m128_f32[y],
	//									plane2tri.p2.m128_f32[z] - plane2tri.p1.m128_f32[z]
	//								};

	//								vector1 = {
	//									plane2tri.p3.m128_f32[x] - plane2tri.p1.m128_f32[x],
	//									plane2tri.p3.m128_f32[y] - plane2tri.p1.m128_f32[y],
	//									plane2tri.p3.m128_f32[z] - plane2tri.p1.m128_f32[z]
	//								};

	//								normX = (vector0.y * vector1.z) - (vector0.z * vector1.y);
	//								normY = (vector0.z * vector1.x) - (vector0.x * vector1.z);
	//								normZ = (vector0.x * vector1.y) - (vector0.y * vector1.x);

	//								normConst = std::sqrtf(normX * normX + normY * normY + normZ * normZ);

	//								Ax = normX / normConst;
	//								Ay = normY / normConst;
	//								Az = normZ / normConst;

	//								A = Ax;
	//								B = Ay;
	//								C = Az;
	//								a = outsidePoint.m128_f32[x];
	//								b = outsidePoint.m128_f32[y];
	//								c = outsidePoint.m128_f32[z];
	//								D = -(A * a + B * b + C * c);

	//								result = (A * insidePoint.m128_f32[x] + B * insidePoint.m128_f32[y] + C * insidePoint.m128_f32[z] + D)
	//									/ std::sqrtf(A * A + B * B + C * C);

	//								if (result >= 0.0f) {
	//									if (result0 > 0.0f)
	//										if (result0 > intersectingDist)
	//											intersectingDist = result1;
	//										else intersectingDist = result0;
	//									collidedTri = tri1;
	//									triNorm = { AMLoadFloat4({ triA, triB, triC, 0.0f }) };
	//								}
	//							}
	//						}
	//					}
	//				}
	//				result = intersectingDist;
	//			}

	//			// If the result is negative, then we know that a collision has taken place
	//			// (result is the amount of distance intersected)
	//			if (result < 0.0f) {

	//				FAMVECTOR velocity = actor.mModel.getObjects().at(0).getSpeed().deltaTranslation;
	//				FAMVECTOR translation = actor.mModel.getObjects().at(0).getPos().translation;

	//				enum Point {
	//					x,
	//					y,
	//					z
	//				};

	//				// Find the length intersected along the velocity vector of the object
	//				float fVelocity = FAMVector4Magnitude(FAMVector4Negate(velocity));

	//				float dotVNSum = FAMVector4SumVector(FAMVector4DotProduct(triNorm, FAMVector4Negate(velocity)));
	//				float magnitudeProduct = FAMVector4Magnitude(triNorm) * fVelocity;
	//				float cosofangle = dotVNSum / magnitudeProduct;

	//				float angle = std::acosf(cosofangle);

	//				#if defined(_DEBUG)
	//				std::cout << "Collision detected!!! Reactionary movement: " << result / std::cos(angle) << '\n';
	//				#endif

	//				std::cout << AMConvertToDegrees(angle) << '\n';

	//							 
	//				for (auto& o : actor.mModel.mObjects) {
	//								
	//					// Push the object out
	//					o.getPos().translation =
	//						_mm_add_ps(
	//							translation,
	//							_mm_mul_ps(
	//								_mm_mul_ps(FAMVector4Negate(velocity),
	//									_mm_set_ps1(result / std::cos(angle))),
	//								_mm_set_ps1(GID::GSO::Update::gTicks)));
	//					// ---

	//					// Determine the reflected momentum vector of the actor
	//					FAMVECTOR refl =
	//						_mm_sub_ps(
	//							velocity,
	//							_mm_mul_ps(
	//								_mm_set_ps1(2.0f * FAMVector4SumVector(FAMVector4DotProduct(velocity, triNorm))),
	//								triNorm));
	//					// ---

	//					// Set the actor's momentum vector to the reflected momentum vector
	//					o.getSpeed().deltaTranslation = refl;

	//					// Multiply the distance the actor was intersected by the new reflected momentum vector to reproduce
	//				// the amount moved by the bounce on the triangle.
	//					o.getPos().translation =
	//						_mm_add_ps(
	//							actor.mModel.getObjects().at(0).getPos().translation,
	//							_mm_mul_ps(
	//								FAMVector4Negate(_mm_mul_ps(
	//									actor.mModel.getObjects().at(0).getSpeed().deltaTranslation,
	//									_mm_set_ps1(GID::GSO::Update::gTicks))),
	//								_mm_set_ps1(result / std::cos(angle))));
	//				}
	//			}
	//		}
	//	}
	//}
}

// Advanced Camera Following Script
namespace GID::GSO::Scripts {
	namespace GlobalVariables {
		inline uint16_t gCameraActorFollowIndex{};
	}
	namespace Update {
		inline void doAdvancedCameraFollow() {
			if (Render::gGFX.getCamera().mbFollow) {

			}
		}
	}
}

// Script Factory (must go after all scripts)
namespace GID::GSO::Scripts::Factory {
	/*void processInputScripts() noexcept {
		for (auto& a : scene.getActors()) {
			for (auto& s : a.getScripts()) {

			}
		}
	}*/
	inline void processUpdateScripts() noexcept {
		for (auto& a : GID::GSO::Scene::gActors) {
			for (auto& s : a.mScripts) {
				switch (s) {
				case DSU::ScriptID::BasicGravity:
					//Update::doBasicGravity(a);
					break;
				case DSU::ScriptID::BasicCollision:
					//Update::doBasicCollision(a, GID::GSO::Scene::gActors);
					break;
				}
			}
		}
	}
	inline void processCameraScripts() noexcept {
		for (auto& script : Render::gGFX.getCamera().getScripts()) {
			switch (script) {
			case DSU::ScriptID::AdvancedCameraFollow:
				Update::doAdvancedCameraFollow();
				break;
			}
		}
	}
}

// Do update stuff
namespace GID::GSO::Update {
	inline void doUpdate() {
		Render::gGFX.mCamera.update();
		Scripts::Factory::processUpdateScripts();
		for (auto& a : Scene::gActors)
			a.update();
		Scripts::Factory::processCameraScripts();
	}
}

// ---
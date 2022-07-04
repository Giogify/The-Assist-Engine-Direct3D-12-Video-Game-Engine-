#pragma once
// Includes below - Note: there shouldn't be any source file includes here.
#include "d3dx12.h"

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

#include <d3dcompiler.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <ammintrin.h>
#include <immintrin.h>
#include <Windows.h>

#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "dxguid.lib")

#define WIN32_LEAN_AND_MEAN

using namespace Microsoft::WRL;

// The Global Information Database. Contains data structures to be instantiated by the Global State Object, which is also contained herein.
// The Global State Object contains instances of data to track the current application state / game state.

// Entire AssistMath lib.
namespace GID {
	namespace DSU {
		namespace AssistMath {
			constexpr float AM_PI = 3.1415926535897932384626433832795;
			constexpr float AM_2PI = 6.283185307179586476925286766559;
			constexpr float AM_1DIVPI = 3.1415926535897932384626433832795;
			constexpr float AM_1DIV2PI = 0.15915494309189533576888376337251;
			constexpr float AM_PIDIV2 = 1.5707963267948966192313216916398;
			constexpr float AM_PIDIV4 = 0.78539816339744830961566084581988;

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
				float ySc = 1.0 / std::tan(fov / 2.0);
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

				//F = FAMVector4Normalize(F);
				//R = FAMVector4Normalize(R);
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
	}
}

// Some basic DSU structs and enums (that don't depend on user-defined types)
namespace GID {
	namespace DSU {
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
			float constAtten{ 1.0f };
			float linAtten{ 0.08f };
			float quadAtten{ 0.0f };

			int32_t type{ (int8_t)LightConst::POINT_LIGHT };
			int32_t isEnabled{ false };
			int32_t padding0{};
			int32_t padding1{};
		};
		struct VertexData {
			struct {
				float x{};
				float y{};
				float z{};
			} pos;
			struct {
				float x{};
				float y{};
			} texcoord;
			struct {
				float x{};
				float y{};
				float z{};
			} norm;
		};
		struct PositionData {
			AssistMath::AMFLOAT3 pos{};
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
		struct PipelineStateStream {
			CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
			CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
			CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopologyType;
			CD3DX12_PIPELINE_STATE_STREAM_VS VS;
			CD3DX12_PIPELINE_STATE_STREAM_PS PS;
			CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
			CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
			CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER RS;
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
}

// Script IDs
namespace GID {
	namespace DSU {
		enum class ScriptID {
			// Input Scripts
			BasicActorMove,

			// Update Scripts
			BasicCameraFollow,

			// Physics Scripts
			BasicGravity,
			BasicCollision,

			INVALID,
		};
	}
}

// Scriptable Interface
namespace GID {
	namespace DSU {
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
}

// Inputtable Interface
namespace GID {
	namespace DSU {
		struct Inputtable {
			virtual void input() noexcept = 0;
		};
	}
}

// Keyboard
namespace GID {
	namespace DSU {
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
				}
				else { return Keyboard::Event(); }
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
				}
				else return 0;
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
}

// Mouse
namespace GID {
	namespace DSU {
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
				}
				else { return Mouse::Event(); }
			}

			// Empty the Buffer
			void clear() noexcept { mBuffer = std::queue<Event>(); }

			// Is the Mouse in the Window
			bool isInsideWindow() const noexcept { return mIsInsideWindow; }

		};
	}
}

// Packed Input
namespace GID {
	namespace DSU {
		struct PackedInput {
			Keyboard kb{};
			std::vector<Keyboard::Event> keys{};
			std::vector<uint8_t> keysChar{};
			Mouse mouse{};
			std::vector<Mouse::Event> mouseEvents{};
		};
	}
}

// General Global State
namespace GID {
	namespace GSO {
		namespace General {
			struct {
				float gCurrentFrameDeltaTime{};
			} gGenVar;
			struct {
				float gMaxFPS{ 0.0f };
				float gFPSCap{ 60.0f };
				float gCameraAngle{ 70.0f };
				float gSpeedMultiplier{ 1.0f };
			} gCfgGen;
		}
	}
}

// Global Per-frame Packed Input Vector
namespace GID {
	namespace GSO {
		namespace Input {
			inline std::vector<GID::DSU::PackedInput> gInput{};
		}
	}
}

// Global Window Count
namespace GID {
	namespace GSO {
		namespace WindowNS {
			inline uint8_t windowCt{};
		}
	}
}

// Window Struct
namespace GID {
	namespace DSU {
		struct Window {

			const std::wstring	wndClassName{ L"AssistEngine_ver0.5_indev" };
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
					if (!(lParam & 0x40000000) || gInput[WindowID].kb.isAutoRepeatEnabled()) {
						gInput[WindowID].kb.onKeyPressed((uint8_t)wParam);
					}
					break;

					// When System Key is Pressed
				case WM_SYSKEYDOWN:
					if (!(lParam & 0x40000000) || gInput[WindowID].kb.isAutoRepeatEnabled()) {
						gInput[WindowID].kb.onKeyPressed((uint8_t)wParam);
					}
					break;

					// When Key is Released
				case WM_KEYUP:
					gInput[WindowID].kb.onKeyReleased((uint8_t)wParam);
					break;

					// When System Key is Released
				case WM_SYSKEYUP:
					gInput[WindowID].kb.onKeyReleased((uint8_t)wParam);
					break;

					// The Value of the Pressed Key
				case WM_CHAR:
					gInput[WindowID].kb.onChar((uint8_t)wParam);
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
						&& point.x < mWidth
						&& point.y >= 0
						&& point.y < mHeight) {
						gInput[WindowID].mouse.onMouseMove(point.x, point.y);
						if (!gInput[WindowID].mouse.isInsideWindow()) {
							SetCapture(hWnd);
							gInput[WindowID].mouse.onMouseEnter();
						}
					}

					// Else (mouse is outside window)
					else {

						// If LMB or RMB is pressed
						if (gInput[WindowID].mouse.isLMBPressed() || gInput[WindowID].mouse.isRMBPressed()) {
							gInput[WindowID].mouse.onMouseMove(point.x, point.y);
						}

						// Else (LMB and RMB not pressed)
						else {
							ReleaseCapture();
							gInput[WindowID].mouse.onMouseExit();
						}
					}
					break;
				}

								 // When the LeftMouseButton is Pressed
				case WM_LBUTTONDOWN: {
					const POINTS point = MAKEPOINTS(lParam);
					gInput[WindowID].mouse.onLMBPressed(point.x, point.y);
					break;
				}

								   // When the LeftMouseButton is Released
				case WM_LBUTTONUP: {
					const POINTS point = MAKEPOINTS(lParam);
					gInput[WindowID].mouse.onLMBReleased(point.x, point.y);
					break;
				}

								 // When the RightMouseButton is Pressed
				case WM_RBUTTONDOWN: {
					const POINTS point = MAKEPOINTS(lParam);
					gInput[WindowID].mouse.onRMBPressed(point.x, point.y);
					break;
				}

								   // When the RightMouseButton is Released
				case WM_RBUTTONUP: {
					const POINTS point = MAKEPOINTS(lParam);
					gInput[WindowID].mouse.onRMBReleased(point.x, point.y);
					break;
				}

								 // When the Mouse Wheel is Moved
				case WM_MOUSEWHEEL: {
					const POINTS point = MAKEPOINTS(lParam);
					const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
					gInput[WindowID].mouse.onWheelDelta(point.x, point.y, delta);
				}

								  // ---- MISCELLANEOUS MESSAGES ---- //

									  // Losing Focus On Window
				case WM_KILLFOCUS:
					gInput[WindowID].kb.clearState();
					break;
					/*case WM_SIZE: {
						RECT clientRc{};
						GetClientRect(hWnd, &clientRc);
						UINT width = clientRc.right - clientRc.left;
						UINT height = clientRc.bottom = clientRc.top;
						pGraphicsOutput->resizeWindow(width, height);
						break;
					}*/
				}
				return DefWindowProc(hWnd, msg, wParam, lParam);
			}

			Window() = default;
			Window(unsigned int width, unsigned int height, const TCHAR* name) :
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
		};
	}
}

// Window Global State
namespace GID {
	namespace GSO {
		namespace WindowNS {
			inline std::vector<std::shared_ptr<GID::DSU::Window>> gWnd{};
			inline void addWindow(UINT w, UINT h, const TCHAR* name) {
				gWnd.push_back(std::make_shared<GID::DSU::Window>(w, h, name));
				GID::GSO::Input::gInput.push_back({ });
				windowCt++;
			}
		}
	}
}

// Viewport Preset Enum
namespace GID {
	namespace DSU {
		enum class ViewportPreset {
			VP1_DEFAULT,
			VP2_HORIZONTAL,
			VP2_VERTICAL,
			VP4_CORNER,
			INVALID
		};
	}
}

// Viewport Preset Array
namespace GID {
	namespace DSU {
		struct VIEWPORT_DESC {
			std::vector<D3D12_VIEWPORT> vps{};
		};
	}
}

// Viewport Preset Container and Initialization
namespace GID {
	namespace GSO {
		namespace Render {
			namespace Viewport {
				inline std::array<GID::DSU::VIEWPORT_DESC, 4u> ViewportPresets;
				inline void initViewportPresets() {
					RECT wnd{ WindowNS::gWnd.at((uint8_t)GID::DSU::WindowType::MAINWINDOW).get()->getWindowInfo().rcClient };
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
		}
	}
}

// GFX Descriptor
namespace GID {
	namespace DSU {
		struct GFX_DESC {
			VIEWPORT_DESC viewportDesc;
		};
	}
}

// Global Timer
namespace GID {
	namespace GSO {
		namespace General {
			inline DSU::Timer gGlobalTimer{};
		}
	}
}

// Global Tick Timer
namespace GID {
	namespace GSO {
		namespace Update {
			inline DSU::Timer gTickTimer{};
		}
	}
}

// Global Tick
namespace GID {
	namespace GSO {
		namespace Update {
			inline uint16_t gTicks{};
		}
	}
}

// Tick Leftover Time
namespace GID {
	namespace GSO {
		namespace Update {
			inline float gTLT{};
		}
	}
}

// Initialize the update cycle (record ticks passed)
namespace GID {
	namespace GSO {
		namespace Update {
			inline void initUpdateCycle() {
				float dt = gTickTimer.mark() * 1000.f + gTLT;
				gTicks = std::truncf(dt);
				gTLT = dt - (float)gTicks;
			}
		}
	}
}

// Camera Container
namespace GID {
	namespace DSU {
		struct Camera : Scriptable, Inputtable {

			enum Rotation { Pitch, Yaw, Roll };
			enum Translation { X, Y, Z };
			enum FollowMode { NO_FOLLOW, FOLLOW };

			AssistMath::FAMVECTOR Y_AXIS{ 0.0f, 1.0f, 0.0f, 0.0f };
			AssistMath::FAMVECTOR mEye{ 0.0f, 0.0f, -5.0f, 1.0f };
			AssistMath::FAMVECTOR mFocus{ 0.0f, 0.0f, 0.0f, 0.0f };
			AssistMath::FAMVECTOR mRotation{};
			AssistMath::FAMVECTOR mDTranslation{};

			bool mbMouseControl{ false };
			bool mbFollow{ false };

			Timer moveTimer{};
			Timer moveTimer2{};

			Camera() = default;

			AssistMath::FAMMATRIX getMatrix() noexcept {
				using namespace AssistMath;
				if (mbFollow) {
					return { FAMMatrixLookAtLH(mEye, mFocus, Y_AXIS) };
				}
				if (!mbFollow) {
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
			}
			void addPitch(float delta) noexcept {
				mRotation.m128_f32[Rotation::Pitch] += delta;
				if (mRotation.m128_f32[Rotation::Pitch] > 90.0f) mRotation.m128_f32[Rotation::Pitch] = 89.999f;
				if (mRotation.m128_f32[Rotation::Pitch] < -90.0f) mRotation.m128_f32[Rotation::Pitch] = -89.999f;
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
				auto ind = (uint8_t)WindowType::MAINWINDOW;

				if (!mbFollow) {
					
					BOOL shift = gInput[ind].kb.isKeyPressed(VK_SHIFT);
					shift++;

					if (gInput[ind].kb.isKeyPressed('W')) {
						float xcom = std::sin(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw]));
						float zcom = std::cos(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw]));
						mDTranslation.m128_f32[Translation::X] += 0.00025f * xcom * shift;
						mDTranslation.m128_f32[Translation::Z] += 0.00025f * zcom * shift;
					}
					if (gInput[ind].kb.isKeyPressed('S')) {
						float xcom = std::sin(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw]));
						float zcom = std::cos(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw]));
						mDTranslation.m128_f32[Translation::X] += 0.00025f * -xcom * shift;
						mDTranslation.m128_f32[Translation::Z] += 0.00025f * -zcom * shift;
					}
					if (gInput[ind].kb.isKeyPressed('A')) {
						float xcom = std::sin(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw] - 90.0f));
						float zcom = std::cos(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw] - 90.0f));
						mDTranslation.m128_f32[Translation::X] += 0.00025f * xcom * shift;
						mDTranslation.m128_f32[Translation::Z] += 0.00025f * zcom * shift;
					}
					if (gInput[ind].kb.isKeyPressed('D')) {
						float xcom = std::sin(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw] + 90.0f));
						float zcom = std::cos(AMConvertToRadians(mRotation.m128_f32[Rotation::Yaw] + 90.0f));
						mDTranslation.m128_f32[Translation::X] += 0.00025f * xcom * shift;
						mDTranslation.m128_f32[Translation::Z] += 0.00025f * zcom * shift;
					}
					if (gInput[ind].kb.isKeyPressed(VK_SPACE)) mDTranslation.m128_f32[Translation::Y] += 0.00025f * shift;
					if (gInput[ind].kb.isKeyPressed(VK_TAB)) mDTranslation.m128_f32[Translation::Y] -= 0.00025f * shift;

					if (gInput[ind].kb.isKeyPressed(VK_LEFT)) addYaw(0.25f);
					if (gInput[ind].kb.isKeyPressed(VK_RIGHT)) addYaw(-0.25f);
					if (gInput[ind].kb.isKeyPressed(VK_UP)) addPitch(0.25f);
					if (gInput[ind].kb.isKeyPressed(VK_DOWN)) addPitch(-0.25f);

					RECT rc{ GSO::WindowNS::gWnd[ind]->getWindowInfo().rcClient };

					if (mbMouseControl) {
						BOOL control = gInput[ind].kb.isKeyPressed(VK_CONTROL);
						control++;
						for (auto& e : gInput[ind].mouseEvents) {
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

					for (auto& e : gInput[ind].mouseEvents) if (e.getType() == Mouse::Event::Type::LMBPress) {
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
			
			void update() noexcept {
				if (!mbFollow) {
					AssistMath::FAMVECTOR vel = _mm_mul_ps(mDTranslation, _mm_set_ps1(GSO::Update::gTicks));
					translate(vel);
					//std::cout << "[X] " << mDTranslation.m128_f32[X] << " " << " [Y] " << mDTranslation.m128_f32[Y] << " " << " [Z] " << mDTranslation.m128_f32[Z] << '\n';
					if (mDTranslation.m128_f32[Translation::X] < 0.0001f
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

					else mDTranslation.m128_f32[Translation::Z] /= 1.02f;
				}
			}
		};
	}
}

// Graphics Pipeline Object (GFX)
namespace GID {
	namespace DSU {
		struct GraphicsOutput {
			// Graphics Configuration
			bool											mError{ false };
			bool											mDebug{ false };
			bool											mUseWARPAdapter{ false };
			bool											mVSync{ false };
			BOOL											mTearingSupport{ false };
			bool											mFullscreen{ false };
			UINT											mBackBufferCount{ 3u };
			UINT											mShaderDebugCompileFlags{ D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION };
			UINT											mShaderReleaseCompileFlags{ 0u };
			D3D_FEATURE_LEVEL								mFeatureLevel{ D3D_FEATURE_LEVEL_11_0 };
			bool											mWireframe{ false };
			// Debug
			#if defined(_DEBUG)
			ComPtr<ID3D12Debug5>							mpDebugController{};
			ComPtr<ID3D12InfoQueue1>						mpInfoQueue{};
			#endif
			// Core
			ComPtr<IDXGIFactory7>							mpFactory{};
			ComPtr<ID3D12Device9>							mpDevice{};
			ComPtr<IDXGISwapChain4>							mpSwapChain{};
			// Adapters
			ComPtr<IDXGIAdapter4>							mpHardwareAdapter{};
			ComPtr<IDXGIAdapter4>							mpWARPAdapter{};
			// Commands
			std::vector<ComPtr<ID3D12CommandAllocator>>		mpCommandAllocator{};
			ComPtr<ID3D12GraphicsCommandList6>				mpCommandList{};
			ComPtr<ID3D12CommandQueue>						mpCommandQueue{};
			// Pipeline state
			GID::DSU::PipelineStateStream					mPSS{};
			ComPtr<ID3D12PipelineState>						mpPipelineState{};
			// Root Signature
			ComPtr<ID3D12RootSignature>						mpRootSignature{};
			ComPtr<ID3DBlob>								mpSignatureRootBlob{};
			ComPtr<ID3DBlob>								mpErrorBlob{};
			// Render Target View(s)
			std::vector<ComPtr<ID3D12Resource2>>			mpRenderTargets{};
			ComPtr<ID3D12DescriptorHeap>					mpRTVHeap{};
			UINT											mRTVHeapSize{};
			std::vector<D3D12_VIEWPORT>						mViewport{};
			D3D12_RECT										mScissorRc{};
			// Depth Stencil
			ComPtr<ID3D12Resource2>							mpDepthStencilTexture{};
			ComPtr<ID3D12DescriptorHeap>					mpDSVHeap{};
			UINT											mDSVHeapSize{};
			// Fence
			ComPtr<ID3D12Fence1>							mpFence{};
			HANDLE											mhFenceEvent{};
			UINT8											mFrameIndex{};
			UINT64											mFenceValue{};
			UINT64											mCurrentFenceValue{};
			// Shaders
			ComPtr<ID3DBlob>								mpVSBytecode{};
			ComPtr<ID3DBlob>								mpPSBytecode{};
			// Projection Matrix
			AssistMath::FAMMATRIX							mProjection{};
			// Misc
			RECT											mWindowRc{};
			HRESULT											mHR{};
			std::string										mLastType{};
			Camera											mCamera{};

			// Constructor
			GraphicsOutput(HWND& hWnd, GFX_DESC& desc) {

				WINDOWINFO wndInfo{};
				wndInfo.cbSize = sizeof(WINDOWINFO);
				GetWindowInfo(hWnd, &wndInfo);

				UINT16 width = wndInfo.rcClient.right - wndInfo.rcClient.left;
				UINT16 height = wndInfo.rcClient.bottom - wndInfo.rcClient.top;

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
					std::cout << "[Init Info] Minimum Feature Level: 11_0\n";
				}

				// Create the viewport
				mViewport = desc.viewportDesc.vps;
				//mViewport = { CD3DX12_VIEWPORT(0.0f, 0.0f, (FLOAT)width, (FLOAT)height) };
				mScissorRc = { CD3DX12_RECT(0u, 0u, LONG_MAX, LONG_MAX) };

				std::thread thread0([this] { createFactory(); });
				std::thread thread1([this] { createDebugLayer(); });
				thread0.join();
				std::thread thread2([this] { enumerateAdapters(); });
				thread2.join();
				std::thread thread3([this] { createDevice(); });
				thread3.join();
				std::thread thread4([this] { createCommandQueue(); });
				std::thread thread5([this] { checkTearingSupport(); });
				std::thread thread6([this] { createFence(); });
				std::thread thread7([this] { createInfoQueue(); });
				std::thread thread8([this, width, height] { createDSV(width, height); });
				thread4.join();
				std::thread thread9([this, hWnd, width, height] { createSwapChain(hWnd, width, height); });
				std::thread thread10([this] { createCommandAllocatorAndList(); });
				thread9.join();
				std::thread thread11([this] { createRTV(); });
				thread1.join(); thread5.join(); thread6.join(); thread7.join(); thread8.join(); thread10.join(); thread11.join();

				initializePipeline();
				flushGPU();
			}
			GraphicsOutput() = default;

			// Destructor
			//~GraphicsOutput() = default;

			void parseGraphicsConfig() noexcept {
				enum GFXConfigDataType {
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
						if (strCurrentLine.starts_with("UIBackBufferCount")) currentData = UIBackBufferCount;
						else if (strCurrentLine.starts_with("BUseWARPAdapter")) currentData = BUseWARPAdapter;
						else if (strCurrentLine.starts_with("BVSync")) currentData = BVSync;
						else if (strCurrentLine.starts_with("BFullScreenOnStartup")) currentData = BFullScreenOnStartup;
						else if (strCurrentLine.starts_with("BEnableWireframe")) currentData = BEnableWireframe;
						else currentData = Invalid;
						switch (currentData) {
						case UIBackBufferCount:
							strCurrentLine.erase(0u, 19u);
							mBackBufferCount = std::stoi(strCurrentLine);
							break;
						case BUseWARPAdapter:
							strCurrentLine.erase(0u, 17u);
							if (strCurrentLine == "TRUE") mUseWARPAdapter = true;
							break;
						case BVSync:
							strCurrentLine.erase(0u, 8u);
							if (strCurrentLine == "TRUE") mVSync = true;
							break;
						case BFullScreenOnStartup:
							strCurrentLine.erase(0u, 22u);
							if (strCurrentLine == "TRUE") mFullscreen = true;
							break;
						case BEnableWireframe:
							strCurrentLine.erase(0u, 18u);
							if (strCurrentLine == "TRUE") mWireframe = true;
						case Invalid:
							if (mDebug) std::cout << "[WARNING] Invalid line parsed from \"gfx_config.txt\"\n"
								<< "[LINE] " << strCurrentLine;
							break;
						}
						strCurrentLine.clear();
					}
				}
			}
			void createDebugLayer() noexcept {
				// Create the Debug Interface
				#if defined(_DEBUG)
				D3D12GetDebugInterface(IID_PPV_ARGS(&mpDebugController));
				mpDebugController->EnableDebugLayer();
				mpDebugController->SetEnableAutoName(TRUE);
				#endif
			}
			void createInfoQueue() noexcept {
				#if defined(_DEBUG)
				mpDevice.As(&mpInfoQueue);
				mpInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
				mpInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
				mpInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_MESSAGE, TRUE);
				#endif
			}
			void createFactory() noexcept {
				// Create factory
				CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&mpFactory));
			}
			void enumerateAdapters() noexcept {
				// Obtain adapter
				if (mUseWARPAdapter) mpFactory->EnumWarpAdapter(IID_PPV_ARGS(&mpWARPAdapter)); // WARP
				else {
					UINT64 maxVideoMem = 0;
					ComPtr<IDXGIAdapter1> refAdapter{};
					for (UINT i = 0; mpFactory->EnumAdapters1(i, refAdapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; i++) {
						DXGI_ADAPTER_DESC1 ad{};
						refAdapter->GetDesc1(&ad);
						if ((ad.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 && D3D12CreateDevice(refAdapter.Get(), mFeatureLevel, __uuidof(ID3D12Device9), nullptr)
							&& ad.DedicatedVideoMemory > maxVideoMem) {
							maxVideoMem = ad.DedicatedVideoMemory; refAdapter.As(&mpHardwareAdapter);
						}
					}
				}
			}
			void createDevice() noexcept {
				// Create the Device
				if (mUseWARPAdapter) D3D12CreateDevice(mpWARPAdapter.Get(), mFeatureLevel, IID_PPV_ARGS(&mpDevice)); // WARP
				else D3D12CreateDevice(mpHardwareAdapter.Get(), mFeatureLevel, IID_PPV_ARGS(&mpDevice)); // Hardware
				mpDevice->SetName(L"[Core] [ID3D12Device9] Member of GraphicsOutput");
			}
			void createCommandQueue() noexcept {
				// Create the command queue
				D3D12_COMMAND_QUEUE_DESC cqd = {};
				cqd.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
				cqd.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
				cqd.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
				mpDevice->CreateCommandQueue1(&cqd, __uuidof(ID3D12Device9), IID_PPV_ARGS(&mpCommandQueue));
				mpCommandQueue->SetName(L"[Command] [ID3D12CommandQueue] Member of GraphicsOutput");
			}
			void checkTearingSupport() noexcept {
				mpFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &mTearingSupport, sizeof(mTearingSupport));
			}
			void createSwapChain(const HWND& hWnd, const UINT16& width, const UINT16& height) noexcept {
				// Get window and monitor info
				HMONITOR hMonitor{ MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST) };
				MONITORINFOEX monitorInfo{};
				monitorInfo.cbSize = sizeof(MONITORINFOEX);
				GetMonitorInfo(hMonitor, &monitorInfo);
				DEVMODE devMode{};
				devMode.dmSize = sizeof(DEVMODE);
				EnumDisplaySettings(monitorInfo.szDevice, ENUM_CURRENT_SETTINGS, &devMode);
				// Create the swap chain
				DXGI_SWAP_CHAIN_DESC1 scd = {}; {
					scd.BufferCount = mBackBufferCount;
					scd.Width = width; scd.Height = height;
					scd.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
					scd.Stereo = FALSE;
					scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
					scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
					scd.SampleDesc.Count = 1u; scd.SampleDesc.Quality = 0u;
					scd.Scaling = DXGI_SCALING_STRETCH;
					scd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
					if (mTearingSupport) scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
				}
				DXGI_SWAP_CHAIN_FULLSCREEN_DESC scfd = {}; {
					scfd.RefreshRate.Numerator = devMode.dmDisplayFrequency;
					scfd.RefreshRate.Denominator = 1u;
					scfd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
					scfd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
				}
				if (mFullscreen) scfd.Windowed = FALSE; else scfd.Windowed = TRUE;
				ComPtr<IDXGISwapChain1> refSwapChain{};
				mpFactory->CreateSwapChainForHwnd(mpCommandQueue.Get(), hWnd, &scd, &scfd, nullptr, refSwapChain.GetAddressOf());
				refSwapChain.As(&mpSwapChain);
				// Manual fullscreen support
				mpFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER); // DXGI_MWA_VALID
			}
			void createRTV() noexcept {
				// Create Render Target View (RTV) heap
				D3D12_DESCRIPTOR_HEAP_DESC RTVHeapDesc = {}; {
					RTVHeapDesc.NumDescriptors = mBackBufferCount;
					RTVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
					RTVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				}
				mpDevice->CreateDescriptorHeap(&RTVHeapDesc, IID_PPV_ARGS(&mpRTVHeap));
				mRTVHeapSize = mpDevice->GetDescriptorHandleIncrementSize(RTVHeapDesc.Type);
				mpRTVHeap->SetName(L"[RTV] [ID3D12DescriptorHeap] Member of GraphicsOutput");
				// Create frame resources
				CD3DX12_CPU_DESCRIPTOR_HANDLE hRTV(mpRTVHeap->GetCPUDescriptorHandleForHeapStart());
				for (UINT i = 0u; i < mBackBufferCount; i++) {
					mpRenderTargets.push_back(*std::make_unique<ComPtr<ID3D12Resource2>>());
					mpSwapChain->GetBuffer(i, IID_PPV_ARGS(&mpRenderTargets.at(i)));
					mpDevice->CreateRenderTargetView(mpRenderTargets[i].Get(), nullptr, hRTV);
					std::wstringstream woss{};
					woss << "[RTV] [ID3D12Resource2] [Render Target View #" << i + 1 << "]";
					mpRenderTargets[i]->SetName(woss.str().c_str());
					hRTV.Offset(1u, mRTVHeapSize);
				}

			}
			void createDSV(const UINT16& width, const UINT16& height) noexcept {
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
				mpDepthStencilTexture->SetName(L"[DSV] [ID3D12Resource2] [DepthStencilTexture]");
			}
			void createCommandAllocatorAndList() noexcept {
				// Setup Graphics Commands
				for (int i = 0; i < mBackBufferCount; i++) {
					mpCommandAllocator.push_back(*std::make_unique<ComPtr<ID3D12CommandAllocator>>());
					mpDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mpCommandAllocator[i]));
					std::wstringstream woss{};
					woss << "[Command] [ID3D12CommandAllocator] [Command Allocator #" << i + 1 << "]";
					mpCommandAllocator[i]->SetName(woss.str().c_str());
				}
				mpDevice->CreateCommandList1(0u, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&mpCommandList));
				mpCommandList->SetName(L"[Command] [ID3D12GraphicsCommandList6] [Command List]");

				mpCommandAllocator[0]->Reset();
				mpCommandList->Reset(mpCommandAllocator[0].Get(), nullptr);
			}
			void createFence() noexcept {
				// Create the Fence for synchronization
				mpDevice->CreateFence(0u, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mpFence));
				mpFence->SetName(L"[Sync] [ID3D12Fence1] [Fence]");
				mhFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			}
			void initializePipeline() noexcept {
				D3DReadFileToBlob(L"VertexShader.cso", &mpVSBytecode);
				D3DReadFileToBlob(L"PixelShader.cso", &mpPSBytecode);
				std::array<D3D12_INPUT_ELEMENT_DESC, 3u> ied{}; {
					ied[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
					ied[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
					ied[2] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
				}
				D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData{}; {
					featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
				}
				D3D12_ROOT_SIGNATURE_FLAGS rsFlags = {
					D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS
					| D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS };
				std::array<CD3DX12_ROOT_PARAMETER1, 2u> rp{}; {
					rp[0].InitAsConstantBufferView(0u, 0u, D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY_VERTEX);
					rp[1].InitAsConstantBufferView(0u, 0u, D3D12_ROOT_DESCRIPTOR_FLAG_NONE, D3D12_SHADER_VISIBILITY_PIXEL);
				}
				CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rsd{}; {
					rsd.Init_1_1(rp.size(), rp.data(), 0u, nullptr, rsFlags);
					D3DX12SerializeVersionedRootSignature(&rsd, featureData.HighestVersion, &mpSignatureRootBlob, &mpErrorBlob);
					mpDevice->CreateRootSignature(0u, mpSignatureRootBlob->GetBufferPointer(), mpSignatureRootBlob->GetBufferSize(), IID_PPV_ARGS(&mpRootSignature));
					mpRootSignature->SetName(L"[Shaders] [Root Signature]");
				}
				D3D12_RT_FORMAT_ARRAY RTVfarr{}; {
					RTVfarr.NumRenderTargets = 1u;
					RTVfarr.RTFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
				}
				D3D12_PIPELINE_STATE_STREAM_DESC pssd{}; {
					mPSS.pRootSignature = mpRootSignature.Get();
					mPSS.InputLayout = { ied.data(), (UINT)ied.size() };
					mPSS.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
					mPSS.VS = CD3DX12_SHADER_BYTECODE(mpVSBytecode.Get());
					mPSS.PS = CD3DX12_SHADER_BYTECODE(mpPSBytecode.Get());
					mPSS.DSVFormat = DXGI_FORMAT_D32_FLOAT;
					mPSS.RTVFormats = RTVfarr;
					if (mDebug) {
						if (mWireframe) {
							mPSS.RS = CD3DX12_RASTERIZER_DESC(D3D12_FILL_MODE_WIREFRAME, D3D12_CULL_MODE_NONE, FALSE, 0u, 0.0f, 0.0f, TRUE, FALSE, FALSE, 0u,
								D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF);
						}
						else {
							mPSS.RS = CD3DX12_RASTERIZER_DESC(D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE, FALSE, 0u, 0.0f, 0.0f, TRUE, FALSE, FALSE, 0u,
								D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF);
						}
					}
					else mPSS.RS = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
					pssd.pPipelineStateSubobjectStream = &mPSS;
					pssd.SizeInBytes = sizeof(GID::DSU::PipelineStateStream);
					mpDevice->CreatePipelineState(&pssd, IID_PPV_ARGS(&mpPipelineState));
					mpPipelineState->SetName(L"[Pipeline State]");
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

			// Frame sync
			void signalFence() noexcept {
				//Signal the fence
				mHR = mpCommandQueue->Signal(mpFence.Get(), ++mFenceValue);
			}
			int waitFence() noexcept {
				// Wait for the fence
				if (mpFence->GetCompletedValue() < mFenceValue) {
					mpFence->SetEventOnCompletion(mFenceValue, mhFenceEvent);
					WaitForSingleObject(mhFenceEvent, (DWORD)INFINITE);
				}
				return 0;
			}
			int flushGPU() noexcept {
				mpCommandList->Close();
				ID3D12CommandList* ppCommandLists[] = { mpCommandList.Get() };
				mpCommandQueue->ExecuteCommandLists(1u, ppCommandLists);
				signalFence();
				waitFence();
				mpCommandAllocator[mFrameIndex]->Reset();
				mpCommandList->Reset(mpCommandAllocator[mFrameIndex].Get(), nullptr);
				return 0;
			}

			// Prepare
			void transitionRTVToWrite() noexcept {
				auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpRenderTargets[mFrameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
				mpCommandList->ResourceBarrier(1u, &barrier);
			}
			void transitionRTVToRead() noexcept {
				auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpRenderTargets[mFrameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
				mpCommandList->ResourceBarrier(1u, &barrier);
			}
			void clearRTV() noexcept {
				CD3DX12_CPU_DESCRIPTOR_HANDLE hRTV(mpRTVHeap->GetCPUDescriptorHandleForHeapStart(), mFrameIndex, mRTVHeapSize);
				auto color = std::make_unique<float[]>(4); color[0] = 0.5294f; color[1] = 0.8078f; color[2] = 0.9216f; color[3] = 1.f;
				//auto color = std::make_unique<float[]>(4); color[0] = 0.0f; color[1] = 0.0f; color[2] = 0.0f; color[3] = 1.f;
				mpCommandList->ClearRenderTargetView(hRTV, color.get(), 0u, nullptr);
			}
			void clearDSV() noexcept {
				CD3DX12_CPU_DESCRIPTOR_HANDLE hDSV(mpDSVHeap->GetCPUDescriptorHandleForHeapStart());
				mpCommandList->ClearDepthStencilView(hDSV, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0u, 0u, nullptr);
			}
			void setRenderTarget() noexcept {
				CD3DX12_CPU_DESCRIPTOR_HANDLE hRTV(mpRTVHeap->GetCPUDescriptorHandleForHeapStart(), mFrameIndex, mRTVHeapSize);
				CD3DX12_CPU_DESCRIPTOR_HANDLE hDSV(mpDSVHeap->GetCPUDescriptorHandleForHeapStart());
				mpCommandList->OMSetRenderTargets(1u, &hRTV, FALSE, &hDSV);
			}

			// Render
			void startFrame() noexcept {
				transitionRTVToWrite();
				clearRTV();
				clearDSV();
				setRenderTarget();
				mpCommandList->SetGraphicsRootSignature(mpRootSignature.Get());
				mpCommandList->SetPipelineState(mpPipelineState.Get());
				mpCommandList->RSSetViewports(mViewport.size(), mViewport.data());
				mpCommandList->RSSetScissorRects(1u, &mScissorRc);
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
				//endFrame();
			}
			void endFrame() noexcept {
				//DXGI_PRESENT_PARAMETERS pp = {};
				//pp.DirtyRectsCount = 0u;
				//pp.pDirtyRects = 0u;
				//pp.pScrollRect = 0u;
				//pp.pScrollOffset = 0u;
				//// Flip the Swap Chain
				//mpSwapChain->Present1(0u, 0u, &pp);
				//// Temporarily gain access to the Back Buffer
				//wrl::ComPtr<ID3D12Resource2> pBackBuffer;
				//mpSwapChain->GetBuffer(
				//	0,
				//	__uuidof(ID3D12Resource2),
				//	&pBackBuffer
				//);
				//// Now create the Render Target View and make it the Back Buffer
				//mpDevice->CreateRenderTargetView(
				//	pBackBuffer.Get(),
				//	&mpRenderTargets[mFrameIndex],
				//	mFenceEvent
				//);
				//mpCommandList->OMSetRenderTargets(1u, reinterpret_cast<ID3D11RenderTargetView* const*>(m_pRenderTargetView.GetAddressOf()), m_pDepthStencilView.Get());
				transitionRTVToRead();
				flushGPU();
				UINT syncInterval = mVSync ? 1u : 0u;
				UINT presentFlags = mTearingSupport && !mVSync ? DXGI_PRESENT_ALLOW_TEARING : 0u;
				mpSwapChain->Present(syncInterval, presentFlags);
				mFrameIndex = mpSwapChain->GetCurrentBackBufferIndex();
			}

			// Util
			int resizeWindow(UINT width, UINT height) noexcept {

				/*HWND hWnd{};
				mpSwapChain->GetHwnd(&hWnd);
				WINDOWINFO wndInfo{};
				wndInfo.cbSize = sizeof(WINDOWINFO);
				GetWindowInfo(hWnd, &wndInfo);

				LONG clientWidth{ wndInfo.rcWindow.right - wndInfo.rcWindow.left };
				LONG clientHeight{ wndInfo.rcWindow.bottom - wndInfo.rcWindow.top };

				if (clientWidth != width || clientHeight != height) {
					clientWidth = max(1u, width); clientHeight = max(1u, height);
					flushGPU();
					for (int i = 0; i < mBackBufferCount; i++) {
						mpRenderTargets[i].Reset();
					}
					DXGI_SWAP_CHAIN_DESC1 scd{};
					mpSwapChain->GetDesc1(&scd);
					mpSwapChain->ResizeBuffers(mBackBufferCount, clientWidth, clientHeight, scd.Format, scd.Flags);
					mFrameIndex = mpSwapChain->GetCurrentBackBufferIndex();
					createRTV();
				}*/
				return 0;
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
				}
				else {
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
			ComPtr<ID3D12GraphicsCommandList6>& getCommandList() noexcept { return mpCommandList; }
			ComPtr<ID3D12Device9>& getDevice() noexcept { return mpDevice; }

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
}

// GFX Pipeline Container
namespace GID {
	namespace DSU {
		struct GFXPipelineContainer {
			GID::DSU::WindowType wndtype{};
			GID::DSU::GraphicsOutput gfx{};
		};
	}
}

// Global GFX Vector and External GFX Functions
namespace GID {
	namespace GSO {
		namespace Render {
			inline std::vector<GID::DSU::GFXPipelineContainer> gGFX;
			
			inline void addGFX(GID::DSU::WindowType type, GID::DSU::GFX_DESC desc) {
				bool found = false;
				for (auto& w : gGFX) if (w.wndtype == type) found = true;
				if (!found) gGFX.push_back({ type, {} });
				for (auto& w : gGFX) if (w.wndtype == type) w.gfx = { WindowNS::gWnd.at((uint8_t)type)->getHandle(), desc };
			}
			
			inline GID::DSU::GraphicsOutput& findGFX(GID::DSU::WindowType type) {
				for (auto& w : gGFX) if (w.wndtype == type) return w.gfx;
			}
			
			inline GID::DSU::GraphicsOutput& mainGFX() { return findGFX(GID::DSU::WindowType::MAINWINDOW); }
			
			inline void setGFXProjection(GID::DSU::WindowType wndType, GID::DSU::AssistMath::FAMMATRIX& projection) {
				for (auto& g : gGFX) if (g.wndtype == wndType) g.gfx.setProjection(projection);
			}
		}
	}
}

// Vertex Buffer Structure
namespace GID {
	namespace DSU {
		struct VertexBuffer {

			ComPtr<ID3D12Resource2>		mpDestRes{};
			ComPtr<ID3D12Resource2>		mpIntermedRes{};
			D3D12_VERTEX_BUFFER_VIEW	mVBView{};

			VertexBuffer() = default;
			VertexBuffer(ComPtr<ID3D12Device9>& pDevice, ComPtr<ID3D12GraphicsCommandList6>& pCommandList, const VertexData* vertices, const UINT64& size) {
				createDestinationResource(pDevice, size);
				createIntermediateResource(pDevice, size);
				createVertexBufferView(pCommandList, vertices, size);
			}
			void createDestinationResource(ComPtr<ID3D12Device9>& pDevice, const UINT64& size) noexcept {
				CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_DEFAULT);
				CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(size * sizeof(VertexData)) };
				pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&mpDestRes));
				mpDestRes->SetName(L"[ID3D12Resource] [Destination Resource - VertexBuffer]");
			}
			void createIntermediateResource(ComPtr<ID3D12Device9>& pDevice, const UINT64& size) noexcept {
				CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_UPLOAD);
				CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(size * sizeof(VertexData)) };
				pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mpIntermedRes));
				mpIntermedRes->SetName(L"[ID3D12Resource] [Intermediate Resource - VertexBuffer]");
			}
			void createVertexBufferView(ComPtr<ID3D12GraphicsCommandList6>& pCommandList, const VertexData* vertices, const UINT64& size) noexcept {
				D3D12_SUBRESOURCE_DATA srd{}; {
					srd.pData = vertices;
					srd.RowPitch = size * sizeof(VertexData);
					srd.SlicePitch = srd.RowPitch;
				}
				UpdateSubresources(pCommandList.Get(), mpDestRes.Get(), mpIntermedRes.Get(), 0u, 0u, 1u, &srd);
				mVBView.BufferLocation = mpDestRes->GetGPUVirtualAddress();
				mVBView.SizeInBytes = sizeof(VertexData) * size;
				mVBView.StrideInBytes = sizeof(VertexData);
			}

			ComPtr<ID3D12Resource2>& getDestinationResource() noexcept { return mpDestRes; }
			ComPtr<ID3D12Resource2>& getIntermediateResource() noexcept { return mpIntermedRes; }
			D3D12_VERTEX_BUFFER_VIEW& getView() noexcept { return mVBView; }
			UINT getCount() const noexcept {
				return mVBView.SizeInBytes / sizeof(VertexData);
			}
			void transitionToRead(ComPtr<ID3D12GraphicsCommandList6>& pCommandList) noexcept {
				auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_COPY_DEST,
					D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
				pCommandList->ResourceBarrier(1u, &barrier);
			}
			void transitionToWrite(ComPtr<ID3D12GraphicsCommandList6>& pCommandList) noexcept {
				auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
					D3D12_RESOURCE_STATE_COPY_DEST);
				pCommandList->ResourceBarrier(1u, &barrier);
			}
		};
	}
}

// Vertex Constant Buffer Structure
namespace GID {
	namespace DSU {
		struct VertexConstantBuffer {

			ComPtr<ID3D12Resource2>			mpDestRes{};
			ComPtr<ID3D12Resource2>			mpIntermedRes{};
			ComPtr<ID3D12DescriptorHeap>	mpCBVHeap{};
			UINT							mpCBVHeapSize{};

			VertexConstantBuffer() = default;
			VertexConstantBuffer(ComPtr<ID3D12Device9>& pDevice, ComPtr<ID3D12GraphicsCommandList6>& pCommandList, const VertexConstantBufferData& data) {
				createDestinationResource(pDevice);
				createIntermediateResource(pDevice);
				createDescriptorHeap(pDevice);
				createConstantBufferView(pDevice, pCommandList, data);
			}

			void createDestinationResource(ComPtr<ID3D12Device9>& pDevice) noexcept {
				CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_DEFAULT);
				CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(alignBytes()) };
				pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&mpDestRes));
			}
			void createIntermediateResource(ComPtr<ID3D12Device9>& pDevice) noexcept {
				CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_UPLOAD);
				CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(alignBytes()) };
				pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mpIntermedRes));
			}
			void createDescriptorHeap(ComPtr<ID3D12Device9>& pDevice) noexcept {
				D3D12_DESCRIPTOR_HEAP_DESC cbvhd{};
				cbvhd.NumDescriptors = 1u;
				cbvhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				cbvhd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				pDevice->CreateDescriptorHeap(&cbvhd, IID_PPV_ARGS(&mpCBVHeap));
				mpCBVHeapSize = pDevice->GetDescriptorHandleIncrementSize(cbvhd.Type);
			}
			void createConstantBufferView(ComPtr<ID3D12Device9>& pDevice, ComPtr<ID3D12GraphicsCommandList6>& pCommandList, const GID::DSU::VertexConstantBufferData& data) noexcept {
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
			void transitionToRead(ComPtr<ID3D12GraphicsCommandList6>& pCommandList) noexcept {
				auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_COPY_DEST,
					D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
				pCommandList->ResourceBarrier(1u, &barrier);
			}
			void transitionToWrite(ComPtr<ID3D12GraphicsCommandList6>& pCommandList) noexcept {
				auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
					D3D12_RESOURCE_STATE_COPY_DEST);
				pCommandList->ResourceBarrier(1u, &barrier);
			}
		};
	}
}

// Pixel Constant Buffer Structure
namespace GID {
	namespace DSU {
		struct PixelConstantBuffer {

			ComPtr<ID3D12Resource2>			mpDestRes{};
			ComPtr<ID3D12Resource2>			mpIntermedRes{};
			ComPtr<ID3D12DescriptorHeap>	mpCBVHeap{};
			UINT							mpCBVHeapSize{};

			PixelConstantBuffer() = default;
			PixelConstantBuffer(ComPtr<ID3D12Device9>& pDevice, ComPtr<ID3D12GraphicsCommandList6>& pCommandList, const PixelConstantBufferData& data) {
				createDestinationResource(pDevice);
				createIntermediateResource(pDevice);
				createDescriptorHeap(pDevice);
				createConstantBufferView(pDevice, pCommandList, data);
			}
			void createDestinationResource(ComPtr<ID3D12Device9>& pDevice) noexcept {
				CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_DEFAULT);
				CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(alignBytes()) };
				pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&mpDestRes));
			}
			void createIntermediateResource(ComPtr<ID3D12Device9>& pDevice) noexcept {
				CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_UPLOAD);
				CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(alignBytes()) };
				pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mpIntermedRes));
			}
			void createDescriptorHeap(ComPtr<ID3D12Device9>& pDevice) noexcept {
				D3D12_DESCRIPTOR_HEAP_DESC cbvhd{};
				cbvhd.NumDescriptors = 1u;
				cbvhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				cbvhd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				pDevice->CreateDescriptorHeap(&cbvhd, IID_PPV_ARGS(&mpCBVHeap));
				mpCBVHeapSize = pDevice->GetDescriptorHandleIncrementSize(cbvhd.Type);
			}
			void createConstantBufferView(ComPtr<ID3D12Device9>& pDevice, ComPtr<ID3D12GraphicsCommandList6>& pCommandList, const PixelConstantBufferData& data) noexcept {
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
				return (UINT)(sizeof(PixelConstantBufferData) % 256 == 0
					? sizeof(PixelConstantBufferData)
					: 256 - sizeof(PixelConstantBufferData) % 256 + sizeof(PixelConstantBufferData));
			}
			void transitionToRead(ComPtr<ID3D12GraphicsCommandList6>& pCommandList) noexcept {
				auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_COPY_DEST,
					D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
				pCommandList->ResourceBarrier(1u, &barrier);
			}
			void transitionToWrite(ComPtr<ID3D12GraphicsCommandList6>& pCommandList) noexcept {
				auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
					D3D12_RESOURCE_STATE_COPY_DEST);
				pCommandList->ResourceBarrier(1u, &barrier);
			}
		};
	}
}

// Model File Data Structures
namespace GID {
	namespace DSU {
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
			std::vector<AssistMath::AMUSHORT3X3> ind{};
			MaterialFileData mtl{};
		};
		struct IndexedObjectFileData {
			std::string name{};
			std::vector<AssistMath::AMFLOAT2> tex{};
			std::vector<AssistMath::AMFLOAT3> pos{}, norm{};
			MaterialFileData mtl{};
		};
		struct ModelFileData {
			std::vector<IndexedObjectFileData> iofd{};
		};
	}
}

// Model File Parsing Functions
namespace GID {
	namespace Util {
		namespace FileParsing {
			inline GID::DSU::AssistMath::AMFLOAT3 parseThreeFloatLine(std::string& line) {
				float x{}, y{}, z{}; std::string temp{};
				for (uint8_t i = 0; i < 3; i++) {
					while (!line.starts_with(" ") && !line.empty()) { temp.append(line.substr(0u, 1u)); line.erase(0u, 1u); }
					if (!line.empty()) line.erase(0u, 1u);
					switch (i) { case 0: x = std::stof(temp); break; case 1: y = std::stof(temp); break; case 2: z = std::stof(temp); break; } 
									   temp.clear();
				}
				return { x, y, z };
			}
			inline GID::DSU::AssistMath::AMFLOAT2 parseTwoFloatLine(std::string& line) {
				float x{}, y{}; std::string temp{};
				for (uint8_t i = 0; i < 2; i++) {
					while (!line.starts_with(" ") && !line.empty()) { temp.append(line.substr(0u, 1u)); line.erase(0u, 1u); }
					if (!line.empty()) line.erase(0u, 1u);
					switch (i) { case 0: x = std::stof(temp); break; case 1: y = std::stof(temp); break; }
									   temp.clear();
				}
				return { x, y };
			}
			inline GID::DSU::MaterialFileData& findMaterial(std::string& name, std::vector<GID::DSU::MaterialFileData>& mtls) {
				for (auto& mtl : mtls) {
					if (name == mtl.name) {
						return mtl;
					}
				}
			}
			inline std::string parseUntilWS(std::string& line) {
				std::string temp{};
				while (!line.starts_with(" ") && !line.empty()) { temp.append(line.substr(0u, 1u)); line.erase(0u, 1u); }
				return temp;
			}
			inline std::string parseUntilFS(std::string& line) {
				std::string temp{};
				while (!line.starts_with("/") && !line.empty()) { temp.append(line.substr(0u, 1u)); line.erase(0u, 1u); }
				return temp;
			}
			inline GID::DSU::AssistMath::AMUSHORT3X3 parseFaceIndexLine(std::string& line) {
				GID::DSU::AssistMath::AMUSHORT3X3 ind{}; std::string point{}, pointcomp{};
				for (int i = 0; i < 3; i++) {
					point = parseUntilWS(line);
					line.erase(0u, 1u);
					for (int j = 0; j < 3; j++) {
						ind.m[i][j] = std::stoi(parseUntilFS(point)) - 1;
						point.erase(0u, 1u);
					}
				}
				return ind;
			}
			inline std::vector<GID::DSU::ObjectFileData> parseObjFile(std::string& path, std::vector<GID::DSU::MaterialFileData> mtls) {
				using GID::DSU::ObjectFileDataType; using GID::DSU::ObjectFileData;
				std::vector<ObjectFileData> ofd{}; std::string strLine{}; std::ifstream file{}; unsigned int lineCt{};
				file.open(path + ".obj");
				if (file.is_open()) {
					while (file) {
						std::getline(file, strLine); lineCt++;
						if (strLine.empty()) continue;
						ObjectFileDataType type;
						if (strLine.starts_with("o ")) type = ObjectFileDataType::O;
						else if (strLine.starts_with("v ")) type = ObjectFileDataType::V;
						else if (strLine.starts_with("vt ")) type = ObjectFileDataType::VT;
						else if (strLine.starts_with("vn ")) type = ObjectFileDataType::VN;
						else if (strLine.starts_with("usemtl ")) type = ObjectFileDataType::USEMTL;
						else if (strLine.starts_with("f ")) type = ObjectFileDataType::F;
						else type = ObjectFileDataType::INVALID;
						switch (type) {
						case ObjectFileDataType::O:
							strLine.erase(0u, 2u);
							ofd.push_back({ });
							ofd.back().name = strLine;
							break;
						case ObjectFileDataType::V:
							strLine.erase(0u, 2u);
							ofd.back().pos.push_back(parseThreeFloatLine(strLine));
							break;
						case ObjectFileDataType::VT:
							strLine.erase(0u, 3u);
							ofd.back().tex.push_back(parseTwoFloatLine(strLine));
							break;
						case ObjectFileDataType::VN:
							strLine.erase(0u, 3u);
							ofd.back().norm.push_back(parseThreeFloatLine(strLine));
							break;
						case ObjectFileDataType::USEMTL:
							strLine.erase(0u, 7u);
							ofd.back().mtl = findMaterial(strLine, mtls);
							break;
						case ObjectFileDataType::F:
							strLine.erase(0u, 2u);
							ofd.back().ind.push_back(parseFaceIndexLine(strLine));
							break;
						case ObjectFileDataType::INVALID:
							#if defined(_DEBUG)
							std::cout << "[WARNING] Invalid line parsed. [FILE: " << path << ".obj] - [LINE: " << lineCt << "] \"" << strLine << '\"' << '\n';
							#endif
							break;
						}
						strLine.clear();
					}
				}
				file.close();
				return ofd;
			}
			inline std::vector<GID::DSU::MaterialFileData> parseMtlFile(std::string& path) {
				using GID::DSU::MaterialFileDataType; using GID::DSU::MaterialFileData;
				std::vector<MaterialFileData> mfd{}; std::string strLine{}; std::ifstream file{}; unsigned int lineCt{};
				file.open(path + ".mtl");
				if (file.is_open()) {
					while (file) {
						std::getline(file, strLine); lineCt++;
						if (strLine.empty()) continue;
						MaterialFileDataType type;
						if (strLine.starts_with("newmtl ")) type = MaterialFileDataType::NEWMTL;
						else if (strLine.starts_with("Ka ")) type = MaterialFileDataType::KA;
						else if (strLine.starts_with("Kd ")) type = MaterialFileDataType::KD;
						else if (strLine.starts_with("Ks ")) type = MaterialFileDataType::KS;
						else if (strLine.starts_with("Tf ")) type = MaterialFileDataType::TF;
						else if (strLine.starts_with("illum ")) type = MaterialFileDataType::IF;
						else if (strLine.starts_with("d ")) type = MaterialFileDataType::D;
						else if (strLine.starts_with("Ns ")) type = MaterialFileDataType::NS;
						else if (strLine.starts_with("Sharpness ")) type = MaterialFileDataType::SHARP;
						else if (strLine.starts_with("Ni ")) type = MaterialFileDataType::NI;
						else type = MaterialFileDataType::INVALID;
						switch (type) {
						case MaterialFileDataType::NEWMTL:
							strLine.erase(0u, 7u);
							mfd.push_back({ });
							mfd.back().name = strLine;
							break;
						case MaterialFileDataType::KA:
							strLine.erase(0u, 3u);
							if (strLine.starts_with("spectral ")) {
								strLine.erase(0u, 9u);
								mfd.back().KA.isSpectral = true;
								mfd.back().KA.file = parseUntilWS(strLine);
								if (strLine.empty()) break;
								strLine.erase(0u, 1u);
								mfd.back().KA.isSpectralFactor = true;
								mfd.back().KA.factor = std::stof(strLine);
							}
							else if (strLine.starts_with("xyz ")) {
								strLine.erase(0u, 4u);
								mfd.back().KA.isXYZ = true;
								mfd.back().KA.XYZ = parseThreeFloatLine(strLine);
							}
							else {
								mfd.back().KA.isRGB = true;
								mfd.back().KA.RGB = parseThreeFloatLine(strLine);
							}
							break;
						case MaterialFileDataType::KD:
							strLine.erase(0u, 3u);
							if (strLine.starts_with("spectral ")) {
								strLine.erase(0u, 9u);
								mfd.back().KD.isSpectral = true;
								mfd.back().KD.file = parseUntilWS(strLine);
								if (strLine.empty()) break;
								strLine.erase(0u, 1u);
								mfd.back().KD.isSpectralFactor = true;
								mfd.back().KD.factor = std::stof(strLine);
							}
							else if (strLine.starts_with("xyz ")) {
								strLine.erase(0u, 4u);
								mfd.back().KD.isXYZ = true;
								mfd.back().KD.XYZ = parseThreeFloatLine(strLine);
							}
							else {
								mfd.back().KD.isRGB = true;
								mfd.back().KD.RGB = parseThreeFloatLine(strLine);
							}
							break;
						case MaterialFileDataType::KS:
							strLine.erase(0u, 3u);
							if (strLine.starts_with("spectral ")) {
								strLine.erase(0u, 9u);
								mfd.back().KS.isSpectral = true;
								mfd.back().KS.file = parseUntilWS(strLine);
								if (strLine.empty()) break;
								strLine.erase(0u, 1u);
								mfd.back().KS.isSpectralFactor = true;
								mfd.back().KS.factor = std::stof(strLine);
							}
							else if (strLine.starts_with("xyz ")) {
								strLine.erase(0u, 4u);
								mfd.back().KS.isXYZ = true;
								mfd.back().KS.XYZ = parseThreeFloatLine(strLine);
							}
							else {
								mfd.back().KS.isRGB = true;
								mfd.back().KS.RGB = parseThreeFloatLine(strLine);
							}
							break;
						case MaterialFileDataType::TF:
							strLine.erase(0u, 3u);
							if (strLine.starts_with("spectral ")) {
								strLine.erase(0u, 9u);
								mfd.back().TF.isSpectral = true;
								mfd.back().TF.file = parseUntilWS(strLine);
								if (strLine.empty()) break;
								strLine.erase(0u, 1u);
								mfd.back().TF.isSpectralFactor = true;
								mfd.back().TF.factor = std::stof(strLine);
							}
							else if (strLine.starts_with("xyz ")) {
								strLine.erase(0u, 4u);
								mfd.back().TF.isXYZ = true;
								mfd.back().TF.XYZ = parseThreeFloatLine(strLine);
							}
							else {
								mfd.back().TF.isRGB = true;
								mfd.back().TF.RGB = parseThreeFloatLine(strLine);
							}
							break;
						case MaterialFileDataType::IF:
							strLine.erase(0u, 6u);
							mfd.back().IF.at(std::stoi(strLine)) = true;
							break;
						case MaterialFileDataType::D:
							strLine.erase(0u, 2u);
							if (strLine.starts_with('-')) {
								strLine.erase(0u, 6u);
								mfd.back().isDHalo = true;
							}
							mfd.back().D = std::stof(strLine);
							break;
						case MaterialFileDataType::NS:
							strLine.erase(0u, 3u);
							mfd.back().NS = std::stof(strLine);
							break;
						case MaterialFileDataType::SHARP:
							strLine.erase(0u, 10u);
							mfd.back().sharpness = std::stoi(strLine);
							break;
						case MaterialFileDataType::NI:
							strLine.erase(0u, 3u);
							mfd.back().NI = std::stof(strLine);
							break;
						case MaterialFileDataType::INVALID:
							#if defined(_DEBUG)
							std::cout << "[WARNING] Invalid line parsed. [FILE: " << path << ".mtl] - [LINE: " << lineCt << "] \"" << strLine << '\"' << '\n';
							#endif
							break;
						}
						strLine.clear();
					}
				}
				file.close();
				return mfd;
			}
			inline std::vector<GID::DSU::IndexedObjectFileData> indexObjectFileData(std::vector<GID::DSU::ObjectFileData> ofd) {
				std::vector<GID::DSU::IndexedObjectFileData> iofd;
				for (auto& o : ofd) {
					iofd.push_back({ });
					for (auto& it : o.ind) {
						iofd.back().name = o.name;
						iofd.back().mtl = o.mtl;
						iofd.back().pos.push_back(o.pos.at(it.m[0][0]));
						iofd.back().pos.push_back(o.pos.at(it.m[1][0]));
						iofd.back().pos.push_back(o.pos.at(it.m[2][0]));
						iofd.back().tex.push_back(o.tex.at(it.m[0][1]));
						iofd.back().tex.push_back(o.tex.at(it.m[1][1]));
						iofd.back().tex.push_back(o.tex.at(it.m[2][1]));
						iofd.back().norm.push_back(o.norm.at(it.m[0][2]));
						iofd.back().norm.push_back(o.norm.at(it.m[1][2]));
						iofd.back().norm.push_back(o.norm.at(it.m[2][2]));
					}
				}
				return iofd;
			}
			inline GID::DSU::ModelFileData parseModelFiles(std::string& path) {
				return { indexObjectFileData(parseObjFile(path, parseMtlFile(path))) };
			}
		}
	}
}

// Scene Data - Lights
namespace GID {
	namespace GSO {
		namespace Scene {
			inline std::vector<GID::DSU::LightData> gLights{};
			inline void addLight(GID::DSU::LightData& light) {
				gLights.push_back(light);
			}
		}
	}
}

// Object Structure
namespace GID {
	namespace DSU {
		struct Object {

			//friend class Model;

			Position mPos{};
			Speed mSpeed{};
			SphereCollisionCheckData mCollision{};

			VertexBuffer mVertexBuffer{};
			VertexConstantBuffer mVCB{};
			PixelConstantBuffer mPCB{};

			// Object Data
			IndexedObjectFileData mObjectFileData{};
			MaterialData mMaterialData{};
			std::vector<VertexData> mObjectData{};

			Timer timer{};

			Object() = default;
			Object(IndexedObjectFileData& data) {
				using namespace AssistMath;
				// Process data
				mObjectFileData = data;
				mObjectData.resize(mObjectFileData.pos.size());
				float highX{ mObjectFileData.pos.at(0).x };
				float lowX{ mObjectFileData.pos.at(0).x };
				float highY{ mObjectFileData.pos.at(0).y };
				float lowY{ mObjectFileData.pos.at(0).y };
				float highZ{ mObjectFileData.pos.at(0).z };
				float lowZ{ mObjectFileData.pos.at(0).z };
				float furthestPoint{ 0.0f };
				for (int i = 0; i < mObjectFileData.pos.size(); i++) {
					mObjectData.at(i).pos.x = mObjectFileData.pos.at(i).x;
					mObjectData.at(i).pos.y = mObjectFileData.pos.at(i).y;
					mObjectData.at(i).pos.z = mObjectFileData.pos.at(i).z;
					mObjectData.at(i).texcoord.x = mObjectFileData.tex.at(i).x;
					mObjectData.at(i).texcoord.y = mObjectFileData.tex.at(i).y;
					mObjectData.at(i).norm.x = mObjectFileData.norm.at(i).x;
					mObjectData.at(i).norm.y = mObjectFileData.norm.at(i).y;
					mObjectData.at(i).norm.z = mObjectFileData.norm.at(i).z;

					if (mObjectData.at(i).pos.x > highX) highX = mObjectData.at(i).pos.x;
					if (mObjectData.at(i).pos.x < lowX) lowX = mObjectData.at(i).pos.x;
					if (mObjectData.at(i).pos.x > highY) highY = mObjectData.at(i).pos.y;
					if (mObjectData.at(i).pos.x < lowY) lowY = mObjectData.at(i).pos.y;
					if (mObjectData.at(i).pos.x > highZ) highZ = mObjectData.at(i).pos.z;
					if (mObjectData.at(i).pos.x < lowZ) lowZ = mObjectData.at(i).pos.z;

				}

				mPos.center = { AMLoadFloat3({ (highX + lowX) / 2, (highY + lowY) / 2, (highZ + lowZ) / 2 }) };

				for (int i = 0; i < mObjectFileData.pos.size(); i++) {

					FAMVECTOR vert{ AMLoadFloat3(mObjectFileData.pos.at(i)) };

					FAMVECTOR res{ _mm_sub_ps(AMLoadFloat3(mObjectFileData.pos.at(i)), mPos.center) };
					res = { _mm_mul_ps(res, res) };
					float delta = std::sqrt(res.m128_f32[0] + res.m128_f32[1] + res.m128_f32[2]);
					if (delta > furthestPoint) furthestPoint = delta;

				}

				// Collision
				//mCollision.center = mPos.center;
				//mCollision.radius = furthestPoint + 0.1f;

				// Color
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

				mVertexBuffer = {
					GSO::Render::mainGFX().getDevice(),
					GSO::Render::mainGFX().getCommandList(),
					mObjectData.data(), mObjectData.size() };

			}

			Position& getPos() noexcept { return mPos; }
			Speed& getSpeed() noexcept { return mSpeed; }
			MaterialData& getMaterialData() noexcept { return mMaterialData; }

			void update() noexcept {
				mPos.translation = _mm_add_ps(mPos.translation, mSpeed.deltaTranslation);
				mPos.rotation = _mm_add_ps(mPos.rotation, mSpeed.deltaRotation);
				mPos.center = _mm_add_ps(mPos.center, mSpeed.deltaTranslation);
			}
			void draw() noexcept {
				using namespace GSO::Render;
				AssistMath::FAMMATRIX transformM{ getTransformMx() };
				AssistMath::FAMVECTOR det{ FAMMatrixDeterminant(transformM) };
				AssistMath::FAMMATRIX tpose{ FAMMatrixTranspose(transformM) };
				VertexConstantBufferData matrices{
					transformM,
					mainGFX().getCamera().getMatrix(),
					mainGFX().getProjection(),
					FAMMatrixInverse(det, tpose)
				};

				PixelConstantBufferData pcbData{};
				pcbData.mtl = mMaterialData;
				pcbData.eyePos = { mainGFX().getCamera().mEye.m128_f32[0], mainGFX().getCamera().mEye.m128_f32[1], mainGFX().getCamera().mEye.m128_f32[2], 1.0f };
				pcbData.globalAmbient = { 0.f, 0.f, 0.f, 1.f };
				for (int i = 0; i < GSO::Scene::gLights.size(); i++)
					pcbData.lights[i] = GSO::Scene::gLights[i];
				

				mVCB = { GSO::Render::mainGFX().getDevice(), mainGFX().getCommandList(), matrices };
				mPCB = { GSO::Render::mainGFX().getDevice(), mainGFX().getCommandList(), pcbData };

				mVertexBuffer.transitionToRead(mainGFX().getCommandList());
				mVCB.transitionToRead(mainGFX().getCommandList());
				mPCB.transitionToRead(mainGFX().getCommandList());
				mainGFX().getCommandList()->SetGraphicsRootConstantBufferView(0u, mVCB.getDestRes()->GetGPUVirtualAddress());
				mainGFX().getCommandList()->SetGraphicsRootConstantBufferView(1u, mPCB.getDestRes()->GetGPUVirtualAddress());
				mainGFX().getCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				mainGFX().getCommandList()->IASetVertexBuffers(0u, 1u, &mVertexBuffer.getView());
				mainGFX().getCommandList()->DrawInstanced(mVertexBuffer.getCount(), 1u, 0u, 0u);

				mVertexBuffer.transitionToWrite(mainGFX().getCommandList());
				mVCB.transitionToWrite(mainGFX().getCommandList());
				mPCB.transitionToWrite(mainGFX().getCommandList());
			}

			AssistMath::FAMMATRIX getTransformMx() noexcept {
				return {
				AssistMath::FAMMatrixRotationRollPitchYaw(mPos.rotation)
				* AssistMath::FAMMatrixTranslation(mPos.translation)
				};
			}
		};
	}
}

// Model Data Collection
namespace GID {
	namespace GSO {
		namespace Collections {
			namespace ModelData {
				inline std::unordered_map<std::string, GID::DSU::ModelFileData> mCollection{};
				inline auto& get() noexcept { return mCollection; }
				inline void cAdd(std::string& name, GID::DSU::ModelFileData mfd) {
					mCollection.insert(std::pair<std::string, GID::DSU::ModelFileData>(name, mfd));
				}
				inline void rAdd(std::string& name, GID::DSU::ModelFileData& mfd) {
					mCollection.insert(std::pair<std::string, GID::DSU::ModelFileData>(name, mfd));
				}
				inline bool bSearch(std::string& name) {
					for (auto& pair : mCollection) {
						if (pair.first == name) {
							return true;
						}
					}
					return false;
				}
				inline GID::DSU::ModelFileData& rSearch(std::string& name) {
					for (auto& pair : mCollection) {
						if (pair.first == name) {
							return pair.second;
						}
					}
				}
				inline GID::DSU::ModelFileData& quickSearchAndAdd(std::string& name) {
					if (bSearch(name)) return rSearch(name);
					else cAdd(name, GID::Util::FileParsing::parseModelFiles(name));
					return rSearch(name);
				}
			}
		}
	}
}

// Model Structure
namespace GID {
	namespace DSU {
		struct Model {

			GID::DSU::ModelFileData mModelFileData{};
			std::vector<Object> mObjects{};

			Model() = default;
			Model(std::string& objPath) {
				mModelFileData = GID::GSO::Collections::ModelData::quickSearchAndAdd(objPath);
				for (auto& objectdata : mModelFileData.iofd) {
					mObjects.push_back({ objectdata });
				}
			}

			void update() noexcept { for (auto& o : mObjects) o.update(); }

			void draw() noexcept { for (auto& o : mObjects) o.draw(); }

			GID::DSU::ModelFileData& getModelData() noexcept { return mModelFileData; }
			std::vector<Object>& getObjects() noexcept { return mObjects; }
		};
	}
}

// ActorID Collection
namespace GID {
	namespace GSO {
		namespace Collections {
			namespace ActorID {
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
		}
	}
}

// Actor Structure
namespace GID {
	namespace DSU {
		struct Actor : GID::DSU::Inputtable, GID::DSU::Scriptable {

			Model mModel{};
			ActorGroundState mGroundState{ ActorGroundState::AIR };

			Timer mInitTimer{};
			Timer mCurrentStateTimer{};
			std::pair<std::string, UINT> mActorID{};

			Actor() = default;
			Actor(GraphicsOutput& gfx, std::string& objPath) {
				mActorID.first = objPath;
				mActorID.second = GID::GSO::Collections::ActorID::add(objPath);
				mModel = { objPath };
				mCurrentStateTimer.mark();
				mInitTimer.mark();
			}

			Model& getModel() noexcept {
				return mModel;
			}

			void input() noexcept override {

			}
			void update() noexcept {
				mModel.update();
			}
			void draw() noexcept {
				mModel.draw();
			}
			auto& getGroundState() noexcept {
				return mGroundState;
			}
		};
	}
}

// Scene Data - Actor
namespace GID {
	namespace GSO {
		namespace Scene {
			inline std::vector<GID::DSU::Actor> gActors{};
			inline void addActor(std::string actorName) {
				gActors.push_back({ Render::mainGFX(), actorName });
			}
		}
	}
}

// Initialize the GSO
namespace GID {
	namespace GSO {
		namespace Util {
			inline void initGSO() {
				GSO::Render::Viewport::initViewportPresets();
			}
		}
	}
}

// General quickstart/preset initialization of the GSO
namespace GID {
	namespace GSO {
		namespace Util {
			inline void initQuickStart() {
				using namespace GSO; using namespace GSO::Render::Viewport;
				WindowNS::addWindow(1280, 720, L"Main Window");
				Util::initGSO();
				Render::addGFX(GID::DSU::WindowType::MAINWINDOW, { ViewportPresets[(uint32_t)GID::DSU::ViewportPreset::VP1_DEFAULT] });
			}
		}
	}
}

// Debug Input Processing
#if defined(_DEBUG)
namespace GID {
	namespace GSO {
		namespace Input {
			inline int doDebugInput() {
				uint8_t iter{};
				for (auto& wnd : WindowNS::gWnd) {
					if (IsWindowEnabled(wnd->getHandle())) {
						if (gInput[iter].kb.isKeyPressed(VK_ESCAPE)) return 1;
					}
					iter++;
				}
				return 0;
			}
		}
	}
}
#endif

// Per-frame Process Input Function
namespace GID {
	namespace GSO {
		namespace Input {
			inline int processInput() {
				using namespace DSU;
				auto ind = (uint8_t)DSU::WindowType::MAINWINDOW;
				#if defined(_DEBUG)
				bool inputDebug{ true };
				#endif
				if (gInput.size() != WindowNS::gWnd.size()) gInput.resize(WindowNS::gWnd.size());

				while (!gInput[ind].kb.isKeyQueueEmpty()) {
					gInput[ind].keys.push_back(gInput.at(ind).kb.readKey());
					if (inputDebug) std::cout << "[Key Pressed (Code)] (" << gInput[ind].keys.back().getCode() << ")" << '\n';
				}

				while (!gInput[ind].kb.isCharQueueEmpty()) {
					gInput[ind].keysChar.push_back(gInput[ind].kb.readChar());
					if (inputDebug) std::cout << "[Character Inputted] (" << gInput[ind].keysChar.back() << ")" << '\n';
				}

				while (!gInput[ind].mouse.isEmpty()) {
					gInput[ind].mouseEvents.push_back(gInput[ind].mouse.readEvent());
					if (inputDebug) {
						std::cout << "[";
						switch (gInput[ind].mouseEvents.back().getType()) {
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
							std::cout << "Move] (" << gInput[ind].mouseEvents.back().getX() << ", " << gInput[ind].mouseEvents.back().getY() << ")";
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

				#if defined(_DEBUG)
				if (doDebugInput() != 0) return 1;
				#endif
				Render::mainGFX().mCamera.input();

				for (auto& input : gInput) {
					input.keys.clear();
					input.keysChar.clear();
					input.mouseEvents.clear();
				}

				return 0;
			}
		}
	}
}

// Do update stuff
namespace GID {
	namespace GSO {
		namespace Update {
			inline void doUpdate() {
				Render::mainGFX().mCamera.update();
			}
		}
	}
}

// ---
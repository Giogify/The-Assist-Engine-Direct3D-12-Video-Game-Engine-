//#pragma once
//#include "IndexedTriangleList.h"
//#include <DirectXMath.h>
//
//class Cube {
//
//public:
//
//	static IndexedTriangleList make() {
//		namespace dx = DirectX;
//
//		constexpr float side = 1.0f / 2.0f;
//
//		std::vector<dx::XMFLOAT3> verticesRaw;
//		verticesRaw.emplace_back(-side, -side, -side);
//		verticesRaw.emplace_back(side, -side, -side);
//		verticesRaw.emplace_back(-side, side, -side);
//		verticesRaw.emplace_back(side, side, -side);
//		verticesRaw.emplace_back(-side, -side, side);
//		verticesRaw.emplace_back(side, -side, side);
//		verticesRaw.emplace_back(-side, side, side);
//		verticesRaw.emplace_back(side, side, side);
//
//		std::vector verticesProcessed(verticesRaw.size());
//		for (size_t i = 0; i < verticesRaw.size(); i++) {
//			verticesProcessed[i] = verticesRaw[i];
//		}
//		return { std::move(verticesProcessed), {
//			0, 2, 1,	2, 3, 1,
//			1, 3, 5,	3, 7, 5,
//			2, 6, 3,	3, 6, 7,
//			4, 5, 7,	4, 7, 6,
//			0, 4, 2,	2, 4, 6,
//			0, 1, 4,	1, 5, 4
//			} 
//		};
//	}
//};
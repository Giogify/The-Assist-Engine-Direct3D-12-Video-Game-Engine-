#pragma once
#include "GraphicsOutput.h"
#include "IndexedTriangleList.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "DataStructures.h"
#include "Timer.h"
#include <DirectXMath.h>

class Object {

	friend class Model;

private:

	DSU::Position mPos{};
	DSU::Speed mSpeed{};

	VertexBuffer mVertexBuffer{};
	VertexConstantBuffer mVCB{};
	//PixelConstantBuffer mPCB{};

	// Object Data
	IndexedTriangleList::Object mObject{};
	std::vector<DSU::VertexData> mObjectData{};
	DSU::MaterialData mMaterialData{};

	Timer timer{};

public:

	Object() = default;
	Object(GraphicsOutput& gfx, IndexedTriangleList::Object& itl_data);
	
	DSU::Position& getPos() noexcept { return mPos; }
	DSU::Speed& getSpeed() noexcept { return mSpeed; }
	DSU::MaterialData& getMaterialData() noexcept { return mMaterialData; }
	
	void update() noexcept;
	void draw(GraphicsOutput& gfx) noexcept;
};
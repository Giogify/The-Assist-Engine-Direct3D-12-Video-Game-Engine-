#pragma once
#include "Object.h"
#include "Material.h"
#include <random>

class Model {

private:

	std::vector<Material> mMTLData{};
	IndexedTriangleList mITLData{};

	// Model Data
	std::vector<Object> mObjects{};

public:

	Model() = default;
	Model(GraphicsOutput& gfx, std::string& objPath);

	void update() noexcept;

	int onCommand(GraphicsOutput& gfx) noexcept;

	std::vector<Object>& getObjects() noexcept { return mObjects; }
};
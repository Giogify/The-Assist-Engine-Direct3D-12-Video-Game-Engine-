#pragma once
#include "Object.h"
#include "Material.h"
#include <random>

class Model {

private:

	// VertexData struct
	struct VertexData {
		struct {
			float x;
			float y;
			float z;
		} pos;
		struct {
			float x;
			float y;
		} texcoord;
		struct {
			float x;
			float y;
			float z;
		} norm;
		struct {
			float r;
			float g;
			float b;
			float a;
		} color;
	};

private:

	bool isInit = false;

	std::vector<Material> m_MaterialData{};
	std::unique_ptr<IndexedTriangleList> m_ModelData{};

	// Model Data
	std::vector<std::unique_ptr<Object>> m_objects{};

public:

	Model() = default;

	Model(GraphicsOutput& gfx, std::string& objPath);

	//Model& operator=(const Model&) = delete;

	void update() noexcept;

	void draw(GraphicsOutput& gfx) const noexcept;

	std::vector<std::unique_ptr<Object>>& getObjects() noexcept { return m_objects; }
};
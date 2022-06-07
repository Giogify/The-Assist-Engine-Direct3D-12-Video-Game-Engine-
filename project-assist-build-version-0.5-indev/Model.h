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

	std::vector<Material> m_MaterialData{};
	IndexedTriangleList m_ModelData{};

	// Model Data
	std::vector<Object> m_objects{};

public:

	Model() = default;

	Model(GraphicsOutput& gfx, std::string& objPath);

	/*Model& operator=(const Model& model) noexcept {
		Model newmodel{};
		newmodel.m_MaterialData = model.m_MaterialData;
		newmodel.m_ModelData = model.m_ModelData;
		newmodel.m_objects = model.m_objects;
		return newmodel;
	}*/

	void update() noexcept;

	int onCommand(GraphicsOutput& gfx) noexcept;

	std::vector<Object>& getObjects() noexcept { return m_objects; }
};
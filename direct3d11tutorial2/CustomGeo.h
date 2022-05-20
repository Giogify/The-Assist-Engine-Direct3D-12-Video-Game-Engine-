#pragma once
#include "IndexedTriangleList.h"
#include <iostream>
#include <fstream>
#include <string>
#include <DirectXMath.h>

class CustomGeo {

public:

	using itl = IndexedTriangleList;
	static IndexedTriangleList make(std::string& file) {

		std::vector<std::string> names{};
		std::vector<DirectX::XMFLOAT3> pos{};
		std::vector<DirectX::XMFLOAT2> tex{};
		std::vector<DirectX::XMFLOAT3> norm{};
		std::vector<DirectX::XMFLOAT3X3> indices{};
		std::vector<unsigned int> objectIndexTable{};

		std::string nextline{};
		std::ifstream objFile{};
		objFile.open(file + ".obj");
		if (objFile.is_open()) {
			while (objFile) {
				std::getline(objFile, nextline);
				if (nextline.starts_with("o ")) {
					
					// Erase "o "
					nextline.erase(0u, 2u);

					// Add the current size of the indices to the index table.
					objectIndexTable.push_back(*std::make_unique<unsigned short>(indices.size()));

					// Add name
					names.push_back(nextline);

				}
				else if (nextline.starts_with("v ")) {

					// Temporary floats for storing the indivdual x, y, z values
					float vFirst{};
					float vSecond{};
					float vThird{};

					// Erase "v "
					nextline.erase(0u, 2u);

					std::string temp{};
					for (short i = 0; !nextline.empty(); i++) {
						while (!nextline.starts_with(" ") && !nextline.empty()) {
							temp.append(nextline.substr(0u, 1u));
							nextline.erase(0u, 1u);
						}

						if (!nextline.empty()) nextline.erase(0u, 1u);

						switch (i) {
						case 0:
							vFirst = std::stof(temp);
							break;
						case 1:
							vSecond = std::stof(temp);
							break;
						case 2:
							vThird = std::stof(temp);
							break;
						}
						temp.clear();
					}
					
					// Add the number to the position vector
					pos.push_back(*std::make_unique<DirectX::XMFLOAT3>(vFirst, vSecond, vThird));
				}
				else if (nextline.starts_with("vt ")) {
					
					// Temporary floats for storing the indivdual x, y values
					float vFirst{};
					float vSecond{};

					// Erase "vt "
					nextline.erase(0u, 3u);

					std::string temp{};
					for (short i = 0; !nextline.empty(); i++) {
						while (!nextline.starts_with(" ") && !nextline.empty()) {
							temp.append(nextline.substr(0u, 1u));
							nextline.erase(0u, 1u);
						}

						if (!nextline.empty()) nextline.erase(0u, 1u);

						switch (i) {
						case 0:
							vFirst = std::stof(temp);
							break;
						case 1:
							vSecond = std::stof(temp);
							break;
						}
						temp.clear();
					}

					// Add the number to the tex vector
					tex.push_back(*std::make_unique<DirectX::XMFLOAT2>(vFirst, vSecond));

				}
				else if (nextline.starts_with("vn ")) {

					// Temporary floats for storing the indivdual x, y, z values
					float vFirst{};
					float vSecond{};
					float vThird{};

					// Erase "vn "
					nextline.erase(0u, 3u);

					std::string temp{};
					for (short i = 0; !nextline.empty(); i++) {
						while (!nextline.starts_with(" ") && !nextline.empty()) {
							temp.append(nextline.substr(0u, 1u));
							nextline.erase(0u, 1u);
						}

						if (!nextline.empty()) nextline.erase(0u, 1u);

						switch (i) {
						case 0:
							vFirst = std::stof(temp);
							break;
						case 1:
							vSecond = std::stof(temp);
							break;
						case 2:
							vThird = std::stof(temp);
							break;
						}
						temp.clear();
					}

					// Add the number to the normal vector
					norm.push_back(*std::make_unique<DirectX::XMFLOAT3>(vFirst, vSecond, vThird));
				}
				else if (nextline.starts_with("f ")) {

					indices.push_back(*std::make_unique<DirectX::XMFLOAT3X3>());

					unsigned int row = 0u;
					unsigned int col = 0u;

					// Erase "f " from line
					nextline.erase(0u, 2u);
					
					// Temporary string to hold number
					std::string temp{};

					// While the string is not empty
					while (!nextline.empty()) {
						
						// If next character is a number
						if (!nextline.starts_with("/") && !nextline.starts_with(" ")) {
							
							// Add the current number to the temp string
							temp.append(nextline.substr(0u, 1u));

							// Erase that number from the line
							nextline.erase(0u, 1u);

							if (col == 2 && row == 2) {
								indices.at(indices.size() - 1).m[col][row] = std::stoi(temp) - 1;
							}
						}

						// If next character is a "/"
						else if (nextline.starts_with("/")) {

							// Convert currently stored string into a number and store that in the next part of the 3x3 matrix
							if (!temp.empty()) {
								indices.at(indices.size() - 1).m[col][row] = std::stoi(temp) - 1;
							}
							else {
								indices.at(indices.size() - 1).m[col][row] = -1;
							}
							

							// Increment row
							row++;
							
							// Erase "/" from line
							nextline.erase(0u, 1u);

							// Clear the temporary string
							temp.clear();

						}

						// If the next character is a " " (space)
						else if (nextline.starts_with(" ")) {

							// Convert currently stored string into a number and store that in the next part of the 3x3 matrix
							indices.at(indices.size() - 1).m[col][row] = std::stoi(temp) - 1;

							// Erase " " (space) from line
							nextline.erase(0u, 1u);

							// Clear the temporary string
							temp.clear();
							
							// Increment column
							col++;

							// Reset row
							row = 0u;
						}
					}
				}
			}
		}

		/*namespace dx = DirectX;

		constexpr float side = 1.0f / 2.0f;

		std::vector<dx::XMFLOAT3> verticesRaw;
		verticesRaw.emplace_back(-side, -side, -side);
		verticesRaw.emplace_back(side, -side, -side);
		verticesRaw.emplace_back(-side, side, -side);
		verticesRaw.emplace_back(side, side, -side);
		verticesRaw.emplace_back(-side, -side, side);
		verticesRaw.emplace_back(side, -side, side);
		verticesRaw.emplace_back(-side, side, side);
		verticesRaw.emplace_back(side, side, side);

		std::vector verticesProcessed(verticesRaw.size());
		for (size_t i = 0; i < verticesRaw.size(); i++) {
			verticesProcessed[i].pos = verticesRaw[i];
		}*/

		// Add the final size.
		objectIndexTable.push_back(*std::make_unique<unsigned int>(indices.size()));

		return { names, pos, tex, norm, indices, objectIndexTable };
	}
};
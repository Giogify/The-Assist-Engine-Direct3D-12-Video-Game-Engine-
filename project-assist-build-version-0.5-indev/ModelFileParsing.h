#pragma once
#include "AssistMath.h"
#include <iostream>
#include <fstream>
#include <string>

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
	namespace Util {
		namespace FileParsing {
			inline AssistMath::AMFLOAT3 parseThreeFloatLine(std::string& line) {
				float x{}, y{}, z{}; std::string temp{};
				for (uint8_t i = 0; i < 3; i++) {
					while (!line.starts_with(" ") && !line.empty()) { temp.append(line.substr(0u, 1u)); line.erase(0u, 1u); }
					if (!line.empty()) line.erase(0u, 1u);
					switch (i) { case 0: x = std::stof(temp); break; case 1: y = std::stof(temp); break; case 2: z = std::stof(temp); break; }
					temp.clear();
				}
				return { x, y, z };
			}
			inline AssistMath::AMFLOAT2 parseTwoFloatLine(std::string& line) {
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
			inline AssistMath::AMUSHORT3X3 parseFaceIndexLine(std::string& line) {
				AssistMath::AMUSHORT3X3 ind{}; std::string point{}, pointcomp{};
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
			std::vector<GID::DSU::ObjectFileData> parseObjFile(std::string& path, std::vector<GID::DSU::MaterialFileData> mtls) {
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
				return ofd;
			}
			std::vector<GID::DSU::MaterialFileData> parseMtlFile(std::string& path) {
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
				return mfd;
			}
			std::vector<GID::DSU::IndexedObjectFileData> indexObjectFileData(std::vector<GID::DSU::ObjectFileData> ofd) {
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
			GID::DSU::ModelFileData parseModelFiles(std::string& path) {
				return { indexObjectFileData(parseObjFile(path, parseMtlFile(path))) };
			}
		}
	}
}
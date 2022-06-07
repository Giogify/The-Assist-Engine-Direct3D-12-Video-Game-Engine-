#pragma once
#include "Material.h"
#include <iostream>
#include <fstream>
#include <string>
#include <DirectXMath.h>
#include <vector>

class MaterialParser {

public:

	static std::vector<Material> make(std::string& file) {

		std::vector<Material> mtl{};
		std::string nextline{};
		std::ifstream objFile{};
		objFile.open(file + ".mtl");
		if (objFile.is_open()) {
			while (objFile) {
				
				// Get the next line in the file
				std::getline(objFile, nextline);

				// If the current line starts with "newmtl"
				if (nextline.starts_with("newmtl ")) {

					// Erase the "newmtl "
					nextline.erase(0u, 7u);

					// Create a new material in the vector.
					mtl.push_back(*std::make_unique<Material>());

					// Assign the name of the material to the name in the line.
					mtl.at(mtl.size() - 1).name = nextline;
				}

				// If the current line starts with "Ns"
				else if (nextline.starts_with("Ns ")) {

					// Erase "Ns "
					nextline.erase(0u, 3u);

					// Assign the Ns Exponent
					mtl.at(mtl.size() - 1).NS_exponent = std::stof(nextline);
				}

				// If the current line starts with "Ka"
				else if (nextline.starts_with("Ka ")) {

					// Erase the "Ka "
					nextline.erase(0u, 3u);

					// If the current line starts with "spectral"
					if (nextline.starts_with("spectral ")) {

						// Erase the "spectral "
						nextline.erase(0u, 9u);

						// This material uses the spectral format for KA values, set it to true
						mtl.at(mtl.size() - 1).isMaterialKASpectral = true;

						// Temporary string for file name
						std::string temp{};
						
						// If the current character is not a space
						while (nextline.at(0) != ' ') {

							// Add that character to the temp string
							temp.push_back(nextline.at(0));

							// Remove that character from the current line
							nextline.erase(0u, 1u);
						}

						// Set the KA_file to the temporary string.
						mtl.at(mtl.size() - 1).KA_file = temp;
						
						// Erase that space
						nextline.erase(0u, 1u);

						// Set the factor (should be the remainder of the line)
						mtl.at(mtl.size() - 1).KA_factor = std::stof(nextline);
					}

					// If the current line starts with "xyz"
					else if (nextline.starts_with("xyz ")) {
						
						// Erase the "xyz "
						nextline.erase(0u, 4u);

						// This material's KA uses the XYZ format, set it to true
						mtl.at(mtl.size() - 1).isMaterialKAXYZ = true;

						// Capture the X, Y, Z values
						for (int i = 0; !nextline.empty(); i++) {
							std::string temp{};
							while (!nextline.starts_with(" ") && !nextline.empty()) {
								temp.append(nextline.substr(0u, 1u));
								nextline.erase(0u, 1u);
							}
							nextline.erase(0u, 1u);
							switch (i) {
							case 0:
								mtl.at(mtl.size() - 1).KA_XYZ.at(0) = std::stof(temp);
								break;
							case 1:
								mtl.at(mtl.size() - 1).KA_XYZ.at(1) = std::stof(temp);
								break;
							case 2:
								mtl.at(mtl.size() - 1).KA_XYZ.at(2) = std::stof(temp);
								break;
							}
						}
					}

					// Must be a RGB Value
					else {

						// This material's KA uses the RGB format, set it to true
						mtl.at(mtl.size() - 1).isMaterialKARGB = true;

						// Capture the R, G, B values
						for (int i = 0; !nextline.empty(); i++) {
							std::string temp{};
							while (!nextline.starts_with(" ") && !nextline.empty()) {
								temp.append(nextline.substr(0u, 1u));
								nextline.erase(0u, 1u);
							}
							nextline.erase(0u, 1u);
							switch (i) {
							case 0:
								mtl.at(mtl.size() - 1).KA_RGB.at(0) = std::stof(temp);
								break;
							case 1:
								mtl.at(mtl.size() - 1).KA_RGB.at(1) = std::stof(temp);
								break;
							case 2:
								mtl.at(mtl.size() - 1).KA_RGB.at(2) = std::stof(temp);
								break;
							}
						}
					}
				}

				// Kd Values
				else if (nextline.starts_with("Kd")) {
					// Erase the "Kd "
					nextline.erase(0u, 3u);

					// If the current line starts with "spectral"
					if (nextline.starts_with("spectral")) {

						// Erase the "spectral "
						nextline.erase(0u, 9u);

						// This material uses the spectral format for KD values, set it to true
						mtl.at(mtl.size() - 1).isMaterialKDSpectral = true;

						// Temporary string for file name
						std::string temp{};

						// If the current character is not a space
						while (nextline.at(0) != ' ') {

							// Add that character to the temp string
							temp.push_back(nextline.at(0));

							// Remove that character from the current line
							nextline.erase(0u, 1u);
						}

						// Set the KD_file to the temporary string.
						mtl.at(mtl.size() - 1).KD_file = temp;

						// Erase that space
						nextline.erase(0u, 1u);

						// Set the factor (should be the remainder of the line)
						mtl.at(mtl.size() - 1).KD_factor = std::stof(nextline);
					}

					// If the current line starts with "xyz"
					else if (nextline.starts_with("xyz")) {

						// Erase the "xyz  "
						nextline.erase(0u, 4u);

						// This material's KA uses the XYZ format, set it to true
						mtl.at(mtl.size() - 1).isMaterialKDXYZ = true;

						// Capture the X, Y, Z values
						for (int i = 0; !nextline.empty(); i++) {
							std::string temp{};
							while (!nextline.starts_with(" ") && !nextline.empty()) {
								temp.append(nextline.substr(0u, 1u));
								nextline.erase(0u, 1u);
							}
							nextline.erase(0u, 1u);
							switch (i) {
							case 0:
								mtl.at(mtl.size() - 1).KD_XYZ.at(0) = std::stof(temp);
								break;
							case 1:
								mtl.at(mtl.size() - 1).KD_XYZ.at(1) = std::stof(temp);
								break;
							case 2:
								mtl.at(mtl.size() - 1).KD_XYZ.at(2) = std::stof(temp);
								break;
							}
						}
					}

					// Must be a RGB Value
					else {

						// This material's KA uses the RGB format, set it to true
						mtl.at(mtl.size() - 1).isMaterialKDRGB = true;

						// Capture the R, G, B values
						for (int i = 0; !nextline.empty(); i++) {
							std::string temp{};
							while (!nextline.starts_with(" ") && !nextline.empty()) {
								temp.append(nextline.substr(0u, 1u));
								nextline.erase(0u, 1u);
							}
							nextline.erase(0u, 1u);
							switch (i) {
							case 0:
								mtl.at(mtl.size() - 1).KD_RGB.at(0) = std::stof(temp);
								break;
							case 1:
								mtl.at(mtl.size() - 1).KD_RGB.at(1) = std::stof(temp);
								break;
							case 2:
								mtl.at(mtl.size() - 1).KD_RGB.at(2) = std::stof(temp);
								break;
							}
						}
					}
				}

				// Ks Values
				else if (nextline.starts_with("Ks ")) {
					// Erase the "Ks "
					nextline.erase(0u, 3u);

					// If the current line starts with "spectral"
					if (nextline.starts_with("spectral ")) {

						// Erase the "spectral "
						nextline.erase(0u, 9u);

						// This material uses the spectral format for KD values, set it to true
						mtl.at(mtl.size() - 1).isMaterialKSSpectral = true;

						// Temporary string for file name
						std::string temp{};

						// If the current character is not a space
						while (nextline.at(0) != ' ') {

							// Add that character to the temp string
							temp.push_back(nextline.at(0));

							// Remove that character from the current line
							nextline.erase(0u, 1u);
						}

						// Set the KS_file to the temporary string.
						mtl.at(mtl.size() - 1).KS_file = temp;

						// Erase that space
						nextline.erase(0u, 1u);

						// Set the factor (should be the remainder of the line)
						mtl.at(mtl.size() - 1).KS_factor = std::stof(nextline);
					}

					// If the current line starts with "xyz"
					else if (nextline.starts_with("xyz")) {

						// Erase the "xyz  "
						nextline.erase(0u, 4u);

						// This material's KS uses the XYZ format, set it to true
						mtl.at(mtl.size() - 1).isMaterialKSXYZ = true;

						// Capture the X, Y, Z values
						for (int i = 0; !nextline.empty(); i++) {
							std::string temp{};
							while (!nextline.starts_with(" ") && !nextline.empty()) {
								temp.append(nextline.substr(0u, 1u));
								nextline.erase(0u, 1u);
							}
							nextline.erase(0u, 1u);
							switch (i) {
							case 0:
								mtl.at(mtl.size() - 1).KS_XYZ.at(0) = std::stof(temp);
								break;
							case 1:
								mtl.at(mtl.size() - 1).KS_XYZ.at(1) = std::stof(temp);
								break;
							case 2:
								mtl.at(mtl.size() - 1).KS_XYZ.at(2) = std::stof(temp);
								break;
							}
						}
					}

					// Must be a RGB Value
					else {

						// This material's KS uses the RGB format, set it to true
						mtl.at(mtl.size() - 1).isMaterialKSRGB = true;

						// Capture the R, G, B values
						for (int i = 0; !nextline.empty(); i++) {
							std::string temp{};
							while (!nextline.starts_with(" ") && !nextline.empty()) {
								temp.append(nextline.substr(0u, 1u));
								nextline.erase(0u, 1u);
							}
							nextline.erase(0u, 1u);
							switch (i) {
							case 0:
								mtl.at(mtl.size() - 1).KS_RGB.at(0) = std::stof(temp);
								break;
							case 1:
								mtl.at(mtl.size() - 1).KS_RGB.at(1) = std::stof(temp);
								break;
							case 2:
								mtl.at(mtl.size() - 1).KS_RGB.at(2) = std::stof(temp);
								break;
							}
						}
					}
				}
				
				// Tf values
				else if (nextline.starts_with("Tf")) {
					// Erase the "Tf "
					nextline.erase(0u, 3u);

					// If the current line starts with "spectral"
					if (nextline.starts_with("spectral")) {

						// Erase the "spectral "
						nextline.erase(0u, 9u);

						// This material uses the spectral format for KD values, set it to true
						mtl.at(mtl.size() - 1).isMaterialTFSpectral = true;

						// Temporary string for file name
						std::string temp{};

						// If the current character is not a space
						while (nextline.at(0) != ' ') {

							// Add that character to the temp string
							temp.push_back(nextline.at(0));

							// Remove that character from the current line
							nextline.erase(0u, 1u);
						}

						// Set the TF_file to the temporary string.
						mtl.at(mtl.size() - 1).TF_file = temp;

						// Erase that space
						nextline.erase(0u, 1u);

						// Set the factor (should be the remainder of the line)
						mtl.at(mtl.size() - 1).TF_factor = std::stof(nextline);
					}

					// If the current line starts with "xyz"
					else if (nextline.starts_with("xyz")) {

						// Erase the "xyz  "
						nextline.erase(0u, 4u);

						// This material's TF uses the XYZ format, set it to true
						mtl.at(mtl.size() - 1).isMaterialTFXYZ = true;

						// Capture the X, Y, Z values
						for (int i = 0; !nextline.empty(); i++) {
							std::string temp{};
							while (!nextline.starts_with(" ") && !nextline.empty()) {
								temp.append(nextline.substr(0u, 1u));
								nextline.erase(0u, 1u);
							}
							nextline.erase(0u, 1u);
							switch (i) {
							case 0:
								mtl.at(mtl.size() - 1).TF_XYZ.at(0) = std::stof(temp);
								break;
							case 1:
								mtl.at(mtl.size() - 1).TF_XYZ.at(1) = std::stof(temp);
								break;
							case 2:
								mtl.at(mtl.size() - 1).TF_XYZ.at(2) = std::stof(temp);
								break;
							}
						}
					}

					// Must be a RGB Value
					else {

						// This material's TF uses the RGB format, set it to true
						mtl.at(mtl.size() - 1).isMaterialTFRGB = true;

						// Capture the R, G, B values
						for (int i = 0; !nextline.empty(); i++) {
							std::string temp{};
							while (!nextline.starts_with(" ") && !nextline.empty()) {
								temp.append(nextline.substr(0u, 1u));
								nextline.erase(0u, 1u);
							}
							nextline.erase(0u, 1u);
							switch (i) {
							case 0:
								mtl.at(mtl.size() - 1).TF_RGB.at(0) = std::stof(temp);
								break;
							case 1:
								mtl.at(mtl.size() - 1).TF_RGB.at(1) = std::stof(temp);
								break;
							case 2:
								mtl.at(mtl.size() - 1).TF_RGB.at(2) = std::stof(temp);
								break;
							}
						}
					}
				}
				
				// Illumination Models
				else if (nextline.starts_with("illum ")) {
					
					// Erase "illum "
					nextline.erase(0u, 6u);

					// Store illumination model
					unsigned short numIllum = std::stoi(nextline);

					// Update Internal Flag
					for (int i = 0; i < 11; i++) {
						if (numIllum == i) {
							mtl.at(mtl.size() - 1).isIllumFlags.at(i) = true;
						}
					}
				}
				
				// Dissolve factor
				else if (nextline.starts_with("d ")) {
					
					// Erase "d "
					nextline.erase(0u, 2u);

					// determine if "-halo" is on
					if (nextline.starts_with("-halo ")) {

						// Erase "-halo "
						nextline.erase(0u, 6u);

						// Enable halo
						mtl.at(mtl.size() - 1).isDHalo = true;
					}
					
					// Store factor
					mtl.at(mtl.size() - 1).D_factor = std::stof(nextline);
				}
				
				// Ns exponent
				else if (nextline.starts_with("Ns ")) {
					
					//Erase "Ns "
					nextline.erase(0u, 3u);

					// Store exponent
					mtl.at(mtl.size() - 1).NS_exponent = std::stof(nextline);
				}

				// Sharpness value
				else if (nextline.starts_with("sharpness ")) {
					
					// Erase "sharpness "
					nextline.erase(0u, 10u);

					// Store sharpness value
					mtl.at(mtl.size() - 1).sharpness = std::stof(nextline);
				}

				// Ni optical density
				else if (nextline.starts_with("Ni ")) {
					
					// Erase "Ni "
					nextline.erase(0u, 3u);

					// Store optical density value
					mtl.at(mtl.size() - 1).NI_optical_density = std::stof(nextline);
				}
			}
		}
		return mtl;
	}
};
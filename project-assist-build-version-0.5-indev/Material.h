/*
/  ----------------------------------------------
/  Material.h
/  
/  Instances of this class are stores of data about materials for models in the scene.
/  
/  The following key helps with specific data:
/  KA is "Ambient Reflectivity"
/  
/  KD is "Diffuse Reflectivity" : 
/  Its values specify the color of the 
/  atmosphere.
/  
/  KS is "Spectral Reflectivity" : 
/  Its values specify the color of the
/  atmosphere.
/  
/  TF is "Transmission Filter" : 
/  Any light passing through the object is 
/  filtered by the transmission filter, which 
/  only allows the specifiec colors to pass 
/  through.
/  
/  Illumination Flags :
/  0		Color on and Ambient off
/  1		Color on and Ambient on
/  2		Highlight on
/  3		Reflection on and Ray trace on
/  4		Transparency: Glass on
/			Reflection: Ray trace on
/  5		Reflection: Fresnel on and Ray trace on
/  6		Transparency: Refraction on
/   		Reflection: Fresnel off and Ray trace on
/  7		Transparency: Refraction on
 /  		Reflection: Fresnel on and Ray trace on
/  8		Reflection on and Ray trace off
/  9		Transparency: Glass on
/   		Reflection: Ray trace off
/  10		Casts shadows onto invisible surfaces
/  
/  d is "Dissolve" :
/  The amount this material dissolves into the 
/  background.  A factor of 1.0 is fully opaque.
/  ----------------------------------------------
*/

#pragma once
#include "AssistMath.h"
#include <vector>
#include <memory>
#include <string>

class Material {

public:

	// Material name
	std::string name{};

	// Material KA type
	bool isMaterialKARGB{ false };

	bool isMaterialKASpectral{ false };
	bool isMaterialKASpectralFactor{ false };

	bool isMaterialKAXYZ{ false };

	// Material KA values
	std::vector<double> KA_RGB{0.0f, 0.0f, 0.0f};

	std::string KA_file{};
	double KA_factor{};

	std::vector<double> KA_XYZ{ 0.0f, 0.0f, 0.0f };

	// Material KD type
	bool isMaterialKDRGB{ false };

	bool isMaterialKDSpectral{ false };
	bool isMaterialKDSpectralFactor{ false };

	bool isMaterialKDXYZ{ false };

	// Material KD values
	std::vector<double> KD_RGB{ 0.0f, 0.0f, 0.0f };

	std::string KD_file{};
	double KD_factor{};

	std::vector<double> KD_XYZ{ 0.0f, 0.0f, 0.0f };

	// Material KS type
	bool isMaterialKSRGB{ false };

	bool isMaterialKSSpectral{ false };
	bool isMaterialKSSpectralFactor{ false };

	bool isMaterialKSXYZ{ false };

	// Material KS values
	std::vector<double> KS_RGB{ 0.0f, 0.0f, 0.0f };

	std::string KS_file{};
	double KS_factor{};

	std::vector<double> KS_XYZ{ 0.0f, 0.0f, 0.0f };

	// Material TF type
	bool isMaterialTFRGB{ false };

	bool isMaterialTFSpectral{ false };
	bool isMaterialTFSpectralFactor{ false };

	bool isMaterialTFXYZ{ false };

	// Material TF values
	std::vector<double> TF_RGB{ 0.0f, 0.0f, 0.0f };

	std::string TF_file{};
	double TF_factor{};

	std::vector<double> TF_XYZ{ 0.0f, 0.0f, 0.0f };

	// Illumination Flags
	std::vector<bool> isIllumFlags{ false, false, false, false, false, false, false, false, false, false, false };

	// D
	double D_factor{};
	bool isDHalo{ false };

	// Material NS values
	double NS_exponent{};

	// Material Sharpness
	double sharpness{};

	// Material NI values
	double NI_optical_density{};

	// Material Textures /* Implement later */

public:

	Material() = default;

};
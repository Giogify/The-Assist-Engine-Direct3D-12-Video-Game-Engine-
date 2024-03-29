#include "Application.h"
#include <memory>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <array>
#include <fstream>

// Top-Level Application Logic
uint8_t Application::applicationUpdate() {

	using namespace GID::GSO::Render;
	using namespace GID::GSO::Input;

	GID::DSU::Timer avgfpstimer{};
	std::wstring output{};

	output.clear(); {
		output.append(L"Elapsed Time: ");
		output.append(std::to_wstring((int32_t)mTimerStart.peek()));
		output.append(L"s");
		gGFX.m2D.addDebugQueue(output);
	}
	output.clear(); {
		output.append(L"FPS: ");
		output.append(std::to_wstring((int32_t)(1 / mTimerRender.mark())));
		gGFX.m2D.addDebugQueue(output);
	}

	[[likely]]
	if (FPScount != 0) {
		output.clear(); {
			output.append(L"Avg. FPS: ");
			output.append(std::to_wstring((int32_t)(std::accumulate(FPSsum.cbegin(), FPSsum.cend(), 0.f) / FPSsum.size())));
			gGFX.m2D.addDebugQueue(output);
		}
	}
	output.clear(); {
		output.append(L"Actor Count: ");
		output.append(std::to_wstring(GID::GSO::Scene::gActors.size()));
		gGFX.m2D.addDebugQueue(output);
	}
	output.clear(); {
		output.append(L"Mouse Position: (");
		output.append(std::to_wstring(gInput.mouse.getX()));
		output.append(L", ");
		output.append(std::to_wstring(gInput.mouse.getY()));
		output.append(L")");
		gGFX.m2D.addDebugQueue(output);
	}
	output.clear(); {
		output.append(L"Camera Position: (");
		output.append(std::to_wstring((int32_t)gGFX.mCamera.mEye.m128_f32[0]));
		output.append(L", ");
		output.append(std::to_wstring((int32_t)gGFX.mCamera.mEye.m128_f32[1]));
		output.append(L", ");
		output.append(std::to_wstring((int32_t)gGFX.mCamera.mEye.m128_f32[2]));
		output.append(L")");
		gGFX.m2D.addDebugQueue(output);
	}
	output.clear(); {
		output.append(L"Fence Value: ");
		output.append(std::to_wstring(gGFX.mFenceValue));
		gGFX.m2D.addDebugQueue(output);
	}

	if (doInput() != 0) return 1;
	if (doUpdate() != 0) return 1;
	if (doRender() != 0) return 1;

	FPSsum.at(FPScount % 1000) = 1 / avgfpstimer.mark();
	FPScount++;


	/*oss << " [Actors] " << mScene.getActors().size();


	//DSU::Position temppos = mScene.getActors().at(0).getModel().getObjects().at(0).getPos();
	//DSU::Speed temp = mScene.getActors().at(0).getModel().getObjects().at(0).getSpeed();
	/*oss << " [x] " << temppos.translation.m128_f32[0] << " [y] " << temppos.translation.m128_f32[1] << " [z] " << temppos.translation.m128_f32[2];
	oss << " [dX] " << temp.deltaTranslation.m128_f32[0] << " [dY] " << temp.deltaTranslation.m128_f32[1] << " [dZ] " << temp.deltaTranslation.m128_f32[2];
	oss << ' '
		<< camera.mRotation.m128_f32[Camera::Rotation::Pitch] << ' '
		<< camera.mRotation.m128_f32[Camera::Rotation::Yaw] << ' '
		<< camera.mRotation.m128_f32[Camera::Rotation::Roll] << '\n';*/
	
	/*oss << " [X: " << camera.mEye.x << "]"
		<< " [Y: " << camera.mEye.y << "]"
		<< " [Z: " << camera.mEye.z << "]"
		<< " [Pitch: " << camera.mPitch << "]"
		<< " [Yaw: " << camera.mYaw << "]"
		<< " [Roll: " << camera.mRoll << "]";*/

	return 0;
}

Application::~Application() {}
Application::Application() {
	using namespace GID;
	using namespace GSO;
	using namespace DSU;
	using namespace Light;
	
	GSO::Util::initQuickStart();

	XMMATRIX projectionTemp{ 
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(General::gCfgGen.gCameraAngle), WindowNS::gWnd.get()->getAspectRatio(), 0.25f, 1000.0f
		) 
	};
	Render::setGFXProjection(projectionTemp);

	LightData light0{}; {
		light0.type = (uint32_t)POINT_LIGHT;
		//light0.direction = { 1.0f, -1.0f, -1.0f, 0.0f };
		light0.pos = { 60.0f, 30.0f, 60.0f, 1.0f };
		light0.constAtten = 0.75f;
		light0.linAtten = 0.0f;
		light0.quadAtten = 0.0f;
		light0.isEnabled = TRUE;
	}
	LightData light1{}; {
		light1.type = (uint32_t)POINT_LIGHT;
		//light0.direction = { 1.0f, -1.0f, -1.0f, 0.0f };
		light1.pos = { -60.0f, 30.0f, -60.0f, 1.0f };
		light1.constAtten = 0.75f;
		light1.linAtten = 0.0f;
		light1.quadAtten = 0.0f;
		light1.isEnabled = TRUE;
	}
	Scene::addLight(light0);
	Scene::addLight(light1);

	Render::gGFX.getCamera().addScript(SCRIPT_ID_CAMERA_UPDATE_ADVANCED_FOLLOW);

	Scene::addActor({ "dragon" });
	//Scene::addActor({ "testCube" });

	gTexture0 = { L"textures\\Floor_C.jpg" };
	gTexture1 = { L"textures\\Dragon_Bump_Col2.jpg" };

	General::gGlobalTimer.mark();
	Update::gTickTimer.mark();



	//GID::Render::gfx.mProjection = { FAMMatrixPerspectiveFovLH(AMConvertToRadians(viewingAngle), 16.0f / 9.0f, 0.25f, 5000.0f) };
	//GID::Scene::gActors.push_back(Actor(GID::Render::gfx, *std::make_unique<std::string>("testCube")));
	//GID::Scene::gActors.push_back(Actor(GID::Render::gfx, *std::make_unique<std::string>("plane2")));
	//mWnd.getGraphicsOutput().getCamera().mbFollow = Camera::FOLLOW;
	//mScene = { mWnd.getGraphicsOutput() };
	//mWnd.getGraphicsOutput().mCamera = { actor.getModel().getObjects().at(0).getPos().center };
	//std::string file{ "scene00" };
	//mScene = std::make_unique<Scene>(mWnd.getGraphicsOutput(), file);
	//mScene->getCamera().translate(0.0f, 0.0f, 30.0f);
}
uint8_t Application::applicationStart() {
	mTimerRender.mark();
	while (true) {
		uint8_t hr{};
		if (const auto msgCode = GID::GSO::WindowNS::gWnd->handleMessages()) {
			GID::GSO::Render::gGFX.flushGPU();
			CloseHandle(GID::GSO::Render::gGFX.mhFenceEvent);
			return (uint8_t)*msgCode;
		}
		hr = applicationUpdate();
		if (hr != 0) {
			GID::GSO::Render::gGFX.flushGPU();
			CloseHandle(GID::GSO::Render::gGFX.mhFenceEvent);
			return hr;
		}
	}
}

uint8_t Application::doInput() noexcept {
	using namespace GID;
	using namespace GSO;
	using namespace Input;
	using namespace Render;
	DSU::Timer timer{};
	std::wstring output{};
	timer.mark();
	
	
	if (Input::doInput() != 0) return 1;
	

	auto time = timer.mark();
	output.clear(); {
		output.append(L"Input Time:\t");
		output.append(std::to_wstring((int32_t)(time * 1e6f)));
		output.append(L"\tus");
		gGFX.m2D.addDebugQueue(output);
	}
	return 0;
}
uint8_t Application::doUpdate() noexcept {
	using namespace GID;
	using namespace GSO;
	using namespace Update;
	using namespace Render;
	DSU::Timer timer{};
	std::wstring output{};
	timer.mark();

	
	initUpdateCycle();
	Update::doUpdate();


	auto time = timer.mark();
	output.clear(); {
		output.append(L"Update Time:\t");
		output.append(std::to_wstring((int32_t)(time * 1e6f)));
		output.append(L"\tus");
		gGFX.m2D.addDebugQueue(output);
	}
	return 0;
}
uint8_t Application::doRender() noexcept {
	using namespace GID;
	using namespace GSO;
	using namespace Render;
	using namespace Scene;
	DSU::Timer timer{};
	std::wstring output{};
	timer.mark();

	
	gGFX.startFrame();
	for (auto& a : gActors) a.draw();
	
	
	auto time = timer.mark();
	output.clear(); {
		output.append(L"Render Time:\t");
		output.append(std::to_wstring((int32_t)(time * 1e6f)));
		output.append(L"\tus");
		gGFX.m2D.addDebugQueue(output);
	}
	
	
	gGFX.endFrame();
	
	
	return 0;
}
#include "Application.h"
#include <memory>
#include <iostream>
#include <iomanip>
#include <array>
#include <fstream>

// Top-Level Application Logic
int Application::applicationUpdate() {

	if (doInput() != 0) return 1;
	if (doUpdate() != 0) return 1;
	if (doRender() != 0) return 1;
	bool wndTitleDebug{ true };
	#if defined(_DEBUG)
	wndTitleDebug = true;
	#endif
	// Create Debug Information
	if (wndTitleDebug) {
		if (mMaxFPS < 1000.0f / (mTimerRender.peek() * 1000.0000000f)) mMaxFPS = 1000.0f / (mTimerRender.peek() * 1000.0000000f);
		std::wostringstream oss = {};
		oss.setf(std::ios::fixed);
		oss << std::setprecision(2);

		//Camera camera = GID::Render::gfx.getCamera();

		const auto t = mTimerStart.peek();
		oss << "[Elasped Time] " << std::fixed << t;
		//oss << " [Mouse Position] (" << mWnd.mouse.getX() << "," << mWnd.mouse.getY() << ")";
		oss << " Highest FPS: " << mMaxFPS;
		//oss << " [Frames: " << this->mFPSCap << "] " << 1000 / (mTimerRender.mark() * 1000) << '\n';
		oss << " [FPS: " << 1000 / (mTimerRender.mark() * 1000) << "] " <<  '\n';
		//DSU::Position temppos = mScene.getActors().at(0).getModel().getObjects().at(0).getPos();
		//DSU::Speed temp = mScene.getActors().at(0).getModel().getObjects().at(0).getSpeed();
		/*oss << " [x] " << temppos.translation.m128_f32[0] << " [y] " << temppos.translation.m128_f32[1] << " [z] " << temppos.translation.m128_f32[2];
		oss << " [dX] " << temp.deltaTranslation.m128_f32[0] << " [dY] " << temp.deltaTranslation.m128_f32[1] << " [dZ] " << temp.deltaTranslation.m128_f32[2];
		oss << ' '
			<< camera.mRotation.m128_f32[Camera::Rotation::Pitch] << ' '
			<< camera.mRotation.m128_f32[Camera::Rotation::Yaw] << ' '
			<< camera.mRotation.m128_f32[Camera::Rotation::Roll] << '\n';*/

		/*oss << " [Actors] " << mScene.getActors().size();
		oss << " [X: " << camera.mEye.x << "]"
			<< " [Y: " << camera.mEye.y << "]"
			<< " [Z: " << camera.mEye.z << "]"
			<< " [Pitch: " << camera.mPitch << "]"
			<< " [Yaw: " << camera.mYaw << "]"
			<< " [Roll: " << camera.mRoll << "]";*/

		// Print Debug Information to Window Title
		GID::GSO::WindowNS::gWnd.at((uint8_t)GID::DSU::WindowType::MAINWINDOW).get()->setTitle(oss.str());
	}
	return 0;
}

Application::~Application() {}
Application::Application() {
	using namespace GID;
	using namespace GSO;
	using namespace DSU::AssistMath;
	
	GSO::Util::initQuickStart();
	
	FAMMATRIX projectionTemp{ 
		FAMMatrixPerspectiveFovLH(
			AMConvertToRadians(General::gCfgGen.gCameraAngle), 16.0f / 9.0f, 0.25f, 5000.0f
		) 
	};
	Render::setGFXProjection(GID::DSU::WindowType::MAINWINDOW, projectionTemp);

	GID::DSU::LightData light0 = {}; {
		light0.pos = { 20.0f, -35.0f, 20.0f, 1.0f };
		light0.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		light0.constAtten = 0.0f;
		light0.linAtten = 0.1f;
		light0.quadAtten = 0.0f;
		light0.isEnabled = true;
		light0.type = (int32_t)GID::DSU::LightConst::POINT_LIGHT;
	}
	GID::GSO::Scene::addLight(light0);
	GID::DSU::LightData light1 = {}; {
		light1.pos = { -20.0f, -35.0f, -20.0f, 1.0f };
		light1.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		light1.constAtten = 0.0f;
		light1.linAtten = 0.1f;
		light1.quadAtten = 0.0f;
		light1.isEnabled = true;
		light1.type = (int32_t)GID::DSU::LightConst::POINT_LIGHT;
	}
	GID::GSO::Scene::addLight(light1);

	for (int i = 0; i < 1; i++)
		Scene::addActor({ "nanosuit" });

	for (int i = 0; i < 1; i++)
		Scene::addActor({ "plane3" });

	//Scene::gActors.at(0).addScript(DSU::ScriptID::BasicGravity);
	//Scene::gActors.at(0).addScript(DSU::ScriptID::BasicCollision);

	//Scene::gActors.at(0).mModel.mObjects.at(0).mSpeed.deltaTranslation.m128_f32[0] += 10.0f;
	//Scene::gActors.at(0).mModel.mObjects.at(0).mSpeed.deltaTranslation.m128_f32[2] += 10.0f;

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
int Application::applicationStart() {
	while (true) {
		int hr{};
		for (auto& w : GID::GSO::WindowNS::gWnd)
			if (const auto msgCode = w->handleMessages()) {
				for (auto& gfx : GID::GSO::Render::gGFX) {
					gfx.gfx.flushGPU();
					CloseHandle(gfx.gfx.mhFenceEvent);
				}
				return *msgCode;
			}
		if (hr = applicationUpdate() != 0) {
			for (auto& gfx : GID::GSO::Render::gGFX) {
				gfx.gfx.flushGPU();
				CloseHandle(gfx.gfx.mhFenceEvent);
			}
			return hr;
		}
	}
}

int Application::doInput() noexcept {
	if (GID::GSO::Input::doInput() != 0) return 1;
	return 0;
}
int Application::doUpdate() noexcept {
	GID::GSO::Update::initUpdateCycle();
	GID::GSO::Update::doUpdate();
	return 0;
}
int Application::doRender() noexcept {
	using namespace GID;
	using namespace GSO;
	using namespace Render;
	mainGFX().startFrame();
	for (auto& a : Scene::gActors) a.draw();
	mainGFX().endFrame();

	return 0;
}
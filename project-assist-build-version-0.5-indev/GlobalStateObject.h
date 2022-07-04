//#pragma once
//
//namespace GID {
//	namespace Collections {
//		namespace ModelData {
//			std::unordered_map<std::string, GID::DSU::ModelFileData> mCollection{};
//			auto& get() noexcept { return mCollection; }
//			inline void cAdd(std::string& name, GID::DSU::ModelFileData mfd) {
//				mCollection.insert(std::pair<std::string, GID::DSU::ModelFileData>(name, mfd));
//			}
//			inline void rAdd(std::string& name, GID::DSU::ModelFileData& mfd) {
//				mCollection.insert(std::pair<std::string, GID::DSU::ModelFileData>(name, mfd));
//			}
//			inline bool bSearch(std::string& name) {
//				for (auto& pair : mCollection) {
//					if (pair.first == name) {
//						return true;
//					}
//				}
//				return false;
//			}
//			inline GID::DSU::ModelFileData& rSearch(std::string& name) {
//				for (auto& pair : mCollection) {
//					if (pair.first == name) {
//						return pair.second;
//					}
//				}
//			}
//			GID::DSU::ModelFileData& quickSearchAndAdd(std::string& name) {
//				if (bSearch(name)) return rSearch(name);
//				else cAdd(name, GID::Util::FileParsing::parseModelFiles(name));
//				return rSearch(name);
//			}
//		}
//		namespace ActorID {
//			std::vector<std::pair<std::string, UINT>> mCollection{};
//			auto& get() noexcept { return mCollection; }
//			UINT add(std::string& actorID) {
//				for (auto& str : mCollection) {
//					if (actorID == str.first) {
//						str.second++;
//						return str.second;
//					}
//				}
//				std::pair<std::string, UINT> newActorID{ actorID, 1u };
//				mCollection.push_back(newActorID);
//				return 1u;
//			}
//		}
//	}
//	namespace DSU {
//		struct VertexData {
//			struct {
//				float x{};
//				float y{};
//				float z{};
//			} pos;
//			struct {
//				float x{};
//				float y{};
//			} texcoord;
//			struct {
//				float x{};
//				float y{};
//				float z{};
//			} norm;
//		};
//		struct PositionData {
//			AssistMath::AMFLOAT3 pos{};
//		};
//		struct MaterialData {
//			struct {
//				float r{};
//				float g{};
//				float b{};
//				float a{};
//			} colorEmissive, colorAmbient, colorDiffuse, colorSpecular;
//
//			float specularPower{};
//			int isTextured{ false };
//			int padding0{};
//			int padding1{};
//		};
//		struct VertexConstantBufferData {
//			AssistMath::FAMMATRIX transform{};
//			AssistMath::FAMMATRIX camera{};
//			AssistMath::FAMMATRIX projection{};
//			AssistMath::FAMMATRIX invtpose{};
//		};
//		struct PixelConstantBufferData {
//			MaterialData mtl{};
//			AssistMath::AMFLOAT4 eyePos{};
//			AssistMath::AMFLOAT4 globalAmbient{};
//			std::array<Light::LightData, Light::MAX_LIGHTS> lights{};
//		};
//		struct Position {
//			AssistMath::FAMVECTOR translation{};
//			AssistMath::FAMVECTOR rotation{};
//			AssistMath::FAMVECTOR center{};
//		};
//		struct Speed {
//			AssistMath::FAMVECTOR deltaTranslation{};
//			AssistMath::FAMVECTOR deltaRotation{};
//		};
//		struct PipelineStateStream {
//			CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
//			CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
//			CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopologyType;
//			CD3DX12_PIPELINE_STATE_STREAM_VS VS;
//			CD3DX12_PIPELINE_STATE_STREAM_PS PS;
//			CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
//			CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
//			CD3DX12_PIPELINE_STATE_STREAM_RASTERIZER RS;
//		};
//		struct ObjectDrawStaticMatrices {
//			AssistMath::FAMMATRIX camera;
//			AssistMath::FAMMATRIX projection;
//		};
//		struct SphereCollisionCheckData {
//			AssistMath::FAMVECTOR center{};
//			float radius{};
//		};
//		enum class ActorGroundState {
//			GROUND,
//			AIR,
//			WATER,
//			INVALID
//		};
//		struct Model {
//
//			GID::DSU::ModelFileData mModelFileData{};
//			std::vector<Object> mObjects{};
//
//			Model() = default;
//			Model(std::string& objPath) {
//				mModelFileData = GID::Collections::ModelData::quickSearchAndAdd(objPath);
//				for (auto& objectdata : mModelFileData.iofd) {
//					mObjects.push_back({ objectdata });
//				}
//			}
//
//			void update() noexcept { for (auto& o : mObjects) o.update(); }
//
//			void draw() noexcept { for (auto& o : mObjects) o.draw(); }
//
//			GID::DSU::ModelFileData& getModelData() noexcept { return mModelFileData; }
//			std::vector<Object>& getObjects() noexcept { return mObjects; }
//		};
//		struct Actor : GID::Inputtable, GID::Scripts::Scriptable {
//
//			Model mModel{};
//			ActorGroundState mGroundState{ ActorGroundState::AIR };
//
//			Timer mInitTimer{};
//			Timer mCurrentStateTimer{};
//			std::pair<std::string, UINT> mActorID{};
//
//			Actor() = default;
//			Actor(GraphicsOutput& gfx, std::string& objPath) {
//				mActorID.first = objPath;
//				mActorID.second = GID::Collections::ActorID::add(objPath);
//				mModel = { objPath };
//				mCurrentStateTimer.mark();
//				mInitTimer.mark();
//			}
//
//			Model& getModel() noexcept {
//				return mModel;
//			}
//
//			void input() noexcept override {
//
//			}
//			void update() noexcept {
//				mModel.update();
//			}
//			void draw() noexcept {
//				mModel.draw();
//			}
//			auto& getGroundState() noexcept {
//				return mGroundState;
//			}
//		};
//		enum class WindowType {
//			MAINWINDOW,
//			SUBWINDOW0,
//			SUBWINDOW1,
//			INVALID
//		};
//		struct Window {
//
//			const std::wstring	wndClassName{ L"AssistEngine_ver0.5_indev" };
//			HINSTANCE			hInst{};
//			uint8_t				WindowID{};
//			unsigned int		mWidth{};
//			unsigned int		mHeight{};
//			HWND				mhWnd{};
//			//RAWINPUTDEVICE	mRID{};
//
//			static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
//				if (msg == WM_NCCREATE) {
//					const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
//					Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
//					SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
//					SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
//					return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
//				}
//				return DefWindowProc(hWnd, msg, wParam, lParam);
//			}
//			static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
//				Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
//				return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
//			}
//			LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
//				using namespace GID::GSO::Input;
//				switch (msg) {
//
//					// Close Window
//				/*case WM_CLOSE:
//					pGraphicsOutput->flushGPU();
//					CloseHandle(pGraphicsOutput->mhFenceEvent);
//					PostQuitMessage(0);
//					return 0;*/
//
//					// ---- KEYBOARD MESSAGES ---- //
//
//						// When Key is Pressed
//				case WM_KEYDOWN:
//					if (!(lParam & 0x40000000) || gInput[WindowID].kb.isAutoRepeatEnabled()) {
//						gInput[WindowID].kb.onKeyPressed((uint8_t)wParam);
//					}
//					break;
//
//					// When System Key is Pressed
//				case WM_SYSKEYDOWN:
//					if (!(lParam & 0x40000000) || gInput[WindowID].kb.isAutoRepeatEnabled()) {
//						gInput[WindowID].kb.onKeyPressed((uint8_t)wParam);
//					}
//					break;
//
//					// When Key is Released
//				case WM_KEYUP:
//					gInput[WindowID].kb.onKeyReleased((uint8_t)wParam);
//					break;
//
//					// When System Key is Released
//				case WM_SYSKEYUP:
//					gInput[WindowID].kb.onKeyReleased((uint8_t)wParam);
//					break;
//
//					// The Value of the Pressed Key
//				case WM_CHAR:
//					gInput[WindowID].kb.onChar((uint8_t)wParam);
//					break;
//
//					// ---- MOUSE MESSAGES ---- //
//
//					/*case WM_INPUT: {
//						UINT dwSize = sizeof(RAWINPUT);
//						static BYTE lpb[sizeof(RAWINPUT)];
//
//						GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
//
//						RAWINPUT* raw = (RAWINPUT*)lpb;
//
//						if (raw->header.dwType == RIM_TYPEMOUSE) {
//							mouse.OnMouseMove(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
//						}
//						break;
//					}*/
//
//					// When the Mouse is Moved
//				case WM_MOUSEMOVE: {
//					const POINTS point = MAKEPOINTS(lParam);
//
//					// If mouse is inside window
//					if (point.x >= 0
//						&& point.x < mWidth
//						&& point.y >= 0
//						&& point.y < mHeight) {
//						gInput[WindowID].mouse.onMouseMove(point.x, point.y);
//						if (!gInput[WindowID].mouse.isInsideWindow()) {
//							SetCapture(hWnd);
//							gInput[WindowID].mouse.onMouseEnter();
//						}
//					}
//
//					// Else (mouse is outside window)
//					else {
//
//						// If LMB or RMB is pressed
//						if (gInput[WindowID].mouse.isLMBPressed() || gInput[WindowID].mouse.isRMBPressed()) {
//							gInput[WindowID].mouse.onMouseMove(point.x, point.y);
//						}
//
//						// Else (LMB and RMB not pressed)
//						else {
//							ReleaseCapture();
//							gInput[WindowID].mouse.onMouseExit();
//						}
//					}
//					break;
//				}
//
//								 // When the LeftMouseButton is Pressed
//				case WM_LBUTTONDOWN: {
//					const POINTS point = MAKEPOINTS(lParam);
//					gInput[WindowID].mouse.onLMBPressed(point.x, point.y);
//					break;
//				}
//
//								   // When the LeftMouseButton is Released
//				case WM_LBUTTONUP: {
//					const POINTS point = MAKEPOINTS(lParam);
//					gInput[WindowID].mouse.onLMBReleased(point.x, point.y);
//					break;
//				}
//
//								 // When the RightMouseButton is Pressed
//				case WM_RBUTTONDOWN: {
//					const POINTS point = MAKEPOINTS(lParam);
//					gInput[WindowID].mouse.onRMBPressed(point.x, point.y);
//					break;
//				}
//
//								   // When the RightMouseButton is Released
//				case WM_RBUTTONUP: {
//					const POINTS point = MAKEPOINTS(lParam);
//					gInput[WindowID].mouse.onRMBReleased(point.x, point.y);
//					break;
//				}
//
//								 // When the Mouse Wheel is Moved
//				case WM_MOUSEWHEEL: {
//					const POINTS point = MAKEPOINTS(lParam);
//					const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
//					gInput[WindowID].mouse.onWheelDelta(point.x, point.y, delta);
//				}
//
//								  // ---- MISCELLANEOUS MESSAGES ---- //
//
//									  // Losing Focus On Window
//				case WM_KILLFOCUS:
//					gInput[WindowID].kb.clearState();
//					break;
//					/*case WM_SIZE: {
//						RECT clientRc{};
//						GetClientRect(hWnd, &clientRc);
//						UINT width = clientRc.right - clientRc.left;
//						UINT height = clientRc.bottom = clientRc.top;
//						pGraphicsOutput->resizeWindow(width, height);
//						break;
//					}*/
//				}
//				return DefWindowProc(hWnd, msg, wParam, lParam);
//			}
//
//			Window() = default;
//			Window(unsigned int width, unsigned int height, const TCHAR* name) :
//				mWidth(width),
//				mHeight(height) {
//
//				WindowID = GID::GSO::WindowNS::gWnd.size();
//
//				// Window Class
//				WNDCLASSEX wcex{};
//				wcex.cbSize = sizeof(wcex);
//				wcex.style = CS_OWNDC;
//				wcex.lpfnWndProc = HandleMsgSetup;
//				wcex.cbClsExtra = 0;
//				wcex.cbWndExtra = 0;
//				wcex.hInstance = hInst;
//				wcex.hIcon = nullptr;
//				wcex.hCursor = nullptr;
//				wcex.hbrBackground = nullptr;
//				wcex.lpszMenuName = nullptr;
//				wcex.lpszClassName = wndClassName.c_str();
//				wcex.hIconSm = nullptr;
//
//				// Register Window
//				RegisterClassEx(&wcex);
//
//				RECT rc = {};
//				rc.left = 100;
//				rc.right = width + rc.left;
//				rc.top = 100;
//				rc.bottom = height + rc.top;
//				AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
//
//				mhWnd = CreateWindow(
//					wndClassName.c_str(),
//					name,
//					WS_OVERLAPPEDWINDOW,
//					(1920 / 2) - ((rc.right - rc.left) / 2),
//					(1080 / 2) - ((rc.bottom - rc.top) / 2),
//					rc.right - rc.left,
//					rc.bottom - rc.top,
//					nullptr,
//					nullptr,
//					hInst,
//					this
//				);
//
//				// Show the window, since it will be hidden on creation.
//				ShowWindow(mhWnd, SW_SHOWDEFAULT);
//
//				/*mRID.usUsagePage = HID_USAGE_PAGE_GENERIC;
//				mRID.usUsage = HID_USAGE_GENERIC_MOUSE;
//				mRID.dwFlags = RIDEV_INPUTSINK;
//				mRID.hwndTarget = mhWnd;
//				RegisterRawInputDevices(&mRID, 1u, sizeof(mRID));*/
//
//			}
//			~Window() { DestroyWindow(mhWnd); }
//
//			void setTitle(const std::wstring& title) { SetWindowText(mhWnd, title.c_str()); }
//			static std::optional<WPARAM> handleMessages() {
//				MSG msg;
//				while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
//					if (msg.message == WM_QUIT) return msg.wParam;
//					TranslateMessage(&msg);
//					DispatchMessage(&msg);
//				}
//				return {}; // empty optional
//			}
//			WINDOWINFO getWindowInfo() noexcept {
//				WINDOWINFO wndInfo = {};
//				wndInfo.cbSize = sizeof(WINDOWINFO);
//				GetWindowInfo(mhWnd, &wndInfo);
//				return wndInfo;
//			}
//			HWND& getHandle() noexcept { return mhWnd; }
//
//
//
//		};
//	}
//	namespace GSO {
//		namespace General {
//			struct {
//				float gCurrentFrameDeltaTime{};
//			} gGenVar;
//			struct {
//				float gMaxFPS{ 0.0f };
//				float gFPSCap{ 60.0f };
//				float gCameraAngle{ 70.0f };
//				float gSpeedMultiplier{ 1.0f };
//			} gCfgGen;
//		}
//		namespace WindowNS {
//			std::vector<std::unique_ptr<GID::DSU::Window>> gWnd{};
//			void addWindow(UINT w, UINT h, const TCHAR* name) {
//				gWnd.push_back(std::make_unique<GID::DSU::Window>(w, h, name));
//				GID::GSO::Input::gInput.push_back({ });
//			}
//		}
//		namespace Input {
//			struct PackedInput {
//				DSU::Keyboard kb{};
//				std::vector<DSU::Keyboard::Event> keys{};
//				std::vector<uint8_t> keysChar{};
//				DSU::Mouse mouse{};
//				std::vector<DSU::Mouse::Event> mouseEvents{};
//			};
//			std::vector<PackedInput> gInput{};
//			void processInput() {
//				if (gInput.size() != WindowNS::gWnd.size()) gInput.resize(WindowNS::gWnd.size());
//				for (auto& wnd : WindowNS::gWnd) {
//
//				}
//			}
//		}
//		namespace Update {}
//		namespace Render {
//			namespace Viewport {
//				enum ViewportPreset {
//					VP1_DEFAULT,
//					VP2_HORIZONTAL,
//					VP2_VERTICAL,
//					VP4_CORNER,
//					INVALID
//				};
//				std::array<GraphicsOutput::VIEWPORT_DESC, 4u> ViewportPresets;
//				void initViewportPresets() {
//					RECT wnd{ WindowNS::gWnd.at((uint8_t)GID::DSU::WindowType::MAINWINDOW).get()->getWindowInfo().rcClient };
//					// VP1_DEFAULT
//					ViewportPresets[0] = {
//						{
//							{
//								CD3DX12_VIEWPORT(
//									0.0f,
//									0.0f,
//									(FLOAT)wnd.right - (FLOAT)wnd.left,
//									(FLOAT)wnd.bottom - (FLOAT)wnd.top
//								)
//							}
//						}
//					};
//					// VP2_HORIZONTAL
//					ViewportPresets[1] = {
//						{
//							{
//								CD3DX12_VIEWPORT(
//								0.0f,
//								0.0f,
//								((FLOAT)wnd.right - (FLOAT)wnd.left) / 2,
//								(FLOAT)wnd.bottom - (FLOAT)wnd.top
//								)
//							},
//							{
//								CD3DX12_VIEWPORT(
//									((FLOAT)wnd.right - (FLOAT)wnd.left) / 2,
//									0.0f,
//									((FLOAT)wnd.right - (FLOAT)wnd.left) / 2,
//									(FLOAT)wnd.bottom - (FLOAT)wnd.top
//								)
//							}
//						}
//					};
//					// VP2_VERTICAL
//					ViewportPresets[2] = {
//						{
//							{
//								CD3DX12_VIEWPORT(
//								0.0f,
//								0.0f,
//								(FLOAT)wnd.right - (FLOAT)wnd.left,
//								((FLOAT)wnd.bottom - (FLOAT)wnd.top) / 2
//								)
//							},
//							{
//								CD3DX12_VIEWPORT(
//									0.0f,
//									((FLOAT)wnd.bottom - (FLOAT)wnd.top) / 2,
//									(FLOAT)wnd.right - (FLOAT)wnd.left,
//									((FLOAT)wnd.bottom - (FLOAT)wnd.top) / 2
//								)
//							}
//						}
//					};
//					// VP4_CORNER
//					ViewportPresets[3] = {
//						{
//							{
//								CD3DX12_VIEWPORT(
//								0.0f,
//								0.0f,
//								((FLOAT)wnd.right - (FLOAT)wnd.left) / 2,
//								((FLOAT)wnd.bottom - (FLOAT)wnd.top) / 2
//								)
//							},
//							{
//								CD3DX12_VIEWPORT(
//									((FLOAT)wnd.right - (FLOAT)wnd.left) / 2,
//									0.0f,
//									((FLOAT)wnd.right - (FLOAT)wnd.left) / 2,
//									((FLOAT)wnd.bottom - (FLOAT)wnd.top) / 2
//								)
//							},
//							{
//								CD3DX12_VIEWPORT(
//									0.0f,
//									((FLOAT)wnd.bottom - (FLOAT)wnd.top) / 2,
//									((FLOAT)wnd.right - (FLOAT)wnd.left) / 2,
//									((FLOAT)wnd.bottom - (FLOAT)wnd.top) / 2
//								)
//							},
//							{
//								CD3DX12_VIEWPORT(
//									((FLOAT)wnd.right - (FLOAT)wnd.left) / 2,
//									((FLOAT)wnd.bottom - (FLOAT)wnd.top) / 2,
//									((FLOAT)wnd.right - (FLOAT)wnd.left) / 2,
//									((FLOAT)wnd.bottom - (FLOAT)wnd.top) / 2
//								)
//							}
//						}
//					};
//				}
//			}
//			struct GFXPipelineContainer {
//				GID::DSU::WindowType wndtype{};
//				GraphicsOutput gfx{};
//			};
//			std::vector<GFXPipelineContainer> gGFX;
//			void addGFX(GID::DSU::WindowType type, GraphicsOutput::GFX_DESC desc) {
//				bool found = false;
//				for (auto& w : gGFX) if (w.wndtype == type) found = true;
//				if (!found) gGFX.push_back({ type, {} });
//				for (auto& w : gGFX) if (w.wndtype == type) w.gfx = { GSO::WindowNS::gWnd.at((uint8_t)type)->getHandle(), desc };
//			}
//			GraphicsOutput& findGFX(GID::DSU::WindowType type) {
//				for (auto& w : gGFX) if (w.wndtype == type) return w.gfx;
//			}
//			GraphicsOutput& mainGFX() { return findGFX(GID::DSU::WindowType::MAINWINDOW); }
//			void setGFXProjection(GID::DSU::WindowType wndType, FAMMATRIX& projection) {
//				for (auto& g : gGFX) if (g.wndtype == wndType) g.gfx.setProjection(projection);
//			}
//		}
//		namespace Scene {
//			std::vector<GID::DSU::Actor> gActors{};
//			void addActor(GID::DSU::WindowType type, std::string actorName) {
//				gActors.push_back({ GSO::Render::findGFX(type), actorName });
//			}
//		}
//		namespace Util {
//			void initGSO() { GSO::Render::Viewport::initViewportPresets(); }
//			void initQuickStart() {
//				using namespace GSO; using namespace GSO::Render::Viewport;
//				WindowNS::addWindow(1280, 720, L"Main Window");
//				Util::initGSO();
//				Render::addGFX(GID::DSU::WindowType::MAINWINDOW, { ViewportPresets[ViewportPreset::VP1_DEFAULT] });
//			}
//		}
//	}
//}
#include <includes.h>
#include <no_hook/no_hook.h>
#include <no_define.h>
#include <no_loop.h>

typedef HRESULT(*present_fn)(IDXGISwapChain*, UINT, UINT);
inline present_fn original_present{ };

typedef HRESULT(*resize_fn)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);
inline resize_fn original_resize{ };

color blue(255, 0, 0, 255);
color red(255, 255, 0, 0);

//rendertarget
ID3D11Texture2D* RenderTargetTexture;
ID3D11RenderTargetView* RenderTargetView = NULL;

UINT vps = 1;
D3D11_VIEWPORT viewport;
bool firstTime = true;

unsigned long RGBAtoHEX(int r, int g, int b)
{
	return ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
}

void functions_line(int x, int y, int x2, int y2, no_menu::color c) noexcept
{
	color cool_col(c.a, c.r, c.g, c.b);
	gui.draw_line(x, y, x2, y2, cool_col);
}

void functions_rect(int x, int y, int x2, int y2, no_menu::color c) noexcept
{
	color cool_col(c.a, c.r, c.g, c.b);
	gui.draw_rect(x, y, x2, y2, cool_col);
}

void functions_filled_rect(int x, int y, int x2, int y2, no_menu::color c) noexcept
{
	color cool_col(c.a, c.r, c.g, c.b);
	gui.fill_rect(x, y, x2, y2, cool_col);
}

const wchar_t* CharToWchar(const char* ch)
{
	const size_t len = strlen(ch) + 1;
	wchar_t* wch = new wchar_t[len];
	mbstowcs(wch, ch, len);
	return wch;
}

UINT32 ColorToUint32(const no_menu::color color)
{
	UINT32 result =
		(BYTE(color.a) << 24) +
		(BYTE(color.r) << 16) +
		(BYTE(color.g) << 8) +
		BYTE(color.b);
	return result;
}

void functions_text(int x, int y, no_menu::color color, bool center, const char* text) noexcept
{
	if (pFontWrapper) {
		char* yh = (char*)text;
		const size_t cSize = strlen(yh) + 1;
		wchar_t* wc = new wchar_t[cSize];
		size_t tmp = 0;
		mbstowcs_s(&tmp, wc, cSize, yh, cSize);

		pFontWrapper->DrawString(pContext, wc, 14, x, y, ColorToUint32(color), FW1_RESTORESTATE);
	}
}

float functions_get_frametime() noexcept
{
	return 60.f;
}

short functions_oGetAsyncKeyState(int key_to_get) noexcept {
	return SDK::Syscall::NtGetAsyncKeyState(key_to_get);
}

auto CreateConsole() -> void
{
	LI_FN(AllocConsole)();
	FILE* fp;
	freopen_s(&fp, _("CONOUT$"), _("w"), stdout);
}

auto InitOnce() -> void
{
	no_menu::functions.draw_line = functions_line;
	no_menu::functions.draw_rect = functions_rect;
	no_menu::functions.draw_filled_rect = functions_filled_rect;
	no_menu::functions.draw_text = functions_text;
	no_menu::functions.get_text_size = functions_get_text_size;
	no_menu::functions.get_frametime = functions_get_frametime;
	no_menu::functions.get_key_state_discord = functions_oGetAsyncKeyState;
}

HRESULT present_hooked(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (firstTime)
	{
		pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice);
		pDevice->GetImmediateContext(&pContext);

		HRESULT hResult = FW1CreateFactory(FW1_VERSION, &pFW1Factory);
		hResult = pFW1Factory->CreateFontWrapper(pDevice, _(L"Tahoma"), &pFontWrapper);
		pFW1Factory->Release();

		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pSwapChain->GetDevice(__uuidof(pDevice), (void**)&pDevice);
			pDevice->GetImmediateContext(&pContext);
		}

		if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&RenderTargetTexture)))
		{
			pDevice->CreateRenderTargetView(RenderTargetTexture, NULL, &RenderTargetView);
			RenderTargetTexture->Release();
			pDevice->Release();
		}

		InitOnce(); //only once
		gui.init(pSwapChain);

		firstTime = false;
	}

	if (!RenderTargetView)
	{
		pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice);

		if (!pDevice) return false;

		pDevice->GetImmediateContext(&pContext);

		if (!pContext) return false;

		if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&RenderTargetTexture)))
		{
			pDevice->CreateRenderTargetView(RenderTargetTexture, NULL, &RenderTargetView);
			RenderTargetTexture->Release();
			pDevice->Release();
		}
	}

	if (RenderTargetView)
	{
		pContext->RSGetViewports(&vps, &viewport);
		X = viewport.Width;
		Y = viewport.Height;

		pContext->OMSetRenderTargets(1, &RenderTargetView, NULL);

		gui.begin_scene();
		{
			no_menu::window_input(_("UnrealWindow"), _("Fortnite  "));

			if (no_menu::begin_window(_("fortnite loverz"), no_menu::vec2({ 545, 580 }), VK_INSERT, no_menu::no_menu_window_flags_none))
			{

				if (tab == 1) { tab1 = true; tab2 = false; tab3 = false; tab4 = false; }
				no_menu::Lineee(no_menu::vec2({ 120, 4 }), tab1); if (no_menu::button(_("Aimbot"), no_menu::vec2({ 120, 37 }))) { tab = 1; }

				no_menu::next_column(130, 0);

				if (tab == 2) { tab1 = false; tab2 = true; tab3 = false; tab4 = false; }
				no_menu::Lineee(no_menu::vec2({ 120, 4 }), tab2); if (no_menu::button(_("ESP"), no_menu::vec2({ 120, 37 }))) { tab = 2; }

				no_menu::next_column(130, 0);

				if (tab == 3) { tab1 = false; tab2 = false; tab3 = true; tab4 = false; }
				no_menu::Lineee(no_menu::vec2({ 120, 4 }), tab3); if (no_menu::button(_("Exploits"), no_menu::vec2({ 120, 37 }))) { tab = 3; }

				no_menu::next_column(130, 0);

				if (tab == 4) { tab1 = false; tab2 = false; tab3 = false; tab4 = true; }
				no_menu::Lineee(no_menu::vec2({ 120, 4 }), tab4); if (no_menu::button(_("Misc"), no_menu::vec2({ 120, 37 }))) { tab = 4; }

				no_menu::back_column(390, 50);

				if (tab == 1) no_menu::begin_groupbox(_("Aimbot"), no_menu::vec2({ 513, 480 }));
				if (tab == 2) no_menu::begin_groupbox(_("ESP"), no_menu::vec2({ 513, 480 }));
				if (tab == 3) no_menu::begin_groupbox(_("Exploits(D)"), no_menu::vec2({ 513, 480 }));
				if (tab == 4) no_menu::begin_groupbox(_("Misc"), no_menu::vec2({ 513, 480 }));


				if (tab == 1) {
					no_menu::checkbox(_("Enable Aimbot"), Settings::Aimbot);
					UI_Help::AimbotTab::Aimbot();

					no_menu::separator(10, false);
					no_menu::slider_int(_("Aimbot Fov"), 10.f, 999.f, Settings::AimbotFovCircle);
					UI_Help::AimbotTab::AimbotFov();

					no_menu::separator(10, false);
					no_menu::slider_int(_("Aimbot Smooth"), 3.0f, 20.f, Settings::AimbotSmooth);
					UI_Help::AimbotTab::AimbotSmooth();

					no_menu::separator(10, false);
					no_menu::combobox(_("Aimbot Bone"), AimbotBone, Settings::AimbotBone_Value);
					UI_Help::AimbotTab::AimbotBone();

					no_menu::separator(10, false);
					no_menu::checkbox(_("Aimbot Prediction"), Settings::AimbotPrediction);
					UI_Help::AimbotTab::AimbotPrediction();
				}

				if (tab == 2) {
					no_menu::checkbox(_("Box ESP"), Settings::ESP);
					UI_Help::ESPTab::ESP();

					no_menu::separator(10, false);
					no_menu::checkbox(_("Skeleton"), Settings::ESP_Skeleton);
					UI_Help::ESPTab::SKELETON();

					no_menu::separator(10, false);
					no_menu::checkbox(_("Lines"), Settings::ESP_Lines);

					//	no_menu::separator(10, false);
					//	no_menu::checkbox(_("Name ESP"), Settings::NameESP);

					no_menu::separator(10, false);
					no_menu::checkbox(_("Aiming Line"), Settings::ESP_AimingLines);

					no_menu::separator(10, false);
					no_menu::checkbox(_("LLama"), Settings::ESP_Llama);

					no_menu::separator(10, false);
					no_menu::checkbox(_("Vehicle"), Settings::ESP_Vehicle);
				}

				if (tab == 3) {
					no_menu::checkbox(_("Aim While Jumping"), Settings::AimWhileJumping);
					UI_Help::ExploitsTab::AimWhileJumping();

					no_menu::separator(10, false);
					no_menu::checkbox(_("No Weapon Switch Delay"), Settings::NoAnimation);
					UI_Help::ExploitsTab::NoAnimations();

					no_menu::separator(10, false);
					no_menu::checkbox(_("No Spread"), Settings::NoSpread);
					UI_Help::ExploitsTab::NoSpread();

					no_menu::separator(10, false);
					no_menu::checkbox(_("Rapid Fire"), Settings::RapidFire);
					UI_Help::ExploitsTab::RapidFire();

					no_menu::separator(10, false);
					no_menu::checkbox(_("Trigger Bot"), Settings::TriggerBot);
					UI_Help::ExploitsTab::TriggerBot();

					no_menu::separator(10, false);
					no_menu::checkbox(_("AirStuck"), Settings::AirStuck);
					UI_Help::ExploitsTab::AirStuck();

					no_menu::separator(10, false);
					no_menu::checkbox(_("Instant Revive"), Settings::InstantRevive);
					UI_Help::ExploitsTab::InstantRevive();
				}


				if (tab == 4) {
					no_menu::checkbox(_("Visible Check"), Settings::VisibleCheck);
					no_menu::separator(10, false);
					no_menu::checkbox(_("Show Fov Circle"), Settings::ShowFovCircle);
					no_menu::separator(10, false);
					//no_menu::checkbox(_("RADAR ESP"), Settings::RadarESP);
					//no_menu::separator(10, false);
					no_menu::checkbox(_("Crosshair"), Settings::Crosshair);
					no_menu::separator(10, false);
					if (no_menu::button(_("Webiste"), no_menu::vec2({ 113, 32 }))) {
						system(_("start https://website.sex"));
					}
					no_menu::separator(10, false);
					if (no_menu::button(_("Discord Server"), no_menu::vec2({ 113, 32 }))) {
						system(_("start https://sex.discord.com"));
					}
				}

				no_menu::end_groupbox();
				no_menu::end_window();
			}

			no_loop::MainLoop();//tick

			if (Settings::ShowFovCircle)
				gui.draw_circle(gui.get_width() / 2, gui.get_height() / 2, Settings::AimbotFovCircle / 2, blue);

			if (Settings::Crosshair)
				gui.draw_cross(gui.get_width() / 2, gui.get_height() / 2, 100, 100, red);

		}
		gui.end_scene();

		if (pFontWrapper)
		{
			pFontWrapper->DrawString(pContext, _(L"sex"), 14, 16.0f, 16.0f, 0xff0000ff, FW1_RESTORESTATE);
		}

		if (SDK::Syscall::NtGetAsyncKeyState(VK_F10))
			LI_FN(MessageBoxA)((HWND)0, (LPCSTR)0, (LPCSTR)0, (UINT)0);
	}

	return original_present(pSwapChain, SyncInterval, Flags);
}

HRESULT resize(IDXGISwapChain* swapchain, const UINT buffer_count, const UINT width, const UINT height, const DXGI_FORMAT new_format, UINT swapchain_flags) {
	gui.reset();
	pDevice->Release();
	pContext->Release();
	pFW1Factory->Release();
	RenderTargetView->Release();
	RenderTargetTexture->Release();
	pDevice = nullptr;
	firstTime = true;

	return original_resize(swapchain, buffer_count, width, height, new_format, swapchain_flags);
}

void Init()
{
	//CreateConsole(); //console

	WNDCLASSEXA wc
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		[](const HWND window, const UINT message, const WPARAM wparam, const LPARAM lparam) -> LRESULT
		{
			return LI_FN(DefWindowProcA).cached()(window, message, wparam, lparam);
		},
		0L,
		0L,
		LI_FN(GetModuleHandleA).cached()(nullptr),
		nullptr,
		nullptr,
		nullptr,
		_("BBV"),
		nullptr
	};

	LI_FN(RegisterClassExA)(&wc);

	auto level = D3D_FEATURE_LEVEL_11_0;
	DXGI_SWAP_CHAIN_DESC sd;
	{
		ZeroMemory(&sd, sizeof sd);
		sd.BufferCount = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = LI_FN(FindWindowA)(_("UnrealWindow"), _("Fortnite  "));
		sd.SampleDesc.Count = 1;
		sd.Windowed = TRUE;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	IDXGISwapChain* swap_chain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;

	LI_FN(D3D11CreateDeviceAndSwapChain)(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &level, 1, D3D11_SDK_VERSION, &sd, &swap_chain, &device, nullptr, &context);

	auto* swap_chainvtable = reinterpret_cast<uintptr_t*>(swap_chain);
	swap_chainvtable = reinterpret_cast<uintptr_t*>(swap_chainvtable[0]);

	DWORD old_protect;
	original_present = reinterpret_cast<present_fn>(reinterpret_cast<DWORD_PTR*>(swap_chainvtable[8]));
	LI_FN(VirtualProtect)(swap_chainvtable, 0x1000, PAGE_EXECUTE_READWRITE, &old_protect);
	swap_chainvtable[8] = reinterpret_cast<DWORD_PTR>(present_hooked);
	LI_FN(VirtualProtect)(swap_chainvtable, 0x1000, old_protect, &old_protect);

	DWORD old_protect_resize;
	original_resize = reinterpret_cast<resize_fn>(reinterpret_cast<DWORD_PTR*>(swap_chainvtable[13]));
	LI_FN(VirtualProtect)(swap_chainvtable, 0x1000, PAGE_EXECUTE_READWRITE, &old_protect_resize);
	swap_chainvtable[13] = reinterpret_cast<DWORD_PTR>(resize);
	LI_FN(VirtualProtect)(swap_chainvtable, 0x1000, old_protect_resize, &old_protect_resize);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Init();
	}
	return TRUE;
}
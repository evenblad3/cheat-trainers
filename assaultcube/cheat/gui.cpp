/*
* Most UI stuff is here...
*/

#include "gui.hpp"
#include "hack.hpp"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter)) return true;

	switch (message)
	{
	case WM_SIZE:
	{
		if (gui::device && wideParameter != SIZE_MINIMIZED)
		{
			gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
			gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
			gui::ResetDevice();
		}
	} return 0;

	case WM_SYSCOMMAND:
	{
		if ((wideParameter & 0xfff0) == SC_KEYMENU) return 0;
	} break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	} return 0;

	case WM_LBUTTONDOWN:
	{
		gui::position = MAKEPOINTS(longParameter);

		if (gui::position.y >= 0 && gui::position.y <= 19)
		{
			gui::dragging = true;
		}

	} return 0;

	case WM_LBUTTONUP:
	{
		gui::dragging = false;
	} return 0;

	case WM_MOUSEMOVE:
	{
		if (gui::dragging && (wideParameter & MK_LBUTTON))
		{
			const auto points = MAKEPOINTS(longParameter);
			auto rect = RECT{};

			GetWindowRect(gui::window, &rect);

			rect.left += points.x - gui::position.x;
			rect.top += points.y - gui::position.y;

			SetWindowPos(
				gui::window,
				nullptr,
				rect.left,
				rect.top,
				0,
				0,
				SWP_NOSIZE | SWP_NOZORDER
			);
		}

		return 0;
	}

	}
	return DefWindowProcW(window, message, wideParameter, longParameter);
}

void gui::CreateHWindow(
	const char* windowName,
	const char* className) noexcept
{
	windowClass.cbSize = sizeof(tagWNDCLASSEXA);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = className;
	windowClass.hIconSm = 0;

	RegisterClassExA(&windowClass);

	window = CreateWindowA(
		className,
		windowName,
		WS_POPUP,
		100, 100,
		WIDTH, HEIGHT,
		0, 0,
		windowClass.hInstance,
		0
	);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void gui::DestroyHWindow() noexcept
{
	DestroyWindow(window);
	UnregisterClassA(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::CreateDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d) return false;

	ZeroMemory(&presentParameters, sizeof(presentParameters));

	presentParameters.Windowed = TRUE;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&presentParameters,
		&device) < 0)
		return false;

	return true;
}
void gui::ResetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&presentParameters);

	if (result == D3DERR_INVALIDCALL) IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::DestroyDevice() noexcept
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

void gui::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
}

void gui::DestroyImGui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void gui::BeginRender() noexcept
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void gui::EndRender() noexcept
{
	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();

		const auto result = device->Present(0, 0, 0, 0);

		if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			ResetDevice();
	}
}

void gui::Render() noexcept
{
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin("Assault Cube Trainer", &exit,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse
	);

	// UI start
	DWORD procId = hack::GetProcId(L"ac_client.exe");
	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	if (procId && hProcess)
	{
		uintptr_t moduleBase = hack::GetModuleBaseAddress(procId, L"ac_client.exe");
		uintptr_t dynamicPtrBaseAddr = moduleBase + 0x0017E0A8;

		// variables
		float currentFOV = 0;
		static bool infiniteAmmo = false;
		static float FOV = 140;
		int tripleNine = 999;

		uintptr_t fovAddr = moduleBase + 0x18A7CC;

		// assault rifle
		int currentAssaultRifleAmmo = 0;
		std::vector<unsigned int>assaultRifleAmmoOffset{ 0x140 };
		uintptr_t assaultRifleAmmoAddr = hack::FindDMAAddy(hProcess, dynamicPtrBaseAddr, assaultRifleAmmoOffset);
		ReadProcessMemory(hProcess, (BYTE*)assaultRifleAmmoAddr, &currentAssaultRifleAmmo, sizeof(float), nullptr);

		// pistol
		int currentPistolAmmo = 0;
		std::vector<unsigned int>pistolAmmoOffset{ 0x12C };
		uintptr_t pistolAmmoAddr = hack::FindDMAAddy(hProcess, dynamicPtrBaseAddr, pistolAmmoOffset);
		ReadProcessMemory(hProcess, (BYTE*)pistolAmmoAddr, &currentPistolAmmo, sizeof(float), nullptr);

		// submachine
		int currentSubmachineAmmo = 0;
		std::vector<unsigned int>submachineOffset{ 0x138 };
		uintptr_t submachineAmmoAddr = hack::FindDMAAddy(hProcess, dynamicPtrBaseAddr, submachineOffset);
		ReadProcessMemory(hProcess, (BYTE*)submachineAmmoAddr, &currentSubmachineAmmo, sizeof(float), nullptr);

		// sniper
		int currentSniperAmmo = 0;
		std::vector<unsigned int>sniperOffset{ 0x13C };
		uintptr_t sniperAmmoAddr = hack::FindDMAAddy(hProcess, dynamicPtrBaseAddr, sniperOffset);
		ReadProcessMemory(hProcess, (BYTE*)sniperAmmoAddr, &currentSniperAmmo, sizeof(float), nullptr);

		// shotgun
		int currentShotgunAmmo = 0;
		std::vector<unsigned int>shotgunOffset{ 0x134 };
		uintptr_t shotgunAmmoAddr = hack::FindDMAAddy(hProcess, dynamicPtrBaseAddr, shotgunOffset);
		ReadProcessMemory(hProcess, (BYTE*)shotgunAmmoAddr, &currentShotgunAmmo, sizeof(float), nullptr);

		// grenade
		int currentGrenadeAmmo = 0;
		std::vector<unsigned int>grenadeOffset{ 0x144 };
		uintptr_t grenadeAmmoAddr = hack::FindDMAAddy(hProcess, dynamicPtrBaseAddr, grenadeOffset);
		ReadProcessMemory(hProcess, (BYTE*)grenadeAmmoAddr, &currentGrenadeAmmo, sizeof(float), nullptr);

		auto HelpMarker = [](const char* desc)
		{
			ImGui::TextDisabled("(?)");
			if (ImGui::BeginItemTooltip())
			{
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted(desc);
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
		};

		static char currentNickname[32] = "NOT_INIT";
		static char nicknameBuf[32] = "*Click Refresh*";
		std::vector<unsigned int>nicknameOffset{ 0x205 };
		uintptr_t nicknameAddr = hack::FindDMAAddy(hProcess, dynamicPtrBaseAddr, nicknameOffset);
		ReadProcessMemory(hProcess, (BYTE*)nicknameAddr, &currentNickname, sizeof(currentNickname), nullptr);
		ImGui::Text("Current Nickname: %s", currentNickname);
		ImGui::InputText("Player Nickname", nicknameBuf, IM_COUNTOF(nicknameBuf));
		ImGui::SameLine(); HelpMarker("Infinite Ammo for all weapons.");
		if (ImGui::Button("Set Nickname"))
		{
			WriteProcessMemory(hProcess, (BYTE*)nicknameAddr, &nicknameBuf, sizeof(nicknameBuf), nullptr);
		}
		ImGui::SameLine();
		if (ImGui::Button("Refresh"))
		{
			ReadProcessMemory(hProcess, (BYTE*)nicknameAddr, &nicknameBuf, sizeof(nicknameBuf), nullptr);
		}

		ImGui::Spacing();
		ImGui::Checkbox("Infinite Ammo", &infiniteAmmo);
		if (infiniteAmmo)
		{
			WriteProcessMemory(hProcess, (BYTE*)assaultRifleAmmoAddr, &tripleNine, sizeof(tripleNine), nullptr);
			WriteProcessMemory(hProcess, (BYTE*)pistolAmmoAddr, &tripleNine, sizeof(tripleNine), nullptr);
			WriteProcessMemory(hProcess, (BYTE*)submachineAmmoAddr, &tripleNine, sizeof(tripleNine), nullptr);
			WriteProcessMemory(hProcess, (BYTE*)sniperAmmoAddr, &tripleNine, sizeof(tripleNine), nullptr);
			WriteProcessMemory(hProcess, (BYTE*)shotgunAmmoAddr, &tripleNine, sizeof(tripleNine), nullptr);
			WriteProcessMemory(hProcess, (BYTE*)grenadeAmmoAddr, &tripleNine, sizeof(tripleNine), nullptr);
		}

		static bool rapidFiring = false;
		ImGui::Checkbox("Rapid Firing", &rapidFiring);
		if (rapidFiring)
		{
			short int fast = 1;

			// assault rifle
			std::vector<unsigned int>assaultRifleRapidFireOffset{ 0x164 };
			WriteProcessMemory(
				hProcess, (BYTE*)hack::FindDMAAddy(hProcess, dynamicPtrBaseAddr, assaultRifleRapidFireOffset),
				&fast, sizeof(fast), nullptr
			);

			// sniper
			std::vector<unsigned int>sniperRapidFireOffset{ 0x160 };
			WriteProcessMemory(
				hProcess, (BYTE*)hack::FindDMAAddy(hProcess, dynamicPtrBaseAddr, sniperRapidFireOffset),
				&fast, sizeof(fast), nullptr
			);

			// shotgun
			std::vector<unsigned int>shotgunRapidFireOffset{ 0x158 };
			WriteProcessMemory(
				hProcess, (BYTE*)hack::FindDMAAddy(hProcess, dynamicPtrBaseAddr, shotgunRapidFireOffset),
				&fast, sizeof(fast), nullptr
			);
		}

		ReadProcessMemory(hProcess, (BYTE*)fovAddr, &currentFOV, sizeof(float), nullptr);
		ImGui::SliderFloat("FOV", &currentFOV, 20, 175);
		WriteProcessMemory(hProcess, (BYTE*)(moduleBase + 0x18A7CC), &currentFOV, sizeof(float), nullptr);

		ImGui::Spacing();
		ImGui::Text("Pistol Ammo: %d", currentPistolAmmo);
		ImGui::Text("Assault Rifle Ammo: %d", currentAssaultRifleAmmo);
		ImGui::Text("Submachine Ammo: %d", currentSubmachineAmmo);
		ImGui::Text("Sniper Ammo %d", currentSniperAmmo);
		ImGui::Text("Grenade Ammo %d", currentGrenadeAmmo);
		
	}
	else
	{
		ImGui::Text("Unable to find Assault Cube...");
	}
	// UI end
	CloseHandle(hProcess);
	ImGui::End();
}

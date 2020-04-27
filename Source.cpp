#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <shlobj.h>


#include "Drawing.h"
#include "SampApi.h"

// DIRECTX
#include <d3d9.h>
#include <d3dx9.h>
#include "dependencies/detours/detours.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


#define D3D_VFUNCTIONS 119			// the number of functions in the D3D9 Device's VTable
#define DEVICE_PTR 0xC97C28			// GTA SA d3d9 device pointer
#define ENDSCENE_INDEX 42			// the index of the endscene function in the VTable

#define THREAD_SLEEP_TIME 5
#define MOVEMENT_SPEED 5
#define DEFAULT_LEFT_POS 200
#define DEFAULT_TOP_POS 200

typedef HRESULT(WINAPI* oEndScene) (LPDIRECT3DDEVICE9 D3DDevice);
static oEndScene originalEndScene;	// remember the original endscene


static bool activated = true;
static bool running = true;
static bool movable = false;
static int leftPos = 200, topPos = 200, fontSize = 30;
HMODULE curDLLmodule;

static std::string curentStreet = "Unknown Street";
static std::string currentZone = "Unknown";
static std::string currentDirection = "";

HRESULT __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice) // Our hooked endscene
{
	__asm {
		pushad
		pushfd
	}

	if (!activated)
		return originalEndScene(pDevice);

	drawing::drawEndScene(pDevice, leftPos, topPos, fontSize, curentStreet, currentZone, currentDirection);


	__asm {
		popfd
		popad
	}
	return originalEndScene(pDevice);
}

void updateDirection()
{
	while (running)
	{
		if (activated)
			sampapi::GetDirection(currentDirection);
		Sleep(THREAD_SLEEP_TIME);
	}
}

void updateStreet()
{
	while (running)
	{
		if (activated)
			sampapi::GetStreetName(curentStreet);
		Sleep(THREAD_SLEEP_TIME);
	}
}

void updateZone()
{
	while (running)
	{
		if (activated)
			sampapi::GetZoneAndCity(currentZone);
		Sleep(THREAD_SLEEP_TIME);
	}
}

void saveConfig()
{
	static wchar_t path[MAX_PATH + 32];
	HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path);

	if (result != S_OK)
		return;

	wcscat(path, L"\\GTA San Andreas User Files\\streethud.data");

	std::ofstream out;
	out.open(path, std::ifstream::trunc);
	out << leftPos << ' ' << topPos;

	out.close();
}

void readConfig()
{
	static wchar_t path[MAX_PATH + 32];
	HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, path);
	
	if (result != S_OK)
		return;

	wcscat(path, L"\\GTA San Andreas User Files\\streethud.data");

	std::ifstream in;
	in.open(path);

	if (!in.is_open())
	{
		in.close();
		saveConfig();
		return;
	}

	in >> leftPos >> topPos;

	in.close();
}


DWORD WINAPI MainThread(LPVOID param)		// Where the magic happens
{
	while (!GetModuleHandleA("d3d9.dll"))	// Make sure the d3d9.dll is loaded in the GTA memory
	{
		Sleep(100);							// if not wait for it to get loaded
	}

	if (sampapi::Module::isSampLoaded())
	{
		sampapi::AddChatMessage::addChatMessage("{FCBA03}[LOCATIONMOD]{FFFFFF} LS-RP Streets and Locations HUD (by The Drunk)");
		sampapi::AddChatMessage::addChatMessage("{FCBA03}[LOCATIONMOD]{FFFFFF} Press F11 to terminate the MOD and unload the DLL");
		sampapi::AddChatMessage::addChatMessage("{FCBA03}[LOCATIONMOD]{FFFFFF} Press F10 to hide the HUD");
		sampapi::AddChatMessage::addChatMessage("{FCBA03}[LOCATIONMOD]{FFFFFF} Press F9 to reposition the HUD");
	}

	readConfig();

	// beginning stuff
	std::thread threadStreet(updateStreet);
	std::thread threadZone(updateZone);
	std::thread threadDir(updateDirection);


	//void** vTableDevice = *(void***)(*(DWORD*)DEVICE_PTR);
	void** vTableDevice = *reinterpret_cast<void***>(*reinterpret_cast<uintptr_t*>(DEVICE_PTR));

	// Replace the endscene with our hooked endscene
	originalEndScene = (oEndScene)DetourFunction((PBYTE)vTableDevice[ENDSCENE_INDEX], (PBYTE)Hooked_EndScene);

	while (true)
	{
		if (GetAsyncKeyState(VK_F9) & 1)
		{
			movable = !movable;

			if (sampapi::Module::isSampLoaded())
			{
				if (movable)
					sampapi::AddChatMessage::addChatMessage("{FCBA03}[LOCATIONMOD]{FFFFFF} USE NUM2, NUM4, NUM6 and NUM8 to move the hud, \
							when finished press F9");
				else
				{
					sampapi::AddChatMessage::addChatMessage("{FCBA03}[LOCATIONMOD]{FFFFFF} Saved");
					saveConfig();
				}
			}
			else {
				if (!movable)
					saveConfig();
			}
		}

		if (GetAsyncKeyState(VK_F10) & 1)	// menu on/off
		{
			activated = !activated;
			if (sampapi::Module::isSampLoaded())
				sampapi::AddChatMessage::addChatMessage("{FCBA03}[LOCATIONMOD]{FFFFFF} HUD hidden/shown");
		}

		if (GetAsyncKeyState(VK_F11) & 1)	// uninject and exit
		{
			running = false;
			DetourRemove(reinterpret_cast<PBYTE>(originalEndScene), reinterpret_cast<PBYTE>(Hooked_EndScene));

			if (movable)
				saveConfig();

			if (sampapi::Module::isSampLoaded())
				sampapi::AddChatMessage::addChatMessage("{FCBA03}[LOCATIONMOD]{FFFFFF} UNLOADING DLL");

			threadStreet.join();
			threadZone.join();
			threadDir.join();
			Sleep(500);
			FreeLibraryAndExitThread(curDLLmodule, 0);
		}

		// Moving
		if (movable)
		{
			if (GetAsyncKeyState(VK_NUMPAD2) & 1)	// menu on/off
			{
				topPos += MOVEMENT_SPEED;
			}
			else if (GetAsyncKeyState(VK_NUMPAD8) & 1)
			{
				topPos -= MOVEMENT_SPEED;
			}
			else if (GetAsyncKeyState(VK_NUMPAD4) & 1)
			{
				leftPos -= MOVEMENT_SPEED;
			}
			else if (GetAsyncKeyState(VK_NUMPAD6) & 1)
			{
				leftPos += MOVEMENT_SPEED;
			}
		}

		Sleep(10);
	}
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		// For debug only
		/*
		AllocConsole(); // Enables the console
		FILE* fDummy;
		freopen_s(&fDummy, "CONIN$", "r", stdin);
		freopen_s(&fDummy, "CONOUT$", "w", stderr);
		freopen_s(&fDummy, "CONOUT$", "w", stdout);
		std::cout << "Console opened!" << std::endl;

		*/
		curDLLmodule = hModule;
		CreateThread(0, 0, MainThread, hModule, 0, 0);
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
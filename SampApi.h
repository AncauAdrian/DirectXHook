#pragma once
#include <Windows.h>
#include <mutex>
#include "Pattern.h"
#include "Structs.h"
#include "PlayerPosition.h"
#include "Polygons.h"

namespace sampapi
{
	namespace Module
	{
		DWORD moduleBase(LPCTSTR file);
		DWORD moduleLength(HMODULE hModule);
		bool isSampLoaded();
	}

	namespace patterns
	{
		namespace AddChatMessage
		{
			static const BYTE* byteMask = (const BYTE*)"\x8B\x15\x00\x00\x00\x00\x68\x00\x00\x00\x00\x52\xE8\x00\x00\x00\x00\x83\xC4\x08\x5F\x5E";
			static const char* useMask = "xx????x????xx????xxxxx";
		}

		namespace Command
		{
			static const BYTE* byteMask = (const BYTE*)"\x64\xA1\x00\x00\x00\x00\x6A\xFF\x68\x00\x00\x00\x00\x50\xA1\x00\x00\x00\x00\x64\x89\x25\x00\x00\x00\x00\x81\xEC\x00\x00\x00\x00\x85\xC0";
			static const char* useMask = "xx????xxx????xx????xxx????xx????xx";
		}

		namespace Chat
		{
			static const BYTE* byteMask = (const BYTE*)"\x64\xA1\x00\x00\x00\x00\x6A\xFF\x68\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x81\xEC\x00\x00\x00\x00\x53\x56\x8B\xB4\x24\x00\x00\x00\x00\x8B\xC6";
			static const char* useMask = "xx????xxx????xxxx????xx????xxxxx????xx";
		}
	}

	namespace AddChatMessage {
		bool addChatMessage(const char* text);
	}

	int GetVehicleRotation(float& x, float& y, float& z);
	int GetDirection(std::string& dirRet);

	extern std::vector<Poly> polyStreet;
	extern std::mutex mtx_location;
	extern std::mutex mtx_street;
	extern std::mutex mtx_direction;
	int GetCityName(std::string& cityName);
	int GetKnownName(std::string& knownName);
	int GetZoneName(std::string& zoneName);
	int GetZoneAndCity(std::string& locationRet);
	int GetStreetName(std::string& streetNameRet);
}


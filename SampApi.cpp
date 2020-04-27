#include "SampApi.h"
#include <Psapi.h>

std::vector<Poly> sampapi::polyStreet;
std::mutex sampapi::mtx_location;
std::mutex sampapi::mtx_street;
std::mutex sampapi::mtx_direction;

DWORD sampapi::Module::moduleBase(LPCTSTR file)
{
	return (DWORD)GetModuleHandle(file);
}

DWORD sampapi::Module::moduleLength(HMODULE hModule)
{
	MODULEINFO info;
	GetModuleInformation(GetCurrentProcess(), hModule, &info, sizeof(info));
	return info.SizeOfImage;
}

bool sampapi::Module::isSampLoaded()
{
	return Module::moduleBase(L"samp.dll") == NULL ? 0 : 1;
}

bool sampapi::AddChatMessage::addChatMessage(const char* text)
{
	DWORD g_dwModuleLength = 0;
	DWORD g_dwModuleBase = 0;

	g_dwModuleBase = Module::moduleBase(L"samp.dll");
	g_dwModuleLength = Module::moduleLength((HMODULE)g_dwModuleBase);

	static auto addr = Pattern::findPattern(
		g_dwModuleBase,
		g_dwModuleLength,
		patterns::AddChatMessage::byteMask,
		patterns::AddChatMessage::useMask
	);

	if (addr == 0)
		return false;

	DWORD dwCallAddr = *(DWORD*)(addr + 0xD) + addr + 0xD + 0x4;
	DWORD dwInfo = *(DWORD*)(addr + 0x2);

	__asm {
		mov edx, dword ptr[dwInfo]
		mov eax, [edx]
		push text
		push eax
		call dwCallAddr
		add esp, 8
	}
	return true;
}

int sampapi::GetVehicleRotation(float& x, float& y, float& z)
{
	const uint currentVehicle = 0xBA18FC;

	if (*(uint*)currentVehicle == 0)
	{
		return 0;
	}
	
	uint step = *(uint*)currentVehicle;
	step += 0x14;
	step = *(uint*)step;

	x = *(float*)step;
	step += 0x4;
	y = *(float*)step;
	step += 0x4;
	z = *(float*)step;

	return 1;
}

int sampapi::GetDirection(std::string& dirRet)
{
	std::string dir = "";

	float x = 0, y = 0, z = 0;

	if (!sampapi::GetVehicleRotation(x, y, z))
	{
		mtx_direction.lock();
		dirRet = dir;
		mtx_direction.unlock();
		return 0;
	}

	float sqr = sqrt(2) / 2;

	if (x >= sqr)
		dir = "|N|";
	else if (x <= sqr * (-1))
		dir = "|S|";
	else if (y >= sqr)
		dir = "|W|";
	else if (y <= sqr * (-1))
		dir = "|E|";

	mtx_direction.lock();
	dirRet = dir;
	mtx_direction.unlock();

	return 1;

}

int sampapi::GetCityName(std::string& cityName)
{
	float x, y, z = 0.0f;
	PlayerPosition::getPosition(x, y, z);

	if (x == 0.0f && y == 0.0f && z == 0.0f)
		return 0;

	for (int i = 0; i < ARRAYSIZE(structs::cities); i++)
	{
		const structs::City& cCity = structs::cities[i];
		if (x > cCity.minX && y > cCity.minY && z > cCity.minZ && x < cCity.maxX && y < cCity.maxY && z < cCity.maxZ)
		{
			cityName = cCity.name;
			return 1;
		}
	}

	return 0;
}

int sampapi::GetKnownName(std::string& knownName)
{
	float x, y, z = 0.0f;
	PlayerPosition::getPosition(x, y, z);

	knownName = "";
	if (x == 0.0f && y == 0.0f && z == 0.0f)
		return 0;

	for (int i = 0; i < ARRAYSIZE(structs::known); i++)
	{
		const structs::Zone2D& cKnown = structs::known[i];
		if (x > cKnown.minX && y > cKnown.minY && x < cKnown.maxX && y < cKnown.maxY && z)
		{
			knownName = cKnown.name;
			return 1;
		}
	}

	return 0;
}

int sampapi::GetZoneName(std::string& zoneName)
{
	float x, y, z = 0.0f;
	PlayerPosition::getPosition(x, y, z);

	if (x == 0.0f && y == 0.0f && z == 0.0f)
		return 0;

	for (int i = 0; i < ARRAYSIZE(structs::zones); i++)
	{
		const structs::Zone& cZone = structs::zones[i];
		if (x > cZone.minX && y > cZone.minY && z > cZone.minZ && x < cZone.maxX && y < cZone.maxY && z < cZone.maxZ)
		{
			sampapi::GetKnownName(zoneName);
			if (zoneName.empty())
				zoneName = cZone.name;
			else
				zoneName += ", " + cZone.name;
			return 1;
		}
	}

	return 0;
}

int sampapi::GetZoneAndCity(std::string& locationRet)
{
	std::string zone, city;
	sampapi::GetCityName(city);
	sampapi::GetZoneName(zone);

	if (zone.empty() && city.empty())
		return 0;

	if (zone != city)
		zone += ", " + city;

	mtx_location.lock();
	locationRet = zone;
	mtx_location.unlock();

	return 1;
}

int sampapi::GetStreetName(std::string& streetNameRet)
{
	float x, y, z = 0.0f;
	PlayerPosition::getPosition(x, y, z);

	std::string streetName = "";

	if (polyStreet.empty())
		polyStreet = loadPolys();

	if (x == 0.0f && y == 0.0f && z == 0.0f)
		return 0;

	for (Poly p : polyStreet)
	{
		if (p.containsPoint(x, y))
		{
			if (streetName.empty())
				streetName = p.getName();
			else if (streetName != p.getName())
				streetName += " / " + p.getName();

		}
	}

	for (int i = 0; i < ARRAYSIZE(structs::street); i++)
	{
		const structs::Zone2D& cStreet = structs::street[i];
		if (x > cStreet.minX && y > cStreet.minY && x < cStreet.maxX && y < cStreet.maxY && z)
		{
			if (streetName.empty())
				streetName = cStreet.name;
			else if (streetName != cStreet.name)
				streetName += " / " + cStreet.name;

		}
	}

	if (streetName.empty())
	{
		streetName = "Unknown Street";
	}

	mtx_street.lock();
	streetNameRet = streetName;
	mtx_street.unlock();

	return 1;
}



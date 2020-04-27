#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

namespace drawing {
	void drawEndScene(IDirect3DDevice9* pDevice, int posLeft, int posTop, int fontSize, std::string street, std::string zone, std::string direction);
}
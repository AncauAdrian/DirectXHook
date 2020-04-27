#include "Drawing.h"
#include <cmath>

void drawing::drawEndScene(IDirect3DDevice9* pDevice, int posLeft, int posTop, int fontSize, std::string street, std::string zone, std::string direction)
{
	LPD3DXFONT m_font = nullptr;
	RECT rect;

	int dirFontSize = (int)round(fontSize * 2.5);

	D3DXCreateFont(pDevice, dirFontSize, 0, FW_EXTRALIGHT, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, MONO_FONT | FF_DONTCARE, TEXT("Lucida Sans Typewriter"), &m_font);

	SetRect(&rect, posLeft, posTop, posLeft + 1000, posTop + 1000);
	m_font->DrawTextA(NULL, direction.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(252, 186, 3));

	m_font->Release();
	D3DXCreateFont(pDevice, fontSize, 0, FW_LIGHT, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_font);

	SetRect(&rect, posLeft + (int)(dirFontSize * 1.5), posTop + 10, posLeft + 1000, posTop + 1000);
	m_font->DrawTextA(NULL, street.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(252, 186, 3));

	SetRect(&rect, posLeft + (int)(dirFontSize * 1.5), posTop + 10 + fontSize, posLeft + 1000, posTop + 1000);
	m_font->DrawTextA(NULL, zone.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));

	m_font->Release();
}

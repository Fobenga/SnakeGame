#pragma once
#include "Colors.h"
#include <string>
#include "ChiliWin.h"
#include "RectI.h"

class Surface
{
public:
	Surface(const std::wstring& filename);
	Surface(int width, int height);
	Surface(const Surface& rhs);
	~Surface();
	Surface& operator=(const Surface& rhs);
	void put_pixel(int x, int y, Color c);
	Color get_pixel(int x, int y) const;
	int get_width() const;
	int get_height() const;
	RectI get_rect() const;

private:
	Color * p_pixels = nullptr;
	int width;
	int height;
};

#include "Surface.h"
#include <fstream>
#include <assert.h>

Surface::Surface(const std::wstring & filename)
{
	std::ifstream file(filename, std::ios::binary);
	assert(file);

	BITMAPFILEHEADER bm_file_header;
	file.read(reinterpret_cast<char*> (&bm_file_header), sizeof bm_file_header);

	BITMAPINFOHEADER bm_info_header;
	file.read(reinterpret_cast<char*> (&bm_info_header), sizeof bm_info_header);

	assert(bm_info_header.biBitCount == 24 || bm_info_header.biBitCount == 32);
	assert(bm_info_header.biCompression == BI_RGB);

	const auto is_32_b = bm_info_header.biBitCount == 32;

	width = bm_info_header.biWidth;

	int y_start;
	int y_end;
	int dy;

	if (bm_info_header.biHeight < 0)
	{
		height = -bm_info_header.biHeight;
		y_start = 0;
		y_end = height;
		dy = 1;
	}
	else
	{
		height = bm_info_header.biHeight;
		y_start = height - 1;
		y_end = -1;
		dy = -1;
	}

	p_pixels = new Color[width*height];

	file.seekg(bm_file_header.bfOffBits);

	// padding is only for 24 bits
	const auto padding = (4 - (width * 3) % 4) % 4;
	for (auto y = y_start; y != y_end; y += dy)
	{
		for (auto x = 0; x < width; x++)
		{
			put_pixel(x, y, Color(file.get(), file.get(), file.get()));

			if (is_32_b)
				file.seekg(1, std::ios::cur);
		}

		if (!is_32_b)
			file.seekg(padding, std::ios::cur);
	}
}

Surface::Surface(int width, int height)
	:
	p_pixels(new Color[width*height]),
	width(width),
	height(height)
{}

Surface::Surface(const Surface & rhs)
	:
	Surface(rhs.width, rhs.height)
{
	const int n_pixels = width * height;
	for (int i = 0; i < n_pixels; ++i)
	{
		p_pixels[i] = rhs.p_pixels[i];
	}
}

Surface::~Surface()
{
	delete[] p_pixels;
	p_pixels = nullptr;
}

Surface & Surface::operator=(const Surface & rhs)
{
	width = rhs.width;
	height = rhs.height;

	delete[] p_pixels;

	p_pixels = new Color[width*height];

	const int n_pixels = width * height;
	for (int i = 0; i < n_pixels; ++i)
	{
		p_pixels[i] = rhs.p_pixels[i];
	}
	return *this;
}

void Surface::put_pixel(int x, int y, Color c)
{
	assert(x >= 0);
	assert(x < width);
	assert(y >= 0);
	assert(y < height);

	p_pixels[y * width + x] = c;
}

Color Surface::get_pixel(int x, int y) const
{
	assert(x >= 0);
	assert(x < width);
	assert(y >= 0);
	assert(y < height);

	return 	p_pixels[y * width + x];
}

int Surface::get_width() const
{
	return width;
}

int Surface::get_height() const
{
	return height;
}

RectI Surface::get_rect() const
{
	return { 0, width, 0, height };
}

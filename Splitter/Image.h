#pragma once
#include <cstdint>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

struct Color
{
	uint8_t r, g, b, a;
	Color()
		: Color(0, 0, 0, 255)
	{
	}
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		: r(r), g(g), b(b), a(a)
	{
	}
	Color(uint8_t r, uint8_t g, uint8_t b)
		: r(r), g(g), b(b), a(255)
	{
	}

	bool operator==(const Color& other) const
	{
		return r == other.r && g == other.g && b == other.b && a == other.a;
	}
	bool operator !=(const Color& other) const
	{
		return !operator==(other);
	}
};

class Image
{
private:
	uint8_t* data = nullptr;
	int width, height, channels;

public:
	Image(int width, int height, int channels = 4)
		: width(width), height(height), channels(channels)
	{
		data = new uint8_t[width * height * channels]{};
	}
	Image(const std::string& filename)
		: Image(filename.c_str())
	{
	}
	Image(const char* filename)
	{
		data = stbi_load(filename, &width, &height, &channels, 0);
		if (!data)
			throw std::exception("Failed to load image");
	}
	~Image()
	{
		stbi_image_free(data);
	}

	static Image Slice(const Image& image, int x, int y, int width, int height)
	{
		Image result(width, height);
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				result.SetPixel(j, i, image.GetPixel(x + j, y + i));
			}
		}
		return result;
	}

	int getWidth()		const { return width; }
	int getHeight()		const { return height; }
	int getChannels()	const { return channels; }

	void SaveAs(const char* filename) const
	{
		if (!stbi_write_png(filename, width, height, channels, data, width * channels))
			throw std::exception("Couldn't save file");
	}
	void SaveAs(const std::string& filename) const
	{
		SaveAs(filename.c_str());
	}

	void Fill(const Color& color) const
	{
		for (size_t y = 0; y < height; y++)
			for (size_t x = 0; x < width; x++)
				SetPixel(x, y, color);
	}

	const Color& GetPixel(int x, int y) const
	{
		int pos = (width * y + x) * channels;
		return Color(data[pos + 0], data[pos + 1], data[pos + 2], (channels == 3) ? 255 : data[pos + 3]);
	}

	void SetPixel(int x, int y, const Color& color) const
	{
		int pos = (width * y + x) * channels;
		data[pos + 0] = color.r;
		data[pos + 1] = color.g;
		data[pos + 2] = color.b;
		if (channels == 4)
			data[pos + 3] = color.a;
	}
};

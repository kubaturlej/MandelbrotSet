#pragma once
class Pixel
{

public:
	~Pixel() {};
	Pixel() {};
	Pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
		:R(r), G(g), B(b){};
	unsigned char R;
	unsigned char G;
	unsigned char B;
};


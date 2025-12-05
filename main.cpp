#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <fstream>

int main()
{
	int fontSize = 19; // magic number that looks good
	std::string name = "bulletinV1_font";

	int maxW = 0;
	int maxH = 0;

	sf::Font font;
	font.loadFromFile(name + ".ttf");
	font.setSmooth(false);
	for (int i = ' ' + 1; i < 127; i++)
	{
		if (isprint(i))
		{
			if (i == 'g')
			{
				int i = 0;
			}
			sf::Glyph g = font.getGlyph(i, fontSize, false);
			if (g.bounds.width > maxW)
			{
				maxW = g.bounds.width;
			}
			if (g.bounds.height + fontSize + g.bounds.top > maxH)
			{
				maxH = g.bounds.height + fontSize + g.bounds.top;
			}
		}
	}

	sf::Text text;
	text.setCharacterSize(fontSize);
	text.setFont(font);
	char str[127 - ' ' + 1] = { 0 };
	for (int i = ' '; i < 127; i++)
		str[i - ' '] = i;
	text.setString(str);


	sf::RenderTexture tex;
	tex.create((maxW + 1) * (127 - ' '), maxH);
	tex.draw(text);
	tex.display();
	sf::Image img = tex.getTexture().copyToImage();
	img.saveToFile(name + ".png");

	std::cout << maxW << " " <<	maxH << "\n";

	std::ofstream file(name + ".h");

	int stride = (maxW * maxH + 31) / 32;
	file << "#pragma once\n" <<
		"#include <stdint.h>\n" << 
		"uint32_t font_w = " << maxW << ";\n"
		"uint32_t font_h = " << maxH << ";\n"
		"uint32_t stride = " << stride << ";\n"
		"const uint32_t font[] = \n{\n";
	for (int i = 0; i < 127 - ' '; i++)
	{
		for (int y = 0; y < maxH; y++)
		{
			file << "    // |";
			for (int x = 0; x < maxW; x++)
				file << ((img.getPixel(i * (maxW + 1) + x, y).a >= 192) ? '#' : ' ');
			file << "|\n";
		}

		file << "    ";
		for (int y = 0; y < maxH; y++)
			for (int x = 0; x < maxW; x++)
			{
				if ((x + y * maxW) % 32 == 0)
					file << "0b";
				file << ((img.getPixel(i * (maxW + 1) + x, y).a >= 192) ? '1' : '0');
				if ((x + y * maxW) % 32 == 31 && !(x == maxW - 1 && y == maxH - 1))
					file << ",\n    ";
			}
		for (int ii = 0; ii < stride * 32 - (maxW * maxH); ii++)
		{
			file << '0';
		}
		if (i != 127 - ' ' - 1)
			file << ",\n    ";
		file << "\n";
	}
	file << "};";
	file.close();

	return 0;
}
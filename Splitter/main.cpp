#include <iostream>
#include <vector>
#include <filesystem>

#include "Image.h"

int findIndexOfElement(const std::vector<int>& vector, int element)
{
	for (int i = 0; i < vector.size(); i++)
		if (vector[i] == element)
			return i;

	return -1;
}

void Split(const std::string& input, const std::string& outputFolder = "", std::vector<int> combinedIndexes = {}, Color backgroundColor = {})
{
	if (!std::filesystem::exists(outputFolder))
	{
		if (!std::filesystem::create_directory(outputFolder))
		{
			std::cerr << "Failed to create folder: " << outputFolder << std::endl;
			return;
		}
	}

	Image image(input);

	bool hasCustomBackground = (backgroundColor != Color{});

	int start = -1;
	int end = -1;
	bool isSettingBegin = true;

	int combinedStart = -1; // (quite) a bit hacky but eh...
	bool hasMoreCombined = false;

	int count = 0;

	for (int x = 0; x < image.getWidth(); x++)
	{
		bool isStraightTransparentLine = true;

		for (int y = 0; y < image.getHeight(); y++)
		{
			if (!hasCustomBackground && image.GetPixel(x, y).a != 0 || 
				hasCustomBackground && image.GetPixel(x, y) != backgroundColor)
			{
				isStraightTransparentLine = false;

				if (isSettingBegin)
				{
					isSettingBegin = false;
					
					start = x;
				}
				
				break;
			}
		}

		if (isStraightTransparentLine && !isSettingBegin)
		{
			isSettingBegin = true;
			
			int index = findIndexOfElement(combinedIndexes, count);
			if (index != -1 && !hasMoreCombined)
			{
				combinedIndexes.erase(combinedIndexes.begin() + index);

				if (combinedStart == -1)
					combinedStart = start;

				if (findIndexOfElement(combinedIndexes, count) == -1)
					hasMoreCombined = true;

				continue;
			}
			
			end = x;
			std::string temp = outputFolder + std::to_string(count++) + std::string(".png");
			
			if (combinedStart == -1) // save normal letter
			{
				Image::Slice(image, start, 0, end - start, image.getHeight()).SaveAs(temp);
			}
			else // save combined letter
			{
				Image::Slice(image, combinedStart, 0, end - combinedStart, image.getHeight()).SaveAs(temp);
				combinedStart = -1;
				hasMoreCombined = false;
			}
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc == 1)
		return -1;

	std::vector<int> combinedIndexes;
	std::string outDir = "out/";
	std::string input = argc == 2 ? argv[1] : "";
	Color backgroundColor = {};

	for (int i = 1; i < argc - 1; i++)
	{
		if (strcmp(argv[i], "-i") == 0)
		{
			input = argv[i + 1];
			i++;
		}
		else if (strcmp(argv[i], "-o") == 0)
		{
			outDir = argv[i + 1];
			
			int last = strlen(argv[i + 1]) - 1;
			if (argv[i + 1][last] != '/' && argv[i + 1][last] != '\\')
				outDir += '/';
			i++;
		}
		else if (strcmp(argv[i], "-c") == 0)
		{
			int counter = 0;
			for (int j = i + 1; j < argc; j++)
			{
				if (!isdigit(argv[j][0]))
					break;
				// unsafe, but we trust the user!
				counter++;
				combinedIndexes.push_back(std::stoi(argv[j]));
			}
			i += counter;
		}
		else if (strcmp(argv[i], "-b3") == 0)
		{
			uint8_t channels[3];
			for (int j = 0; j < 3; j++)
			{
				if (i + 1 + j >= argc)
				{
					std::cerr << "Background argument is missing a channel/channels...";
					return -1;
				}

				channels[j] = std::stoi(argv[i + 1 + j]);
			}

			backgroundColor = {
				channels[0],
				channels[1],
				channels[2],
				255
			};
			i += 3;
		}
		else if (strcmp(argv[i], "-b4") == 0)
		{
			uint8_t channels[4];
			for (int j = 0; j < 4; j++)
			{
				if (i + 1 + j >= argc)
				{
					std::cerr << "Background argument is missing a channel/channels...";
					return -1;
				}

				channels[j] = std::stoi(argv[i + 1 + j]);
			}

			backgroundColor = {
				channels[0],
				channels[1],
				channels[2],
				channels[3],
			};
			i += 4;
		}

	}
	
	if (input == "")
	{
		std::cerr << "No input file path...";
		return -1;
	}

	try
	{
		Split(input, outDir, combinedIndexes, backgroundColor);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what();
	}
}
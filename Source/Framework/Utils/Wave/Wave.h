#pragma once

#include <string>


namespace GameDev2D
{
	//Forward declaration
	struct WaveData;

	//A class that provides conveniance methods to load PNG image data
	class Wave
	{
	public:
		static bool LoadFromPath(const std::string& path, WaveData** waveData);
	};
}
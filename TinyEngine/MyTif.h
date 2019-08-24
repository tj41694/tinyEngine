#pragma once
#include <tiff/tiffio.h>
#include <vector>

namespace TEngine {
	class Mesh;
	class MyTif {
	private:
		char* path = nullptr;
		TIFF* tif = nullptr;
		unsigned short* raster = nullptr; //从上之下，从左至右，取每一像素
	public:
		int width = -1;
		int height = -1;
	public:
		MyTif(const char* path_);
		//获取数据。排列：从上之下，从左至右
		const unsigned short* GetRasterData();
		void ClearRasterData();
		int GetBitsPerSample();
		~MyTif();
	};
}
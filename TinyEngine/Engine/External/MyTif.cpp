#include "MyTif.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>

namespace TEngine {
	MyTif::MyTif(const char* path_) {
		path = (char*)malloc(strlen(path_) + 1);
		if (path) {
			strcpy(path, path_);
		}
		tif = TIFFOpen(path, "r");										//使用TIFFOpen函数以只读形式打开图像。
		if (tif == nullptr) {
			std::cout << "读入图像路径错误,请重新确认";
			return;
		}
		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
	}

	const unsigned short* MyTif::GetRasterData() {
		if (path == nullptr || tif == nullptr || width == -1 || height == -1) { return nullptr; }
		if (raster == nullptr) {
			raster = (unsigned short*)malloc(width * height * sizeof(unsigned short));
			unsigned short* tmpPtr = raster;
			for (int r = 0; r < height; r++) {
				TIFFReadScanline(tif, tmpPtr, r);
				tmpPtr += width;
			}
		}
		return raster;
	}

	void MyTif::ClearRasterData() {
		if (raster != nullptr) {
			free(raster);
			raster = nullptr;
		}
	}

	int MyTif::GetBitsPerSample() {
		TIFFRGBAImage imgTiff = { 0 };
		char emsg[1024];
		if (TIFFRGBAImageBegin(&imgTiff, tif, 0, emsg)) {
			return imgTiff.bitspersample;
			TIFFRGBAImageEnd(&imgTiff);
		}
		return -1;
	}

	MyTif::~MyTif() {
		delete[]raster;
		TIFFClose(tif);
		free(path);
	}
}
#pragma once
#ifdef DLLEXPORT
#define DLLTYPE extern "C" __declspec(dllexport) 
#else
#define DLLTYPE extern "C" __declspec(dllimport) 
#endif
#include <windows.h>

DLLTYPE void _libInitCtrl(CWnd* parentWnd, int id);
DLLTYPE void _libOpenImage(const char* strpath);
DLLTYPE void _libSetImage(cv::Mat matImg);
//DLLTYPE void _libSetImage(unsigned char* buff, int width, int height, int channel);
//DLLTYPE void _libFinalCtrl();
DLLTYPE void _libOnSize(UINT Type);

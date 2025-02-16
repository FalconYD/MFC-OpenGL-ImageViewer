#pragma once
#ifdef DLLEXPORT
#define DLLTYPE extern "C" __declspec(dllexport) 
#else
#define DLLTYPE extern "C" __declspec(dllimport) 
#endif
#include <windows.h>

DLLTYPE void _libAddCtrl(int id, CWnd* parentWnd);
DLLTYPE void _libInitCtrl();
DLLTYPE void _libOpenImage(int id, const char* strpath);
DLLTYPE void _libSetImage(int id, cv::Mat matImg);
//DLLTYPE void _libSetImage(unsigned char* buff, int width, int height, int channel);
//DLLTYPE void _libFinalCtrl();
DLLTYPE void _libOnSize(int id, UINT Type);

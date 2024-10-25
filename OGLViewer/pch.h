// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "C:\Program Files (x86)\Visual Leak Detector\include\vld.h"

#include "framework.h"

#include <vector>
#include <stdarg.h>  // For va_start, etc.

#define GLEW_STATIC 1
#define GLFW_EXPOSE_NATIVE_WIN32

#include "glew/glew.h"
#include "glew/wglew.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ft2build.h"
#include FT_FREETYPE_H

#include "opencv2/opencv.hpp"


#ifdef _DEBUG
#pragma comment(lib, "../lib/opencv_world454d.lib")
#else
#pragma comment(lib, "../lib/opencv_world454.lib")
#endif

#pragma comment(lib,"../lib/freetype/freetype.lib")

#include "GLUtil.h"
#include "GLBase.h"

#pragma comment(lib,"../lib/glew/glew32s.lib")
#pragma comment(lib,"../lib/glfw/glfw3_mt.lib")
#pragma comment(lib, "opengl32.lib")

#include "ImageViewer.h"

#endif //PCH_H

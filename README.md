# OGLImageViewer
 MFC OpenGL Image Viewer. dll Lib형태로 작업하였으며, MFC에 Picture Frame에 Attach하는 방식.

## Done
	[x] Image Load
	[x] Mouse Position And Image Infomation

## Todo
	[ ] Shape Draw

## Library
	- freetype-2.13.3
	- glew-2.1.0
	- glfw-3.3.8
	- glm-1.0.1
	- opencv 4.5.4

## Directory Architecture
- $(SolutionDir)
	- Include
		- opencv
		- glm
		- glew
		- glfw
		- glm
  	-  Lib
		- freetype
		- glew
		- glfw
		- opencv_world454.dll
		- opencv_world454.lib
		- opencv_world454d.dll
		- opencv_world454d.lib
  - Bin

### ImageViewer : https://www.opengl-tutorial.org/
### Font :  https://learnopengl.com/In-Practice/Text-Rendering
### Shape :  https://stackoverflow.com/questions/3484260/opengl-line-width
// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "TextureShader.h"
#include "TexturedQuad.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void gui();

private:
	TextureShader* textureShader;
	TexturedQuad* mesh;
	TexturedQuad* mesh2;
	TexturedQuad* blendmesh;

	float mesh1rotation;
	float mesh2rotation;
};

#endif
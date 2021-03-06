// Lab1.cpp
// Lab 2 example, textured quad
#include "App1.h"

App1::App1()
{
	mesh = nullptr;
	textureShader = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	//textureMgr->loadTexture(L"brick", L"res/brick1.dds");
	textureMgr->loadTexture(L"mytexture", L"res/limmychin.png");
	textureMgr->loadTexture(L"mysecondtexture", L"res/limmybed.png");

	// Create Mesh object and shader object
	mesh = new TexturedQuad(renderer->getDevice(), renderer->getDeviceContext());
	mesh2 = new TexturedQuad(renderer->getDevice(), renderer->getDeviceContext());
	blendmesh = new TexturedQuad(renderer->getDevice(), renderer->getDeviceContext());
	textureShader = new TextureShader(renderer->getDevice(), hwnd);

	mesh1rotation = 0.0f;
	mesh2rotation = 0.0f;
}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (mesh)
	{
		delete mesh;
		mesh = 0;
	}
	if (mesh2)
	{
		delete mesh2;
		mesh2 = 0;
	}
	if (blendmesh)
	{
		delete blendmesh;
		blendmesh = 0;
	}

	if (textureShader)
	{
		delete textureShader;
		textureShader = 0;
	}
}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	// 6.28f is approx 2pi
	if (mesh1rotation < 6.28f)
		mesh1rotation += 0.1f;
	else
		mesh1rotation = 0.0f;

	return true;
}

bool App1::render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();


	// Rotate the world matrix
	worldMatrix = XMMatrixRotationZ(mesh1rotation);
	// Send geometry data, set shader parameters, render object with shader
	mesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"mytexture"));
	textureShader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	// Translate the world matrix
	worldMatrix = XMMatrixTranslation(3.0f, 0.0f, 0.0f);
	// Send geometry data, set shader parameters, render object with shader
	blendmesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"mytexture"), textureMgr->getTexture(L"mysecondtexture"));
	textureShader->renderBlend(renderer->getDeviceContext(), blendmesh->getIndexCount());


	// Rotate and translate the world matrix
	XMMATRIX rotation = XMMatrixRotationZ(mesh2rotation);
	XMMATRIX translation = XMMatrixTranslation(6.0f, 0.0f, 0.0f);
	worldMatrix = XMMatrixMultiply(rotation, translation);
	// Send geometry data, set shader parameters, render object with shader
	mesh2->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"mysecondtexture"));
	textureShader->render(renderer->getDeviceContext(), mesh2->getIndexCount());

	// Render GUI
	gui();

	// Swap the buffers
	renderer->endScene();

	return true;
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);
	ImGui::SliderFloat("Z Rotation: mesh2", &mesh2rotation, 0.0f, 6.28f);

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
	mesh = nullptr;
	shader = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	textureMgr->loadTexture(L"brick", L"res/brick1.dds");
	textureMgr->loadTexture(L"check", L"res/DefaultDiffuse.png");

	// Create Mesh object and shader object
	mesh = new SphereMesh(renderer->getDevice(), renderer->getDeviceContext());
	planeMesh = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext(), 100);
	shader = new LightShader(renderer->getDevice(), hwnd);
	light = new Light;
	light->setDiffuseColour(1.0f, 0.9f, 0.85f, 1.0f);
	light->setAmbientColour(0.2f, 0.0f, 0.0f, 1.0f);
	light->setPosition(50, 10, 50);
	light->setSpecularColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setSpecularPower(100.0f);
	light->setDirection(1.0f, 0.0f, 0.0f);
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
	if (planeMesh)
	{
		delete planeMesh;
		planeMesh = 0;
	}

	if (shader)
	{
		delete shader;
		shader = 0;
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

	// Send geometry data, set shader parameters, render object with shader
	mesh->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"brick"), light, camera->getPosition());
	shader->render(renderer->getDeviceContext(), mesh->getIndexCount());

	// Translate the world matrix to move the mesh
	worldMatrix = XMMatrixTranslation(0.0f, -10.0f, 0.0f);
	// Send geometry data, set shader parameters, render object with shader
	planeMesh->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"check"), light, camera->getPosition());
	shader->render(renderer->getDeviceContext(), planeMesh->getIndexCount());


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

	// Control light position
	float translation[3] = { light->getPosition().x, light->getPosition().y, light->getPosition().z };
	ImGui::SliderFloat3("Point light pos", translation, 0.0f, 100.0f);
	light->setPosition(translation[0], translation[1], translation[2]);

	// Control light colour
	float colour[4] = { light->getDiffuseColour().x, light->getDiffuseColour().y, light->getDiffuseColour().z, light->getDiffuseColour().w };
	ImGui::ColorEdit3("Point light colour", colour);
	light->setDiffuseColour(colour[0], colour[1], colour[2], colour[3]);

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


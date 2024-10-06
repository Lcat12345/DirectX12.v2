#include "stdafx.h"
#include "MyGame.h"
#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include <vector>

MyGame::MyGame(std::string name) : _name(name), _hWnd(nullptr)
{
}

void MyGame::Init(HWND hWnd)
{
	_hWnd = hWnd;

	SetWindowTextA(_hWnd, _name.c_str());

	_viewport = { 0, 0, static_cast<FLOAT>(FRAME_BUFFER_WIDTH), static_cast<FLOAT>(FRAME_BUFFER_HEIGHT), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);

	Device::Init();
	CommandQueue::Init();
	SwapChain::Init(_hWnd);
	RootSignature::Init();

	_shader.Init(L"shaders.hlsli");

	std::vector<Vertex> vec(3);
	vec[0].pos = Vec3(0.f, 0.5f, 0.5f);
	vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);
	vec[1].pos = Vec3(0.5f, -0.5f, 0.5f);
	vec[1].color = Vec4(0.f, 1.0f, 0.f, 1.f);
	vec[2].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);

	_mesh.Init(vec);
}

void MyGame::Render()
{
	RenderBegin();

	// TODO...
	_shader.Update();
	_mesh.Render();

	RenderEnd();
}

void MyGame::RenderBegin()
{
	CommandQueue::RenderBegin(&_viewport, &_scissorRect);
}

void MyGame::RenderEnd()
{
	CommandQueue::RenderEnd();
}

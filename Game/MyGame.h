#pragma once

#include "Shader.h"
#include "Mesh.h"

class MyGame
{
public:
	MyGame(std::string name);

	void Init(HWND hWnd);
	
	void Render();
	void RenderBegin();
	void RenderEnd();

private:
	Shader _shader;
	Mesh _mesh;

	std::string _name;
	HWND _hWnd;

	D3D12_VIEWPORT	_viewport = {};
	D3D12_RECT		_scissorRect = {};
};
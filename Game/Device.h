#pragma once

class Device
{
public:
	static void Init();

	static ComPtr<IDXGIFactory> GetDXGI() { return _dxgi; }
	static ComPtr<ID3D12Device> GetDevice() { return _device; }

private:
	static ComPtr<ID3D12Debug>		_debugController;

	static ComPtr<IDXGIFactory>		_dxgi;
	static ComPtr<ID3D12Device>		_device;
};


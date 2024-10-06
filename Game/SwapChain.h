#pragma once

class SwapChain
{
public:
	static void Init(HWND hWnd);
	static void Present();
	static void SwapIndex();

	static ComPtr<IDXGISwapChain> GetSwapChain() { return _swapChain; }
	static ComPtr<ID3D12Resource> GetBackRTVBuffer() { return _rtvBuffer[_backBufferIndex]; }
	static D3D12_CPU_DESCRIPTOR_HANDLE GetBackRTVCpuHandle() { return _rtvCpuHandle[_backBufferIndex]; }

	static D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCpuHandle() { return _dsvCpuHandle; }
	
private:
	static void CreateSwapChain(HWND hWnd);
	static void CreateRTV();
	static void CreateDSV();

private:
	static ComPtr<IDXGISwapChain> _swapChain;

	static ComPtr<ID3D12Resource> _rtvBuffer[SWAP_CHAIN_BUFFER_COUNT];
	static ComPtr<ID3D12DescriptorHeap> _rtvHeap;
	static D3D12_CPU_DESCRIPTOR_HANDLE _rtvCpuHandle[SWAP_CHAIN_BUFFER_COUNT];

	static UINT32 _backBufferIndex;

	// Depth Stencil View
	static ComPtr<ID3D12Resource>				_dsvBuffer;
	static ComPtr<ID3D12DescriptorHeap>			_dsvHeap;
	static D3D12_CPU_DESCRIPTOR_HANDLE			_dsvCpuHandle;
	static DXGI_FORMAT							_dsvFormat;
};


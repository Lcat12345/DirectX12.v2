#include "stdafx.h"
#include "SwapChain.h"
#include "Device.h"
#include "CommandQueue.h"

ComPtr<IDXGISwapChain> SwapChain::_swapChain;

ComPtr<ID3D12Resource> SwapChain::_rtvBuffer[SWAP_CHAIN_BUFFER_COUNT];
ComPtr<ID3D12DescriptorHeap> SwapChain::_rtvHeap;
D3D12_CPU_DESCRIPTOR_HANDLE SwapChain::_rtvCpuHandle[SWAP_CHAIN_BUFFER_COUNT];

UINT32 SwapChain::_backBufferIndex = 0;

ComPtr<ID3D12Resource>				SwapChain::_dsvBuffer;
ComPtr<ID3D12DescriptorHeap>		SwapChain::_dsvHeap;
D3D12_CPU_DESCRIPTOR_HANDLE			SwapChain::_dsvCpuHandle;
DXGI_FORMAT							SwapChain::_dsvFormat;

void SwapChain::Init(HWND hWnd)
{
	CreateSwapChain(hWnd);
	CreateRTV();
	CreateDSV();
}

void SwapChain::CreateSwapChain(HWND hWnd)
{
	// 이전에 만든 정보 날린다
	_swapChain.Reset();

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = static_cast<UINT32>(FRAME_BUFFER_WIDTH); // 버퍼의 해상도 너비
	sd.BufferDesc.Height = static_cast<UINT32>(FRAME_BUFFER_HEIGHT); // 버퍼의 해상도 높이
	sd.BufferDesc.RefreshRate.Numerator = 60; // 화면 갱신 비율
	sd.BufferDesc.RefreshRate.Denominator = 1; // 화면 갱신 비율
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 버퍼의 디스플레이 형식
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1; // 멀티 샘플링 OFF
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 후면 버퍼에 렌더링할 것 
	sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT; // 전면+후면 버퍼
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // 전면 후면 버퍼 교체 시 이전 프레임 정보 버림
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	

	Device::GetDXGI()->CreateSwapChain(CommandQueue::GetCmdQueue().Get(), &sd, &_swapChain);

	for (UINT32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
		_swapChain->GetBuffer(i, IID_PPV_ARGS(&_rtvBuffer[i]));
}

void SwapChain::CreateRTV()
{
	INT32 rtvHeapSize = Device::GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvDesc.NodeMask = 0;

	// 같은 종류의 데이터끼리 배열로 관리
	// RTV 목록 : [ ] [ ]
	Device::GetDevice()->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&_rtvHeap));

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = _rtvHeap->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		_rtvCpuHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * rtvHeapSize);
		Device::GetDevice()->CreateRenderTargetView(_rtvBuffer[i].Get(), nullptr, _rtvCpuHandle[i]);
	}
}

void SwapChain::CreateDSV()
{
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optimizedClearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	Device::GetDevice()->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&optimizedClearValue,
		IID_PPV_ARGS(&_dsvBuffer));

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	Device::GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&_dsvHeap));

	_dsvCpuHandle = _dsvHeap->GetCPUDescriptorHandleForHeapStart();
	Device::GetDevice()->CreateDepthStencilView(_dsvBuffer.Get(), nullptr, _dsvCpuHandle);
}

void SwapChain::Present()
{
	// Present the frame.
	_swapChain->Present(0, 0);
}

void SwapChain::SwapIndex()
{
	_backBufferIndex = (_backBufferIndex + 1) % SWAP_CHAIN_BUFFER_COUNT;
}

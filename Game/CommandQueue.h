#pragma once

class CommandQueue
{
public:
	static void Init();
	static void WaitSync();

	static void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	static void RenderEnd();

	static ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	static ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return _cmdList; }

private:
	static ComPtr<ID3D12CommandQueue>			_cmdQueue;
	static ComPtr<ID3D12CommandAllocator>		_cmdAllocator;
	static ComPtr<ID3D12GraphicsCommandList>	_cmdList;

	static ComPtr<ID3D12Fence>					_fence;
	static UINT32								_fenceValue;
	static HANDLE								_fenceEvent;
};


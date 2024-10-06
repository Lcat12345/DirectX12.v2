#pragma once

class RootSignature
{
public:
	static void Init();

	static ComPtr<ID3D12RootSignature> GetRootSignature() { return _signature; }

private:
	static ComPtr<ID3D12RootSignature>	_signature;
};


#pragma once
#include<wrl.h>
#include"Singleton.h"
#include<d3d12.h>
#include<string>
#include<vector>
#include<DirectXMath.h>
#include"imgui/imgui.h"
#include"imgui/imgui_impl_win32.h"
#include"imgui/imgui_impl_dx12.h"
#include"imgui/imgui_stdlib.h"

using namespace std;

class imguiCommon : public Singleton<imguiCommon>
{
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	friend Singleton<imguiCommon>;
	imguiCommon();
	vector<void(*)()>commandlist;
	ComPtr<ID3D12DescriptorHeap>heap;

public:
	~imguiCommon();
	void CommandExcute(bool Flag);
};


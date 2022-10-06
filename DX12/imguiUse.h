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

class imguiUse : public Singleton<imguiUse>
{
private:
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	friend Singleton<imguiUse>;
	imguiUse();
	//vector<void(*)()>commandlist;
	ComPtr<ID3D12DescriptorHeap>heap;

public:
	~imguiUse();
	void CommandExcute(bool Flag);
};


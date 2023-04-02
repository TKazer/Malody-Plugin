#include "OS-ImGui_Struct.h"
#include "OS-ImGui_Exception.hpp"
#include <iostream>
#include <string>
#include <functional>
#include <codecvt>
#include <vector>
#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")

/****************************************************
* Copyright (C)	: Liv
* @file			: OS-ImGui.h
* @author		: Liv
* @email		: 1319923129@qq.com
* @version		: 1.0
* @date			: 2023/4/2	13:16
****************************************************/

namespace OSImGui
{
	class D3DDevice
	{
	public:
		ID3D11Device* g_pd3dDevice = nullptr;
		ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
		IDXGISwapChain* g_pSwapChain = nullptr;
		ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
		bool CreateDeviceD3D(HWND hWnd);
		void CleanupDeviceD3D();
		void CreateRenderTarget();
		void CleanupRenderTarget();
	};

	static D3DDevice Device;

	enum WindowType
	{
		NEW,
		ATTACH
	};

	class WindowData
	{
	public:
		HWND  hWnd = NULL;
		HINSTANCE hInstance;
		std::string Name;
		std::wstring wName;
		std::string ClassName;
		std::wstring wClassName;
		Vec2 Pos;
		Vec2 Size;
		ImColor BgColor{ 255, 255, 255 };
	};

	class OSImGui_Base
	{
	private:
		// �ص�����
		std::function<void()> CallBackFn = nullptr;
		// �˳���ʶ
		bool EndFlag = false;
		// ��������
		WindowType Type = NEW;
	protected:
		// ��������
		WindowData Window;
		// Ŀ�괰������
		WindowData DestWindow;
	public:
		// ����һ���´���
		void NewWindow(std::string WindowName, Vec2 WindowSize, std::function<void()> CallBack);
		// ���ӵ���һ��������
		void AttachAnotherWindow(std::string DestWindowName, std::string DestWindowClassName, std::function<void()> CallBack);
		// �˳�
		void Quit();
	private:
		void MainLoop();
		bool CreateMyWindow();
		bool UpdateWindowData();
		bool InitImGui();
		void CleanImGui();
		bool PeekEndMessage();
		std::wstring StringToWstring(std::string& str);
	};

	class OSImGui : public OSImGui_Base, public Singleton<OSImGui>
	{
	public:
		// �ı�
		void Text(std::string Text, Vec2 Pos, ImColor Color, float FontSize = 15, bool KeepCenter = false);
		// ����ı�
		void StrokeText(std::string Text, Vec2 Pos, ImColor Color, float FontSize = 15, bool KeepCenter = false);
		// ����
		void Rectangle(Vec2 Pos, Vec2 Size, ImColor Color, float Thickness, float Rounding = 0);
		void RectangleFilled(Vec2 Pos, Vec2 Size, ImColor Color, float Rounding = 0, float Nums = 15);
		// ��
		void Line(Vec2 From, Vec2 To, ImColor Color, float Thickness);
		// Բ��
		void Circle(Vec2 Center, float Radius, ImColor Color,float Thickness, int Num = 50);
		void CircleFilled(Vec2 Center, float Radius, ImColor Color, int Num = 50);
		// Բ��
		void Arc(Vec2 Pos, float Radius, ImColor Color, float Angel, float Proportion, float Thickness);
		// ���ӵ�
		void ConnectPoints(std::vector<Vec2> Points, ImColor Color, float Thickness);
		// Բ��
		void Arc(ImVec2 Center, float Radius, ImColor Color, float Thickness, float Aangle_begin, float Angle_end, float Nums = 15);
		// ��ѡ��
		void MyCheckBox(const char* str_id, bool* v);
		void MyCheckBox2(const char* str_id, bool* v);
		void MyCheckBox3(const char* str_id, bool* v);
		void MyCheckBox4(const char* str_id, bool* v);
	};
}


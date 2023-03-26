#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_internal.h"
#include "../Singleton.hpp"
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxgi.lib")
#include <d3d11.h>

/****************************************************
* Copyright (C)	: Liv
* @file			: OS-ImGui_Struct.h
* @author		: Liv
* @email		: 1319923129@qq.com
* @version		: 1.0
* @date			: 2023/3/26	11:32
****************************************************/

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class Vec2
{
public:
	float x, y;
public:
	Vec2() :x(0.f), y(0.f) {}
	Vec2(float x_, float y_) :x(x_), y(y_) {}
	Vec2(ImVec2 ImVec2_) :x(ImVec2_.x), y(ImVec2_.y) {}
	Vec2 operator=(ImVec2 ImVec2_)
	{
		x = ImVec2_.x;
		y = ImVec2_.y;
		return *this;
	}
	Vec2& operator+(Vec2 Vec2_)
	{
		x += Vec2_.x;
		y += Vec2_.y;
		return *this;
	}
	Vec2& operator-(Vec2 Vec2_)
	{
		x -= Vec2_.x;
		y -= Vec2_.y;
		return *this;
	}
	Vec2& operator*(Vec2 Vec2_)
	{
		x *= Vec2_.x;
		y *= Vec2_.y;
		return *this;
	}
	Vec2& operator/(Vec2 Vec2_)
	{
		x /= Vec2_.x;
		y /= Vec2_.y;
		return *this;
	}
	bool operator==(Vec2 Vec2_)
	{
		return x == Vec2_.x && y == Vec2_.y;
	}
	bool operator!=(Vec2 Vec2_)
	{
		return x != Vec2_.x || y != Vec2_.y;
	}
	ImVec2 ToImVec2()
	{
		return ImVec2(x, y);
	}
};

class Vec3
{
public:
	float x, y, z;
public:
	Vec3() :x(0.f), y(0.f), z(0.f) {}
	Vec3(float x_, float y_, float z_) :x(x_), y(y_), z(z_) {}
	Vec3& operator+(Vec3 Vec3_)
	{
		x += Vec3_.x;
		y += Vec3_.y;
		z += Vec3_.z;
		return *this;
	}
	Vec3& operator-(Vec3 Vec3_)
	{
		x -= Vec3_.x;
		y -= Vec3_.y;
		z -= Vec3_.z;
		return *this;
	}
	Vec3& operator*(Vec3 Vec3_)
	{
		x *= Vec3_.x;
		y *= Vec3_.y;
		z *= Vec3_.z;
		return *this;
	}
	Vec3& operator/(Vec3 Vec3_)
	{
		x /= Vec3_.x;
		y /= Vec3_.y;
		z /= Vec3_.z;
		return *this;
	}
	bool operator==(Vec3 Vec3_)
	{
		return x == Vec3_.x && y == Vec3_.y && z == Vec3_.z;
	}
	bool operator!=(Vec3 Vec3_)
	{
		return x != Vec3_.x || y != Vec3_.y || z != Vec3_.z;
	}
};

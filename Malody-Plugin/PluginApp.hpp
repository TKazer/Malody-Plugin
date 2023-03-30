#pragma once
#include "OSImGui/OS-ImGui.h"
#include "MalodyReader/MalodyReader.hpp"
#include "KeyboardManager/Kps.hpp"
#include <iostream>
#include <Windows.h>
#include <deque>

class PluginApp
{
public:
	void Run()
	{
		std::cout << "Waiting for \"malody.exe\" running,[End] to stop." << std::endl;
		if (!WaitingForProgress())
			return;

		std::cout << "Attaching progress succeed." << std::endl;
		std::cout << "[Module]:" << Malody::MalodyReader::get().Process.ModuleAddress << std::endl;
		std::cout << "[Pid]:" << Malody::MalodyReader::get().Process.ProcessID << std::endl;
		std::cout << "[End] to quit." << std::endl;
		std::cout << "Running..." << std::endl;

		// 注: 此函数堵塞消息
		OSImGui::OSImGui::get().AttachAnotherWindow("Malody", "", PluginApp::DataLoop);
	}
private:
	bool WaitingForProgress()
	{
		while (true)
		{
			if (GetAsyncKeyState(VK_END))
				return false;
			if (Malody::MalodyReader::get().Init())
			{
				break;
			}
		}
		return true;
	}
	// 滚动字幕
	static void ScrollText(std::string Text, ImColor Color, Vec2 Pos, Vec2 Size, float FontSize)
	{
		static const float Speed = 20;
		static std::deque<Vec2> ScrollTextData;
		ImVec4 ClipRect{ Pos.x,Pos.y,Pos.x + Size.x,Pos.y + Size.y };
		float TextWidth = ImGui::GetFont()->CalcTextSizeA(FontSize, FLT_MAX, 0.f, Text.c_str()).x;
		if (ScrollTextData.size() == 0)
		{
			ScrollTextData.push_back({ 0,0 });
		}
		if (ScrollTextData[0].x + TextWidth < (Size.x / 4 * 3)
			&& ScrollTextData.size() < 2)
		{
			ScrollTextData.push_back({ Size.x,0 });
		}
		if (ScrollTextData[0].x < -TextWidth)
		{
			ScrollTextData.pop_front();
		}
		for (auto& ScrollText : ScrollTextData)
		{
			ImGui::GetForegroundDrawList()->AddText(ImGui::GetFont(), FontSize, { ScrollText.x + Pos.x,ScrollText.y + Pos.y }, Color, Text.c_str(), nullptr, NULL, &ClipRect);
			ScrollText.x -= ImGui::GetIO().DeltaTime * Speed;
		}
	}

	// 圆角标签
	static void RoundingLeble(std::string Head, std::string Value, ImColor Color, ImVec2 Pos, ImVec2 Size, float FontSize)
	{
		float ValueTextSize = ImGui::GetFont()->CalcTextSizeA(FontSize, FLT_MAX, 0.f, Value.c_str()).x;
		ImVec2 R2 = { Pos.x + Size.x - ValueTextSize - 8,Pos.y + (float)2.5 };

		OSImGui::OSImGui::get().RectangleFilled(Pos, Size, Color, 20, 20);
		OSImGui::OSImGui::get().RectangleFilled(R2, { ValueTextSize + 5, Size.y - 5 }, ImColor(10, 10, 10, 230), 20, 20);
		OSImGui::OSImGui::get().Text(Head, { Pos.x + 15,Pos.y + Size.y / 2 - FontSize / 2 }, ImColor(10, 10, 10,255), FontSize);
		OSImGui::OSImGui::get().Text(Value, { R2.x + (float)3,Pos.y + Size.y / 2 - FontSize / 2 }, ImColor(255, 255, 255,255), FontSize);
	}

	// 主线程循环
	static void DataLoop()
	{
		Malody::GameStatus GameStatus;
		Malody::MapData MapData;
		static KpsApp KpsManager;
		
		if (GetAsyncKeyState(VK_END))
		{
			OSImGui::OSImGui::get().Quit();
			return;
		}

		// 获取Map数据
		if (!Malody::MalodyReader::get().Map.GetData(MapData))
			return;
		// 获取游戏状态
		if (!Malody::MalodyReader::get().Game.GetGameStatus(GameStatus))
			return;

		// 仅在游戏中或结算界面显示窗口
		if (GameStatus == Malody::STATUS_GAMING ||
			GameStatus == Malody::STATUS_RESULT)
		{
			Malody::ResultData Data;
			// 获取游戏 数据
			if (GameStatus == Malody::STATUS_GAMING)
			{
				if (!Malody::MalodyReader::get().DataInGame.GetData(Data))
					return;
			}
			else
			{
				if (!Malody::MalodyReader::get().DataInResult.GetData(Data))
					return;
			}

			// 菜单
			static bool Init = false;
			static ImVec2 MenuSize{ 360,190 };
			static ImVec2 LebleSize{ 130,23 };

			ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
			{
				ImVec2 MenuPos = ImGui::GetWindowPos();
				std::string MM_Value = "0.0";

				if (!Init) {
					// 初始化设置
					ImGui::GetStyle().WindowRounding = 9;
					ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImColor(30, 30, 30, 230);
					ImGui::SetWindowSize(MenuSize);
					KpsManager.AddKey({0,1,2,3});
					Init = true;
				}
				if (MapData.Level > 0)
				{
					float MM = Malody::Algorithm::CalcMM(Data, MapData.Level);
					char Buffer[50]{};
					sprintf_s(Buffer,50, "%0.1f", MM);
					MM_Value = Buffer;
				}
				// 滚动字幕
				ScrollText(MapData.SongName,ImColor(255, 255, 255), { MenuPos.x + 40,MenuPos.y + 20 }, { MenuSize.x - 80,23 }, 23);
				// 标签
				RoundingLeble("Kps", std::to_string(KpsManager.GetCurrentKps()), ImColor(63, 204, 246), {MenuPos.x + 40,MenuPos.y + 60}, LebleSize, 18);
				RoundingLeble("MaxKps", std::to_string(KpsManager.GetMaxKps()), ImColor(63, 204, 246), { MenuPos.x + MenuSize.x - LebleSize.x - 40 ,MenuPos.y + 60 }, LebleSize, 18);
				RoundingLeble("MM", MM_Value, ImColor(255, 106, 122), {MenuPos.x + 40,MenuPos.y + 90}, LebleSize, 18);
				// 分割线
				ImGui::GetForegroundDrawList()->AddLine({ MenuPos.x,MenuPos.y + 130 }, { MenuPos.x + MenuSize.x,MenuPos.y + 130 }, ImColor(60, 60, 60, 245), 2);
				// 打击数据
				OSImGui::OSImGui::get().Text("Best", { MenuPos.x + 40,MenuPos.y + 140 }, ImColor(234, 131, 131), 18);
				OSImGui::OSImGui::get().Text(std::to_string(Data.Hit.Best), { MenuPos.x + 55,MenuPos.y + 160 }, ImColor(240, 240, 240), 17, true);

				OSImGui::OSImGui::get().Text("Cool",{ MenuPos.x + 120,MenuPos.y + 140 }, ImColor(240, 184, 96), 18);
				OSImGui::OSImGui::get().Text(std::to_string(Data.Hit.Cool), { MenuPos.x + 135,MenuPos.y + 160 },  ImColor(240, 240, 240), 17, true);

				OSImGui::OSImGui::get().Text("Great", { MenuPos.x + 200,MenuPos.y + 140 }, ImColor(128, 229, 137), 18);
				OSImGui::OSImGui::get().Text(std::to_string(Data.Hit.Great), { MenuPos.x + 218,MenuPos.y + 160 }, ImColor(240, 240, 240), 17, true);

				OSImGui::OSImGui::get().Text("Miss", { MenuPos.x + 290,MenuPos.y + 140 }, ImColor(151, 151, 151),18);
				OSImGui::OSImGui::get().Text(std::to_string(Data.Hit.Miss), { MenuPos.x + 305,MenuPos.y + 160 },  ImColor(240, 240, 240), 17, true);
			}ImGui::End();
		}
		else
		{
			// 非游玩状态
			if (KpsManager.GetMaxKps() != 0)
			{
				KpsManager.ResetMaxKps();
			}
			if (KpsManager.GetCounts() != 0)
			{
				KpsManager.ResetCounts();
			}
		}
	}
};
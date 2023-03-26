#pragma once
#include "OSImGui/OS-ImGui.h"
#include "MalodyReader/MalodyReader.hpp"
#include <iostream>
#include <Windows.h>

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

	static void DataLoop()
	{
		Malody::GameStatus GameStatus;
		Malody::MapData MapData;

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

			ImGui::Begin("Malody", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
			{
				float MM = 0.f;
				if (MapData.Level != 0)
				{
					MM = Malody::Algorithm::CalcMM(Data, MapData.Level);
				}
				ImGui::Text("MM: %.2f", MM);

				ImGui::TextColored(ImColor(234, 131, 131, 255), "Best:  %d",Data.Hit.Best);

				ImGui::TextColored(ImColor(240, 184, 96, 255), "Cool:  %d", Data.Hit.Cool);

				ImGui::TextColored(ImColor(128, 229, 137, 255), "Great: %d", Data.Hit.Great);

				ImGui::TextColored(ImColor(151, 151, 151, 255), "Miss:  %d", Data.Hit.Miss);
			}ImGui::End();
		}
	}
};
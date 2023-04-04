#pragma once
#include "KeyBoard_Base.hpp"
#include "../MalodyReader/MalodyReader.hpp"

namespace KeyBoardManager
{
	class KeyBoard_Memory : public _keyboard_base
	{
	private:
		std::thread ListenThread;
		bool ExitThread = false;
	public:
		void StartListening()
		{
			this->ListenThread = std::thread{ &KeyBoard_Memory::_listen_thread, this };
			this->ListenThread.detach();
		}
		void StopListening()
		{
			this->ExitThread = true;
		}
		void _listen_thread()
		{
			std::vector<int> InKeys{ 0x1,0x100,0x10000,0x1000000 };
			while (true)
			{
				if (ExitThread)
					break;
				DWORD KeyAddress = Malody::MalodyReader::get().Process.TraceAddress(Malody::MalodyReader::get().Process.ModuleAddress + 0x8311A8, { 0x228,0x25C,0x4B0 });
				int KeyStatus = 0;
				if (!Malody::MalodyReader::get().Process.ReadMemory<int>(KeyAddress, KeyStatus))
					continue;
				for (int i = 0; i < InKeys.size(); i++)
				{
					if ((InKeys[i] & KeyStatus ) == InKeys[i])
					{
						this->_key_down(i, std::chrono::steady_clock::now());
					}
					else
					{
						this->_key_up(i, std::chrono::steady_clock::now());
					}
				}
			}
		}
	};
}

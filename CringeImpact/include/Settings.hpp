#pragma once
#include <string>
#include <list>
#include <imgui.h>
#include <imgui-sfml.h>
#include <imgui_stdlib.h>

class Settings
{
public:
	static bool is_console_shown;
	static bool is_game_runing;
private:
	Settings() {};
};
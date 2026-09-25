#include "Drawing.h"
#include "gamestate.hpp"

LPCSTR Drawing::lpWindowName = "Yamagi Quake II Trainer";
ImVec2 Drawing::vWindowSize = { 640, 480 };
ImGuiWindowFlags Drawing::WindowFlags = 0;
bool Drawing::bDraw = true;

void Drawing::Active()
{
	bDraw = true;
}

bool Drawing::isActive()
{
	return bDraw == true;
}

void Drawing::Draw()
{
	if (isActive())
	{
		ImGui::SetNextWindowSize(vWindowSize, ImGuiCond_Once);
		ImGui::SetNextWindowBgAlpha(1.0f);
		if (ImGui::Begin(lpWindowName, &bDraw))
		{
			if (ImGui::BeginTabBar("TabBarOne"))
			{
				if (ImGui::BeginTabItem("Player"))
				{
					ImGui::Text("Player info and configurations.");

					// level information
					ImGui::SeparatorText("Level Information");
					ImGui::Text("Map Name: base1");

					// player information
					ImGui::SeparatorText("Player Information");
					ImGui::Text("Player Name: Bitterman");
					ImGui::Text("Player Health: %d%", g_state.playerHealth);
					ImGui::Text(
						"Player Position [%0.00f, %0.00f, %0.00f]",
						g_state.playerCoords[g_state.Coord::X],
						g_state.playerCoords[g_state.Coord::Y],
						g_state.playerCoords[g_state.Coord::Z]
					);

					// Hacks
					ImGui::SeparatorText("Hacks");
					ImGui::Checkbox("God Mode", &g_state.godmode);

					// Settings
					ImGui::SeparatorText("Settings");
					static int playerFOV = 90;
					ImGui::SliderInt("FOV", &playerFOV, 40, 120);

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Inventory"))
				{
					ImGui::Text("Manage items in inventory.");

					// ammo
					ImGui::SeparatorText("Ammo");
					ImGui::Text("Shells: %d", g_state.shells);
					ImGui::Text("Bullets: %d", g_state.bullets);
					ImGui::Text("Grenades: %d", g_state.grenades);
					ImGui::Text("Rockets: %d", g_state.rockets);
					ImGui::Text("Slugs: %d", g_state.slugs);
					ImGui::Text("Cells: %d", g_state.cells);

					ImGui::Spacing();
					ImGui::Checkbox("Infinite Ammo", &g_state.infiniteAmmo);

					// items
					ImGui::SeparatorText("Items");
					ImGui::Checkbox("Quad Damage", &g_state.quadDamage);
					ImGui::Checkbox("Invulnerability", &g_state.invulnuerability);
					ImGui::Checkbox("Silencer", &g_state.silencer);
					ImGui::Checkbox("Rebreather", &g_state.rebreather);
					ImGui::Checkbox("EnvironmentSuit", &g_state.environmentSuit);

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

			ImGui::SetCursorPosY(
				ImGui::GetWindowSize().y -
				ImGui::GetTextLineHeightWithSpacing() -
				ImGui::GetStyle().WindowPadding.y
			);
			ImGui::Text("trainer by evenblad3");
		}
		ImGui::End();
	}

	#ifdef _WINDLL
	if (GetAsyncKeyState(VK_INSERT) & 1)
		bDraw = !bDraw;
	#endif
}

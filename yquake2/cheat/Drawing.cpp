#include "Drawing.h"

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
					ImGui::Text("Player Name: Bitterman");
					
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
					static int shells = 95;
					ImGui::Text("Shells: %d", shells);
					static int bullets  = 96;
					ImGui::Text("Bullets: %d", bullets);
					static int grenades = 97;
					ImGui::Text("Grenades: %d", grenades);
					static int rockets  = 98;
					ImGui::Text("Rockets: %d", rockets);
					static int slugs    = 99;
					ImGui::Text("Slugs: %d", slugs);
					static int cells    = 100;
					ImGui::Text("Cells: %d", cells);

					static bool infiniteAmmo = false;
					ImGui::Checkbox("Infinite Ammo", &infiniteAmmo);

					// items
					ImGui::SeparatorText("Items");
					static bool hasQuadDamage = false;
					ImGui::Checkbox("Quad Damage", &hasQuadDamage);
					
					static bool hasInvulnerability = false;
					ImGui::Checkbox("Invulnerability", &hasInvulnerability);

					static bool hasSilencer = false;
					ImGui::Checkbox("Silencer", &hasSilencer);

					static bool hasRebreather = false;
					ImGui::Checkbox("Rebreather", &hasRebreather);
					
					static bool hasEnvironmentSuit = false;
					ImGui::Checkbox("EnvironmentSuit", &hasEnvironmentSuit);

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

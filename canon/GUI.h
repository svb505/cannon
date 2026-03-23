#pragma once
#include <vector>
#include <string>
#include "imgui.h"
#include "canon.h"
#include "shell.h"
#include "lightning.h"
#include "environnement.h"
#include <cmath>

extern int level;

class GUI {
private:
	std::vector<std::string> levels = { "1", "2" };
	std::vector<const char*> cstrs;
	int currentIndex = 0;
	std::string control = "\nW - Raise gun\nS - Lower gun\nA - Gun to left\nD - Gun to right"
		"\nSPACE - Shot\n< - Turn cam to left\n> - Turn cam to right\n^ - Turn cam to up\n"
		"v - Turn cam to down\nNPAD 7 - Camera to forward\nNPAD 9 - Camera to back"
		"\nNPAD 6 - Camera to right\nNPAD 4 - Camera to left\nNPAD 8 - Camera to up\nNPAD 2 - Camera to down";
public:
	GUI() {
		for (auto& s : levels)
			cstrs.push_back(s.c_str());
	}
	void drawGui(int score,float reloadTime,float fps, Cannon& cn,Light& l, Target& trg) {
		float yawRad = cn.cannonYaw * 3.1415926f / 180.0f;
		float pitchRad = cn.cannonPitch * 3.1415926f / 180.0f;
		float L = cn.barrelLength;
		float yTop = cn.baseHeight + L * sin(pitchRad);
		auto hit = computeImpact({ 0.0f,yTop,-5.0f }, shellSpeed, cn.cannonPitch, cn.cannonYaw);

		ImGui::Begin("Settings & Info");

		ImGui::Text("Gun pitch: %.1f", cn.cannonPitch);
		ImGui::Text("Gun yaw: %.1f", cn.cannonYaw);
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		ImGui::SliderFloat("Shell speed", &shellSpeed, 10.0f, 50.0f);
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		ImGui::Text("Distance to target: %.1f", sqrt(pow(trg.getCoordinates()[0],2) + 
			pow(trg.getCoordinates()[1],2) + pow(trg.getCoordinates()[2], 2)));
		ImGui::Text("Target position:(%.1f;%.1f;%.1f)", trg.getCoordinates()[0], trg.getCoordinates()[1],
			-trg.getCoordinates()[2]);
		ImGui::Text("Approximate coordinates of impact: (%.1f;%.1f;%.1f)",hit.x,hit.y,-hit.z);
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		ImGui::Checkbox("Enable light", &l.enabled);
		ImGui::Checkbox("Enable grid text", &gridTextEnadled);
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		ImGui::Text("Score: %d", score);
		ImGui::Text("Reload: %.1f", reloadTime);
		ImGui::Text("FPS: %.0f", fps);
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		if (ImGui::Combo("Select level", &currentIndex, cstrs.data(), cstrs.size())) level = std::stoi(levels[currentIndex]);
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		ImGui::Text("Control: %s", control.c_str());

		ImGui::End();
	}
};
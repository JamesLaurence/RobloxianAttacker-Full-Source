#pragma once
#include <no_renderer/FW1FontWrapper/FW1FontWrapper.h>
#include <no_renderer/no_menu/no_menu.h>
#include <no_renderer/no_gui/no_gui.h>

static int tab = 0;

bool tab1 = false;
bool tab2 = false;
bool tab3 = false;
bool tab4 = false;

std::vector<std::string> AimbotBone = {
	_("Head"),
	_("Neck"),
	_("Pelvis"),
	_("Bottom")
};

namespace Settings
{
	bool Aimbot = false;
	bool NameESP = false;
	bool ShowFovCircle = true;
	bool RadarESP = true;
	int AimbotFovCircle = 100;
	int AimbotSmooth = 3.0f;
	static int AimbotBone_Value = 0;
	bool AimbotPrediction = false;
	bool ESP = false;
	static int ESPType_Value = 1;
	bool ESP_Skeleton = true;
	bool ESP_Lines = false;
	bool ESP_AimingLines = false;
	bool ESP_Llama = false;
	bool ESP_Vehicle = false;
	bool Variable_For_test = false;
	bool NoAnimation = false;
	bool AimWhileJumping = false;
	bool InstantRevive = false;
	bool NoSpread = false;
	bool TriggerBot = false;
	bool VisibleCheck = false;
	bool RapidFire = false;
	bool AirStuck = false;
	bool Crosshair = false;
	bool DebugForTest = false;
}

namespace UI_Help {
	namespace AimbotTab {
		void Aimbot() {
			no_menu::SetWidgetPosition(129, 58);

			if (no_menu::TextHoverable(_("(?)"), no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox("[Memory Aimbot] Locks your aim to the player");
			}

			no_menu::SetWidgetPosition(8, 75);
		}

		void AimbotFov() {
			no_menu::SetWidgetPosition(167, 96);

			if (no_menu::TextHoverable(_("(?)"), no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox(_("[Aimbot Fov] The size of the circle where the Aimbot should lock"));
			}

			no_menu::SetWidgetPosition(8, 120);
		}

		void AimbotSmooth() {
			no_menu::SetWidgetPosition(167, 140);

			if (no_menu::TextHoverable(_("(?)"), no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox(_("[Aimbot Smooth] The time that the camera takes to lock the player"));
			}

			no_menu::SetWidgetPosition(8, 164);
		}

		void AimbotBone() {
			no_menu::SetWidgetPosition(118, 183);

			if (no_menu::TextHoverable(_("(?)"), no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox(_("[Aimbot Bone] You can choose on what bone the Aimbot should lock"));
			}

			no_menu::SetWidgetPosition(8, 214);
		}

		void AimbotPrediction() {
			no_menu::SetWidgetPosition(159, 222);

			if (no_menu::TextHoverable(_("(?)"), no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox(_("[Aimbot Prediction BETA] It predicts the player movement"));
			}
		}
	}


	namespace ESPTab {
		void ESP() {
			no_menu::SetWidgetPosition(107, 58);

			if (no_menu::TextHoverable(_("(?)"), no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox(_("[ESP] You can see Players, Objects, Loot through walls"));
			}

			no_menu::SetWidgetPosition(8, 78);
		}

		void SKELETON() {
			no_menu::SetWidgetPosition(77, 81);

			if (no_menu::TextHoverable(_("(?)"), no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox(_("[SKELETON] You can see Players Bones, Movement, etc"));
			}

			no_menu::SetWidgetPosition(8, 103);
		}
	}

	namespace ExploitsTab {
		void AimWhileJumping() {
			no_menu::SetWidgetPosition(154, 59);

			if (no_menu::TextHoverable(_("(?)"), no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox(_("[Aim While Jumping] You can Aim while you are jumping"));
			}

			no_menu::SetWidgetPosition(8, 74);
		}

		void NoAnimations() {
			no_menu::SetWidgetPosition(124, 82);

			if (no_menu::TextHoverable(_("(?)"), no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox(_("[No Animation] It skips your switch weapon animation"));
			}

			no_menu::SetWidgetPosition(8, 99);
		}

		void NoSpread() {
			no_menu::SetWidgetPosition(96, 107);

			if (no_menu::TextHoverable(_("(?)"), no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox(_("[No Spread] It reduces the bullet spread on every weapon"));
			}

			no_menu::SetWidgetPosition(8, 126);
		}

		void RapidFire() {
			no_menu::SetWidgetPosition(102, 134);

			if (no_menu::TextHoverable(_("(?)"), no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox(_("[Rapid Fire] It increase the fire rate of some weapons"));
			}

			no_menu::SetWidgetPosition(8, 153);


		}

		void TriggerBot() {
			no_menu::SetWidgetPosition(110, 161);

			if (no_menu::TextHoverable(_("(?)"), no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox(_("[Trigger Bot] Automatically shoot when a player is visible"));
			}

			no_menu::SetWidgetPosition(8, 181);

		}

		void AirStuck() {
			no_menu::SetWidgetPosition(88, 189);

			if (no_menu::TextHoverable(_("(?)"), no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox(_("[AirStuck] Your player will be frozen and stuck in air when pressing ALT"));
			}

			no_menu::SetWidgetPosition(8, 208);
		}

		void InstantRevive() {
			no_menu::SetWidgetPosition(129, 243);

			if (no_menu::TextHoverable(_("(?)"), no_menu::color({ 255, 231, 94, 230 }))) {
				no_menu::MiniBox(_("[Instant Revive] It sets the revive time to 0 for your team"));
			}
		}
	}

}
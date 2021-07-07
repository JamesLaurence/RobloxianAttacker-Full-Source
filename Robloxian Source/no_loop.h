#pragma once
#include <UnrealClass.h>
#include <UnrealStructs.h>
#include <no_renderer/FW1FontWrapper/FW1FontWrapper.h>
#include <no_renderer/no_menu/no_menu.h>
#include <no_renderer/no_gui/no_gui.h>

no_gui gui;

IFW1Factory* pFW1Factory = NULL;
IFW1FontWrapper* pFontWrapper = NULL;

ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;

float X;
float Y;

namespace no_loop
{
	SDK::Structs::FBox GetFBox(SDK::Classes::AActor* Actor)
	{
		if (!Actor) return {};

		static uintptr_t addr = 0;
		if (!addr)
		{
			addr = sigscan(Fortnite, _("E8 ? ? ? ? F2 0F 10 45 ? B3 01"));
			addr = (uintptr_t)RVA(addr, 5);
		}

		SDK::Structs::Vector3 Origin, BoxExtend;

		auto fGetActorBounds = reinterpret_cast<void(__fastcall*)(SDK::Classes::AActor*, char, SDK::Structs::Vector3*, SDK::Structs::Vector3*)> (addr);

		SpoofCall(fGetActorBounds, Actor, (char)true, &Origin, &BoxExtend);

		SDK::Structs::FBox NewBox;
		NewBox.IsValid = 1;
		NewBox.Min = Origin - BoxExtend;
		NewBox.Max = Origin + BoxExtend;

		return NewBox;
	}

	bool MainLoop()
	{
		std::vector<SDK::Classes::AActor*> PlayerPawns;
		std::vector<SDK::Classes::AActor*> VehiclePawns;
		std::vector<SDK::Classes::AActor*> TeamCache;

		float Dist = FLT_MAX;
		SDK::Structs::FVector2D Aimto;
		bool AcknowledgedPawnUsable = true;

		auto UWorld = SDK::Classes::GetWorldContextFromOOBE();
		if (!UWorld) return false;

		auto PersistentLevel = UWorld->PersistentLevel;
		if (!PersistentLevel) return false;

		auto OwningGameInstance = UWorld->OwningGameInstance;
		if (!OwningGameInstance) return false;

		auto AController = OwningGameInstance->LocalPlayers[0]->Controller;
		if (!AController) return false;

		auto AcknowledgedPawn = AController->AcknowledgedPawn;
		if (!AcknowledgedPawn) AcknowledgedPawnUsable = false;

		auto AActors = (SDK::Structs::TArray<SDK::Classes::AActor*>*) & PersistentLevel->AActors;

		if (!AActors->Data[0]) return false;

		/*	if (AcknowledgedPawn)
			{
				auto LocalWeapon = AcknowledgedPawn->CurrentWeapon;
				if (!AcknowledgedPawn) AcknowledgedPawnUsable = false;

				if (Settings::InstantRevive)
					AcknowledgedPawn->ReviveFromDBNOTime = 0.101;

				if (Settings::AimWhileJumping)
					AcknowledgedPawn->bADSWhileNotOnGround = true;
				else
					AcknowledgedPawn->bADSWhileNotOnGround = false;

				if (Settings::NoAnimation)
					LocalWeapon->bDisableEquipAnimation = true;
				else
					LocalWeapon->bDisableEquipAnimation = false;
			}*/

		for (int i = 0; i < AActors->Num(); i++)
		{
			auto CurrentActor = AActors->Data[i];
			if (!CurrentActor) continue;

			auto ActorName = SDK::Structs::GetObjectName(CurrentActor);

			if (strstr(ActorName.c_str(), _("PlayerPawn")))
				PlayerPawns.push_back(CurrentActor);

			if (strstr(ActorName.c_str(), _("Valet_")) && strstr(ActorName.c_str(), _("Vehicle")))
				VehiclePawns.push_back(CurrentActor);
		}

		for (auto Actor : PlayerPawns)
		{
			if (!Actor) continue;
			if (!AController) continue;

			auto Mesh = Actor->Mesh;
			if (!Mesh) continue;

			auto RootComponent = Actor->RootComponent;
			if (!RootComponent) continue;

			auto RootLocation = RootComponent->RelativeLocation;
			if (!RootLocation.x && !RootLocation.y) continue;

			if (AcknowledgedPawnUsable)
			{
				auto ActorState = Actor->PlayerState;
				if (!ActorState) continue;

				auto ActorTeamIndex = ActorState->TeamIndex;
				if (!ActorTeamIndex) continue;

				auto LocalState = AcknowledgedPawn->PlayerState;
				if (!LocalState) continue;

				auto LocalTeamIndex = LocalState->TeamIndex;
				if (!LocalTeamIndex) continue;

				if (Actor == AcknowledgedPawn) continue;
				if (ActorTeamIndex == LocalTeamIndex) continue;
			}

			auto HeadWorldLocation = Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_HEAD);

			auto HeadScreenLocation = AController->ProjectWorldToScreen(HeadWorldLocation);
			if (!HeadScreenLocation.x && !HeadScreenLocation.y) continue;

			color BoxesColor = color(230, 255, 255, 255);

			auto Neck = AController->ProjectWorldToScreen(Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_NECK));

			auto Spine = AController->ProjectWorldToScreen(Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_CHEST_LOW));
			auto Spine2 = AController->ProjectWorldToScreen(Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_CHEST));
			auto Spine3 = AController->ProjectWorldToScreen(Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_TORSO));
			auto Spine4 = AController->ProjectWorldToScreen(Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_PELVIS_1));

			auto LeftArm1 = AController->ProjectWorldToScreen(Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_L_SHOULDER_1));
			auto LeftArm2 = AController->ProjectWorldToScreen(Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_L_ELBOW));
			auto LeftArm3 = AController->ProjectWorldToScreen(Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_L_HAND_ROOT_1));

			auto RightArm1 = AController->ProjectWorldToScreen(Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_R_SHOULDER));
			auto RightArm2 = AController->ProjectWorldToScreen(Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_R_ELBOW));
			auto RightArm3 = AController->ProjectWorldToScreen(Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_R_HAND_ROOT_1));

			auto LeftLeg1 = AController->ProjectWorldToScreen(Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_L_THIGH));
			auto LeftLeg2 = AController->ProjectWorldToScreen(Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_L_KNEE));
			auto LeftLeg3 = AController->ProjectWorldToScreen(Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_L_SHIN));
			auto LeftLeg4 = AController->ProjectWorldToScreen(Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_L_FOOT_ROOT));
			auto LeftLeg5 = AController->ProjectWorldToScreen(Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_L_FOOT_LOW));

			auto RightLeg1 = AController->ProjectWorldToScreen(Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_R_THIGH));
			auto RightLeg2 = AController->ProjectWorldToScreen(Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_R_KNEE));
			auto RightLeg3 = AController->ProjectWorldToScreen(Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_R_SHIN));
			auto RightLeg4 = AController->ProjectWorldToScreen(Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_R_FOOT_ROOT));
			auto RightLeg5 = AController->ProjectWorldToScreen(Mesh->GetBoneLocation(SDK::Structs::eBone::BONE_R_FOOT_LOW));

			auto box = GetFBox(Actor);

			if (!box.IsValid) continue;

			box.Min = AController->W2SVector3(box.Min);
			box.Max = AController->W2SVector3(box.Max);

			auto topLeft = SDK::Structs::FVector2D(box.Min.x, box.Min.y);
			auto bottomRight = SDK::Structs::FVector2D(box.Max.x, box.Max.y);

			auto topRight = SDK::Structs::FVector2D(bottomRight.x, topLeft.y);
			auto bottomLeft = SDK::Structs::FVector2D(topLeft.x, bottomRight.y);
			auto bottomCenter = SDK::Structs::FVector2D(((topLeft.x + bottomRight.x) - 20.0f) / 2.0f, topLeft.y + 20.0f);
			auto centerTop = SDK::Structs::FVector2D((topLeft.x + bottomRight.x) / 2.0f, topLeft.y);

			if (Settings::ESP)
			{
				gui.draw_line(topLeft.x, topLeft.y, topLeft.x, topLeft.y + ((bottomRight.y - topLeft.y) / 3), BoxesColor);
				gui.draw_line(topLeft.x, topLeft.y, topLeft.x + ((bottomRight.x - topLeft.x) / 3), topLeft.y, BoxesColor);
				gui.draw_line(bottomLeft.x, bottomLeft.y, bottomLeft.x, bottomLeft.y - ((bottomRight.y - topLeft.y) / 3), BoxesColor);
				gui.draw_line(bottomLeft.x, bottomLeft.y, bottomLeft.x + ((bottomRight.x - topLeft.x) / 3), bottomLeft.y, BoxesColor);
				gui.draw_line(topRight.x, topRight.y, topRight.x, topRight.y + ((bottomRight.y - topLeft.y) / 3), BoxesColor);
				gui.draw_line(topRight.x, topRight.y, topRight.x - ((bottomRight.x - topLeft.x) / 3), topRight.y, BoxesColor);
				gui.draw_line(topRight.x, topRight.y, topRight.x - ((bottomRight.x - topLeft.x) / 3), topRight.y, BoxesColor);
				gui.draw_line(bottomRight.x, bottomRight.y, bottomRight.x, bottomRight.y - ((bottomRight.y - topLeft.y) / 3), BoxesColor);
				gui.draw_line(bottomRight.x, bottomRight.y, bottomRight.x - ((bottomRight.x - topLeft.x) / 3), bottomRight.y, BoxesColor);
			}

			if (Settings::ESP_Lines)
				gui.draw_line(HeadScreenLocation.x, RightLeg5.y, X / 2, Y, BoxesColor);

			if (Settings::NameESP)
			{
			}

			if (Settings::ESP_Skeleton)
			{
				gui.draw_line(HeadScreenLocation.x, HeadScreenLocation.y, Neck.x, Neck.y, BoxesColor);
				gui.draw_line(Neck.x, Neck.y, Spine.x, Spine.y, BoxesColor);
				gui.draw_line(Spine.x, Spine.y, Spine2.x, Spine2.y, BoxesColor);
				gui.draw_line(Spine2.x, Spine2.y, Spine3.x, Spine3.y, BoxesColor);
				gui.draw_line(Spine3.x, Spine3.y, Spine4.x, Spine4.y, BoxesColor);

				gui.draw_line(Neck.x, Neck.y, LeftArm1.x, LeftArm1.y, BoxesColor);
				gui.draw_line(LeftArm1.x, LeftArm1.y, LeftArm2.x, LeftArm2.y, BoxesColor);
				gui.draw_line(LeftArm2.x, LeftArm2.y, LeftArm3.x, LeftArm3.y, BoxesColor);

				gui.draw_line(Neck.x, Neck.y, RightArm1.x, RightArm1.y, BoxesColor);
				gui.draw_line(RightArm1.x, RightArm1.y, RightArm2.x, RightArm2.y, BoxesColor);
				gui.draw_line(RightArm2.x, RightArm2.y, RightArm3.x, RightArm3.y, BoxesColor);

				gui.draw_line(Spine4.x, Spine4.y, LeftLeg1.x, LeftLeg1.y, BoxesColor);
				gui.draw_line(LeftLeg1.x, LeftLeg1.y, LeftLeg2.x, LeftLeg2.y, BoxesColor);
				gui.draw_line(LeftLeg2.x, LeftLeg2.y, LeftLeg3.x, LeftLeg3.y, BoxesColor);
				gui.draw_line(LeftLeg3.x, LeftLeg3.y, LeftLeg4.x, LeftLeg4.y, BoxesColor);
				gui.draw_line(LeftLeg4.x, LeftLeg4.y, LeftLeg5.x, LeftLeg5.y, BoxesColor);

				gui.draw_line(Spine4.x, Spine4.y, RightLeg1.x, RightLeg1.y, BoxesColor);
				gui.draw_line(RightLeg1.x, RightLeg1.y, RightLeg2.x, RightLeg2.y, BoxesColor);
				gui.draw_line(RightLeg2.x, RightLeg2.y, RightLeg3.x, RightLeg3.y, BoxesColor);
				gui.draw_line(RightLeg3.x, RightLeg3.y, RightLeg4.x, RightLeg4.y, BoxesColor);
				gui.draw_line(RightLeg4.x, RightLeg4.y, RightLeg5.x, RightLeg5.y, BoxesColor);
			}

			if (Settings::Aimbot)
			{
				if (AcknowledgedPawnUsable)
				{
					float Radius = (Settings::AimbotFovCircle * X / 80.f) / 2;

					auto x = HeadScreenLocation.x - (X / 2);
					auto y = HeadScreenLocation.y - (Y / 2);

					auto distance = sqrtf_(x * x + y * y);

					if (distance < 300 && distance < Dist)
					{
						if (Settings::VisibleCheck)
							if (AController->LineOfSightTo(Actor))
								Aimto = HeadScreenLocation;
						if (!Settings::VisibleCheck)
							Aimto = HeadScreenLocation;

						Dist = distance;
					}
				}
			}
		}

		if (AcknowledgedPawnUsable && Settings::Aimbot)
		{
			if (SDK::Syscall::NtGetAsyncKeyState(VK_RBUTTON))
				SDK::Syscall::mousemove(Aimto.x, Aimto.y, X, Y, Settings::AimbotSmooth, SDK::Syscall::NtGetAsyncKeyState(VK_CONTROL));
		}

		if (Settings::ESP_AimingLines && AcknowledgedPawn)
		{
			if (Aimto.x != 0 && Aimto.y != 0)
				gui.draw_line(X / 2, Y / 2, Aimto.x, Aimto.y, color(230, 255, 255, 255));
		}
	}
}
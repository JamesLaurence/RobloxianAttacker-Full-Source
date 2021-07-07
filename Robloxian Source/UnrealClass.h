#pragma once
#include <windows.h>
#include "UnrealStructs.h"

namespace SDK
{
	namespace Classes
	{
		enum class EFortItemType : uint8_t
		{
			WorldItem,
			Ammo,
			Badge,
			BackpackPickup,
			BuildingPiece,
			CharacterPart,
			Consumable,
			Deco,
			EditTool,
			Ingredient,
			ItemCache,
			Food,
			Gadget,
			AthenaGadget,
			HomebaseGadget,
			BattleLabDevice,
			SpyTechPerk,
			HeroAbility,
			MissionItem,
			Trap,
			Weapon,
			WeaponMelee,
			WeaponRanged,
			WeaponHarvest,
			WeaponCreativePhone,
			WeaponMod,
			WorldResource,
			CreativeUserPrefab,
			CreativePlayset,
			Vehicle,
			Npc,
			AccountItem,
			AccountResource,
			CollectedResource,
			Alteration,
			CardPack,
			Currency,
			Hero,
			Schematic,
			Worker,
			TeamPerk,
			PlayerTech,
			Token,
			DailyRewardScheduleToken,
			CodeToken,
			Stat,
			Buff,
			BuffCredit,
			Quest,
			Accolades,
			MedalsPunchCard,
			RepeatableDailiesCard,
			ChallengeBundle,
			ChallengeBundleSchedule,
			ChallengeBundleCompletionToken,
			GameplayModifier,
			Outpost,
			HomebaseNode,
			Defender,
			ConversionControl,
			DeployableBaseCloudSave,
			ConsumableAccountItem,
			Quota,
			Expedition,
			HomebaseBannerIcon,
			HomebaseBannerColor,
			AthenaSkyDiveContrail,
			PersonalVehicle,
			AthenaGlider,
			AthenaPickaxe,
			AthenaHat,
			AthenaBackpack,
			AthenaCharacter,
			AthenaDance,
			AthenaConsumableEmote,
			AthenaLoadingScreen,
			AthenaBattleBus,
			AthenaVehicleCosmetic,
			AthenaItemWrap,
			AthenaCallingCard,
			AthenaMapMarker,
			AthenaMusicPack,
			AthenaPetCosmetic,
			AthenaCharmCosmetic,
			AthenaVictoryPose,
			AthenaSeasonTreasure,
			AthenaSeason,
			AthenaRewardGraph,
			AthenaExtResource,
			EventDescription,
			BattleLabDeviceAccount,
			MatchAward,
			AthenaEventToken,
			EventPurchaseTracker,
			CosmeticVariantToken,
			CampaignHeroLoadout,
			Playset,
			PrerollData,
			CreativePlot,
			PlayerSurveyToken,
			CosmeticLocker,
			BannerToken,
			RestedXpBoosterToken,
			RewardEventGraphPurchaseToken,
			HardcoreModifier,
			EventDependentItem,
			ItemAccessToken,
			SpecialItem,
			Emote,
			Stack,
			CollectionBookPage,
			BGAConsumableWrapper,
			GiftBox,
			GiftBoxUnlock,
			PlaysetProp,
			RegCosmeticDef,
			Profile,
			Max_None,
			EFortItemType_MAX,
		};

		class USceneComponent
		{
		public:
			unsigned char _pad1[0x11c];
			SDK::Structs::Vector3 RelativeLocation; // 0x11c(0xc)
			// No Padding Required Here
			SDK::Structs::Vector3 RelativeRotation; // 0x128(0xc)
		};

		class PlayerState
		{
		public:
			unsigned char _pad1[0xed0];
			unsigned char TeamIndex; // 0xed0(0x1)
			unsigned char _pad2[0x1054];
			unsigned char SquadID; // 0x1054(0x1)
		};

		class UFortWeaponItemDefinition
		{
		public:
			unsigned char _pad1[0x69];
			EFortItemType ItemType; // 0x69(0x1)
		};

		class AFortWeapon
		{
		public:
			unsigned char _pad1[0x378];
			UFortWeaponItemDefinition* WeaponData; // 0x378(0x8)
			unsigned char _pad2[0xC08];
			float LastFireAbilityTime; // 0xC08(0x4)
			unsigned char _pad3[0x2B3];
			bool bDisableEquipAnimation; // 0x2B3(0x4)
		};

		class USkeletalMeshComponent
		{
		private:
			struct FMatrix
			{
				float M[4][4];
			};

		public:

			Structs::Vector3 GetBoneLocation(int BoneIndex)
			{
				static auto addr = (uintptr_t)0;
				if (!addr) {
					addr = sigscan(Fortnite, _("E8 ? ? ? ? 48 8B 47 30 F3 0F 10 45"));
					addr = (uintptr_t)RVA(addr, 5);
				}

				auto GetBoneMatrix = ((FMatrix * (__fastcall*)(USkeletalMeshComponent * Mesh, FMatrix*, int BoneID))(addr));

				FMatrix Matrix = { 0 };

				SpoofCall(GetBoneMatrix, this, &Matrix, BoneIndex);
				return Structs::Vector3(Matrix.M[3][0], Matrix.M[3][1], Matrix.M[3][2]);
			}
		};

		class AActor
		{
		public:
			unsigned char _pad1[0x130];
			USceneComponent* RootComponent; // 0x130(0x8)
			unsigned char _pad2[0x108];
			PlayerState* PlayerState; // 0x240(0x8)
			unsigned char _pad3[0x38];
			USkeletalMeshComponent* Mesh; // 0x280(0x8)
			unsigned char _pad4[0x600];
			AFortWeapon* CurrentWeapon; // 0x600(0x8)
			unsigned char _pad5[0x57E];
			unsigned char bIsDBNO; // 0x57E(0x1)
			unsigned char _pad6[0x540];
			unsigned char bIsDying; // 0x540(0x1)
			unsigned char _pad7[0x3A71];
			bool bADSWhileNotOnGround; // 0x3A71(0x4)
			unsigned char _pad8[0x3398];
			float ReviveFromDBNOTime; // 0x3398(0x4)
		};

		class ULevel
		{
		public:
			unsigned char _pad7[0x98];
			SDK::Structs::TArray<struct AActor*> AActors; // 0x98(0x10)
		};

		class UCanvas
		{
		public:
			unsigned char _pad8[0x28];
			float OrgX; // 0x28(0x4)
			// No Padding Required Here
			float OrgY; // 0x2c(0x4)
			// No Padding Required Here
			float ClipX; // 0x30(0x4)
			// No Padding Required Here
			float ClipY; // 0x34(0x4)
			// No Padding Required Here
			struct SDK::Structs::FColor DrawColor; // 0x38(0x4)
			// No Padding Required Here
			char bCenterX : 1; // 0x3c(0x1)
			// No Padding Required Here
			char bCenterY : 1; // 0x3c(0x1)
			// No Padding Required Here
			char bNoSmooth : 1; // 0x3c(0x1)
			// No Padding Required Here
			unsigned char UnknownBuffer_GsD24 : 5; // 0x3c(0x1)
			// No Padding Required Here
			unsigned char UnknownBuffer_Pu72n[0x3]; // 0x3d(0x3)
			// No Padding Required Here
			int32_t SizeX; // 0x40(0x4)
			// No Padding Required Here
			int32_t SizeY; // 0x44(0x4)
			// No Padding Required Here
			unsigned char UnknownBuffer_67hdW[0x8]; // 0x48(0x8)
			// No Padding Required Here
			unsigned char ColorModulate[0x10]; // 0x50(0x10)
			// No Padding Required Here
			PVOID DefaultTexture; // 0x60(0x8)
			// No Padding Required Here
			PVOID GradientTexture0; // 0x68(0x8)
			// No Padding Required Here
			PVOID ReporterGraph; // 0x70(0x8)
		};

		class AHUD
		{
		public:
			unsigned char Padding_d24Be[0x228];
			char bLostFocusPaused : 1; // 0x228(0x1)
			// No Padding Required Here
			char bShowHUD : 1; // 0x228(0x1)
			// No Padding Required Here
			char bShowDebugInfo : 1; // 0x228(0x1)
			// No Padding Required Here
			unsigned char UnknownBuffer_HaB72 : 5; // 0x228(0x1)
			// No Padding Required Here
			unsigned char UnknownBuffer_7vY26[0x3]; // 0x229(0x3)
			// No Padding Required Here
			int32_t CurrentTargetIndex; // 0x22c(0x4)
			// No Padding Required Here
			char bShowHitBoxDebugInfo : 1; // 0x230(0x1)
			// No Padding Required Here
			char bShowOverlays : 1; // 0x230(0x1)
			// No Padding Required Here
			char bEnableDebugTextShadow : 1; // 0x230(0x1)
			// No Padding Required Here
			unsigned char UnknownBuffer_2aNx7 : 5; // 0x230(0x1)
			// No Padding Required Here
			unsigned char UnknownBuffer_8HAfk[0x7]; // 0x231(0x7)
			// No Padding Required Here
			struct SDK::Structs::TArray<struct AActor*> PostRenderedActors; // 0x238(0x10)
			// No Padding Required Here
			unsigned char UnknownBuffer_h7Nua[0x8]; // 0x248(0x8)
			// No Padding Required Here
			struct SDK::Structs::TArray<struct SDK::Structs::FName> DebugDisplay; // 0x250(0x10)
			// No Padding Required Here
			struct SDK::Structs::TArray<struct SDK::Structs::FName> ToggledDebugCategories; // 0x260(0x10)
			// No Padding Required Here
			struct UCanvas* Canvas; // 0x270(0x8)
			// No Padding Required Here
			struct UCanvas* DebugCanvas; // 0x278(0x8)
		};

		class AController
		{
		public:
			unsigned char _pad1[0x2a0];
			AActor* AcknowledgedPawn; // 0x2a0(0x8)
			unsigned char _pad2[0x8];
			AHUD* MyHUD; // 0x2b0(0x8)
			// No Padding Required Here
			void* PlayerCameraManager; // 0x2b8(0x8)

			bool LineOfSightTo(AActor* Actor)
			{
				static uintptr_t lost;
				if (!lost)
					lost = sigscan(Fortnite, _("48 8B C4 48 89 58 20 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 0F 29 70 B8 0F 29 78 A8 44 0F 29 40 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 20 45 8A E9"));
				auto LineOfSightTo_ = ((bool(__fastcall*)(AController * Controller, AActor * Other, SDK::Structs::Vector3 ViewPoint, bool bAlternativeChecks))(lost));
				return SpoofCall(LineOfSightTo_, this, Actor, SDK::Structs::Vector3(0, 0, 0), false);
			}

			void ClientSetRotation(SDK::Structs::Vector3 NewRotation, bool bResetCamera = false)
			{
				auto ClientSetRotation_ = (*(void(__fastcall**)(AController * Controller, SDK::Structs::Vector3 NewRotation, bool bResetCamera))(*(uintptr_t*)this + 0x640));
				SpoofCall(ClientSetRotation_, this, NewRotation, bResetCamera);
			}

			void SetIgnoreLookInput(bool bNewLookInput)
			{
				auto SetIgnoreLookInput_ = (*(void(__fastcall**)(AController * Controller, bool bNewLookInput))(*(uintptr_t*)this + 0x748));
				SpoofCall(SetIgnoreLookInput_, this, bNewLookInput);
			}

			SDK::Structs::FVector2D ProjectWorldToScreen(SDK::Structs::Vector3 World)
			{
				static auto addr = (uintptr_t)0;
				if (!addr) {
					addr = sigscan(Fortnite, _("E8 ? ? ? ? 41 88 07 48 83 C4 30"));
					addr = (uintptr_t)(RVA(addr, 5));
				}
				SDK::Structs::FVector2D B(0, 0);
				auto _ProjectWorldToScreen = ((bool(__fastcall*)(AController * Controller, SDK::Structs::Vector3 WorldPosition, SDK::Structs::FVector2D * pScreenPosition, bool bPlayerViewportRelative))(addr));
				if (SpoofCall(_ProjectWorldToScreen, this, World, &B, false))
					return B;

				return { 0,0 };
			}

			SDK::Structs::Vector3 W2SVector3(SDK::Structs::Vector3 World)
			{
				return Structs::Vector3(ProjectWorldToScreen(World).x, ProjectWorldToScreen(World).y, 0);
			}
		};

		class ULocalPlayer
		{
		public:
			unsigned char _pad1[0x30];
			AController* Controller; // 0x30(0x8)
		};

		class UGameInstance
		{
		public:
			unsigned char _pad1[0x38];
			SDK::Structs::TArray<struct ULocalPlayer*> LocalPlayers; // 0x38(0x8)
		};

		class UWorld
		{
		public:
			unsigned char _pad1[0x30];
			ULevel* PersistentLevel; // 0x30(0x8)
			unsigned char _pad2[0x148];
			UGameInstance* OwningGameInstance; // 0x180(0x8)
		};


		uintptr_t GetCurrentImageBase()
		{
			return *(uintptr_t*)(__readgsqword(0x60) + 0x10);
		}

		uintptr_t ResolveRelativeAddress(uintptr_t Address, int InstructionLength)
		{
			DWORD Offset = *(DWORD*)(Address + (InstructionLength - 4));
			return Address + InstructionLength + Offset;
		}

		UWorld* GetWorldContextFromOOBE()
		{
			static auto addr = (uintptr_t)0;
			if (!addr)
				addr = sigscan(Fortnite, _("48 8B 05 ? ? ? ? 4D 8B C2"));

			static uintptr_t WorldPtr;
			if (!WorldPtr)
			{
				WorldPtr = ResolveRelativeAddress(addr, 7);
			}
			return (UWorld*)(*(uintptr_t*)WorldPtr);
		}
	}
}

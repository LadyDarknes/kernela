#pragma once

#include "../../offsets/animationsystem_dll.hpp"
#include "../../offsets/client_dll.hpp"
#include "../../offsets/engine2_dll.hpp"
#include "../../offsets/offsets.hpp"

namespace Offsets {
inline constexpr auto &dwEntityList =
    cs2_dumper::offsets::client_dll::dwEntityList;
inline constexpr auto &dwLocalPlayerPawn =
    cs2_dumper::offsets::client_dll::dwLocalPlayerPawn;
inline constexpr auto &dwLocalPlayerController =
    cs2_dumper::offsets::client_dll::dwLocalPlayerController;
inline constexpr auto &dwViewMatrix =
    cs2_dumper::offsets::client_dll::dwViewMatrix;
inline constexpr auto &dwViewAngles =
    cs2_dumper::offsets::client_dll::dwViewAngles;
inline constexpr auto &dwGlowManager =
    cs2_dumper::offsets::client_dll::dwGlowManager;
inline constexpr auto &dwGameRules =
    cs2_dumper::offsets::client_dll::dwGameRules;
inline constexpr auto &dwGlobalVars =
    cs2_dumper::offsets::client_dll::dwGlobalVars;
inline constexpr auto &dwPlantedC4 =
    cs2_dumper::offsets::client_dll::dwPlantedC4;
inline constexpr auto &dwWeaponC4 = cs2_dumper::offsets::client_dll::dwWeaponC4;
inline constexpr auto &dwSensitivity =
    cs2_dumper::offsets::client_dll::dwSensitivity;
inline constexpr auto &dwSensitivity_sensitivity =
    cs2_dumper::offsets::client_dll::dwSensitivity_sensitivity;
inline constexpr auto &dwCSGOInput =
    cs2_dumper::offsets::client_dll::dwCSGOInput;
inline constexpr auto &dwPrediction =
    cs2_dumper::offsets::client_dll::dwPrediction;
inline constexpr auto &dwViewRender =
    cs2_dumper::offsets::client_dll::dwViewRender;
inline constexpr auto &dwGameEntitySystem =
    cs2_dumper::offsets::client_dll::dwGameEntitySystem;
inline constexpr auto &dwGameEntitySystem_highestEntityIndex =
    cs2_dumper::offsets::client_dll::dwGameEntitySystem_highestEntityIndex;
inline constexpr auto &dwNetworkGameClient =
    cs2_dumper::offsets::engine2_dll::dwNetworkGameClient;
inline constexpr auto &dwBuildNumber =
    cs2_dumper::offsets::engine2_dll::dwBuildNumber;
inline constexpr auto &dwWindowWidth =
    cs2_dumper::offsets::engine2_dll::dwWindowWidth;
inline constexpr auto &dwWindowHeight =
    cs2_dumper::offsets::engine2_dll::dwWindowHeight;

namespace BaseEntity {
inline constexpr auto m_iHealth =
    cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth; // 0x34C
inline constexpr auto m_iTeamNum =
    cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum; // 0x3EB
inline constexpr auto m_iMaxHealth =
    cs2_dumper::schemas::client_dll::C_BaseEntity::m_iMaxHealth; // 0x348
inline constexpr auto m_lifeState =
    cs2_dumper::schemas::client_dll::C_BaseEntity::m_lifeState; // 0x354
inline constexpr auto m_bTakesDamage =
    cs2_dumper::schemas::client_dll::C_BaseEntity::m_bTakesDamage; // 0x355
inline constexpr auto m_pGameSceneNode =
    cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode; // 0x330
} // namespace BaseEntity

namespace PlayerPawnBase {
inline constexpr auto m_flFlashDuration = cs2_dumper::schemas::client_dll::
    C_CSPlayerPawnBase::m_flFlashDuration; // 0x1400
inline constexpr auto m_flFlashMaxAlpha =
    cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_flFlashMaxAlpha;
} // namespace PlayerPawnBase

namespace Player {
inline constexpr auto m_angEyeAngles =
    cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_angEyeAngles; // 0x3360
inline constexpr auto m_angEyeAnglesVelocity =
    cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_angEyeAnglesVelocity;
inline constexpr auto m_iShotsFired =
    cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_iShotsFired; // 0x1C64
inline constexpr auto m_bIsScoped =
    cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_bIsScoped; // 0x1C50
inline constexpr auto m_bIsDefusing =
    cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_bIsDefusing; // 0x1C52
inline constexpr auto m_bIsGrabbingHostage =
    cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_bIsGrabbingHostage;
inline constexpr auto m_ArmorValue =
    cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_ArmorValue; // 0x1C7C
inline constexpr auto m_iIDEntIndex =
    cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_iIDEntIndex; // 0x343C
inline constexpr auto m_entitySpottedState =
    cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_entitySpottedState;
inline constexpr auto m_bInBuyZone =
    cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_bInBuyZone;
inline constexpr auto m_bInBombZone =
    cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_bInBombZone;
} // namespace Player
namespace SceneNode {
inline constexpr auto m_vecOrigin =
    cs2_dumper::schemas::client_dll::CGameSceneNode::m_vecOrigin;
inline constexpr auto m_angRotation =
    cs2_dumper::schemas::client_dll::CGameSceneNode::m_angRotation;
inline constexpr auto m_vecAbsOrigin =
    cs2_dumper::schemas::client_dll::CGameSceneNode::m_vecAbsOrigin;
inline constexpr auto m_angAbsRotation =
    cs2_dumper::schemas::client_dll::CGameSceneNode::m_angAbsRotation;
} // namespace SceneNode
namespace Glow {
inline constexpr auto m_fGlowColor =
    cs2_dumper::schemas::client_dll::CGlowProperty::m_fGlowColor;
inline constexpr auto m_iGlowType =
    cs2_dumper::schemas::client_dll::CGlowProperty::m_iGlowType;
inline constexpr auto m_iGlowTeam =
    cs2_dumper::schemas::client_dll::CGlowProperty::m_iGlowTeam;
inline constexpr auto m_nGlowRange =
    cs2_dumper::schemas::client_dll::CGlowProperty::m_nGlowRange;
inline constexpr auto m_bGlowing =
    cs2_dumper::schemas::client_dll::CGlowProperty::m_bGlowing;
} // namespace Glow

namespace Skeleton {
inline constexpr auto m_modelState =
    cs2_dumper::schemas::client_dll::CSkeletonInstance::m_modelState;
inline constexpr auto m_nHitboxSet =
    cs2_dumper::schemas::client_dll::CSkeletonInstance::m_nHitboxSet;
} // namespace Skeleton
namespace ModelState {
inline constexpr auto m_hModel =
    cs2_dumper::schemas::client_dll::CModelState::m_hModel;
}
namespace ModelSkeleton {
inline constexpr auto m_nParent =
    cs2_dumper::schemas::animationsystem_dll::ModelSkeletonData_t::m_nParent;
inline constexpr auto m_nFlag =
    cs2_dumper::schemas::animationsystem_dll::ModelSkeletonData_t::m_nFlag;
} // namespace ModelSkeleton
namespace Controller {
inline constexpr auto &m_pInGameMoneyServices = cs2_dumper::schemas::
    client_dll::CCSPlayerController::m_pInGameMoneyServices;
inline constexpr auto &m_pInventoryServices =
    cs2_dumper::schemas::client_dll::CCSPlayerController::m_pInventoryServices;
inline constexpr auto &m_pActionTrackingServices = cs2_dumper::schemas::
    client_dll::CCSPlayerController::m_pActionTrackingServices;
inline constexpr auto &m_iPing =
    cs2_dumper::schemas::client_dll::CCSPlayerController::m_iPing;
inline constexpr auto &m_sSanitizedPlayerName = cs2_dumper::schemas::
    client_dll::CCSPlayerController::m_sSanitizedPlayerName;
inline constexpr auto &m_pControllerPawn = cs2_dumper::schemas::client_dll::
    CCSPlayerController::m_hPlayerPawn; // 0x610
} // namespace Controller
namespace EntityIdentity {
inline constexpr auto m_designerName =
    cs2_dumper::schemas::engine2_dll::CEntityIdentity::m_designerName;
}
inline constexpr ptrdiff_t BONE_MATRIX_OFFSET = 0x1C0; // m_pBoneTransform

} // namespace Offsets

inline constexpr auto &m_iHealth = Offsets::BaseEntity::m_iHealth;
inline constexpr auto &m_iTeamNum = Offsets::BaseEntity::m_iTeamNum;
inline constexpr auto &m_iMaxHealth = Offsets::BaseEntity::m_iMaxHealth;
inline constexpr auto &m_lifeState = Offsets::BaseEntity::m_lifeState;
inline constexpr auto &m_angEyeAngles = Offsets::Player::m_angEyeAngles;
inline constexpr auto &m_iShotsFired = Offsets::Player::m_iShotsFired;
inline constexpr auto &m_bIsScoped = Offsets::Player::m_bIsScoped;
inline constexpr auto &m_bIsDefusing = Offsets::Player::m_bIsDefusing;
inline constexpr auto &m_ArmorValue = Offsets::Player::m_ArmorValue;
inline constexpr auto &m_iIDEntIndex = Offsets::Player::m_iIDEntIndex;
inline constexpr auto &m_flFlashDuration =
    Offsets::PlayerPawnBase::m_flFlashDuration;

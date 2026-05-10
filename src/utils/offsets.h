#pragma once

#include <stddef.h>

namespace std {
    using ::ptrdiff_t;
}

#include "../../offsets/animationsystem_dll.hpp"
#include "../../offsets/client_dll.hpp"
#include "../../offsets/engine2_dll.hpp"
#include "../../offsets/offsets.hpp"

namespace Offsets {
    // Core Pointers
    inline constexpr auto dwEntityList = cs2_dumper::offsets::client_dll::dwEntityList;
    inline constexpr auto dwLocalPlayerPawn = cs2_dumper::offsets::client_dll::dwLocalPlayerPawn;
    inline constexpr auto dwLocalPlayerController = cs2_dumper::offsets::client_dll::dwLocalPlayerController;
    inline constexpr auto dwViewMatrix = cs2_dumper::offsets::client_dll::dwViewMatrix;
    inline constexpr auto dwViewAngles = cs2_dumper::offsets::client_dll::dwViewAngles;
    inline constexpr auto dwGlowManager = cs2_dumper::offsets::client_dll::dwGlowManager;
    inline constexpr auto dwGameRules = cs2_dumper::offsets::client_dll::dwGameRules;
    inline constexpr auto dwGlobalVars = cs2_dumper::offsets::client_dll::dwGlobalVars;
    inline constexpr auto dwPlantedC4 = cs2_dumper::offsets::client_dll::dwPlantedC4;
    inline constexpr auto dwWeaponC4 = cs2_dumper::offsets::client_dll::dwWeaponC4;
    inline constexpr auto dwCSGOInput = cs2_dumper::offsets::client_dll::dwCSGOInput;

    // C_BaseEntity
    namespace BaseEntity {
        inline constexpr auto m_iHealth = cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth;
        inline constexpr auto m_iTeamNum = cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum;
        inline constexpr auto m_iMaxHealth = cs2_dumper::schemas::client_dll::C_BaseEntity::m_iMaxHealth;
        inline constexpr auto m_lifeState = cs2_dumper::schemas::client_dll::C_BaseEntity::m_lifeState;
        inline constexpr auto m_pGameSceneNode = cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode;
        inline constexpr auto m_pCollision = cs2_dumper::schemas::client_dll::C_BaseEntity::m_pCollision;
        inline constexpr auto m_vecAbsVelocity = cs2_dumper::schemas::client_dll::C_BaseEntity::m_vecAbsVelocity;
    }

    // C_CSPlayerPawn
    namespace Player {
        inline constexpr auto m_angEyeAngles = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_angEyeAngles;
        inline constexpr auto m_iShotsFired = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_iShotsFired;
        inline constexpr auto m_bIsScoped = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_bIsScoped;
        inline constexpr auto m_bIsDefusing = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_bIsDefusing;
        inline constexpr auto m_flFlashDuration = cs2_dumper::schemas::client_dll::C_CSPlayerPawnBase::m_flFlashDuration;
        inline constexpr auto m_iIDEntIndex = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_iIDEntIndex;
        inline constexpr auto m_vOldOrigin = cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin;
        inline constexpr auto m_ArmorValue = cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_ArmorValue;
    }

    // CCSPlayerController
    namespace Controller {
        inline constexpr auto m_hPlayerPawn = cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn;
        inline constexpr auto m_pActionTrackingServices = cs2_dumper::schemas::client_dll::CCSPlayerController::m_pActionTrackingServices;
        inline constexpr auto m_iPing = cs2_dumper::schemas::client_dll::CCSPlayerController::m_iPing;
        inline constexpr auto m_sSanitizedPlayerName = cs2_dumper::schemas::client_dll::CCSPlayerController::m_sSanitizedPlayerName;
        inline constexpr auto m_bPawnIsAlive = cs2_dumper::schemas::client_dll::CCSPlayerController::m_bPawnIsAlive;
        inline constexpr auto m_iPawnArmor = cs2_dumper::schemas::client_dll::CCSPlayerController::m_iPawnArmor;
        inline constexpr auto m_iPawnHealth = cs2_dumper::schemas::client_dll::CCSPlayerController::m_iPawnHealth;
        inline constexpr auto m_pControllerPawn = cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn;
    }

    // CEntityIdentity
    namespace EntityIdentity {
        inline constexpr auto m_designerName = cs2_dumper::schemas::engine2_dll::CEntityIdentity::m_designerName;
        inline constexpr auto m_flags = cs2_dumper::schemas::engine2_dll::CEntityIdentity::m_flags;
    }

    // CGameSceneNode
    namespace GameSceneNode {
        inline constexpr auto m_vecOrigin = cs2_dumper::schemas::client_dll::CGameSceneNode::m_vecAbsOrigin;
        inline constexpr auto m_modelState = cs2_dumper::schemas::client_dll::CSkeletonInstance::m_modelState;
    }
    // simdilik hardcoded, ileride dumper ile alınacak
    namespace Physics {
        // vphysics2.dll
        inline constexpr auto VPhysics2_Interface = 0x40DDA0;

        // mesh verisi(CModelState içerisindeki offset)
        inline constexpr auto m_pVPhysicsAggregate = 0xE0; // CModelState -> m_pVPhysicsAggregate
        // m_pCollision (C_BaseEntity)
        inline constexpr auto m_pCollision = 0x340;
    }


    inline constexpr ptrdiff_t BONE_MATRIX_OFFSET = 0x1D0;

}
inline constexpr auto m_iHealth = Offsets::BaseEntity::m_iHealth;
inline constexpr auto m_iTeamNum = Offsets::BaseEntity::m_iTeamNum;
inline constexpr auto m_iMaxHealth = Offsets::BaseEntity::m_iMaxHealth;
inline constexpr auto m_lifeState = Offsets::BaseEntity::m_lifeState;
inline constexpr auto m_pGameSceneNode = Offsets::BaseEntity::m_pGameSceneNode;

inline constexpr auto m_angEyeAngles = Offsets::Player::m_angEyeAngles;
inline constexpr auto m_iShotsFired = Offsets::Player::m_iShotsFired;
inline constexpr auto m_bIsScoped = Offsets::Player::m_bIsScoped;
inline constexpr auto m_bIsDefusing = Offsets::Player::m_bIsDefusing;
inline constexpr auto m_iIDEntIndex = Offsets::Player::m_iIDEntIndex;
inline constexpr auto m_vOldOrigin = Offsets::Player::m_vOldOrigin;

inline constexpr auto m_hPlayerPawn = Offsets::Controller::m_hPlayerPawn;
inline constexpr auto m_iPing = Offsets::Controller::m_iPing;
inline constexpr auto m_sSanitizedPlayerName = Offsets::Controller::m_sSanitizedPlayerName;
inline constexpr auto m_pControllerPawn = Offsets::Controller::m_pControllerPawn;
inline constexpr auto m_pActionTrackingServices = Offsets::Controller::m_pActionTrackingServices;

inline constexpr auto m_designerName = Offsets::EntityIdentity::m_designerName;
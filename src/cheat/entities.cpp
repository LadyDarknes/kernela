#include "../utils/common.h"
#include "../utils/offsets.h"
#include <intrin.h>

#ifndef HEADONLYFP
#define HEADONLYFP 0
#endif

__forceinline float sqrt(float number) {
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = *(long*)&y;                       // evil
    i = 0x5f3759df - (i >> 1);            // what the actualy fuck?
    y = *(float*)&i;
    y = y * (threehalfs - (x2 * y * y));   // 1st iteration
    y = y * (threehalfs - (x2 * y * y));   // 2nd iteration, this can be removed for speed

    return 1.0f / y;
}

namespace entities {
    SimpleVector<PLAYER_DATA>* playersPtr = nullptr;
    PLAYER_DATA localPlayer;
    int count = 0;
    constexpr size_t STRIDE = 0x70;
    constexpr int headBone = 7;
    BYTE ctrlSnap[MAX_ENTITIES * STRIDE];
    BYTE boneSnap[MAX_BONES * 32];

    template <typename T>
    __forceinline bool ReadValue(ULONG_PTR address, T& out) {
        return ReadMemory(address, &out, sizeof(T)) != FALSE;
    }

    __forceinline void WPC(SimpleVector<PLAYER_DATA>& players) {
        players.reserve(MAX_ENTITIES);
        players.clear();
    }

    extern "C" void UpdateEntities() {
        if (!g_client_base || !cs2) return;

        if (!entities::playersPtr) {
            entities::playersPtr = (SimpleVector<PLAYER_DATA> *)ExAllocatePool2(POOL_FLAG_PAGED, sizeof(SimpleVector<PLAYER_DATA>), 'tceV');
            if (entities::playersPtr) {
                new (entities::playersPtr) SimpleVector<PLAYER_DATA>();
                WPC(*entities::playersPtr);
            }
        }
        if (!entities::playersPtr) return;
        auto& players = *entities::playersPtr;
        ULONG_PTR entList = 0;
        ULONG_PTR localPawn = 0;
        ULONG_PTR localCtrl = 0;
        ReadValue(g_client_base + Offsets::dwEntityList, entList);
        ReadValue(g_client_base + Offsets::dwLocalPlayerPawn, localPawn);
        ReadValue(g_client_base + Offsets::dwLocalPlayerController, localCtrl);
        if (!entList) return;
        entities::localPlayer.pawn_address = localPawn;
        if (localPawn) {
            entities::localPlayer.team = R<int>(localPawn + Offsets::BaseEntity::m_iTeamNum);
            entities::localPlayer.shots_fired = R<int>(localPawn + Offsets::Player::m_iShotsFired);
            ReadValue(g_client_base + Offsets::dwViewAngles, entities::localPlayer.eye_angles);
            V3 origin = R<V3>(localPawn + Offsets::Player::m_vOldOrigin);
            V3 viewOffset = R<V3>(localPawn + cs2_dumper::schemas::client_dll::C_BaseModelEntity::m_vecViewOffset);
            entities::localPlayer.origin.x = origin.x + viewOffset.x;
            entities::localPlayer.origin.y = origin.y + viewOffset.y;
            entities::localPlayer.origin.z = origin.z + viewOffset.z;
        }

        players.clear();

        ULONG_PTR ctrlPage = 0;
        ReadValue(entList + 0x10, ctrlPage);
        if (!ctrlPage) return;
        if (!ReadMemory(ctrlPage, ctrlSnap, sizeof(ctrlSnap))) return;

        ULONG_PTR pawnPages[64] = {};
        bool pawnPageOk[64] = {};

        for (int i = 1; i < MAX_ENTITIES; ++i) {
            ULONG_PTR controller = 0;
            RtlCopyMemory(&controller, ctrlSnap + i * STRIDE, sizeof(controller));
            if (!controller || controller == localCtrl) continue;

            uint32_t pawnHandle = R<uint32_t>(controller + Offsets::Controller::m_pControllerPawn);
            if (!pawnHandle || pawnHandle == 0xFFFFFFFF) continue;

            uint32_t pawnIdx = pawnHandle & 0x7FFF;
            uint32_t pawnPageIdx = pawnIdx >> 9;
            if (pawnPageIdx >= 64) continue;

            if (!pawnPageOk[pawnPageIdx]) {
                pawnPages[pawnPageIdx] = R<ULONG_PTR>(entList + 0x10 + (0x8 * pawnPageIdx));
                pawnPageOk[pawnPageIdx] = true;
            }

            ULONG_PTR pawnPage = pawnPages[pawnPageIdx];
            if (!pawnPage) continue;

            ULONG_PTR pawn = R<ULONG_PTR>(pawnPage + (pawnIdx & 0x1FF) * STRIDE);
            if (!pawn || pawn == localPawn) continue;

            PLAYER_DATA p{};
            p.valid = TRUE;
            p.pawn_address = pawn;
            p.controller_address = controller;
            p.health = R<int>(pawn + Offsets::BaseEntity::m_iHealth);
            p.team = R<int>(pawn + Offsets::BaseEntity::m_iTeamNum);

            if (p.health <= 0 || p.health > 100) continue;
            RtlZeroMemory(p.name, sizeof(p.name));
            ULONG_PTR namePtr = controller + Offsets::Controller::m_sSanitizedPlayerName;
            ULONG_PTR pszString = R<ULONG_PTR>(namePtr);
            if (pszString) {
                char nameBuf[64] = {};
                ReadMemory(pszString, nameBuf, sizeof(nameBuf) - 1);
                for (int k = 0; k < 31 && nameBuf[k]; k++) {
                    p.name[k] = (wchar_t)(unsigned char)nameBuf[k];
                }
                p.name[31] = L'\0';
            }

            ULONG_PTR sceneNode = R<ULONG_PTR>(pawn + Offsets::BaseEntity::m_pGameSceneNode);
            if (!sceneNode) continue;

            V3 absOrigin = R<V3>(sceneNode + 0xC8);
            p.origin = absOrigin;
            ULONG_PTR boneArr = R<ULONG_PTR>(sceneNode + Offsets::BONE_MATRIX_OFFSET);
            if (boneArr && ReadMemory(boneArr, boneSnap, sizeof(boneSnap))) {
                for (int j = 0; j < MAX_BONES; j++) {
                    RtlCopyMemory(&p.bones[j].Pos, boneSnap + j * 32, sizeof(V3));
                }
                p.head_pos = p.bones[headBone].Pos;
            }

            players.push_back(p);
        }
        entities::count = (int)players.size();
    }

    extern "C" void UpdatePlist() {
        if (!entities::playersPtr) return;
        auto& players = *entities::playersPtr;

        ULONG_PTR lp = entities::localPlayer.pawn_address;
        if (lp) {
            entities::localPlayer.team = R<int>(lp + Offsets::BaseEntity::m_iTeamNum);
            entities::localPlayer.shots_fired = R<int>(lp + Offsets::Player::m_iShotsFired);
        }

        for (int i = 0; i < (int)players.size(); ++i) {
            PLAYER_DATA& p = players[i];

            p.health = R<int>(p.pawn_address + Offsets::BaseEntity::m_iHealth);
            p.team = R<int>(p.pawn_address + Offsets::BaseEntity::m_iTeamNum);
            ULONG_PTR namePtr = p.controller_address + Offsets::Controller::m_sSanitizedPlayerName;
            ULONG_PTR pszString = R<ULONG_PTR>(namePtr);

            if (pszString) {
                char nameBuf[64] = {};
                ReadMemory(pszString, nameBuf, sizeof(nameBuf) - 1);
                RtlZeroMemory(p.name, sizeof(p.name));
                for (int k = 0; k < 31 && nameBuf[k]; k++) {
                    p.name[k] = (wchar_t)(unsigned char)nameBuf[k];
                }
                p.name[31] = L'\0';
            }
        }
    }
}
#include "../utils/common.h"
#include "../utils/offsets.h"
#include <intrin.h>

#ifndef HEADONLYFP
#define HEADONLYFP 0
#endif

namespace entities {
    SimpleVector<PLAYER_DATA> *playersPtr = nullptr;
    PLAYER_DATA localPlayer;
    int count = 0;
    constexpr size_t STRIDE = 0x70;
    constexpr int headBone = 6;
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
        ReadValue(g_client_base + Offsets::dwViewAngles, entities::localPlayer.eye_angles);
        ULONG_PTR localNode = 0;
        ReadValue(localPawn + Offsets::BaseEntity::m_pGameSceneNode, localNode);
        if (localNode) {
            V3 viewOff{};
            ReadValue(localNode + Offsets::SceneNode::m_vecAbsOrigin, entities::localPlayer.origin);
            ReadValue(localPawn + Offsets::BaseEntity::m_vecViewOffset, viewOff);
            entities::localPlayer.origin.x += viewOff.x; entities::localPlayer.origin.y += viewOff.y; entities::localPlayer.origin.z += viewOff.z;
        }

        players.clear();
        ULONG_PTR ctrlPage = 0;
        ReadValue(entList + 0x10, ctrlPage);
        if (!ctrlPage) {  return; }
        if (!ReadMemory(ctrlPage, ctrlSnap, sizeof(ctrlSnap))) { return; }

        ULONG_PTR pawnPages[64] = {};
        bool pawnPageOk[64] = {};

        for (int i = 1; i < MAX_ENTITIES; ++i) {
            ULONG_PTR controller = 0;
            RtlCopyMemory(&controller, ctrlSnap + i * STRIDE, sizeof(controller));
            if (!controller || controller == localCtrl) continue;

            uint32_t pawnHandle = 0;
            ReadValue(controller + Offsets::Controller::m_pControllerPawn, pawnHandle);
            if (!pawnHandle || pawnHandle == 0xFFFFFFFF) continue;

            uint32_t pawnIdx = pawnHandle & 0x7FFF;
            uint32_t pawnPageIdx = pawnIdx >> 9;
            if (pawnPageIdx >= 64) continue;
            if (!pawnPageOk[pawnPageIdx]) {
                ReadValue(entList + 0x10 + (0x8 * pawnPageIdx), pawnPages[pawnPageIdx]);
                pawnPageOk[pawnPageIdx] = true;
            }
            ULONG_PTR pawnPage = pawnPages[pawnPageIdx];
            if (!pawnPage) continue;

            ULONG_PTR pawn = 0;
            ReadValue(pawnPage + (pawnIdx & 0x1FF) * STRIDE, pawn);
            if (!pawn || pawn == localPawn) continue;

            PLAYER_DATA p{};
            p.valid = TRUE;
            p.pawn_address = pawn;
            p.controller_address = controller;
            ReadValue(pawn + 0x1390, p.origin);

            ULONG_PTR sceneNode = 0;
            ReadValue(pawn + Offsets::BaseEntity::m_pGameSceneNode, sceneNode);
            if (sceneNode) {
                ULONG_PTR boneArr = 0;
                ReadValue(sceneNode + Offsets::BONE_MATRIX_OFFSET, boneArr);
                if (boneArr) {
#if HEADONLYFP
                    ReadValue(boneArr + headBone * 32, p.head_pos);
                    p.bones[headBone].Pos = p.head_pos;
#else
                    if (ReadMemory(boneArr, boneSnap, sizeof(boneSnap))) {
                        for (int j = 0; j < MAX_BONES; j++) {
                            RtlCopyMemory(&p.bones[j].Pos, boneSnap + j * 32, sizeof(V3));
                        }
                        p.head_pos = p.bones[headBone].Pos;
                    }
#endif
                }
            }
            players.push_back(p);
        }
        entities::count = (int)players.size();
    }

    extern "C" void UpdatePlist() {
        if (!entities::playersPtr || entities::count == 0) return;
        auto& players = *entities::playersPtr;

        ULONG_PTR lp = entities::localPlayer.pawn_address;
        if (lp) {
            entities::localPlayer.team = R<int>(lp + Offsets::BaseEntity::m_iTeamNum);
            entities::localPlayer.shots_fired = R<int>(lp + Offsets::Player::m_iShotsFired);
        }
        for (int i = 0; i < (int)players.size(); ++i) {
            PLAYER_DATA& p = players[i];

            p.health = R<int>(p.pawn_address + Offsets::BaseEntity::m_iHealth);
            p.team = (int)R<uint8_t>(p.pawn_address + Offsets::BaseEntity::m_iTeamNum);

            ULONG_PTR namePtr = p.controller_address + Offsets::Controller::m_sSanitizedPlayerName;
            ReadMemory(namePtr, p.name, 31 * sizeof(wchar_t));
        }
    }
}

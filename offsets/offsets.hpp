#pragma once

#include <stddef.h>
#include <stdint.h>

namespace cs2_dumper {
    namespace offsets {
        // Module: client.dll
        namespace client_dll {
            constexpr ptrdiff_t dwCSGOInput = 0x2340E00;
            constexpr ptrdiff_t dwEntityList = 0x24D1DF0;
            constexpr ptrdiff_t dwGameEntitySystem = 0x24D1DF0;
            constexpr ptrdiff_t dwGameEntitySystem_highestEntityIndex = 0x2090;
            constexpr ptrdiff_t dwGameRules = 0x232BFB8;
            constexpr ptrdiff_t dwGlobalVars = 0x204C5D8;
            constexpr ptrdiff_t dwGlowManager = 0x2328DB0;
            constexpr ptrdiff_t dwLocalPlayerController = 0x230B5D0;
            constexpr ptrdiff_t dwLocalPlayerPawn = 0x2057720;
            constexpr ptrdiff_t dwPlantedC4 = 0x2339AC8;
            constexpr ptrdiff_t dwPrediction = 0x2057630;
            constexpr ptrdiff_t dwSensitivity = 0x23298C8;
            constexpr ptrdiff_t dwSensitivity_sensitivity = 0x58;
            constexpr ptrdiff_t dwViewAngles = 0x2341488;
            constexpr ptrdiff_t dwViewMatrix = 0x2331B30;
            constexpr ptrdiff_t dwViewRender = 0x2330D38;
            constexpr ptrdiff_t dwWeaponC4 = 0x22A9D58;
        }
        // Module: engine2.dll
        namespace engine2_dll {
            constexpr ptrdiff_t dwBuildNumber = 0x60CC74;
            constexpr ptrdiff_t dwNetworkGameClient = 0x90A0C0;
            constexpr ptrdiff_t dwNetworkGameClient_clientTickCount = 0x378;
            constexpr ptrdiff_t dwNetworkGameClient_deltaTick = 0x24C;
            constexpr ptrdiff_t dwNetworkGameClient_isBackgroundMap = 0x2C141F;
            constexpr ptrdiff_t dwNetworkGameClient_localPlayer = 0xF8;
            constexpr ptrdiff_t dwNetworkGameClient_maxClients = 0x240;
            constexpr ptrdiff_t dwNetworkGameClient_serverTickCount = 0x24C;
            constexpr ptrdiff_t dwNetworkGameClient_signOnState = 0x230;
            constexpr ptrdiff_t dwWindowHeight = 0x90E4EC;
            constexpr ptrdiff_t dwWindowWidth = 0x90E4E8;
        }
        // Module: inputsystem.dll
        namespace inputsystem_dll {
            constexpr ptrdiff_t dwInputSystem = 0x42B50;
        }
        // Module: matchmaking.dll
        namespace matchmaking_dll {
            constexpr ptrdiff_t dwGameTypes = 0x1B0F80;
        }
        // Module: soundsystem.dll
        namespace soundsystem_dll {
            constexpr ptrdiff_t dwSoundSystem = 0x512360;
            constexpr ptrdiff_t dwSoundSystem_engineViewData = 0x7C;
        }
    }
}

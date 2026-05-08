// Generated using https://github.com/a2x/cs2-dumper
// 2026-05-02 22:51:30.129229100 UTC

#pragma once

#include <stddef.h>
#include <stdint.h>

namespace cs2_dumper {
    namespace interfaces {
        // Module: animationsystem.dll
        namespace animationsystem_dll {
            constexpr ptrdiff_t AnimationSystemUtils_001 = 0x812190;
            constexpr ptrdiff_t AnimationSystem_001 = 0x80A0B0;
        }
        // Module: client.dll
        namespace client_dll {
            constexpr ptrdiff_t ClientToolsInfo_001 = 0x20501C0;
            constexpr ptrdiff_t EmptyWorldService001_Client = 0x2013A90;
            constexpr ptrdiff_t GameClientExports001 = 0x204CE60;
            constexpr ptrdiff_t LegacyGameUI001 = 0x20685E0;
            constexpr ptrdiff_t Source2Client002 = 0x2326F90;
            constexpr ptrdiff_t Source2ClientConfig001 = 0x2298FD0;
            constexpr ptrdiff_t Source2ClientPrediction001 = 0x2057630;
            constexpr ptrdiff_t Source2ClientUI001 = 0x2066B40;
        }
        // Module: engine2.dll
        namespace engine2_dll {
            constexpr ptrdiff_t BenchmarkService001 = 0x612950;
            constexpr ptrdiff_t BugService001 = 0x8CA900;
            constexpr ptrdiff_t ClientServerEngineLoopService_001 = 0x90C000;
            constexpr ptrdiff_t ClientServerSharedHandleSystem001 = 0x90B5B0;
            constexpr ptrdiff_t EngineGameUI001 = 0x6102A0;
            constexpr ptrdiff_t EngineServiceMgr001 = 0x90B8E0;
            constexpr ptrdiff_t GameEventSystemClientV001 = 0x90BBC0;
            constexpr ptrdiff_t GameEventSystemServerV001 = 0x90BCF0;
            constexpr ptrdiff_t GameResourceServiceClientV001 = 0x612A50;
            constexpr ptrdiff_t GameResourceServiceServerV001 = 0x612AB0;
            constexpr ptrdiff_t GameUIService_001 = 0x8CAD30;
            constexpr ptrdiff_t HostStateMgr001 = 0x613290;
            constexpr ptrdiff_t INETSUPPORT_001 = 0x60C040;
            constexpr ptrdiff_t InputService_001 = 0x8CB020;
            constexpr ptrdiff_t KeyValueCache001 = 0x613340;
            constexpr ptrdiff_t MapListService_001 = 0x909E90;
            constexpr ptrdiff_t NetworkClientService_001 = 0x90A020;
            constexpr ptrdiff_t NetworkP2PService_001 = 0x90A360;
            constexpr ptrdiff_t NetworkServerService_001 = 0x90A510;
            constexpr ptrdiff_t NetworkService_001 = 0x612C20;
            constexpr ptrdiff_t RenderService_001 = 0x90A780;
            constexpr ptrdiff_t ScreenshotService001 = 0x90AA40;
            constexpr ptrdiff_t SimpleEngineLoopService_001 = 0x6133A0;
            constexpr ptrdiff_t SoundService_001 = 0x612C60;
            constexpr ptrdiff_t Source2EngineToClient001 = 0x60FBA0;
            constexpr ptrdiff_t Source2EngineToClientStringTable001 = 0x60FC00;
            constexpr ptrdiff_t Source2EngineToServer001 = 0x60FC78;
            constexpr ptrdiff_t Source2EngineToServerStringTable001 = 0x60FCA0;
            constexpr ptrdiff_t SplitScreenService_001 = 0x612F40;
            constexpr ptrdiff_t StatsService_001 = 0x90AE00;
            constexpr ptrdiff_t ToolService_001 = 0x613100;
            constexpr ptrdiff_t VENGINE_GAMEUIFUNCS_VERSION005 = 0x610330;
            constexpr ptrdiff_t VProfService_001 = 0x613140;
        }
        // Module: filesystem_stdio.dll
        namespace filesystem_stdio_dll {
            constexpr ptrdiff_t VAsyncFileSystem2_001 = 0x2159E0;
            constexpr ptrdiff_t VFileSystem017 = 0x2157A0;
        }
        // Module: host.dll
        namespace host_dll {
            constexpr ptrdiff_t DebugDrawQueueManager001 = 0x139060;
            constexpr ptrdiff_t GameModelInfo001 = 0x1390A0;
            constexpr ptrdiff_t GameSystem2HostHook = 0x1390E0;
            constexpr ptrdiff_t HostUtils001 = 0x148AB0;
            constexpr ptrdiff_t PredictionDiffManager001 = 0x1391F0;
            constexpr ptrdiff_t SaveRestoreDataVersion001 = 0x139320;
            constexpr ptrdiff_t SinglePlayerSharedMemory001 = 0x139350;
            constexpr ptrdiff_t Source2Host001 = 0x1393C0;
        }
        // Module: imemanager.dll
        namespace imemanager_dll {
            constexpr ptrdiff_t IMEManager001 = 0x36B20;
        }
        // Module: inputsystem.dll
        namespace inputsystem_dll {
            constexpr ptrdiff_t InputStackSystemVersion001 = 0x40E30;
            constexpr ptrdiff_t InputSystemVersion001 = 0x42B50;
        }
        // Module: localize.dll
        namespace localize_dll {
            constexpr ptrdiff_t Localize_001 = 0x62180;
        }
        // Module: matchmaking.dll
        namespace matchmaking_dll {
            constexpr ptrdiff_t GameTypes001 = 0x1B0F80;
            constexpr ptrdiff_t MATCHFRAMEWORK_001 = 0x1B9060;
        }
        // Module: materialsystem2.dll
        namespace materialsystem2_dll {
            constexpr ptrdiff_t FontManager_001 = 0x15DE60;
            constexpr ptrdiff_t MaterialUtils_001 = 0x145D40;
            constexpr ptrdiff_t PostProcessingSystem_001 = 0x145C50;
            constexpr ptrdiff_t TextLayout_001 = 0x145CD0;
            constexpr ptrdiff_t VMaterialSystem2_001 = 0x15D750;
        }
        // Module: meshsystem.dll
        namespace meshsystem_dll {
            constexpr ptrdiff_t MeshSystem001 = 0x150C20;
        }
        // Module: navsystem.dll
        namespace navsystem_dll {
            constexpr ptrdiff_t NavSystem001 = 0x122930;
        }
        // Module: networksystem.dll
        namespace networksystem_dll {
            constexpr ptrdiff_t FlattenedSerializersVersion001 = 0x26D700;
            constexpr ptrdiff_t NetworkMessagesVersion001 = 0x2959D0;
            constexpr ptrdiff_t NetworkSystemVersion001 = 0x286E50;
            constexpr ptrdiff_t SerializedEntitiesVersion001 = 0x286F40;
        }
        // Module: panorama.dll
        namespace panorama_dll {
            constexpr ptrdiff_t PanoramaUIEngine001 = 0x506D30;
        }
        // Module: panorama_text_pango.dll
        namespace panorama_text_pango_dll {
            constexpr ptrdiff_t PanoramaTextServices001 = 0x2B8A40;
        }
        // Module: panoramauiclient.dll
        namespace panoramauiclient_dll {
            constexpr ptrdiff_t PanoramaUIClient001 = 0x296420;
        }
        // Module: particles.dll
        namespace particles_dll {
            constexpr ptrdiff_t ParticleSystemMgr003 = 0x54A3C0;
        }
        // Module: pulse_system.dll
        namespace pulse_system_dll {
            constexpr ptrdiff_t IPulseSystem_001 = 0x1F36A0;
        }
        // Module: rendersystemdx11.dll
        namespace rendersystemdx11_dll {
            constexpr ptrdiff_t RenderDeviceMgr001 = 0x42B530;
            constexpr ptrdiff_t RenderUtils_001 = 0x42BE28;
            constexpr ptrdiff_t VRenderDeviceMgrBackdoor001 = 0x42B5D0;
        }
        // Module: resourcesystem.dll
        namespace resourcesystem_dll {
            constexpr ptrdiff_t ResourceSystem013 = 0x831E0;
        }
        // Module: scenefilecache.dll
        namespace scenefilecache_dll {
            constexpr ptrdiff_t ResponseRulesCache001 = 0xF58F0;
            constexpr ptrdiff_t SceneFileCache002 = 0xF5A78;
        }
        // Module: scenesystem.dll
        namespace scenesystem_dll {
            constexpr ptrdiff_t RenderingPipelines_001 = 0x663BA0;
            constexpr ptrdiff_t SceneSystem_002 = 0x8DB490;
            constexpr ptrdiff_t SceneUtils_001 = 0x664AB0;
        }
        // Module: schemasystem.dll
        namespace schemasystem_dll {
            constexpr ptrdiff_t SchemaSystem_001 = 0x76800;
        }
        // Module: server.dll
        namespace server_dll {
            constexpr ptrdiff_t EmptyWorldService001_Server = 0x1BEFD30;
            constexpr ptrdiff_t EntitySubclassUtilsV001 = 0x1B9B3B0;
            constexpr ptrdiff_t NavGameTest001 = 0x1C9C1D0;
            constexpr ptrdiff_t ServerToolsInfo_001 = 0x1C491B8;
            constexpr ptrdiff_t Source2GameClients001 = 0x1C47C10;
            constexpr ptrdiff_t Source2GameDirector001 = 0x1DE17F0;
            constexpr ptrdiff_t Source2GameEntities001 = 0x1C488C0;
            constexpr ptrdiff_t Source2Server001 = 0x1C48710;
            constexpr ptrdiff_t Source2ServerConfig001 = 0x1EFEA98;
            constexpr ptrdiff_t customnavsystem001 = 0x1B7C698;
        }
        // Module: soundsystem.dll
        namespace soundsystem_dll {
            constexpr ptrdiff_t SoundOpSystem001 = 0x5129C0;
            constexpr ptrdiff_t SoundOpSystemEdit001 = 0x512880;
            constexpr ptrdiff_t SoundSystem001 = 0x512360;
            constexpr ptrdiff_t VMixEditTool001 = 0x59487BF;
        }
        // Module: steamaudio.dll
        namespace steamaudio_dll {
            constexpr ptrdiff_t SteamAudio001 = 0x25E620;
        }
        // Module: steamclient64.dll
        namespace steamclient64_dll {
            constexpr ptrdiff_t IVALIDATE001 = 0x16BD098;
            constexpr ptrdiff_t SteamClient006 = 0x16BA500;
            constexpr ptrdiff_t SteamClient007 = 0x16BA508;
            constexpr ptrdiff_t SteamClient008 = 0x16BA510;
            constexpr ptrdiff_t SteamClient009 = 0x16BA518;
            constexpr ptrdiff_t SteamClient010 = 0x16BA520;
            constexpr ptrdiff_t SteamClient011 = 0x16BA528;
            constexpr ptrdiff_t SteamClient012 = 0x16BA530;
            constexpr ptrdiff_t SteamClient013 = 0x16BA538;
            constexpr ptrdiff_t SteamClient014 = 0x16BA540;
            constexpr ptrdiff_t SteamClient015 = 0x16BA548;
            constexpr ptrdiff_t SteamClient016 = 0x16BA550;
            constexpr ptrdiff_t SteamClient017 = 0x16BA558;
            constexpr ptrdiff_t SteamClient018 = 0x16BA560;
            constexpr ptrdiff_t SteamClient019 = 0x16BA568;
            constexpr ptrdiff_t SteamClient020 = 0x16BA570;
            constexpr ptrdiff_t SteamClient021 = 0x16BA578;
            constexpr ptrdiff_t SteamClient022 = 0x16BA580;
            constexpr ptrdiff_t SteamClient023 = 0x16BA588;
            constexpr ptrdiff_t p2pvoice002 = 0x14E627F;
            constexpr ptrdiff_t p2pvoicesingleton002 = 0x16950F0;
        }
        // Module: tier0.dll
        namespace tier0_dll {
            constexpr ptrdiff_t TestScriptMgr001 = 0x39E6F0;
            constexpr ptrdiff_t VEngineCvar007 = 0x3A93B0;
            constexpr ptrdiff_t VProcessUtils002 = 0x39E690;
            constexpr ptrdiff_t VStringTokenSystem001 = 0x3D00B0;
        }
        // Module: v8system.dll
        namespace v8system_dll {
            constexpr ptrdiff_t Source2V8System001 = 0x31730;
        }
        // Module: vphysics2.dll
        namespace vphysics2_dll {
            constexpr ptrdiff_t VPhysics2_Interface_001 = 0x40DDA0;
        }
        // Module: vscript.dll
        namespace vscript_dll {
            constexpr ptrdiff_t VScriptManager010 = 0x13B410;
        }
        // Module: vstdlib_s64.dll
        namespace vstdlib_s64_dll {
            constexpr ptrdiff_t IVALIDATE001 = 0x6F990;
            constexpr ptrdiff_t VEngineCvar002 = 0x6E070;
        }
        // Module: worldrenderer.dll
        namespace worldrenderer_dll {
            constexpr ptrdiff_t WorldRendererMgr001 = 0x225C40;
        }
    }
}

#include "../utils/common.h"
#include "../utils/offsets.h"
#include <intrin.h>

namespace entities {
SimpleVector<PLAYER_DATA> *players_ptr = nullptr;
PLAYER_DATA local_player;
int count = 0;

extern "C" void UpdateEntities() {
  if (!g_client_base || !cs2) {
    return;
  }

  if (!entities::players_ptr) {
    entities::players_ptr = (SimpleVector<PLAYER_DATA> *)ExAllocatePool2(POOL_FLAG_PAGED, sizeof(SimpleVector<PLAYER_DATA>), 'tceV');
    if (entities::players_ptr) {
      new (entities::players_ptr) SimpleVector<PLAYER_DATA>();
    }
  }

  if (!entities::players_ptr) {
    return;
  }
  auto &players = *entities::players_ptr;

  ULONG_PTR entity_list = R<ULONG_PTR>(g_client_base + Offsets::dwEntityList);
  ULONG_PTR local_pawn = R<ULONG_PTR>(g_client_base + Offsets::dwLocalPlayerPawn);
  ULONG_PTR local_controller = R<ULONG_PTR>(g_client_base + Offsets::dwLocalPlayerController);

  if (!entity_list)
    return;

  entities::local_player.pawn_address = local_pawn; // local player pawn
  entities::local_player.team = R<int>(local_pawn + Offsets::BaseEntity::m_iTeamNum);

  ULONG_PTR local_scene_node = R<ULONG_PTR>(local_pawn + Offsets::BaseEntity::m_pGameSceneNode);
  if (local_scene_node) {
    entities::local_player.origin = R<V3>(local_scene_node + Offsets::SceneNode::m_vecAbsOrigin);
  }
  players.clear();
  constexpr size_t STRIDE = 0x70;

  ULONG_PTR controller_page = R<ULONG_PTR>(entity_list + 0x10);
  if (!controller_page) return;

  for (int i = 1; i < 64; ++i) {
    ULONG_PTR controller = R<ULONG_PTR>(controller_page + i * STRIDE);
    if (!controller) continue;
    if (controller == local_controller) continue;

    uint32_t pawn_handle = R<uint32_t>(controller + 0x904);
    if (!pawn_handle || pawn_handle == 0xFFFFFFFF) {
      pawn_handle = R<uint32_t>(controller + Offsets::Controller::m_pControllerPawn);
      if (!pawn_handle || pawn_handle == 0xFFFFFFFF) continue;
    }

    uint32_t pawn_index = pawn_handle & 0x7FFF;
    ULONG_PTR pawn_page_ptr = R<ULONG_PTR>(entity_list + 0x10 + (0x8 * (pawn_index >> 9)));
    if (!pawn_page_ptr) continue;

    ULONG_PTR pawn = R<ULONG_PTR>(pawn_page_ptr + (pawn_index & 0x1FF) * STRIDE);
    if (!pawn || pawn == local_pawn) continue;

    int health = R<int>(pawn + Offsets::BaseEntity::m_iHealth);
    if (health <= 0 || health > 100) continue;

    int team = (int)R<uint8_t>(pawn + Offsets::BaseEntity::m_iTeamNum);

    PLAYER_DATA p{};
    p.valid = TRUE;
    p.health = health;
    p.team = team;
    p.pawn_address = pawn;
    p.controller_address = controller;
    p.origin = R<V3>(pawn + 0x1390);

    ULONG_PTR scene_node = R<ULONG_PTR>(pawn + Offsets::BaseEntity::m_pGameSceneNode);// vts 
    if (scene_node) {
      ULONG_PTR bone_array = R<ULONG_PTR>(scene_node + 0x1D0);
      if (bone_array) {
        p.head_pos = R<V3>(bone_array + 6 * 32);

        for (int j = 0; j < MAX_BONES; j++) {
          p.bones[j].Pos = R<V3>(bone_array + j * 32);
        }
      } else {
        p.head_pos = p.origin;
        p.head_pos.z += 65.0f;
      }
    } else {
      p.head_pos = p.origin;
      p.head_pos.z += 65.0f;
    }
    ULONG_PTR name_ptr =
        controller + Offsets::Controller::m_sSanitizedPlayerName;
    ReadMemory(name_ptr, p.name, 31 * sizeof(wchar_t));
    p.name[31] = 0;

    players.push_back(p);
  }

  entities::count = (int)players.size();
}
}
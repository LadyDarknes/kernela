#include "../utils/common.h"
#include "../utils/offsets.h"

extern "C" void mouse_move(long x, long y, unsigned short button_flags);
extern BOOLEAN g_Aim;
extern BOOLEAN g_Unload;
static __forceinline void Sleep(LONGLONG ms) {
    LARGE_INTEGER interval;
    interval.QuadPart = -(ms * 10000);
    KeDelayExecutionThread(KernelMode, FALSE, &interval);
}

namespace entities {
extern SimpleVector<PLAYER_DATA> *playersPtr;
extern PLAYER_DATA localPlayer;
extern int count;
extern "C" void UpdateEntities();
extern "C" void UpdatePlist();
} 

bool W2S(const V3& pos, const view_matrix_t& matrix, V2& out) {
    if (pos.x == 0 && pos.y == 0 && pos.z == 0) return false;

    float screen_x = matrix.matrix[0][0] * pos.x + matrix.matrix[0][1] * pos.y + matrix.matrix[0][2] * pos.z + matrix.matrix[0][3];
    float screen_y = matrix.matrix[1][0] * pos.x + matrix.matrix[1][1] * pos.y + matrix.matrix[1][2] * pos.z + matrix.matrix[1][3];
    float w = matrix.matrix[3][0] * pos.x + matrix.matrix[3][1] * pos.y + matrix.matrix[3][2] * pos.z + matrix.matrix[3][3];

    if (w < 0.01f) return false;

    float inv_w = 1.0f / w;
    screen_x *= inv_w;
    screen_y *= inv_w;

    constexpr float width = 1920.0f;
    constexpr float height = 1080.0f;
    float x = width * 0.5f;
    float y = height * 0.5f;

    x += 0.5f * screen_x * width;
    y -= 0.5f * screen_y * height;

    out.x = x;
    out.y = y;

    return true;
}

void RunAimbot() {
    view_matrix_t vm;
    if (!ReadMemory(g_client_base + Offsets::dwViewMatrix, &vm, sizeof(vm))) return;

    constexpr float width = 1920.0f;
    constexpr float height = 1080.0f;
    constexpr float fov = 150.0f;
    constexpr float fov_sq = fov * fov;
    V2 screen_center = { width * 0.5f, height * 0.5f };
    float best_dist = 99999.0f;
    V2 best_target = { 0, 0 };
    bool found = false;

    if (!entities::playersPtr)
        return;
    auto& players = *entities::playersPtr;

    for (const auto& p : players) {
        if (p.team == entities::localPlayer.team) continue;

        V2 screen_pos;
        if (W2S(p.head_pos, vm, screen_pos)) {
            float dx = screen_pos.x - screen_center.x;
            float dy = screen_pos.y - screen_center.y;
            float dist_sq = dx * dx + dy * dy;

            if (dist_sq < best_dist && dist_sq < fov_sq) {
                best_dist = dist_sq;
                best_target = screen_pos;
                found = true;
            }
        }
    }

    if (found) {
        float dx = best_target.x - screen_center.x;
        float dy = best_target.y - screen_center.y;
        mouse_move((long)(dx / 1.5f), (long)(dy / 1.5f), 0);
    }
}


extern "C" void CheatLoop(PVOID sc) {
  UNREFERENCED_PARAMETER(sc);
  DbgPrint("[+] Cheat loop started\n");
  int plistDelay = 0;
  while (TRUE) {
	  if (g_Unload){
      while (true) {
		  Sleep(100000000);
	  }
	}
    if (!cs2) {
      InitProcessAccess();
      Sleep(10);
    } else {
      if (!g_client_base) {
        g_client_base = GetModuleBase(cs2, L"client.dll");
        if (g_client_base)
          DbgPrint("[+] client.dll found: 0x%llX\n", g_client_base);
        Sleep(10);
      } else {
        entities::UpdateEntities();
        if (plistDelay <= 0) {
          entities::UpdatePlist();
          plistDelay = 300;
        } else {
          --plistDelay;
        }
        if(g_Aim)
         RunAimbot();
         Sleep(1);
      }
    }
  }
}

/**
 * @file debug_frame.h
 * @author KOOLER FAN
 * @date 2025/09/09
 */
#ifndef DEBUG_FRAME
#define DEBUG_FRAME

#include <d3d11.h>
#include "collision.h"

void DebugFrame_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
void DebugFrame_Finalize(void);
void DebugFrame_Update(double elapsed_time);

void DebugFrame_AABB_Draw(const AABB& aabb);

#endif // DEBUG_FRAME

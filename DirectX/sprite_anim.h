/**
 * @file sprite_anim.h
 * @brief スプライトアニメーション描画
 * @author KOOLER FAN
 * @date 2025/06/17
 */

#ifndef SPRITE_ANIM_H
#define SPRITE_ANIM_H

void SpriteAnim_Initialize();
void SpriteAnim_Finalize(void);

void SpriteAnim_Update(double elapsed_time);
void SpriteAnim_Draw(int playid, float dx, float dy, float dw, float dh);

#endif

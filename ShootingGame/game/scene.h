/**
 * @file scene.h
 * @brief 画面遷移制御
 * @author KOOLER FAN
 * @date 2025/07/10
 */

#ifndef SCENE_H
#define SCENE_H

void Scene_Initialize(void);
void Scene_Finalize(void);
void Scene_Update(double elapsed_time);
void Scene_Draw(void);
void Scene_Refresh(void);

enum class Scene
{
    TITLE,
    GAME,
    RESULT,
    MAX,
};

void Scene_Change(Scene scene);

#endif

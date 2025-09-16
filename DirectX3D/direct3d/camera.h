/**
 * @file camera.h
 * @brief カメラ制御
 * 
 * @author KOOLER FAN
 * @date 2025/09/11
 */

#ifndef CAMERA_H
#define CAMERA_H

void Camera_Initialize(void);
void Camera_Finalize(void);
void Camera_Update(double elapsed_time);

#endif // CAMERA_H

#ifndef FLATCLOTH_CAMERA_SYSTEM_H
#define FLATCLOTH_CAMERA_SYSTEM_H

#include "ecs.h"

constexpr auto PITCH_MIN = -89.0f;
constexpr auto PITCH_MAX = 89.0f;
constexpr auto YAW_MIN = -180.0f;
constexpr auto YAW_MAX = 180.0f;
constexpr auto ROLL_MIN = 0.0f;
constexpr auto ROLL_MAX = 360.0f;

#define UNWIND(val) (val - (int)(val / 180) * 360.0f)

class CameraSystem : public System
{
public:
        void init();
        void update_camera_vectors();
        void temporary_steering(float dt);
        void mouse_movement(int dx, int dy);
};

#endif // FLATCLOTH_CAMERA_SYSTEM_H


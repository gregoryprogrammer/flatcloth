#ifndef FLATCLOTH_DYNAMIC_SYSTEM_H
#define FLATCLOTH_DYNAMIC_SYSTEM_H

#include "ecs.h"

class DynamicSystem : public System
{
public:
    void pre_update();
    void post_update();
};

#endif // FLATCLOTH_DYNAMIC_SYSTEM_H

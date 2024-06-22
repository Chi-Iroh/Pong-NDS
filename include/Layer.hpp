#pragma once
#include <nds.h>

/**
 * A convenient way to represent drawing priority is to think about it as layers.
 * libnds functions for backgrounds and sprites (as oamSet, for instance) expect a drawing priority from 0 to 3, 0 being the highest and 3 the lowest.
 * A high priority might be counter-intuitive as it means drawn last, thus it'll be rendered over sprites with lower priority.
 * On the contrary, a low priority means drawn first, but another sprite/background will eventually be drawn on top of it, so be careful not to accidentally hide your sprites/backgrounds.
 *
 * As said right above, there are 4 priority levels (from 0 to 3).
 * They can be defined as 4 layers : 0 = upper layer (top) > 1 = upper mid layer > 2 = lower mid layer > 3 = lower layer (bottom)
 * For instance, each sprite drawn on the upper layer (priority 0) will be drawn on top of every sprite on the other layers.
 * On a same layer/priority, if two sprites overlap, the last drawn sprite (last call of oamSet) is displayed on top of the other one.
*/
enum Layer {
    UPPER_LAYER = OBJPRIORITY_0,
    UPPER_MID_LAYER = OBJPRIORITY_1,
    LOWER_MID_LAYER = OBJPRIORITY_2,
    LOWER_LAYER = OBJPRIORITY_3
};
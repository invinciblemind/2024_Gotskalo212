#include "Class_Gun.h"
#include <cstring>
#include "Class_152mm_M-10_gun.h"

gun_152mm_M_10::gun_152mm_M_10() {
    strcpy(g_name, "152мм М-10");
    g_damage = 640;
    g_cooldown = 22.01;
    g_break_through = 5.95;
}

#include "Class_Gun.h"
#include <cstring>
#include "Class_122mm_U-11_gun.h"

gun_122mm_U_11::gun_122mm_U_11() {
    strcpy(g_name, "122мм У-11");
    g_damage = 230;
    g_cooldown = 21.57;
    g_break_through = 7;
}

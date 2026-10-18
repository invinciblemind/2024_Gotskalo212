#ifndef __C_Gun
#define __C_Gun

class Gun {
public:
    char g_name[20];
    int g_damage;
    double g_cooldown, g_break_through;
    
    void print_gun_info();
};

#endif

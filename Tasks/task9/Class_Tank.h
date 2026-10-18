#include "Abs_Class_print.h"

#ifndef __C_Tank
#define __C_Tank

class Tank: Abs_Class_print {
public:
    int health, damage;
    double cooldown, armor, break_through;
    char name[20];
    
    virtual void print();
    
    virtual void install_gun(int number){}
    
    virtual void print_about_guns(){}
};

void PvP(Tank tank1, Tank tank2);

void Battle(Tank *team_red, const char *red_strategy, Tank *team_blue, const char *blue_strategy);

#endif

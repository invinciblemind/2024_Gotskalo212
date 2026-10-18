#include "Class_Tank.h"
#include "Class_85mm_D-5S_gun.h"
#include "Class_85mm_D-5S-85BM_gun.h"
#include "Class_SU-85.h"
#include <cstring>
#include <iostream>
using namespace std;

SU_85::SU_85() {
    strcpy(name, "СУ-85");
    health = 590;
    armor = 4.7;
    damage = gun_85mm_D_5S::g_damage;
    cooldown = gun_85mm_D_5S::g_cooldown;
    break_through = gun_85mm_D_5S::g_break_through;
}

void SU_85::install_gun(int number) {
    if (number == 1) {
        damage = gun_85mm_D_5S::g_damage;
        cooldown = gun_85mm_D_5S::g_cooldown;
        break_through = gun_85mm_D_5S::g_break_through;
    }
    else if (number == 2) {
        damage = gun_85mm_D_5S_85BM::g_damage;
        cooldown = gun_85mm_D_5S_85BM::g_cooldown;
        break_through = gun_85mm_D_5S_85BM::g_break_through;
    }
}

void SU_85::print() {
    cout<<endl;
    cout<<"Танк: "<<name<<endl;
    cout<<"Уровень: "<<5<<endl;
    cout<<"Тип: "<<"пт-сау"<<endl;
    Tank::print();
}

void SU_85::print_about_guns() {
    cout<<"Орудия танка "<<name<<": "<<endl;;
    cout<<"  1:";
    gun_85mm_D_5S::print_gun_info();
    cout<<"  2:";
    gun_85mm_D_5S_85BM::print_gun_info();
}

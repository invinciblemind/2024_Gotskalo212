#include "Class_Tank.h"
#include "Class_85mm_D-5S_gun.h"
#include "Class_85mm_D-5S-85BM_gun.h"
#include "Class_100mm_D-10S_gun.h"
#include "Class_122mm_D2-5S_gun.h"
#include "Class_SU-100.h"
#include <cstring>
#include <iostream>
using namespace std;

SU_100::SU_100() {
    strcpy(name, "СУ-100");
    health = 850;
    armor = 7.9;
    damage = gun_85mm_D_5S::g_damage;
    cooldown = gun_85mm_D_5S::g_cooldown;
    break_through = gun_85mm_D_5S::g_break_through;
}

void SU_100::install_gun(int number) {
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
    else if (number == 3) {
        damage = gun_100mm_D_10S::g_damage;
        cooldown = gun_100mm_D_10S::g_cooldown;
        break_through = gun_100mm_D_10S::g_break_through;
    }
    else if (number == 4) {
        damage = gun_122mm_D2_5S::g_damage;
        cooldown = gun_122mm_D2_5S::g_cooldown;
        break_through = gun_122mm_D2_5S::g_break_through;
    }
}

void SU_100::print() {
    cout<<endl;
    cout<<"Танк: "<<name<<endl;
    cout<<"Уровень: "<<6<<endl;
    cout<<"Тип: "<<"пт-сау"<<endl;
    Tank::print();
}

void SU_100::print_about_guns() {
    cout<<"Орудия танка "<<name<<": "<<endl;;
    cout<<"  1:";
    gun_85mm_D_5S::print_gun_info();
    cout<<"  2:";
    gun_85mm_D_5S_85BM::print_gun_info();
    cout<<"  3:";
    gun_100mm_D_10S::print_gun_info();
    cout<<"  4:";
    gun_122mm_D2_5S::print_gun_info();
}

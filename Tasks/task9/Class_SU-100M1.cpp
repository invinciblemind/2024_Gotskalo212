#include "Class_Tank.h"
#include "Class_100mm_D-54S_gun.h"
#include "Class_100mm_D-10S_gun.h"
#include "Class_SU-100M1.h"
#include <cstring>
#include <iostream>
using namespace std;

SU_100M1::SU_100M1() {
    strcpy(name, "СУ-100М1");
    health = 1150;
    armor = 9;
    damage = gun_100mm_D_10S::g_damage;
    cooldown = gun_100mm_D_10S::g_cooldown;
    break_through = gun_100mm_D_10S::g_break_through;
}

void SU_100M1::install_gun(int number) {
    if (number == 1) {
        damage = gun_100mm_D_10S::g_damage;
        cooldown = gun_100mm_D_10S::g_cooldown;
        break_through = gun_100mm_D_10S::g_break_through;
    }
    else if (number == 2) {
        damage = gun_100mm_D_54S::g_damage;
        cooldown = gun_100mm_D_54S::g_cooldown;
        break_through = gun_100mm_D_54S::g_break_through;
    }
}

void SU_100M1::print() {
    cout<<endl;
    cout<<"Танк: "<<name<<endl;
    cout<<"Уровень: "<<7<<endl;
    cout<<"Тип: "<<"пт-сау"<<endl;
    Tank::print();
}

void SU_100M1::print_about_guns() {
    cout<<"Орудия танка "<<name<<": "<<endl;;
    cout<<"  1:";
    gun_100mm_D_10S::print_gun_info();
    cout<<"  2:";
    gun_100mm_D_54S::print_gun_info();
}

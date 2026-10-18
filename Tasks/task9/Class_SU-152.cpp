#include "Class_Tank.h"
#include "Class_152mm_ML-20_gun.h"
#include "Class_122mm_D-25S_gun.h"
#include "Class_SU-152.h"
#include <cstring>
#include <iostream>
using namespace std;

SU_152::SU_152() {
    strcpy(name, "СУ-152");
    health = 1040;
    armor = 7.9;
    damage = gun_152mm_ML_20::g_damage;
    cooldown = gun_152mm_ML_20::g_cooldown;
    break_through = gun_152mm_ML_20::g_break_through;
}

void SU_152::install_gun(int number) {
    if (number == 1) {
        damage = gun_152mm_ML_20::g_damage;
        cooldown = gun_152mm_ML_20::g_cooldown;
        break_through = gun_152mm_ML_20::g_break_through;
    }
    else if (number == 2) {
        damage = gun_122mm_D_25S::g_damage;
        cooldown = gun_122mm_D_25S::g_cooldown;
        break_through = gun_122mm_D_25S::g_break_through;
    }
}

void SU_152::print() {
    cout<<endl;
    cout<<"Танк: "<<name<<endl;
    cout<<"Уровень: "<<7<<endl;
    cout<<"Тип: "<<"пт-сау"<<endl;
    Tank::print();
}

void SU_152::print_about_guns() {
    cout<<"Орудия танка "<<name<<": "<<endl;;
    cout<<"  1:";
    gun_152mm_ML_20::print_gun_info();
    cout<<"  2:";
    gun_122mm_D_25S::print_gun_info();
}

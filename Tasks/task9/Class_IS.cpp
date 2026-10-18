#include "Class_Tank.h"
#include "Class_100mm_D-10T_gun.h"
#include "Class_122mm_D2-5T_gun.h"
#include "Class_122mm_D-25T_gun.h"
#include "Class_IS.h"
#include <cstring>
#include <iostream>
using namespace std;

IS::IS() {
    strcpy(name, "ИС");
    health = 1400;
    armor = 15.5;
    damage = gun_100mm_D_10T::g_damage;
    cooldown = gun_100mm_D_10T::g_cooldown;
    break_through = gun_100mm_D_10T::g_break_through;
}

void IS::install_gun(int number) {
    if (number == 1) {
        damage = gun_100mm_D_10T::g_damage;
        cooldown = gun_100mm_D_10T::g_cooldown;
        break_through = gun_100mm_D_10T::g_break_through;
    }
    else if (number == 2) {
        damage = gun_122mm_D2_5T::g_damage;
        cooldown = gun_122mm_D2_5T::g_cooldown;
        break_through = gun_122mm_D2_5T::g_break_through;
    }
    else if (number == 3) {
        damage = gun_122mm_D_25T::g_damage;
        cooldown = gun_122mm_D_25T::g_cooldown;
        break_through = gun_122mm_D_25T::g_break_through;
    }
}

void IS::print() {
    cout<<endl;
    cout<<"Танк: "<<name<<endl;
    cout<<"Уровень: "<<7<<endl;
    cout<<"Тип: "<<"тяжелый"<<endl;
    Tank::print();
}

void IS::print_about_guns() {
    cout<<"Орудия танка "<<name<<": "<<endl;;
    cout<<"  1:";
    gun_100mm_D_10T::print_gun_info();
    cout<<"  2:";
    gun_122mm_D2_5T::print_gun_info();
    cout<<"  3:";
    gun_122mm_D_25T::print_gun_info();
}

#include "Class_Gun.h"
#include <cstring>
#include <iostream>
using namespace std;

void Gun::print_gun_info() {
    cout<<endl;
    cout<<"   Орудие: "<<g_name<<endl;
    cout<<"   Урон: "<<g_damage<<endl;
    cout<<"   Время перезарядки: "<<g_cooldown<<endl;
    cout<<"   Пробитие: "<<g_break_through<<endl;
    cout<<"   ДПМ: "<<(int)(g_damage * 60 / g_cooldown)<<endl;
    cout<<endl;
}

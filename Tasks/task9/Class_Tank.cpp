#include "Abs_Class_print.h"
#include "Class_Tank.h"
#include <ctime>
#include <iostream>
#include <iomanip>
#include <cstring>
using namespace std;

void Tank::print() {
    cout<<"   Прочность: "<<health<<endl;
    cout<<"   Броня: "<<armor<<endl;
    cout<<"   Урон: "<<damage<<endl;
    cout<<"   Пробитие: "<<break_through<<endl;
    cout<<"   Время перезарядки: "<<cooldown<<endl;
    cout<<"   ДПМ: "<<(int)(damage * 60 / cooldown)<<endl;
    cout<<endl;
}

void PvP(Tank tank1, Tank tank2) {
    double i, time1 = tank1.cooldown, time2 = tank2.cooldown, ttime, time_interval = 1, vbr, cur_damage;
    cout<<"         PVP: "<<tank1.name<<" vs "<<tank2.name<<endl;
    cout<<tank1.name<<":"<<endl;
    cout<<"Коэффициент пробития (чем больше, тем выше шанс пробития): "<<(2 + (int)(11-tank2.armor+tank1.break_through) / 2)<<endl;
    tank1.print();
    cout<<tank2.name<<":"<<endl;
    cout<<"Коэффициент пробития (чем больше, тем выше шанс пробития): "<<(2 + (int)(11-tank1.armor+tank2.break_through) / 2)<<endl;
    tank2.print();
    srand(time(0));
    for (i = 0; i <= 1000000000; i++) {
        ttime = i / 5000000;
        if (ttime >= time1) {
            cout<<endl;
            cout<<"Время - "<<time1<<", "<<tank1.name<<": ";
            time1 += tank1.cooldown;
            if (rand() % (2 + (int)(11-tank2.armor+tank1.break_through) / 2) != 0) {
                vbr = (75 + rand() % 51);
                cur_damage = tank1.damage * vbr / 100;
                if ((int)cur_damage >= tank2.health) {
                    cur_damage = tank2.health;
                    tank2.health = 0;
                }
                else
                    tank2.health -= (int)cur_damage;
                cout<<"Есть пробитие! На "<<(int)cur_damage<<". ";
            }
            else
                cout<<"Рикошет! ";
            cout<<"Прочность "<<tank2.name<<": "<<tank2.health<<endl;
            if (tank2.health == 0) {
                cout<<tank1.name<<" победил!"<<endl;
                break;
            }
        }
        if (ttime >= time2) {
            cout<<endl;
            cout<<"Время - "<<time2<<", "<<tank2.name<<": ";
            time2 += tank2.cooldown;
            if (rand() % (2 + (int)(11-tank1.armor+tank2.break_through) / 2) != 0) {
                vbr = (75 + rand() % 51);
                cur_damage = tank2.damage * vbr / 100;
                if ((int)cur_damage >= tank1.health) {
                    cur_damage = tank1.health;
                    tank1.health = 0;
                }
                else
                    tank1.health -= (int)cur_damage;
                cout<<"Есть пробитие! На "<<(int)cur_damage<<". ";
            }
            else
                cout<<"Рикошет! ";
            cout<<"Прочность "<<tank1.name<<": "<<tank1.health<<endl;
            if (tank1.health == 0) {
                cout<<tank2.name<<" победил!"<<endl;
                break;
            }
        }
        if (ttime > time_interval) {
            cout<<time_interval<<endl;
            time_interval++;
        }
    }
    cout<<endl;
}

void Battle(Tank *team_red, const char *red_strategy, Tank *team_blue, const char *blue_strategy) {
    double i, ttime, vbr, cur_damage, red_times[7], blue_times[7];
    int j, k, min_health, max_health, num_of_alives, blue_i, red_i, time_interval = 1;
    
    for (j = 0; j < 7; j++) {
        red_times[j] = team_red[j].cooldown;
        blue_times[j] = team_blue[j].cooldown;
    }
    
    cout<<"             Battle: "<<endl;
    cout<<endl<<"Red team:"<<endl;
    for (j = 0; j < 7; j++) {
        cout<<"   "<<team_red[j].name<<endl;
    }
    cout<<endl<<"Blue team:"<<endl;
    for (j = 0; j < 7; j++) {
        cout<<"   "<<team_blue[j].name<<endl;
    }
    cout<<endl;
    
    srand(time(0));
    for (i = 0; i <= 1000000000; i++) {
        ttime = i / 5000000*10;
        if (ttime > time_interval) {
            cout<<time_interval<<endl;
            time_interval++;
        }
        
        for (j = 0; j < 7; j++) {
            if (team_red[j].health == 0)
                continue;
            if (ttime >= red_times[j]) {
                cout<<endl;
                cout<<"Время - "<<red_times[j]<<", "<<team_red[j].name<<" (красный): ";
                red_times[j] += team_red[j].cooldown;
                
                if (red_strategy[0] == 'r') { //random
                    num_of_alives = 0;
                    for (k = 0; k < 7; k++)
                        if (team_blue[k].health > 0)
                            num_of_alives++;
                    blue_i = rand() % num_of_alives;
                    num_of_alives = 0;
                    for (k = 0; k < 7; k++) {
                        if (team_blue[k].health > 0)
                            num_of_alives++;
                        if (num_of_alives - 1 == blue_i) {
                            blue_i = k;
                            break;
                        }
                    }
                }
                else if (red_strategy[0] == 'f') { //focus
                    min_health = 9999;
                    for (k = 0; k < 7; k++) {
                        if (team_blue[k].health < min_health && team_blue[k].health > 0) {
                            min_health = team_blue[k].health;
                            blue_i = k;
                        }
                    }
                }
                else if (red_strategy[0] == 'd') { //damage
                    max_health = 0;
                    for (k = 0; k < 7; k++) {
                        if (team_blue[k].health > max_health) {
                            max_health = team_blue[k].health;
                            blue_i = k;
                        }
                    }
                }
                
                if (rand() % (2 + (int)(11-team_blue[blue_i].armor+team_red[j].break_through) / 2) != 0) {
                    vbr = (75 + rand() % 51);
                    cur_damage = team_red[j].damage * vbr / 100;
                    if ((int)cur_damage >= team_blue[blue_i].health) {
                        cur_damage = team_blue[blue_i].health;
                        team_blue[blue_i].health = 0;
                    }
                    else
                        team_blue[blue_i].health -= (int)cur_damage;
                    cout<<"Есть пробитие! На "<<(int)cur_damage<<". ";
                }
                else
                    cout<<"Рикошет! ";
                cout<<"Прочность "<<team_blue[blue_i].name<<": "<<team_blue[blue_i].health<<endl;
                if (team_blue[blue_i].health == 0) {
                    cout<<team_blue[blue_i].name<<" уничтожен танком "<<team_red[j].name<<endl;
                    cout<<endl<<"Red team:"<<endl;
                    for (k = 0; k < 7; k++) {
                        cout<<"   "<<team_red[k].name<<": "<<team_red[k].health<<endl;
                    }
                    cout<<endl<<"Blue team:"<<endl;
                    for (k = 0; k < 7; k++) {
                        cout<<"   "<<team_blue[k].name<<": "<<team_blue[k].health<<endl;
                    }
                    cout<<endl;
                }
            }
            num_of_alives = 0;
            for (k = 0; k < 7; k++)
                if (team_blue[k].health > 0) {
                    num_of_alives = 1;
                    break;
                }
            if (num_of_alives == 0) {
                cout<<"Команда красных победила!"<<endl;
                break;
            }
        }
        
        if (num_of_alives == 0)
            break;
        
        for (j = 0; j < 7; j++) {
            if (team_blue[j].health == 0)
                continue;
            if (ttime >= blue_times[j]) {
                cout<<endl;
                cout<<"Время - "<<blue_times[j]<<", "<<team_blue[j].name<<" (синий): ";
                blue_times[j] += team_blue[j].cooldown;
                
                if (blue_strategy[0] == 'r') { //random
                    num_of_alives = 0;
                    for (k = 0; k < 7; k++)
                        if (team_red[k].health > 0)
                            num_of_alives++;
                    red_i = rand() % num_of_alives;
                    num_of_alives = 0;
                    for (k = 0; k < 7; k++) {
                        if (team_red[k].health > 0)
                            num_of_alives++;
                        if (num_of_alives - 1 == red_i) {
                            red_i = k;
                            break;
                        }
                    }
                }
                else if (blue_strategy[0] == 'f') { //focus
                    min_health = 9999;
                    for (k = 0; k < 7; k++) {
                        if (team_red[k].health < min_health && team_red[k].health > 0) {
                            min_health = team_red[k].health;
                            red_i = k;
                        }
                    }
                }
                else if (blue_strategy[0] == 'd') { //damage
                    max_health = 0;
                    for (k = 0; k < 7; k++) {
                        if (team_red[k].health > max_health) {
                            max_health = team_red[k].health;
                            red_i = k;
                        }
                    }
                }
                
                if (rand() % (2 + (int)(11-team_red[red_i].armor+team_blue[j].break_through) / 2) != 0) {
                    vbr = (75 + rand() % 51);
                    cur_damage = team_blue[j].damage * vbr / 100;
                    if ((int)cur_damage >= team_red[red_i].health) {
                        cur_damage = team_red[red_i].health;
                        team_red[red_i].health = 0;
                    }
                    else
                        team_red[red_i].health -= (int)cur_damage;
                    cout<<"Есть пробитие! На "<<(int)cur_damage<<". ";
                }
                else
                    cout<<"Рикошет! ";
                cout<<"Прочность "<<team_red[red_i].name<<": "<<team_red[red_i].health<<endl;
                if (team_red[red_i].health == 0) {
                    cout<<team_red[red_i].name<<" уничтожен танком "<<team_blue[j].name<<endl;
                    cout<<endl<<"Red team:"<<endl;
                    for (k = 0; k < 7; k++) {
                        cout<<"   "<<team_red[k].name<<": "<<team_red[k].health<<endl;
                    }
                    cout<<endl<<"Blue team:"<<endl;
                    for (k = 0; k < 7; k++) {
                        cout<<"   "<<team_blue[k].name<<": "<<team_blue[k].health<<endl;
                    }
                    cout<<endl;
                }
            }
            num_of_alives = 0;
            for (k = 0; k < 7; k++)
                if (team_red[k].health > 0) {
                    num_of_alives = 1;
                    break;
                }
            if (num_of_alives == 0) {
                cout<<"Команда синих победила!"<<endl;
                break;
            }
        }
        
        if (num_of_alives == 0)
            break;
    }
}

#include "Class_Tank.h"
#include "Class_100mm_D-54S_gun.h"
#include "Class_100mm_D-10S_gun.h"

#ifndef __C_SU_100M1
#define __C_SU_100M1

class SU_100M1: public Tank, virtual public gun_100mm_D_10S, virtual public gun_100mm_D_54S {
public:
    SU_100M1();

    void install_gun(int number);
    
    void print();
    
    void print_about_guns();
};

#endif

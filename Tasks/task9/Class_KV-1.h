#include "Class_Tank.h"
#include "Class_122mm_U-11_gun.h"
#include "Class_85mm_F-30_gun.h"

#ifndef __C_KV_1
#define __C_KV_1

class KV_1: public Tank, virtual public gun_122mm_U_11, virtual public gun_85mm_F_30 {
public:
    KV_1();

    void install_gun(int number);
    
    void print();
    
    void print_about_guns();
};

#endif

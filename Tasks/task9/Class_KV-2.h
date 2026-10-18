#include "Class_Tank.h"
#include "Class_122mm_U-11_gun.h"
#include "Class_107mm_ZiS-6_gun.h"
#include "Class_152mm_M-10_gun.h"

#ifndef __C_KV_2
#define __C_KV_2

class KV_2: public Tank, virtual public gun_122mm_U_11, virtual public gun_107mm_ZiS_6, virtual public gun_152mm_M_10 {
public:
    KV_2();

    void install_gun(int number);
    
    void print();
    
    void print_about_guns();
};

#endif

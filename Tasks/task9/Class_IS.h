#include "Class_Tank.h"
#include "Class_100mm_D-10T_gun.h"
#include "Class_122mm_D2-5T_gun.h"
#include "Class_122mm_D-25T_gun.h"

#ifndef __C_IS
#define __C_IS

class IS: public Tank, virtual public gun_100mm_D_10T, virtual public gun_122mm_D2_5T, virtual public gun_122mm_D_25T {
public:
    IS();

    void install_gun(int number);
    
    void print();
    
    void print_about_guns();
};

#endif

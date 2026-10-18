#include "Class_Tank.h"
#include "Class_85mm_ZiS_S-53_gun.h"
#include "Class_85mm_D-5T-85BM_gun.h"

#ifndef __C_T_43
#define __C_T_43

class T_43: public Tank, virtual public gun_85mm_ZiS_S_53, virtual public gun_85mm_D_5T_85BM {
public:
    T_43();
    
    void install_gun(int number);
    
    void print();
    
    void print_about_guns();
};

#endif

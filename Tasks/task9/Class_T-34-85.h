#include "Class_Tank.h"
#include "Class_85mm_ZiS_S-53_gun.h"
#include "Class_76mm_S-54_gun.h"
#include "Class_85mm_D-5T-85BML_gun.h"

#ifndef __C_T_34_85
#define __C_T_34_85

class T_34_85: public Tank, virtual public gun_76mm_S_54, virtual public gun_85mm_ZiS_S_53, virtual public gun_85mm_D_5T_85BML {
public:
    T_34_85();
    
    void install_gun(int number);
    
    void print();
    
    void print_about_guns();
};

#endif

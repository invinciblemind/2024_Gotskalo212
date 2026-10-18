#include "Class_Tank.h"
#include "Class_85mm_ZiS_S-53_gun.h"
#include "Class_85mm_D-5T-85BML_gun.h"

#ifndef __C_LTTB
#define __C_LTTB

class LTTB: public Tank, virtual public gun_85mm_ZiS_S_53, virtual public gun_85mm_D_5T_85BML {
public:
    LTTB();
    
    void install_gun(int number);
    
    void print();
    
    void print_about_guns();
};

#endif

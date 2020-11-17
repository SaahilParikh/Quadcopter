typedef struct{
    float M1;
    float M2;
    float M3;
    float M4;
} indiv_moments;

typedef struct{
    float F1;
    float F2;
    float F3;
    float F4;
} indiv_forces;

typedef struct{
    float m_z;
    float m_x;
    float m_y;
} moment;

typedef struct{
    float f_z;
    float f_x;
    float f_y;
} force;

float get_RPM(int voltage, int duty_cycle);
float get_force(int angular_velocity);
float get_moment(int angular_velocity);
void get_moments(moment* moment, indiv_moments *ind_moment, indiv_forces *ind_forces);
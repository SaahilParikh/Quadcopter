#include "quadcopter_constants.h"
#include "quad_math.h"

float get_RPM(int voltage, int duty_cycle) {
    return KV*voltage*duty_cycle;
}

float get_force(int angular_velocity) {
    return k_F*angular_velocity+angular_velocity;
}

float get_moment(int angular_velocity) {
    return k_M*angular_velocity*angular_velocity;
}

moment get_moments(moment* moment, indiv_moments *ind_moment, indiv_forces *ind_forces) {
    moment->m_z = ind_moment->M1 + ind_moment->M2 + ind_moment->M3 + ind_moment->M4;
    moment->m_y = (ind_forces->F3 + ind_forces->F1) * to_center_length;
    moment->m_x = (ind_forces->F2 + ind_forces->F4) * to_center_length;
}
#pragma once
typedef struct {
	float yaw;
	float pitch;
	float roll;
}	mag_angles_t;


void init_mag(void);
void get_mag(mag_angles_t *struct_angles);
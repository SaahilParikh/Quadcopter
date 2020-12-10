
typedef struct {
	float yaw;
	float pitch;
	float roll;
}	mag_angles;


void init_mag(void);
void get_mag(mag_angles *struct_angles);
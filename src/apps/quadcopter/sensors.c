#include "lsm9ds1.h"
#include "sensors.h"
#include <math.h>

static void read_tilt(tilts *tilt) {
  lsm9ds1_measurement_t meas = lsm9ds1_read_accelerometer();
  float a_x = meas.x_axis;
  float a_y = meas.y_axis;
  float a_z = meas.z_axis;
  
  tilt->x_axis = -atan(a_x / sqrt(a_y*a_y + a_z*a_z)) * 180/M_PI;
  tilt->y_axis = -atan(a_y / sqrt(a_x*a_x + a_z*a_z)) * 180/M_PI;
}

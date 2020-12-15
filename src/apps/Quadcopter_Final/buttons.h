#pragma once

enum Buttons {
  NONE = 0,
  RLEFT = 1,
  RRIGHT = 2,
  PFWD = 3,
  PBACK = 4,
  YLEFT = 5,
  YRIGHT = 6,
  ALTUP = 7,
  ALTDOWN = 8,
  ARM = 9,
  INIT = 10,
  KILL = 11
};

const char* get_button_name(enum Buttons b);


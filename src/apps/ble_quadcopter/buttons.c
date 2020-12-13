#include "buttons.h"

const char* get_button_name(enum Buttons b)
{
  switch (b) {
    case NONE:
      return "NONE";
    case RLEFT:
      return "RLEFT";
    case RRIGHT:
      return "RRIGHT";
    case PFWD:
      return "RFWD";
    case PBACK:
      return "PBACK";
    case YLEFT:
      return "YLEFT";
    case YRIGHT:
      return "YRIGHT";
    case ALTUP:
      return "ALTUP";
    case ALTDOWN:
      return "ALTDOWN";
    case ARM:
      return "ARM";
    case INIT:
      return "INIT";
    case KILL:
      return "KILL";
    default:
      return "unknown!";
  }
}


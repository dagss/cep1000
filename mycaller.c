#include <stdlib.h>
#include "cep1000test.h"

static char *interned_dd;
static char *mismatch0, *mismatch1, *mismatch2, *mismatch3;

double docall_direct(callable_func_t callable, double argument) {
  return (*callable)(argument);
}

double docall_intern(PyMyCallable *obj, double argument) {
  if (obj->ob_type->tp_flags | TPFLAGS_UNOFFICIAL) {
    size_t nativecall_offset = ((PyUnofficialTypeObject*)obj->ob_type)->tp_nativecall_offset;
    if (nativecall_offset != 0) {
      intern_call_slot_t *slots = *(intern_call_slot_t**)((char*)obj + nativecall_offset);
      while (1) {
        char *sig = slots->interned_signature;
        if (sig == mismatch0) {
          exit(4);
        } else if (sig == mismatch1) {
          exit(4);
        } else if (sig == mismatch2) {
          exit(4);
        } else if (sig == mismatch3) {
          exit(4);
        }
        if (sig == interned_dd) {
          callable_func_t pfunc = slots->funcptr;
          return (*pfunc)(argument);
        } else if (sig == NULL) {
          printf("NO MATCHING sig\n");
          exit(3);
        }
        slots++;
      }
    } else {
      exit(1);
    }
  } else {
    exit(2);
  }
}

double docall_key(PyMyCallable *obj, double argument) {
  if (obj->ob_type->tp_flags | TPFLAGS_UNOFFICIAL) {
    size_t nativecall_offset = ((PyUnofficialTypeObject*)obj->ob_type)->tp_nativecall_offset;
    if (nativecall_offset != 0) {
      key_call_slot_t *slots = *(key_call_slot_t**)((char*)obj + nativecall_offset);
      while (1) {
        switch (slots->key) {
        case 1:
          exit(4);
        case 0x34524561234:
          exit(4);
        case 0x324563243223:
          exit(4);
        case 0x8534f324234:
          exit(4);
        case 0x5343276534324:
          exit(4);
        case FUNC_KEY:
          {
            callable_func_t pfunc = slots->funcptr;
            return (*pfunc)(argument);
          }
        case 0:
          exit(3);
        }
        slots++;
      }
    } else {
      exit(1);
    }
  } else {
    exit(2);
  }
}

void initialize_mycaller() {
  interned_dd = get_interned_dd();
  mismatch0 = get_interned_something_else(0);
  mismatch1 = get_interned_something_else(1);
  mismatch2 = get_interned_something_else(2);
  mismatch3 = get_interned_something_else(3);
}

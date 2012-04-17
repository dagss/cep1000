#include <stdlib.h>
#include "cep1000test.h"

#if USE_LIKELY
#define perhaps_likely(x) __builtin_expect((x),1)
#else
#define perhaps_likely(x) x
#endif

#define likely(x)       __builtin_expect((x),1) 
#define unlikely(x)     __builtin_expect((x),0)

static char *interned_dd;
static char *mismatch0, *mismatch1, *mismatch2, *mismatch3;

double docall_dispatch(callable_func_t callable, double argument) {
  return (*callable)(argument);
}

double docall_intern(PyMyCallable *obj, double argument) {
  if (obj->ob_type->tp_flags | TPFLAGS_UNOFFICIAL) {
    size_t nativecall_offset = ((PyUnofficialTypeObject*)obj->ob_type)->tp_nativecall_offset;
    if (nativecall_offset != 0) {
      intern_call_slot_t *slots = *(intern_call_slot_t**)((char*)obj + nativecall_offset);
      for (int i = 0; i != N; ++i) {
        char *sig = slots[i].interned_signature;
#if MISMATCHES
        if (sig == mismatch0) {
          exit(4);
        } else if (sig == mismatch1) {
          exit(5);
        } else if (sig == mismatch2) {
          exit(6);
        } else if (sig == mismatch3) {
          exit(7);
        } else
#endif
          if (perhaps_likely(sig == interned_dd)) {
          callable_func_t pfunc = slots[i].funcptr;
          return (*pfunc)(argument);
        }
      }
    }
  }
  exit(10);
}

double docall_key(PyMyCallable *obj, double argument) {
  if (obj->ob_type->tp_flags | TPFLAGS_UNOFFICIAL) {
    size_t nativecall_offset = ((PyUnofficialTypeObject*)obj->ob_type)->tp_nativecall_offset;
    if (nativecall_offset != 0) {
      key_call_slot_t *slots = *(key_call_slot_t**)((char*)obj + nativecall_offset);
      for (int i = 0; i != N; ++i) {
        size_t key = slots[i].key1;
#if MISMATCHES
        if (key == 1) {
          exit(8);
        } else if (key == 0x34524561234) {
          exit(5);
        } else if (key == 0x324563243223) {
          exit(6);
        } else if (key == 0x8534f324234) {
          exit(7);
        } else
#endif
          if (perhaps_likely(key == FUNC_KEY1)) {
          callable_func_t pfunc = slots[i].funcptr;
          return (*pfunc)(argument);
        }
      }
    }
  }
  exit(4);
}


void initialize_mycaller() {
  interned_dd = get_interned_dd();
  mismatch0 = get_interned_something_else(0);
  mismatch1 = get_interned_something_else(1);
  mismatch2 = get_interned_something_else(2);
  mismatch3 = get_interned_something_else(3);
}

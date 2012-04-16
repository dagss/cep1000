#include <stddef.h>
#include "cep1000test.h"

#define N 1

PyUnofficialTypeObject my_callable_type;


double func(double x) {
  return x * x;
}

char *get_interned_dd() {
  return "dd";
}

char *get_interned_something_else(int dummy) {
  return "something else";
}

void *get_f_callable_intern() {
  int i;
  PyMyCallable *result = malloc(sizeof(PyMyCallable) + sizeof(intern_call_slot_t) * (N + 1));
  intern_call_slot_t *table = (void*)((char*)result + sizeof(PyMyCallable));

  result->ob_type = (void*)&my_callable_type;
  result->ob_call_slots = table;
  
  for (i = 0; i != N; ++i) {
    table[i] = (intern_call_slot_t){"nonmatching", NULL};
  }
  table[N - 1] = (intern_call_slot_t){get_interned_dd(), &func};
  table[N] = (intern_call_slot_t){NULL, NULL};

  return result;
}

void *get_f_callable_key() {
  int i;
  PyMyCallable *result = malloc(sizeof(PyMyCallable) + sizeof(intern_call_slot_t) * (N + 1));
  key_call_slot_t *table = (void*)((char*)result + sizeof(PyMyCallable));

  result->ob_type = (void*)&my_callable_type;
  result->ob_call_slots = table;
  
  for (i = 0; i != N; ++i) {
    table[i] = (key_call_slot_t){2, 3, 4, NULL};
  }
  table[N - 1] = (key_call_slot_t){FUNC_KEY1, FUNC_KEY2, FUNC_KEY3, &func};
  table[N] = (key_call_slot_t){0, 0, 0, NULL};

  return result;
}

void initialize_mycallable() {
  int i;

  memset(&my_callable_type, 0, sizeof(PyUnofficialTypeObject));
  my_callable_type.tp_base.tp_flags = TPFLAGS_UNOFFICIAL;
  my_callable_type.tp_nativecall_offset = offsetof(PyMyCallable, ob_call_slots);
}

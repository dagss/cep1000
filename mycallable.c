#include <stddef.h>
#include "cep1000test.h"


PyUnofficialTypeObject my_callable_type_intern;
PyUnofficialTypeObject my_callable_type_key;

char *interned_dd;

double func(double x) {
  return x * x;
}

static void *get_func_ptr_intern(void *obj, char *interned_signature, int has_gil) {
  if (interned_signature == interned_dd) {
    return &func;
  } else {
    return NULL;
  }
}

static void *get_func_ptr_key(void *obj, size_t key, int has_gil) {
  if (perhaps_likely(key == FUNC_KEY1)) {
    return &func;
  } else {
    return NULL;
  }
}

char *get_interned_something_else(int dummy) {
  return "something else";
}

void *get_f_callable_intern() {
  int i;
  PyMyCallable *result = malloc(sizeof(PyMyCallable) + sizeof(intern_call_slot_t) * (N + 1));
  intern_call_slot_t *table = (void*)((char*)result + sizeof(PyMyCallable));


  result->ob_type = (void*)&my_callable_type_intern;
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

  result->ob_type = (void*)&my_callable_type_key;
  result->ob_call_slots = table;
  
  for (i = 0; i != N; ++i) {
    table[i] = (key_call_slot_t){2, NULL};
  }
  table[N - 1] = (key_call_slot_t){FUNC_KEY1, &func};
  table[N] = (key_call_slot_t){0, NULL};

  return result;
}

void initialize_mycallable() {
  int i;
  interned_dd = get_interned_dd();

  memset(&my_callable_type_intern, 0, sizeof(PyUnofficialTypeObject));
  my_callable_type_intern.tp_base.tp_flags = TPFLAGS_UNOFFICIAL;
  my_callable_type_intern.tp_nativecall_offset = offsetof(PyMyCallable, ob_call_slots);
  my_callable_type_intern.tp_nativecall_getfunc = &get_func_ptr_intern;

  memset(&my_callable_type_key, 0, sizeof(PyUnofficialTypeObject));
  my_callable_type_key.tp_base.tp_flags = TPFLAGS_UNOFFICIAL;
  my_callable_type_key.tp_nativecall_offset = offsetof(PyMyCallable, ob_call_slots);
  my_callable_type_key.tp_nativecall_getfunc = &get_func_ptr_key;
}

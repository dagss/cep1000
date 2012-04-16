#include <Python.h>

#define TPFLAGS_UNOFFICIAL 20


#define FUNC_KEY1 0x1234bbbbbbbb
#define FUNC_KEY2 0x12345bbbbbbb
#define FUNC_KEY3 0x12366bbbbbbb
#define FUNC_KEY4 0x12377aaaaaaa
#define FUNC_KEY5 0x123b7aaaaaaa
#define FUNC_KEY6 0x223b7aaaaaaa
#define FUNC_KEY7 0x323b7aaaaaaa

typedef struct {
  PyTypeObject tp_base;
  unsigned long tp_unofficial_flags;
  size_t tp_nativecall_offset;
} PyUnofficialTypeObject;

typedef struct {
  Py_ssize_t ob_refcnt;
  PyTypeObject *ob_type;
  void *ob_call_slots;
} PyMyCallable;

typedef struct {
  char *interned_signature;
  void *funcptr;
} intern_call_slot_t;

typedef struct {
  size_t key1, key2, key3, key4, key5, key6, key7;
  void *funcptr;
} key_call_slot_t;


typedef double (*callable_func_t)(double);


/* implemented in mycallable */
double func(double);
void *get_f_callable_intern();
void *get_f_callable_key();
char *get_interned_dd();
char *get_interned_something_else(int);
void initialize_mycallable();

/* implemented in mycaller */
double docall_direct(callable_func_t callable, double argument);
double docall_intern(PyMyCallable *func, double argument);
double docall_key(PyMyCallable *func, double argument);
void initialize_mycaller();

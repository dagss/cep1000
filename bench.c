#include <time.h>
#include "cep1000test.h"

#define J 100000
#define K 5000

static double walltime() {
  struct timespec tv;
  clock_gettime(CLOCK_REALTIME, &tv);
  return tv.tv_sec + 1e-9 * tv.tv_nsec;
}

static void snftime(char *buf, size_t n, double time) {
  char *units;
  if (time < 1e-06) {
    units = "ns";
    time *= 1e9;
  } else if (time < 1e-03) {
    units = "us";
    time *= 1e6;
  } else if (time < 10) {
    units = "ms";
    time *= 1e3;
  } else {
    units = "s";
  }
  snprintf(buf, n, "%.1f %s", time, units);
  buf[n - 1] = '\0';
}

double arrmin(double *arr, size_t n) {
  double result = arr[0];
  for (size_t i = 0; i != n; ++i) {
    result = (arr[i] < result) ? arr[i] : result;
  }
  return result;
}

int main(int argc, char *argv[]) {
  char tbuf[100];

  initialize_mycallable();
  initialize_mycaller();

  void *obj_intern = get_f_callable_intern();
  void *obj_key = get_f_callable_key();
  printf("Direct result: %f\n", docall_direct(&func, 2.0));
  printf("Intern result: %f\n", docall_intern(obj_intern, 2.0));
  printf("Key result: %f\n", docall_key(obj_key, 2.0));


  {
    double s = 0;
    double times[K];
    for (int k = 0; k != K; ++k) {
      double t0 = walltime();
      for (int i = 0; i != J; i++) {
        s += docall_direct(&func, 2.0);
      }
      times[k] = walltime() - t0;
    }
    snftime(tbuf, 100, arrmin(times, K) / (double)J);
    printf("Direct took %s (result %f)\n", tbuf, s);
  }

  {
    double s = 0;
    double times[K];
    for (int k = 0; k != K; ++k) {
      double t0 = walltime();
      for (int i = 0; i != J; i++) {
        s += docall_intern(obj_intern, 2.0);
      }
      times[k] = walltime() - t0;
    }
    snftime(tbuf, 100, arrmin(times, K) / (double)J);
    printf("Intern method took %s (result %f)\n", tbuf, s);
  }

  {
    double s = 0;
    double times[K];
    for (int k = 0; k != K; ++k) {
      double t0 = walltime();
      for (int i = 0; i != J; i++) {
        s += docall_key(obj_key, 2.0);
      }
      times[k] = walltime() - t0;
    }
    snftime(tbuf, 100, arrmin(times, K) / (double)J);
    printf("Key method took %s (result %f)\n", tbuf, s);
  }

  return 0;
}

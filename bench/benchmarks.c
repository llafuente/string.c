#include "../include/string.h"

#include "bench.h"

#include "bench-memory.c"

int main(int argc, char *argv[]) {

  if ( argc != 2 ) {
    printf( "usage: %s version", argv[0] );
  } else {

    BENCHMARK(argv[1], 100, 1E7, bench_memory);
  }

  return 0;
}

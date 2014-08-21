
#include "log.h"
#include "program.h"

#include "util.h"

extern struct program_b *program;

int memory_blocks=0;
int memory_blocks_total=0;

void *MALLOC(size_t size) {
  if(size == 0) log_warn("allocating zero bytes");
  void *ptr=malloc(size);
  if(!ptr && size) log_fatal("out of memory; requested %d byte%s",size,S(size));
  memory_blocks++;
  memory_blocks_total++;
  return(ptr);
}

void *REALLOC(void *ptr,size_t size) {
  if(size == 0) log_warn("reallocating zero bytes");
  ptr=realloc(ptr,size);
  if(!ptr && size) log_fatal("out of memory; requested a total of %d byte%s",size,S(size));
  return(ptr);
}

void *FREE(void *ptr) {
  assert(ptr);
  free(ptr);
  memory_blocks--;
  return(NULL);
}

void EXIT(int code) {
  if(program) program->exit_code=code;
  exit(code);
}

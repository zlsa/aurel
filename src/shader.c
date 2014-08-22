
#include <GL/glew.h>

#include "log.h"
#include "util.h"

#include "shader.h"

struct shader_b *shader_new(void) {
  struct shader_b *shader=MALLOC(sizeof(struct shader_b));
  shader->references    = 0;

  if((shader->program = glCreateProgram()) == 0) {
    print_gl_error(false);
    log_fatal("could not create shader program.");
  }

  return(shader_reference(shader));
}

struct shader_b *shader_reference(struct shader_b *shader) {
  ASSERT(shader);
  shader->references++;
  return(shader);
}

bool shader_free(struct shader_b *shader) {
  ASSERT(shader);
  shader->references--;
  if(shader->references == 0) {
    if(shader->program) glDeleteProgram(shader->program);
    FREE(shader);
  }
  return(true);
}

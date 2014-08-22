
#ifndef OBJECT_H
#define OBJECT_H

/* INCLUDES */
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "shader.h"

/* DEFINES */
// vertices, normals
#define OBJECT_ARRAY_NUMBER 2

/* BLOCKS */
struct object_b {
  int references;

  struct shader_b *shader;

  GLint **vertex_arrays;

  GLuint vertex_buffer;
  GLuint normal_buffer;
};

/* FUNCTIONS */
struct object_b *object_new(void);
struct object_b *object_reference(struct object_b *object);
bool object_free(struct object_b *object);

#endif

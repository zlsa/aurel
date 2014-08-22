
#ifndef SHADER_H
#define SHADER_H

/* INCLUDES */
#include <GLFW/glfw3.h>
#include <stdbool.h>

/* DEFINES */

/* BLOCKS */
struct shader_b {
  int references;

  GLuint program;

  GLuint vertex;
  GLuint fragment;
};

/* FUNCTIONS */
struct shader_b *shader_new(void);
struct shader_b *shader_reference(struct shader_b *shader);
bool shader_free(struct shader_b *shader);

#endif

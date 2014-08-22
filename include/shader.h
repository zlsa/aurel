
#ifndef SHADER_H
#define SHADER_H

/* INCLUDES */
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "file.h"

/* DEFINES */

/* BLOCKS */
struct shader_b {
  int references;

  GLuint program;

  GLuint vertex;
  GLuint fragment;

  char *vertex_source;
  char *fragment_source;

  int   vertex_source_length;
  int   fragment_source_length;
  
};

/* FUNCTIONS */
struct shader_b *shader_new(void);
struct shader_b *shader_reference(struct shader_b *shader);
bool shader_free(struct shader_b *shader);

bool shader_vertex_file(struct shader_b *shader, struct file_b *file);
bool shader_fragment_file(struct shader_b *shader, struct file_b *file);

bool shader_compile_vertex(struct shader_b *shader);
bool shader_compile_fragment(struct shader_b *shader);
bool shader_compile(struct shader_b *shader);

#endif

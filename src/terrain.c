
#include <GL/glew.h>

#include "util.h"
#include "log.h"
#include "shader.h"

#include "terrain.h"

struct terrain_b *terrain_new(void) {
  struct terrain_b *terrain = MALLOC(sizeof(struct terrain_b));
  terrain->references = 0;

  terrain->object     = object_new();

  struct shader_b *shader = shader_new();

  struct file_b *file = file_new();
  char *filename      = "../data/shaders/terrain.vert";

  if(file_open(file, filename, FILE_MODE_READ)) {
    shader_vertex_file(shader, file);
    file_close(file);
  } else {
    log_warn("could not open terrain vert shader '%s' for reading: %s", filename, file_error(file));
  }

  filename = "../data/shaders/terrain.frag";
  if(file_open(file, filename, FILE_MODE_READ)) {
    shader_fragment_file(shader, file);
    file_close(file);
  } else {
    log_warn("could not open terrain frag shader '%s' for reading: %s", filename, file_error(file));
  }

  file_free(file);

  shader_compile(shader);

  shader_use(shader);

  shader_uniform_new(shader, "u_Time");

  object_set_shader(terrain->object, shader);
  
  return(terrain_reference(terrain));
}

struct terrain_b *terrain_reference(struct terrain_b *terrain) {
  ASSERT(terrain);
  terrain->references++;
  return(terrain);
}

bool terrain_free(struct terrain_b *terrain) {
  ASSERT(terrain);
  terrain->references--;
  if(terrain->references == 0) {
    shader_free(terrain->object->shader);
    object_free(terrain->object);
    FREE(terrain);
  }
  return(true);
}

bool terrain_generate_object(struct terrain_b *terrain) {
  static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f
  };

  terrain->object->vertices = 4;

  glBindVertexArray(terrain->object->vertex_arrays);

  glBindBuffer(GL_ARRAY_BUFFER, terrain->object->vertex_buffer);
  
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  return(true);
}

bool terrain_draw(struct terrain_b *terrain) {

  shader_use(terrain->object->shader);
  shader_uniform_set_float(terrain->object->shader, "u_Time", glfwGetTime());

  object_draw(terrain->object);

  return(true);
}

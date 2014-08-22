
#include <GL/glew.h>

#include "log.h"
#include "util.h"
#include "file.h"

#include "shader.h"

#include <string.h>

struct shader_b *shader_new(void) {
  struct shader_b *shader=MALLOC(sizeof(struct shader_b));
  shader->references    = 0;

  shader->vertex        = 0;
  shader->fragment      = 0;

  shader->vertex_source   = NULL;
  shader->fragment_source = NULL;

  shader->vertex_source_length   = 0;
  shader->fragment_source_length = 0;

  if((shader->program = glCreateProgram()) == 0) {
    print_gl_error(false);
    log_fatal("could not create shader program.");
  }

  shader->vertex   = glCreateShader(GL_VERTEX_SHADER);
  shader->fragment = glCreateShader(GL_FRAGMENT_SHADER);

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
    if(shader->vertex_source)   FREE(shader->vertex_source);
    if(shader->fragment_source) FREE(shader->fragment_source);

    if(shader->vertex)   glDeleteShader(shader->vertex);
    if(shader->fragment) glDeleteShader(shader->fragment);

    if(shader->program) glDeleteProgram(shader->program);
    FREE(shader);
  }
  return(true);
}

bool shader_vertex_file(struct shader_b *shader, struct file_b *file) {
  char *content = file_read_all(file);

  shader->vertex_source        = content;
  shader->vertex_source_length = strlen(content);

  return(true);
}

bool shader_fragment_file(struct shader_b *shader, struct file_b *file) {
  char *content = file_read_all(file);

  shader->fragment_source        = content;
  shader->fragment_source_length = strlen(content);

  return(true);
}

bool shader_compile_vertex(struct shader_b *shader) {
  if(shader->vertex_source) {
    glShaderSource(shader->vertex, 1, (const GLchar**)&shader->vertex_source, &shader->vertex_source_length);
    GLint compile_status;
    glGetShaderiv(shader->vertex, GL_COMPILE_STATUS, &compile_status);

    if(compile_status != GL_TRUE) {
      GLint log_length = -1;
      glGetShaderiv(shader->vertex, GL_INFO_LOG_LENGTH, &log_length);

      print_gl_error(false);

      if(log_length > 0) {
        char *log = MALLOC(sizeof(char) * log_length);
        glGetShaderInfoLog(shader->vertex, log_length, NULL, log);

        log_warn("could not compile vertex shader: %s", log);
      } else {
        log_warn("could not compile vertex shader");
      }
      log_warn("--[    VERTEX SHADER SOURCE    ]------------");
      printf("%s", shader->vertex_source);
      log_warn("--[  END VERTEX SHADER SOURCE  ]------------");

      glDeleteShader(shader->vertex);
      shader->vertex = 0;

      return(false);
    }

    printf("YAY! %s\n", shader->vertex_source);
      
    return(true);
  }
  return(true);
}

bool shader_compile_fragment(struct shader_b *shader) {
  if(shader->fragment_source) {
    glShaderSource(shader->fragment, 1, (const GLchar**)&shader->fragment_source, &shader->fragment_source_length);
    GLint compile_status;
    glGetShaderiv(shader->fragment, GL_COMPILE_STATUS, &compile_status);

    if(!compile_status) {
      GLint log_length = -1;
      glGetShaderiv(shader->fragment, GL_INFO_LOG_LENGTH, &log_length);

      print_gl_error(false);

      if(log_length > 0) {
        char *log = MALLOC(sizeof(char) * log_length);
        glGetShaderInfoLog(shader->fragment, log_length, NULL, log);

        log_warn("could not compile fragment shader: %s", log);
      } else {
        log_warn("could not compile fragment shader");
      }
      log_warn("--[   FRAGMENT SHADER SOURCE   ]------------");
      printf("%s", shader->fragment_source);
      log_warn("--[ END FRAGMENT SHADER SOURCE ]------------");

      glDeleteShader(shader->fragment);
      shader->fragment = 0;

      return(false);
    }

    printf("YAY! %s\n", shader->fragment_source);
      
    return(true);
  }
  return(true);
}

bool shader_compile(struct shader_b *shader) {
  return(shader_compile_vertex(shader));
}

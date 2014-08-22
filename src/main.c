
#include "util.h"
#include "log.h"
#include "program.h"
#include "file.h"
#include "config.h"
#include "window.h"
#include "shader.h"
#include "object.h"
#include "terrain.h"

#include "main.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

extern int memory_blocks;
extern int memory_blocks_total;

struct program_b *program=NULL;

void exit_cleanup(void) {
  bool normal=false;
  bool quiet=false;
  if(program) {
    if(program->exit_code == EXIT_SUCCESS) normal=true;
    if(program->quiet > 0) quiet=true;
    program_free(program);
    program_end();
    program=NULL;
  } else {
    log_warn("we haven't even got going yet...");
  }
  if(memory_blocks != 0) {
    log_warn("did not free %d block%s (cumulative blocks: %d)",memory_blocks,
             S(memory_blocks),memory_blocks_total);
  } else if(normal) {
    if(!quiet) log_info("exiting normally");
  }
}

int main(int argc,char **argv) {
  /* KICKSTART */
  program=program_new();
  program->argc=argc;
  program->argv=argv;

  program_parse();
  
  /* ATEXIT */
  if(atexit(exit_cleanup) != 0) log_fatal("cannot bind atexit");

  if(program->quiet <= 0) program_version();

  /* INITIALIZE STUFF */

  /* TESTING ONLY */
  if(program->test) {
    int passed=0;
    int total=2;

    log_test("(note that these tests require you to run from $AUREL_ROOT/src)");

    if(file_test()) passed++;
    if(config_test()) passed++;

    log_test("   == RESULTS =================================================");
    if(passed == total) log_test("%sall tests passed",BTOF(true));
    else if(passed == 0) log_test("%sall tests failed...",BTOF(false));
    else log_test("%s%d/%d test%s passed",BTOF(false),passed,total,S(total));
    /* log_test("test is for extra-low-priority self-testing messages"); */
    /* log_debug("debug is for very low-priority messages that might diagnose problems"); */
    /* log_info("info is for low-priority user-facing information"); */
    /* log_notice("notice is for noncritical but potentially unexpected warnings"); */
    /* log_warn("warning is for unexpected but noncritical errors"); */
    /* log_never("something impossible has happened (the proverbial 'default')"); */
    /* log_fatal("something has gone seriously wrong. BAIL OUT FOLKS, WE'RE GOING DOWN!"); */
    return(EXIT_SUCCESS);
  }

  /* RELEASE ONLY */

  program_start();

  glfwMakeContextCurrent(program->window->window);

  struct terrain_b *terrain = terrain_new();
  terrain_generate_object(terrain);

  glClearColor(0.0f, 0.2f, 0.3f, 1.0f);

  /* MAIN LOOP */
  while(!glfwWindowShouldClose(program->window->window)) {
    int width, height;
    glfwGetFramebufferSize(program->window->window, &width, &height);
    glViewport(0, 0, width, height);

    int ratio = ((float) width) / ((float) height);

    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float fov    = DEG_TO_RAD(90);
    float near   = 1.0;
    float far    = 100.0;

    float left   = -1 * near * tan((ratio * fov) * 0.5);
    float right  = far * tan((ratio * fov) * 0.5);
    float bottom = 1 * near * tan(fov * 0.5);
    float top    = far * tan(fov * 0.5);

    glOrtho(left, right, bottom, top, near, far);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    terrain_draw(terrain);

    if(false) {
      glBegin(GL_TRIANGLES);
      glColor3f(   1.0f,  0.0f, 0.0f);
      glVertex3f( -0.6f, -0.4f, 0.0f);
      glColor3f(   0.0f,  1.0f, 0.0f);
      glVertex3f(  0.6f, -0.4f, 0.0f);
      glColor3f(   0.0f,  0.0f, 1.0f);
      glVertex3f(  0.0f,  0.6f, 0.0f);
      glEnd();
    }

    glfwSwapBuffers(program->window->window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  terrain_free(terrain);

  return(EXIT_SUCCESS);
}

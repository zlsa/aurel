#version 330

varying vec4 v_Position;

attribute vec4 a_Position;

uniform float u_Time;
uniform mat4 u_ModelView;
uniform mat4 u_Projection;

void main() {
  float t = u_Time * 0.5;
  gl_Position = (u_Projection * u_ModelView * a_Position);
  v_Position = a_Position;
}

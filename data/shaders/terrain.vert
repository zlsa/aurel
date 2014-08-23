#version 330

varying vec4 v_Position;

attribute vec4 position;

uniform float u_Time;

void main() {
  gl_Position = vec4(position.x + u_Time * 0.0, position.y, position.z, 1.0);
  v_Position = gl_Position;
}

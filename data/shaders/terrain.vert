#version 330

attribute vec4 position;

void main() {
  gl_Position = vec4(position.x, position.y * 0.5, position.z, 1.0);
}

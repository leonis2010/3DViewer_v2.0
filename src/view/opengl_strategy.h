#ifndef PROJECTION_STRATEGY_H
#define PROJECTION_STRATEGY_H

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#elif defined(__linux__)
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <QOpenGLFunctions>

namespace s21 {

// Длинные штрихи: --------        --------
const int kLongStripplePattern = 0x00FF;
// Средние штрихи: ----    ----    ----
const int kMediumStipplePattern = 0x0F0F;
// Частые штрихи: - - - - - - - -
const int kShortStripplePattern = 0xAAAA;

static void PerspectiveProjection(int width, int height) {
  float aspect = static_cast<float>(width) / height;
  gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

static void OrthographicProjection(int width, int height) {
  float aspect = static_cast<float>(width) / height;
  float orthoScale = 1.2f;
  glOrtho(-aspect * orthoScale, aspect * orthoScale, -orthoScale, orthoScale,
          -10.0f, 10.0f);
}

static void SolidEdges() { glDisable(GL_LINE_STIPPLE); }

static void DashedEdges() {
  glEnable(GL_LINE_STIPPLE);
  glLineStipple(1, kMediumStipplePattern);
}

static void SquareVertices() { glDisable(GL_POINT_SMOOTH); }

static void CircleVertices() {
  glEnable(GL_POINT_SMOOTH);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
}

class OpenGLStrategy {
 public:
  typedef void (*ProjectionFunction)(int width, int height);
  typedef void (*EdgeDisplayFunction)();
  typedef void (*VertexDisplayFunction)();

  void SetProjectionFunction(ProjectionFunction func) {
    projection_func_ = func;
  }

  void SetEdgeDisplayFunction(EdgeDisplayFunction func) {
    edge_display_func_ = func;
  }

  void SetVertexDisplayFunction(VertexDisplayFunction func) {
    vertex_display_func_ = func;
  }

  void ApplyProjection(int width, int height) {
    projection_func_(width, height);
  }

  void ApplyEdgeDisplay() { edge_display_func_(); }

  void ApplyVertexDisplay() { vertex_display_func_(); }

 private:
  ProjectionFunction projection_func_ = OrthographicProjection;
  EdgeDisplayFunction edge_display_func_ = SolidEdges;
  VertexDisplayFunction vertex_display_func_ = SquareVertices;
};  // class OpenGLStrategy
}  // namespace s21

#endif

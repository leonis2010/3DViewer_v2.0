#ifndef VIEW_SCENE_H_
#define VIEW_SCENE_H_

#ifdef __APPLE__
#include <OpenGL/gl.h>
#elif defined(__linux__)
#include <GL/gl.h>
#endif

#include <QOpenGLBuffer>
#include <QOpenGLWidget>
#include <QVector3D>
#include <QWheelEvent>
#include <memory>

#include "model/parser.h"
#include "model/s21_matrix_oop.h"
#include "view/settings_facade.h"

namespace s21 {

const float kBaseSensitivity = 0.005f;
const float kMinSensitivity = 0.00001f;
const float kMaxSensitivity = 0.1f;

/**
 * @class Scene
 * @brief OpenGL-based 3D scene viewer widget
 *
 * The Scene class provides functionality for rendering and manipulating 3D
 * wireframe models. It handles user interactions such as:
 * - Mouse-based rotation of the model
 * - Zooming via mouse wheel
 * - Model translation via mouse drag
 *
 * The class manages the OpenGL rendering context and maintains the model's
 * transformation state including rotation, scale, and position. It integrates
 * with the application's settings system to apply user-defined rendering
 * preferences.
 */
class Scene : public QOpenGLWidget {
  Q_OBJECT

 public:
  Scene(QWidget* parent = nullptr);
  ~Scene() override;
  void SetModel(const std::shared_ptr<WireframeObject> model);

  void wheelEvent(QWheelEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

  S21Matrix CreateRotationMatrix(char axis, float angle);

 protected:
  void initializeGL() override;
  void paintGL() override;
  void DrawAllVertices();

  QOpenGLBuffer vbo_;
  std::shared_ptr<WireframeObject> model_{nullptr};

 private:
  SettingsFacade* settings_{nullptr};

  float scale_factor_ = 1.0f;  // Начальный масштаб
  bool is_dragging_ = false;
  QPoint last_mouse_pos_;
  QVector2D model_position_;

  bool is_rotating_ = false;
  QPoint last_rotate_pos_;
  S21Matrix rotation_matrix_;  // Матрица поворота 4x4
  QVector3D rotation_angles_;  // Углы поворота по осям X,Y,Z
};
}  // namespace s21

#endif  // VIEW_SCENE_H_

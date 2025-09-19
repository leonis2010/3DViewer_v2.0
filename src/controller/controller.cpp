#include "view/scene.h"
#include "view/viewer_widget.h"

namespace s21 {
void ViewerWidget::OpenFile() {
  QString file_path = QFileDialog::getOpenFileName(this, "Open Object File", "",
                                                   "Object Files (*.obj)");
  if (file_path != nullptr)
    current_object_ =
        std::make_shared<WireframeObject>(file_path.toStdString());
  UpdateObjectInfo();
}

void Scene::wheelEvent(QWheelEvent* event) {
  // Увеличиваем или уменьшаем масштаб в зависимости от направления прокрутки
  if (event->angleDelta().y() > 0) {
    scale_factor_ *= 1.1f;  // Увеличение
  } else {
    scale_factor_ /= 1.1f;  // Уменьшение
  }
  update();
}

void Scene::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    is_dragging_ = true;
    last_mouse_pos_ = event->pos();
  } else if (event->button() == Qt::RightButton) {
    is_rotating_ = true;
    last_rotate_pos_ = event->pos();
  }
}

void Scene::mouseMoveEvent(QMouseEvent* event) {
  if (is_dragging_) {
    // Вычисляем смещение мыши
    QPoint delta = event->pos() - last_mouse_pos_;
    last_mouse_pos_ = event->pos();

    const float kBaseSensitivity = 0.005f;
    const float kMinSensitivity = 0.00001f;
    const float kMaxSensitivity = 0.1f;

    // Чувствительность обратно пропорциональна масштабу
    float effective_sensitivity = kBaseSensitivity / scale_factor_;

    // Ограничиваем чувствительность минимальным и максимальным значениями
    effective_sensitivity =
        qBound(kMinSensitivity, effective_sensitivity, kMaxSensitivity);

    // -delta.y() потому что ось Y направлена вниз а в OpenGL - вверх
    model_position_ += QVector2D(delta.x(), -delta.y()) * effective_sensitivity;
    update();
  } else if (is_rotating_) {
    QPoint delta = event->pos() - last_rotate_pos_;
    last_rotate_pos_ = event->pos();

    // Обновляем углы поворота
    rotation_angles_.setY(rotation_angles_.y() + delta.x() * 0.5f);
    rotation_angles_.setX(rotation_angles_.x() + delta.y() * 0.5f);

    // Создаем матрицы поворота
    S21Matrix rotX = CreateRotationMatrix('X', rotation_angles_.x());
    S21Matrix rotY = CreateRotationMatrix('Y', rotation_angles_.y());

    // Комбинируем повороты: сначала Y, потом X
    rotation_matrix_ = rotY * rotX;

    update();
  }
}

S21Matrix Scene::CreateRotationMatrix(char axis, float angle) {
  S21Matrix matrix(4, 4);
  float rad = qDegreesToRadians(angle);
  float cosA = cos(rad);
  float sinA = sin(rad);

  // Initialize as identity matrix
  for (int i = 0; i < 4; ++i) {
    matrix(i, i) = 1.0f;
  }

  // Set rotation values based on axis
  switch (axis) {
    case 'X':
      matrix(1, 1) = cosA;
      matrix(1, 2) = -sinA;
      matrix(2, 1) = sinA;
      matrix(2, 2) = cosA;
      break;
    case 'Y':
      matrix(0, 0) = cosA;
      matrix(0, 2) = sinA;
      matrix(2, 0) = -sinA;
      matrix(2, 2) = cosA;
      break;
  }

  return matrix;
}

void Scene::mouseReleaseEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    is_dragging_ = false;
  }
}

}  // namespace s21

#include "view/scene.h"

namespace s21 {
Scene::Scene(QWidget* parent)
    : QOpenGLWidget(parent), vbo_(QOpenGLBuffer::VertexBuffer) {
  setFocusPolicy(Qt::StrongFocus);
  rotation_matrix_ = S21Matrix(4, 4);
  // Создаем единичную матрицу
  for (int i = 0; i < 4; ++i) {
    rotation_matrix_(i, i) = 1.0;
  }
  settings_ = SettingsFacade::GetInstance();
}

Scene::~Scene() {
  vbo_.destroy();
  settings_->SaveSettingsToFile();
}

void Scene::SetModel(const std::shared_ptr<WireframeObject> model) {
  model_ = model;
  vbo_.create();
  vbo_.bind();
  vbo_.allocate(model_->GetVertices().data(),
                model_->GetVertices().size() * sizeof(QVector3D));
  vbo_.release();
}

void Scene::initializeGL() { glEnable(GL_DEPTH_TEST); }

void Scene::paintGL() {
  auto bg = settings_->background_color_.GetOption();
  glClearColor(bg[0], bg[1], bg[2], bg[3]);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //--------------------------------------------------------------------
  // Настройка проекции
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  settings_->strategy_->ApplyProjection(width(), height());

  // Переключение на матрицу моделирования
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Настройка камеры (немного отодвинем сцену)
  glTranslatef(0.0f, 0.0f, -3.0f);

  // Применяем трансформации модели
  glScalef(scale_factor_, scale_factor_, scale_factor_);
  glTranslatef(model_position_.x(), model_position_.y(), 0.0f);
  glMultMatrixd(rotation_matrix_.get_matrix());

  // Отрисовка модели
  if (model_) {
    glLineWidth(settings_->edge_width_.GetSize());

    settings_->strategy_->ApplyEdgeDisplay();

    auto edge_color = settings_->edge_color_.GetOption();
    glColor3f(edge_color[0], edge_color[1], edge_color[2]);

    glBegin(GL_LINES);
    for (const auto face : model_->GetFaces()) {
      for (int i = 0; i < 3; ++i) {
        int j = (i + 1) % 3;
        glVertex3f(face.position[i]->x, face.position[i]->y,
                   face.position[i]->z);
        glVertex3f(face.position[j]->x, face.position[j]->y,
                   face.position[j]->z);
      }
    }
    glEnd();

    if (settings_->vertex_display_method_.GetName() != "none") {
      DrawAllVertices();
    }
  }
}

void Scene::DrawAllVertices() {
  auto vertex_color = settings_->vertex_color_.GetOption();
  glColor3f(vertex_color[0], vertex_color[1], vertex_color[2]);

  glPointSize(settings_->vertex_size_.GetSize());

  settings_->strategy_->ApplyVertexDisplay();

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, model_->GetVertices().data());
  glDrawArrays(GL_POINTS, 0, model_->GetVertices().size());
  glDisableClientState(GL_VERTEX_ARRAY);
}

}  // namespace s21

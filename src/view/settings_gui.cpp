
#include "view/settings_gui.h"

#include <QGraphicsDropShadowEffect>
namespace s21 {

void SettingsWindow::InitializeUI() {
  main_layout_ = new QVBoxLayout(this);
  main_layout_->setSpacing(10);
  this->SetBackgroundSettings();
  this->SetVerticesSettings();
  this->SetEdgesSettings();
  this->SetAnimationSettings();
  this->SetResetButton();

  ConnectObjectsWithButtons();
}

void SettingsWindow::AlignWithSettings() {
  for (const auto& [combo_box, setting] : kComboBoxes) {
    combo_box->setCurrentText(setting->GetName());
  }
  for (const auto& [slider, setting] : kSliders) {
    slider->setValue(setting->GetSize());
  }
  for (const auto& [spin_box, setting] : kSpinBoxes) {
    spin_box->setValue(setting->GetSize());
  }
}

void SettingsWindow::SetBackgroundSettings() {
  auto bg_group = new QGroupBox(tr("Background Settings"));
  auto bg_layout = new QGridLayout;

  bg_color_combo_ = new QComboBox;
  bg_color_combo_->setFixedWidth(kComboWidth);
  settings_->background_color_.SetQComboBox(bg_color_combo_);

  projection_combo_ = new QComboBox;
  settings_->projection_type_.SetQComboBox(projection_combo_);

  bg_layout->addWidget(new QLabel(tr("Color:")), 0, 0);
  bg_layout->addWidget(bg_color_combo_, 0, 1);
  bg_layout->addWidget(new QLabel(tr("Projection:")), 1, 0);
  bg_layout->addWidget(projection_combo_, 1, 1);

  bg_group->setLayout(bg_layout);
  bg_group->setStyleSheet(commonStyle);
  main_layout_->addWidget(bg_group);
}

void SettingsWindow::SetVerticesSettings() {
  auto vertex_group = new QGroupBox(tr("Vertex Settings"));
  auto vertex_layout = new QGridLayout;

  vertex_color_combo_ = new QComboBox;
  vertex_color_combo_->setFixedWidth(kComboWidth);
  settings_->vertex_color_.SetQComboBox(vertex_color_combo_);

  vertex_size_slider_ = new QSlider(Qt::Horizontal);
  vertex_size_slider_->setFixedSize(kComboWidth, kSliderHeight);
  vertex_size_slider_->setRange(kMinVertexSize, kMaxVertexSize);

  vertex_type_combo_ = new QComboBox;
  settings_->vertex_display_method_.SetQComboBox(vertex_type_combo_);

  vertex_layout->addWidget(new QLabel(tr("Color:")), 0, 0);
  vertex_layout->addWidget(vertex_color_combo_, 0, 1);
  vertex_layout->addWidget(new QLabel(tr("Size:")), 1, 0);
  vertex_layout->addWidget(vertex_size_slider_, 1, 1);
  vertex_layout->addWidget(new QLabel(tr("Type:")), 2, 0);
  vertex_layout->addWidget(vertex_type_combo_, 2, 1);

  vertex_group->setLayout(vertex_layout);
  vertex_group->setStyleSheet(commonStyle);
  main_layout_->addWidget(vertex_group);
}

void SettingsWindow::SetEdgesSettings() {
  auto edge_group = new QGroupBox(tr("Edge Settings"));
  auto edge_layout = new QGridLayout;

  edge_color_combo_ = new QComboBox;
  edge_color_combo_->setFixedWidth(kComboWidth);
  settings_->edge_color_.SetQComboBox(edge_color_combo_);

  edge_thickness_slider_ = new QSlider(Qt::Horizontal);
  edge_thickness_slider_->setFixedSize(kComboWidth, kSliderHeight);
  edge_thickness_slider_->setRange(kMinEdgeSize, kMaxEdgeSize);

  edge_type_combo_ = new QComboBox;
  settings_->edge_display_method_.SetQComboBox(edge_type_combo_);

  edge_layout->addWidget(new QLabel(tr("Color:")), 0, 0);
  edge_layout->addWidget(edge_color_combo_, 0, 1);
  edge_layout->addWidget(new QLabel(tr("Thickness:")), 1, 0);
  edge_layout->addWidget(edge_thickness_slider_, 1, 1);
  edge_layout->addWidget(new QLabel(tr("Type:")), 2, 0);
  edge_layout->addWidget(edge_type_combo_, 2, 1);

  edge_group->setLayout(edge_layout);
  edge_group->setStyleSheet(commonStyle);
  main_layout_->addWidget(edge_group);
}

void SettingsWindow::SetAnimationSettings() {
  auto anim_group = new QGroupBox(tr("Animation Settings"));
  auto anim_layout = new QGridLayout;

  // Настройка длительности
  duration_spin_ = new QSpinBox;
  duration_spin_->setFixedWidth(kDualSpinWidth);
  duration_spin_->setRange(kMinDurationSec,
                           kMaxDurationSec);  // от 1 до 60 секунд
  anim_layout->addWidget(new QLabel(tr("Duration (sec):")), 0, 0);
  anim_layout->addWidget(duration_spin_, 0, 1);

  // Настройка FPS
  fps_spin_ = new QSpinBox;
  fps_spin_->setRange(kMinFps, kMaxFps);  // от 1 до 60 кадров в секунду
  anim_layout->addWidget(new QLabel(tr("Frames per second:")), 1, 0);
  anim_layout->addWidget(fps_spin_, 1, 1);

  // Настройка ширины
  width_spin_ = new QSpinBox;
  width_spin_->setRange(kMinTargetWidth,
                        kMaxTargetWidth);  // от 100 до 1920 пикселей
  anim_layout->addWidget(new QLabel(tr("Width:")), 2, 0);
  anim_layout->addWidget(width_spin_, 2, 1);

  // Настройка высоты
  height_spin_ = new QSpinBox;
  height_spin_->setRange(kMinTargetHeight,
                         kMaxTargetHeight);  // от 100 до 1080 пикселей
  anim_layout->addWidget(new QLabel(tr("Height:")), 3, 0);
  anim_layout->addWidget(height_spin_, 3, 1);

  anim_group->setLayout(anim_layout);
  anim_group->setStyleSheet(commonStyle);
  main_layout_->addWidget(anim_group);
}

void SettingsWindow::SetResetButton() {
  reset_button_ = new QPushButton(tr("Reset settings"), this);
  reset_button_->setStyleSheet(commonStyle);
  reset_button_->setCursor(QCursor(Qt::PointingHandCursor));
  if (QFile::exists(":files/gif_lib/gif_image/icon_settings.png")) {
    QPixmap pixmap(":files/gif_lib/gif_image/icon_settings.png");
    if (!pixmap.isNull()) {
      reset_button_->setIcon(pixmap);
    }
  }
  main_layout_->addWidget(reset_button_);
}

void SettingsWindow::InititializeMaps() {
  kComboBoxes = {{bg_color_combo_, &settings_->background_color_},
                 {projection_combo_, &settings_->projection_type_},
                 {vertex_color_combo_, &settings_->vertex_color_},
                 {vertex_type_combo_, &settings_->vertex_display_method_},
                 {edge_color_combo_, &settings_->edge_color_},
                 {edge_type_combo_, &settings_->edge_display_method_}};
  kSliders = {{vertex_size_slider_, &settings_->vertex_size_},
              {edge_thickness_slider_, &settings_->edge_width_}};
  kSpinBoxes = {{duration_spin_, &settings_->duration_sec_},
                {fps_spin_, &settings_->fps_},
                {width_spin_, &settings_->target_width_},
                {height_spin_, &settings_->target_height_}};
}

void SettingsWindow::ConnectObjectsWithButtons() {
  InititializeMaps();

  for (const auto& combo_box : kComboBoxes) {
    QComboBox* combo_ptr = combo_box.first;
    connect(combo_ptr, &QComboBox::currentTextChanged, this,
            [this, combo_ptr](const QString& new_name) {
              this->OnComboChanged(combo_ptr, new_name);
            });
  }
  for (const auto& slider : kSliders) {
    QSlider* slider_ptr = slider.first;
    connect(slider_ptr, &QSlider::valueChanged, this,
            [this, slider_ptr](float new_size) {
              this->OnSliderChanged(slider_ptr, new_size);
            });
  }
  for (const auto& spin_box : kSpinBoxes) {
    QSpinBox* spin_box_ptr = spin_box.first;
    connect(spin_box_ptr, QOverload<int>::of(&QSpinBox::valueChanged), this,
            [this, spin_box_ptr](int new_size) {
              this->OnSpinBoxChanged(spin_box_ptr, new_size);
            });
  }

  connect(reset_button_, &QPushButton::clicked, this, &SettingsWindow::OnReset);
}
}  // namespace s21

#include "view/viewer_widget.h"

namespace s21 {

ViewerWidget::ViewerWidget(QWidget* parent) : QWidget(parent) {
  InitializeWidgets();
  DefineLayouts();
}

void ViewerWidget::InitializeWidgets() {
  open_file_button_ = new QPushButton(tr("Open File"));
  SetupRoundButton(open_file_button_, kIconWidth, kIconHeight);
  SetButtonIcon(":files/gif_lib/gif_image/icon_open_file.png",
                open_file_button_, kIconWidth, kIconHeight);

  save_image_button_ = new QPushButton(tr("Take screenshot"));
  SetupRoundButton(save_image_button_, kIconWidth, kIconHeight);
  SetButtonIcon(":files/gif_lib/gif_image/icon_screenshot.png",
                save_image_button_, kIconWidth, kIconHeight);

  record_animation_button_ = new QPushButton(tr("Record GIF"));
  SetupRoundButton(record_animation_button_, kIconWidth, kIconHeight);
  SetButtonIcon(":files/gif_lib/gif_image/icon_record.png",
                record_animation_button_, kIconWidth, kIconHeight);

  object_info_label_ = new QLabel(tr("No object loaded"), this);

  // Initialize panels
  left_panel_ = new QScrollArea(this);

  main_panel_ = new QWidget(this);

  // Initialize viewers
  log_viewer_ = new QTextEdit(this);
  main_viewer_ = new Scene(this);
  settings_window_ = new SettingsWindow(this, main_viewer_);

  connect(open_file_button_, &QPushButton::clicked, this,
          &ViewerWidget::OpenFile);
  connect(save_image_button_, &QPushButton::clicked, this,
          &ViewerWidget::SaveImage);
  connect(record_animation_button_, &QPushButton::clicked, this,
          &ViewerWidget::StartRecordingAnimation);
}

void ViewerWidget::SetupRoundButton(QPushButton* button, int width,
                                    int height) {
  button->setStyleSheet(commonStyle);
  button->setFixedWidth(width * 6);
  button->setCursor(QCursor(Qt::PointingHandCursor));
}

void ViewerWidget::DefineLayouts() {
  SetLeftPanel();
  SetMainPanel();

  // QHBox - horizontal appends, QVBox - vertical appends
  auto* main_widget_layout = new QHBoxLayout(this);
  main_widget_layout->addWidget(left_panel_);
  main_widget_layout->addWidget(main_panel_);
}

void ViewerWidget::SetLeftPanel() {
  auto* button_layout = new QVBoxLayout();
  button_layout->addWidget(open_file_button_);
  button_layout->addWidget(record_animation_button_);
  button_layout->addWidget(save_image_button_);

  auto* temp_widget = new QWidget;
  auto* temp_layout = new QVBoxLayout(temp_widget);
  temp_layout->addLayout(button_layout);
  temp_layout->addWidget(settings_window_);
  temp_layout->addStretch();
  temp_layout->addWidget(object_info_label_);

  left_panel_->setWidget(temp_widget);
  left_panel_->setWidgetResizable(true);
  left_panel_->setFixedWidth(kLeftPanelWidth);
  left_panel_->setStyleSheet("background-color: " + kCSSBlackColor + ";");
}

void ViewerWidget::SetMainPanel() {
  auto* main_layout = new QVBoxLayout(main_panel_);
  main_layout->addWidget(main_viewer_);

  // Configure log viewer
  log_viewer_->setReadOnly(true);
  log_viewer_->setFixedHeight(kLogViewerHeight);
  log_viewer_->setStyleSheet(commonStyle);
  log_viewer_->setPlaceholderText(
      "Application log messages will appear here...");
  main_layout->addWidget(log_viewer_);
}

void ViewerWidget::ShowError() {
  const char* kLogPath = "logs/debug.log";
  std::ifstream input_fstream(kLogPath, std::ios::in);

  if (!input_fstream) {
    QMessageBox::critical(this, "Error",
                          "Log file not found. Creating new one");
    std::ofstream(kLogPath, std::ios::out);
    return;
  }

  std::string error_message;
  GetLastLine(input_fstream, error_message);
  input_fstream.close();

  if (!error_message.empty()) {
    QMessageBox::critical(this, "Error", QString::fromStdString(error_message));
    log_viewer_->append(QString::fromStdString(error_message));
  }
}

void ViewerWidget::UpdateObjectInfo() {
  if (!current_object_ || current_object_->GetId() < 0) {
    ShowError();
    return;
  }

  std::ostringstream info;
  info << "Object Name: " << current_object_->GetName() << "\n"
       << "Object ID: " << current_object_->GetId() << "\n"
       << "Number of vertices: " << current_object_->GetVertices().size()
       << "\n"
       << "Number of faces: " << current_object_->GetFaces().size();

  object_info_label_->setText(QString::fromStdString(info.str()));
  main_viewer_->SetModel(current_object_);
  main_viewer_->update();
}

void ViewerWidget::GetLastLine(std::ifstream& input_fstream,
                               std::string& error_message) {
  std::string line;
  while (std::getline(input_fstream, line)) {
    error_message = std::move(line);
  }
}

void ViewerWidget::SaveImage() {
  QString selected_filter;
  QString file_name = QFileDialog::getSaveFileName(
      this, tr("Save Image"), QString(),
      tr("BMP Image (*.bmp);;JPEG Image (*.jpeg)"), &selected_filter);

  if (!file_name.isEmpty()) {
#ifdef __linux__
    // This part is specific for Debian/Ubuntu linux
    // because it doesn't save the file with extension suffix
    if (QFileInfo(file_name).suffix().isEmpty()) {
      if (selected_filter.startsWith("BMP"))
        file_name += ".bmp";
      else
        file_name += ".jpeg";
    }
#endif
    QImage img = main_viewer_->grabFramebuffer();
    // All these conversions are for save(), cause it requires
    // const char* instead of QChar
    img.save(file_name,
             QFileInfo(file_name).suffix().toUpper().toLatin1().constData(),
             100);
    QMessageBox::information(this, tr("Done"),
                             tr("Saved screenshot at %1").arg(file_name));
  }
}

void ViewerWidget::StartRecordingAnimation() {
  // Параметры анимации
  auto* settings = SettingsFacade::GetInstance();

  int duration_sec = settings->duration_sec_.GetSize();
  int fps = settings->fps_.GetSize();
  int target_width = settings->target_width_.GetSize();
  int target_height = settings->target_height_.GetSize();

  const int max_colors = 256;

  // Получаем имя файла
  QString filename =
      QFileDialog::getSaveFileName(this, tr("Save GIF"), "", tr("GIF (*.gif)"));
  if (filename.isEmpty()) return;

  // 1. Захватываем пробный кадр для построения палитры
  QImage sample_frame = main_viewer_->grabFramebuffer();
  sample_frame =
      sample_frame.scaled(target_width, target_height, Qt::KeepAspectRatio,
                          Qt::SmoothTransformation);

  // 2. Создаем оптимальную палитру (256 цветов)
  QImage indexed =
      sample_frame.convertToFormat(QImage::Format_Indexed8, Qt::AutoColor);
  QList<QRgb> color_table = indexed.colorTable();

  // Если цветов больше чем нужно - уменьшаем
  if (color_table.size() > max_colors) {
    indexed = sample_frame.convertToFormat(QImage::Format_Indexed8,
                                           Qt::DiffuseDither | Qt::ColorOnly);
    color_table = indexed.colorTable();
  }

  // 3. Подготавливаем палитру для gifenc
  uint8_t palette[max_colors * 3];
  for (int i = 0; i < color_table.size() && i < max_colors; i++) {
    palette[i * 3] = qRed(color_table[i]);
    palette[i * 3 + 1] = qGreen(color_table[i]);
    palette[i * 3 + 2] = qBlue(color_table[i]);
  }

  // 4. Создаем GIF с нашей палитрой
  ge_GIF* gif = ge_new_gif(filename.toStdString().c_str(), target_width,
                           target_height, palette, 8,  // 8 бит = 256 цветов
                           -1, 0  // Без прозрачности, бесконечный цикл
  );

  // Таймер для захвата кадров
  QTimer* timer = new QTimer(this);
  int frame_count = 0;
  const int total_frames = duration_sec * fps;

  connect(timer, &QTimer::timeout, [=]() mutable {
    if (frame_count >= total_frames) {
      timer->stop();
      ge_close_gif(gif);
      timer->deleteLater();
      QMessageBox::information(this, tr("Done"),
                               tr("Saved %1 frames").arg(total_frames));
      return;
    }

    QImage frame = main_viewer_->grabFramebuffer();
    frame = frame.scaled(target_width, target_height, Qt::IgnoreAspectRatio,
                         Qt::SmoothTransformation);
    frame = frame.convertToFormat(QImage::Format_Indexed8, color_table);

    // Копируем данные в GIF из буфера Qimage по строчно
    for (int y = 0; y < target_height; y++) {
      memcpy(gif->frame + y * target_width, frame.constScanLine(y),
             target_width);
    }

    ge_add_frame(gif, 100 / fps);  // Задержка в 1/100 сек
    frame_count++;
  });

  timer->start(1000 / fps);  // Интервал в мс
}

void ViewerWidget::SetButtonIcon(const QString& imagePath, QPushButton* button,
                                 int width, int height) {
  if (QFile::exists(imagePath)) {
    button->setIcon(QIcon(imagePath));
    button->setIconSize(QSize(kIconWidth, kIconHeight));
  }
}

}  // namespace s21

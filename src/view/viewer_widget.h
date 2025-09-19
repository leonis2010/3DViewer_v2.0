#ifndef VIEW_VIEWER_WIDGET_H
#define VIEW_VIEWER_WIDGET_H

#include <stdio.h>

extern "C" {
#include "gif_lib/gifenc.h"
}

#include <QApplication>
#include <QDateTime>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include "model/errors.h"
#include "model/parser.h"
#include "view/scene.h"
#include "view/settings_facade.h"
#include "view/settings_gui.h"

namespace s21 {

const int kLeftPanelWidth = 320;

const int kMainViewerWidth = 1920;
const int kMainViewerHeight = 1080;

const int kLogViewerHeight = 150;

const int kIconWidth = 35;
const int kIconHeight = 35;

const int kStdMarginLeft = 40;
const int kStdMarginUp = 20;

/**
 * @class ViewerWidget
 * @brief Main widget for the 3D model viewer application
 *
 * This widget provides a user interface for loading and viewing 3D wireframe
 * models. It consists of two main panels:
 * - Left panel: Contains controls for file operations and settings
 * - Main panel: Displays the 3D model viewer and debugging information
 */
class ViewerWidget : public QWidget {
  Q_OBJECT

 public:
  /**
   * @brief Constructs the viewer widget
   * @param parent Parent widget is nullptr, because it's a root widget
   */
  ViewerWidget(QWidget* parent = nullptr);
  ~ViewerWidget() = default;

 private:
  Q_SLOT void OpenFile();
  Q_SLOT void SaveImage();

 private:
  // UI initialization methods
  void InitializeWidgets();
  void DefineLayouts();
  void SetLeftPanel();
  void SetMainPanel();

  // Logic methods
  void ShowError();
  void UpdateObjectInfo();
  void SetButtonIcon(const QString& imagePath, QPushButton* button, int width,
                     int height);

  /**
   * @brief Helper function to get the last line from a file
   * @param input_fstream Input file stream
   * @param error_message String to store the last line
   */
  void GetLastLine(std::ifstream& input_fstream, std::string& error_message);

 private:
  // UI Components
  QScrollArea* left_panel_{nullptr};
  QWidget* main_panel_{nullptr};
  QPushButton* open_file_button_{nullptr};
  QPushButton* save_image_button_{nullptr};
  QPushButton* record_animation_button_{nullptr};

  QLabel* object_info_label_{nullptr};
  Scene* main_viewer_{nullptr};
  QTextEdit* log_viewer_{nullptr};

  SettingsWindow* settings_window_{nullptr};
  void StartRecordingAnimation();
  void SetupRoundButton(QPushButton* button, int width, int height);

  // Model data
  std::shared_ptr<WireframeObject> current_object_{nullptr};
};

}  // namespace s21

#endif  // VIEW_VIEWER_WIDGET_H

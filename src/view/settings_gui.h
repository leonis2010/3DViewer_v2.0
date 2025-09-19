#ifndef VIEW_SETTINGS_GUI_H
#define VIEW_SETTINGS_GUI_H

#include <QApplication>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QImage>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <memory>
#include <string>

#include "model/errors.h"
#include "view/scene.h"
#include "view/settings_facade.h"

namespace s21 {

const int kComboWidth = 120;

const int kDualSpinWidth = 50;

const int kSliderHeight = 20;

const QString kCSSBlackColor = "#000000";
const QString kCSSWhiteColor = "#FFFFFF";
const QString kCSSDarkGrayColor = "#6A6A6A";
const QString kCSSGoldishColor = "#FFE8A1";

const QString commonStyle =
    "QTextEdit {"
    "  background-color: " +
    kCSSBlackColor +
    ";"
    "}"

    "QLabel {"
    "  font-family: 'Quicksand';"
    "  font-size: default;"
    "  color: " +
    kCSSWhiteColor +
    ";"
    "}"

    "QComboBox {"
    "  font-family: 'Quicksand';"
    "  font-size: default;"
    "  border: 1px solid " +
    kCSSDarkGrayColor +
    ";"
    "  color: " +
    kCSSGoldishColor +
    ";"
    "  background-color: " +
    kCSSBlackColor +
    ";"
    "}"
    "QComboBox:hover {"
    "  border: 1px solid " +
    kCSSGoldishColor +
    ";"
    "}"

    "QSpinBox {"
    "  font-family: 'Quicksand';"
    "  font-size: default;"
    "  border: 1px solid " +
    kCSSDarkGrayColor +
    ";"
    "  color: " +
    kCSSGoldishColor +
    ";"
    "  background-color: " +
    kCSSBlackColor +
    ";"
    "}"
    "QSpinBox:hover {"
    "  border: 1px solid " +
    kCSSGoldishColor +
    ";"
    "}"

    "QSpinBox::up-button, QSpinBox::down-button {"
    "  background-color: " +
    kCSSBlackColor +
    ";"
    "  border: 1px solid " +
    kCSSDarkGrayColor +
    ";"
    "}"
    "QSpinBox::up-button:hover, QSpinBox::down-button:hover {"
    "  background-color: " +
    kCSSGoldishColor +
    ";"
    "}"

    "QSlider {"
    "  margin-left: 10px;"
    "}"
    "QSlider::groove:horizontal {"
    "  background: " +
    kCSSBlackColor +
    ";"
    "  height: 6px;"
    "  border-radius: 4px;"
    "}"
    "QSlider::handle:horizontal {"
    "  background: " +
    kCSSDarkGrayColor +
    ";"
    "  width: 18px;"
    "  height: 18px;"
    "  margin: -6px 0;"
    "  border-radius: 9px;"
    "}"
    "QSlider::sub-page:horizontal {"
    "  background: " +
    kCSSGoldishColor +
    ";"
    "  border-radius: 4px;"
    "}"
    "QSlider::handle:horizontal:hover {"
    "  background: " +
    kCSSGoldishColor +
    ";"
    "}"

    "QPushButton {"
    "  background-color: " +
    kCSSBlackColor +
    ";"
    "  text-align: left;"
    "  margin-left: 10px;"
    "  border: none;"
    "}"
    "QPushButton:hover {"
    "  color: " +
    kCSSGoldishColor +
    ";"
    "}";

/**
 * @class SettingsWindow
 * @brief A window for adjusting viewer settings
 *
 * The SettingsWindow class provides a GUI interface for modifying various
 * display settings including background color, vertex properties (color, size,
 * type), and edge properties (color, thickness, type). Settings are
 * automatically saved when the window is closed.
 *
 * This window is created as a separate top-level window and manages its own
 * lifecycle, destroying itself when closed.
 */

class SettingsWindow : public QWidget {
  Q_OBJECT

 public:
  SettingsWindow(QWidget *parent = nullptr, Scene *main_viewer = nullptr)
      : QWidget(parent), main_viewer_(main_viewer) {
    settings_ = SettingsFacade::GetInstance();
    InitializeUI();
    AlignWithSettings();
  }
  ~SettingsWindow() = default;

 private:
  void InitializeUI();
  void AlignWithSettings();

  void SetBackgroundSettings();
  void SetVerticesSettings();
  void SetEdgesSettings();
  void SetResetButton();
  void SetAnimationSettings();

  void InititializeMaps();
  void ConnectObjectsWithButtons();

  Q_SLOT void OnComboChanged(QComboBox *combo_box, const QString &new_name) {
    auto it = kComboBoxes.find(combo_box);
    if (it != kComboBoxes.end()) {
      it->second->SetOptionByName(new_name);
      settings_->UpdateStrategy();
      main_viewer_->update();
    }
  }

  Q_SLOT void OnSliderChanged(QSlider *slider, float new_size) {
    auto it = kSliders.find(slider);
    if (it != kSliders.end()) {
      it->second->SetSize(new_size);
      main_viewer_->update();
    }
  }

  Q_SLOT void OnSpinBoxChanged(QSpinBox *spin_box, int new_size) {
    auto it = kSpinBoxes.find(spin_box);
    if (it != kSpinBoxes.end()) {
      it->second->SetSize(new_size);
      main_viewer_->update();
    }
  }

  Q_SLOT void OnReset() {
    settings_->ResetSettings();
    main_viewer_->update();
    AlignWithSettings();
  }

 private:
  SettingsFacade *settings_{nullptr};

  Scene *main_viewer_{nullptr};

  QVBoxLayout *main_layout_{nullptr};

  QComboBox *bg_color_combo_{nullptr};
  QComboBox *projection_combo_{nullptr};

  QComboBox *vertex_color_combo_{nullptr};
  QSlider *vertex_size_slider_{nullptr};
  QComboBox *vertex_type_combo_{nullptr};

  QComboBox *edge_color_combo_{nullptr};
  QSlider *edge_thickness_slider_{nullptr};
  QComboBox *edge_type_combo_{nullptr};

  QSpinBox *duration_spin_{nullptr};
  QSpinBox *fps_spin_{nullptr};
  QSpinBox *width_spin_{nullptr};
  QSpinBox *height_spin_{nullptr};

  QPushButton *reset_button_{nullptr};

 private:
  struct PtrComparator {
    bool operator()(const QObject *a, const QObject *b) const { return a < b; };
  };
  std::map<QComboBox *, IBasicSettings *, PtrComparator> kComboBoxes;
  std::map<QSlider *, FloatSize *, PtrComparator> kSliders;
  std::map<QSpinBox *, IntSize *, PtrComparator> kSpinBoxes;
};
}  // namespace s21

#endif  // VIEW_SETTINGS_GUI_H

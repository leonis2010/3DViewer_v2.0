#ifndef MODEL_SETTINGS_H_
#define MODEL_SETTINGS_H_

#include <QComboBox>
#include <QString>
#include <array>
#include <fstream>
#include <map>
#include <string>

#include "model/errors.h"
#include "model/nlohmann/json.hpp"
#include "view/opengl_strategy.h"

namespace s21 {

const float kMinVertexSize = 1.0f;
const float kMaxVertexSize = 30.0f;
const float kDefaultVertexSize = 10.0f;

const float kMinEdgeSize = 1.0f;
const float kMaxEdgeSize = 15.0f;
const float kDefaultEdgeSize = 2.0f;

const int kMinDurationSec = 1;
const int kMaxDurationSec = 60;
const int kDurationSec = 5;

const int kMinFps = 10;
const int kMaxFps = 60;
const int kFps = 24;

const int kMinTargetWidth = 160;
const int kMaxTargetWidth = 1280;
const int kTargetWidth = 640;

const int kMinTargetHeight = 90;
const int kMaxTargetHeight = 720;
const int kTargetHeight = 480;

typedef enum {
  kOrtographic,  // Параллельная
  kPerspective   // Центральная
} ProjectionTypeT;

typedef enum {
  kVertexDisplayNone = 0,
  kVertexDisplayCircle,
  kVertexDisplaySquare,
} VertexDisplayMethodT;

typedef enum {
  kEdgeDisplaySolid = 0,
  kEdgeDisplayDashed,
} EdgeDisplayMethodT;

const std::array<float, 4> kDarkBlueBackgroundColor = {0.1f, 0.2f, 0.4f, 1.0f};
const std::array<float, 4> kLightBlueBackgroundColor = {0.5f, 0.7f, 0.9f, 1.0f};
const std::array<float, 4> kWhiteBackgroundColor = {1.0f, 1.0f, 1.0f, 1.0f};
const std::array<float, 4> kBlackBackgroundColor = {0.0f, 0.0f, 0.0f, 1.0f};

const std::array<float, 3> kRedColor = {1.0f, 0.0f, 0.0f};
const std::array<float, 3> kGreenColor = {0.0f, 1.0f, 0.0f};
const std::array<float, 3> kBlueColor = {0.0f, 0.0f, 1.0f};

const std::array<float, 3> kGoldColor = {0.953f, 0.792f, 0.1255f};
const std::array<float, 3> kCyanColor = {0.0f, 1.0f, 1.0f};
const std::array<float, 3> kMagentaColor = {1.0f, 0.0f, 1.0f};
const std::array<float, 3> kYellowColor = {1.0f, 1.0f, 0.0f};
const std::array<float, 3> kBlackColor = {0.0f, 0.0f, 0.0f};
const std::array<float, 3> kWhiteColor = {1.0f, 1.0f, 1.0f};

const QString kDarkBlueString = "dark_blue";
const QString kLightBlueString = "light_blue";
const QString kBlackString = "black";
const QString kWhiteString = "white";
const QString kRedString = "red";
const QString kGreenString = "green";
const QString kBlueString = "blue";
const QString kGoldString = "gold";
const QString kCyanString = "cyan";
const QString kMagentaString = "magenta";
const QString kYellowString = "yellow";

const QString kOrtographicString = "ortographic";
const QString kPerspectiveString = "perspective";

const QString kNoneString = "none";
const QString kCircleString = "circle";
const QString kSquareString = "square";

const QString kSolidString = "solid";
const QString kDashedString = "dashed";

const std::map<QString, const std::array<float, 4>> kBackgroundColors = {
    {kDarkBlueString, kDarkBlueBackgroundColor},
    {kLightBlueString, kLightBlueBackgroundColor},
    {kWhiteString, kWhiteBackgroundColor},
    {kBlackString, kBlackBackgroundColor}};
const std::map<QString, ProjectionTypeT> kProjectionTypes = {
    {kOrtographicString, kOrtographic}, {kPerspectiveString, kPerspective}};
const std::map<QString, const std::array<float, 3>> kVertexColors = {
    {kRedString, kRedColor},
    {kGreenString, kGreenColor},
    {kBlueString, kBlueColor},
    {kBlackString, kBlackColor},
    {kWhiteString, kWhiteColor}};
const std::map<QString, VertexDisplayMethodT> kVertexDisplayMethods = {
    {kNoneString, kVertexDisplayNone},
    {kCircleString, kVertexDisplayCircle},
    {kSquareString, kVertexDisplaySquare}};
const std::map<QString, const std::array<float, 3>> kEdgeColors = {
    {kGoldString, kGoldColor},       {kCyanString, kCyanColor},
    {kMagentaString, kMagentaColor}, {kYellowString, kYellowColor},
    {kBlackString, kBlackColor},     {kWhiteString, kWhiteColor}};
const std::map<QString, EdgeDisplayMethodT> kEdgeDisplayMethods = {
    {kSolidString, kEdgeDisplaySolid}, {kDashedString, kEdgeDisplayDashed}};

class IBasicSettings {
 public:
  virtual const std::string GetStdStringName() const = 0;
  virtual const QString GetName() const = 0;

  virtual void SetOptionByName(const std::string&) = 0;
  virtual void SetOptionByName(const QString&) = 0;
  virtual void SetQComboBox(QComboBox*) = 0;

  virtual void SetDefaultValues() = 0;
};

template <typename Option, typename OptionMap>
class BasicSettings : public IBasicSettings {
 public:
  const std::string GetStdStringName() const { return name_.toStdString(); }
  const QString GetName() const { return name_; }
  const Option GetOption() const { return *option_; }

  void SetOptionByName(const std::string& name) {
    SetOptionByName(QString::fromStdString(name));
  }
  void SetOptionByName(const QString& name) {
    auto it = option_map_->find(name);
    if (it != option_map_->end()) {
      name_ = name;
      option_ = &it->second;
    }
  }
  void SetQComboBox(QComboBox* combo_box) {
    for (const auto& option : *option_map_) {
      combo_box->addItem(option.first);
    }
    combo_box->setCurrentText(name_);
  }

 protected:
  QString name_;
  const Option* option_;
  const OptionMap* option_map_;
};  // class BasicSettings

class BackgroundSettings
    : public BasicSettings<std::array<float, 4>,
                           std::map<QString, const std::array<float, 4>>> {
 public:
  BackgroundSettings() {
    option_map_ = &kBackgroundColors;
    SetDefaultValues();
  }
  void SetDefaultValues() { SetOptionByName(kBlackString); }
};  // class BackgroundSettings

class ProjectionSettings
    : public BasicSettings<ProjectionTypeT,
                           std::map<QString, ProjectionTypeT>> {
 public:
  ProjectionSettings() {
    option_map_ = &kProjectionTypes;
    SetDefaultValues();
  }
  void SetDefaultValues() { SetOptionByName(kOrtographicString); }
};  // class ProjectionSettings

class VertexColorSettings
    : public BasicSettings<std::array<float, 3>,
                           std::map<QString, const std::array<float, 3>>> {
 public:
  VertexColorSettings() {
    option_map_ = &kVertexColors;
    SetDefaultValues();
  }
  void SetDefaultValues() { SetOptionByName(kWhiteString); }
};  // class VertexColorSettings

class VertexDisplaySettings
    : public BasicSettings<VertexDisplayMethodT,
                           std::map<QString, VertexDisplayMethodT>> {
 public:
  VertexDisplaySettings() {
    option_map_ = &kVertexDisplayMethods;
    SetDefaultValues();
  }
  void SetDefaultValues() { SetOptionByName(kNoneString); }
};  // class VertexDisplaySettings

class EdgeColorSettings
    : public BasicSettings<std::array<float, 3>,
                           std::map<QString, const std::array<float, 3>>> {
 public:
  EdgeColorSettings() {
    option_map_ = &kEdgeColors;
    SetDefaultValues();
  }
  void SetDefaultValues() { SetOptionByName(kGoldString); }
};  // class EdgeColorSettings

class EdgeDisplaySettings
    : public BasicSettings<EdgeDisplayMethodT,
                           std::map<QString, EdgeDisplayMethodT>> {
 public:
  EdgeDisplaySettings() {
    option_map_ = &kEdgeDisplayMethods;
    SetDefaultValues();
  }
  void SetDefaultValues() { SetOptionByName(kSolidString); }
};  // class EdgeDisplaySettings

class FloatSize {
 public:
  explicit FloatSize(float default_size) {
    default_size_ = default_size;
    SetSize(default_size);
  }
  const float GetSize() const { return size_; }
  void SetSize(float new_size) { size_ = new_size; }
  void SetDefaultValues() { size_ = default_size_; }

 protected:
  float size_;
  float default_size_;
};  // class FloatSize

class IntSize {
 public:
  explicit IntSize(int default_size) {
    default_size_ = default_size;
    SetSize(default_size);
  }
  const int GetSize() const { return size_; }
  void SetSize(int new_size) { size_ = new_size; }
  void SetDefaultValues() { size_ = default_size_; }

 protected:
  int size_;
  int default_size_;
};  // class IntSize

/**
 * @class SettingsFacade
 * @brief Singleton facade class for managing 3D viewer display settings
 *
 * This class implements the Singleton pattern and serves as a facade for
 * managing all display-related settings for the 3D viewer application. It
 * handles:
 * - Background color configuration
 * - Vertex display properties (color, size, display method)
 * - Edge display properties (color, width, display method)
 *
 * The class provides methods for loading/saving settings from/to a file and
 * offers a comprehensive interface for getting and setting various display
 * parameters.
 */
class SettingsFacade {
 public:
  static SettingsFacade* GetInstance() {
    if (!instance_) {
      instance_ = new SettingsFacade();
    }
    return instance_;
  }

  void LoadSettingsFromFile();
  void SaveSettingsToFile();
  void ResetSettings();
  void UpdateStrategy();

 public:
  OpenGLStrategy* strategy_{nullptr};

  BackgroundSettings background_color_;
  ProjectionSettings projection_type_;

  VertexColorSettings vertex_color_;
  VertexDisplaySettings vertex_display_method_;
  FloatSize vertex_size_ = FloatSize(kDefaultVertexSize);

  EdgeColorSettings edge_color_;
  EdgeDisplaySettings edge_display_method_;
  FloatSize edge_width_ = FloatSize(kDefaultEdgeSize);

  IntSize duration_sec_ = IntSize(kDurationSec);
  IntSize fps_ = IntSize(kFps);
  IntSize target_width_ = IntSize(kTargetWidth);
  IntSize target_height_ = IntSize(kTargetHeight);

 protected:
  SettingsFacade() {
    LoadSettingsFromFile();
    strategy_ = new OpenGLStrategy();
    UpdateStrategy();
  }
  ~SettingsFacade() = default;

 private:
  static SettingsFacade* instance_;
  const std::map<std::string, IBasicSettings*> settings_name_map_ = {
      {"background_color_", &background_color_},
      {"projection_type_", &projection_type_},
      {"vertex_color_", &vertex_color_},
      {"vertex_display_method_", &vertex_display_method_},
      {"edge_color_", &edge_color_},
      {"edge_display_method_", &edge_display_method_}};

  const std::map<std::string, FloatSize*> settings_float_map_ = {
      {"vertex_size_", &vertex_size_}, {"edge_width_", &edge_width_}};

  const std::map<std::string, IntSize*> settings_int_map_ = {
      {"duration_sec_", &duration_sec_},
      {"fps_", &fps_},
      {"target_width_", &target_width_},
      {"target_height_", &target_height_}};
};  // class SettingsFacade
}  // namespace s21

#endif  // MODEL_SETTINGS_H_

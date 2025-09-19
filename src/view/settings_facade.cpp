#include "view/settings_facade.h"

namespace s21 {
SettingsFacade* SettingsFacade::instance_ = nullptr;

void SettingsFacade::LoadSettingsFromFile() {
  try {
    std::ifstream input_fstream("logs/viewer_settings.json");
    if (input_fstream.is_open()) {
      nlohmann::json json_db;
      input_fstream >> json_db;

      for (const auto& [name, setting] : settings_name_map_) {
        if (json_db.contains(name)) {
          setting->SetOptionByName(json_db[name]);
        }
      }
      for (const auto& [name, setting] : settings_float_map_) {
        if (json_db.contains(name)) {
          setting->SetSize((float)json_db[name]);
        }
      }
      for (const auto& [name, setting] : settings_int_map_) {
        if (json_db.contains(name)) {
          setting->SetSize((int)json_db[name]);
        }
      }
    }
  } catch (...) {
    LogError("LoadSettingsFromFile",
             "Failed to load viewer settings from file.");
  }
}

void SettingsFacade::SaveSettingsToFile() {
  try {
    nlohmann::json json_db;
    for (const auto& [name, setting] : settings_name_map_) {
      json_db[name] = setting->GetStdStringName();
    }
    for (const auto& [name, setting] : settings_float_map_) {
      json_db[name] = setting->GetSize();
    }
    for (const auto& [name, setting] : settings_int_map_) {
      json_db[name] = setting->GetSize();
    }

    std::ofstream output_fstream("logs/viewer_settings.json");
    output_fstream << json_db.dump(2);  // dump with 2 spaces indentation
  } catch (...) {
    LogError("SaveSettingsToFile", "Failed to save viewer settings to file.");
  }
}

void SettingsFacade::ResetSettings() {
  for (const auto& setting : settings_name_map_) {
    setting.second->SetDefaultValues();
  }
  for (const auto& setting : settings_float_map_) {
    setting.second->SetDefaultValues();
  }
  for (const auto& setting : settings_int_map_) {
    setting.second->SetDefaultValues();
  }
}

void SettingsFacade::UpdateStrategy() {
  if (projection_type_.GetName() == "ortographic") {
    strategy_->SetProjectionFunction(OrthographicProjection);
  } else {
    strategy_->SetProjectionFunction(PerspectiveProjection);
  }
  if (edge_display_method_.GetName() == "solid") {
    strategy_->SetEdgeDisplayFunction(SolidEdges);
  } else {
    strategy_->SetEdgeDisplayFunction(DashedEdges);
  }
  if (vertex_display_method_.GetName() == "circle") {
    strategy_->SetVertexDisplayFunction(CircleVertices);
  } else {
    strategy_->SetVertexDisplayFunction(SquareVertices);
  }
}
}  // namespace s21
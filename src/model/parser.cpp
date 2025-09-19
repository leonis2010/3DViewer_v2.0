#include "model/parser.h"

namespace s21 {
int WireframeObject::next_id_ = 0;

WireframeObject::WireframeObject(const std::string file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    LogError("WireframeObject", file_not_found);
  } else {
    if (AllocateMemory(file) == success_code) {
      id_ = next_id_++;
      GetValues(file);
      AssignName(file_path);
    }
    file.close();
  }
}

WireframeObject::~WireframeObject() {
  vertices_.clear();
  textures_.clear();
  normals_.clear();
  faces_.clear();
  count_ = Counter();
  id_ = -1;
  name_.clear();
}

WireframeObject::WireframeObject(const WireframeObject &other)
    : name_(other.name_),
      vertices_(other.vertices_),
      faces_(other.faces_),
      textures_(other.textures_),
      normals_(other.normals_),
      count_(other.count_) {
  id_ = next_id_++;
}

WireframeObject &WireframeObject::operator=(const WireframeObject &other) {
  if (this != &other) {
    name_ = other.name_;
    vertices_ = other.vertices_;
    faces_ = other.faces_;
    textures_ = other.textures_;
    normals_ = other.normals_;
    count_ = other.count_;
    id_ = next_id_++;
  }
  return *this;
}

ErrorCode WireframeObject::AllocateMemory(std::ifstream &file) {
  const std::map<std::string, InputFunction> inspect_types = {
      {"v", &WireframeObject::CheckVertex},
      {"vt", &WireframeObject::CheckTexture},
      {"vn", &WireframeObject::CheckNormal},
      {"f", &WireframeObject::CheckFace}};

  std::string line;
  ErrorCode result_code = success_code;

  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') continue;

    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix;

    auto it = inspect_types.find(prefix);
    if (it != inspect_types.end() && !(this->*(it->second))(iss)) {
      LogError("AllocateMemory", "Invalid format in line: " + line);
      result_code = invalid_format;
      break;
    }
  }

  if (result_code == success_code && !ValidateCounters()) {
    result_code = invalid_format;
    LogError("AllocateMemory", invalid_format);
  }

  if (result_code == success_code) {
    try {
      vertices_.reserve(count_.v);
      textures_.reserve(count_.vt);
      normals_.reserve(count_.vn);
      faces_.reserve(count_.f);
    } catch (const std::bad_alloc &) {
      result_code = memory_error;
    }
  }

  file.clear();
  file.seekg(0, std::ios::beg);
  return result_code;
}

void WireframeObject::GetValues(std::ifstream &file) noexcept {
  const std::map<std::string, ParseFunction> parse_types = {
      {"v", &WireframeObject::ParseVertex},
      {"vt", &WireframeObject::ParseTextureCoordinate},
      {"vn", &WireframeObject::ParseNormal},
      {"f", &WireframeObject::ParseFace}};

  std::string line;

  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#') continue;

    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix;

    auto it = parse_types.find(prefix);
    if (it != parse_types.end()) (this->*(it->second))(iss);
  }
}

void WireframeObject::AssignName(const std::string file_path) noexcept {
  size_t last_slash = file_path.find_last_of("/\\");
  if (last_slash != std::string::npos) {
    name_ = file_path.substr(last_slash + 1);
  } else {
    name_ = file_path;
  }
}

bool WireframeObject::CheckVertex(std::istringstream &iss) {
  Coordinate vertex;
  if (!(iss >> vertex.x >> vertex.y >> vertex.z)) return false;
  if (!vertex.IsValid()) return false;
  count_.v++;
  return true;
}

bool WireframeObject::CheckTexture(std::istringstream &iss) {
  TextureCoordinate texture;
  if (!(iss >> texture.u >> texture.v)) return false;
  if (!texture.IsValid()) return false;
  count_.vt++;
  return true;
}

bool WireframeObject::CheckNormal(std::istringstream &iss) {
  Coordinate normal;
  if (!(iss >> normal.x >> normal.y >> normal.z)) return false;
  if (!normal.IsValid()) return false;
  // Normal vector components should be in [-1, 1]
  if (std::abs(normal.x) > 1.01f || std::abs(normal.y) > 1.01f ||
      std::abs(normal.z) > 1.01f) {
    return false;
  }
  count_.vn++;
  return true;
}

bool WireframeObject::CheckFace(std::istringstream &iss) {
  // Face face;
  std::string vertex_data;
  for (int i = 0; i < 3; i++) {
    if (!(iss >> vertex_data)) {
      return false;
    }
    size_t slash_count =
        std::count(vertex_data.begin(), vertex_data.end(), '/');

    int v_idx = 0, vt_idx = 0, vn_idx = 0;
    if (slash_count == 2) {
      if (sscanf(vertex_data.c_str(), "%d/%d/%d", &v_idx, &vt_idx, &vn_idx) !=
          3) {
        return false;
      }
      if (v_idx < 1 || v_idx > count_.v || vt_idx < 1 || vt_idx > count_.vt ||
          vn_idx < 1 || vn_idx > count_.vn) {
        return false;
      }
    } else if (slash_count == 1) {
      if (sscanf(vertex_data.c_str(), "%d/%d", &v_idx, &vt_idx) != 2) {
        return false;
      }
      if (v_idx < 1 || v_idx > count_.v || vt_idx < 1 || vt_idx > count_.vt) {
        return false;
      }
    } else if (slash_count == 0) {
      if (sscanf(vertex_data.c_str(), "%d", &v_idx) != 1) {
        return false;
      }
      if (v_idx < 1 || v_idx > count_.v) {
        return false;
      }
    } else {
      return false;
    }
  }
  count_.f++;
  return true;
}

void WireframeObject::ParseVertex(std::istringstream &iss) {
  Coordinate vertex;
  iss >> vertex.x >> vertex.y >> vertex.z;
  vertices_.push_back(vertex);
}

void WireframeObject::ParseTextureCoordinate(std::istringstream &iss) {
  TextureCoordinate texture;
  iss >> texture.u >> texture.v;
  textures_.push_back(texture);
}

void WireframeObject::ParseNormal(std::istringstream &iss) {
  Coordinate normal;
  iss >> normal.x >> normal.y >> normal.z;
  normals_.push_back(normal);
}

void WireframeObject::ParseFace(std::istringstream &iss) {
  Face face;
  std::string vertex_data;

  for (int i = 0; i < 3; i++) {
    iss >> vertex_data;
    size_t slash_count =
        std::count(vertex_data.begin(), vertex_data.end(), '/');

    int v_idx = 0, vt_idx = 0, vn_idx = 0;
    if (slash_count == 2) {
      // v/vt/vn format
      sscanf(vertex_data.c_str(), "%d/%d/%d", &v_idx, &vt_idx, &vn_idx);
      face.position[i] = &vertices_[v_idx - 1];
      face.texture[i] = &textures_[vt_idx - 1];
      face.normal = &normals_[vn_idx - 1];
    } else if (slash_count == 1) {
      // v/vt format
      sscanf(vertex_data.c_str(), "%d/%d", &v_idx, &vt_idx);
      face.position[i] = &vertices_[v_idx - 1];
      face.texture[i] = &textures_[vt_idx - 1];
      face.normal = nullptr;
    } else if (slash_count == 0) {
      // v format
      sscanf(vertex_data.c_str(), "%d", &v_idx);
      face.position[i] = &vertices_[v_idx - 1];
      face.texture[i] = nullptr;
      face.normal = nullptr;
    }
  }

  faces_.push_back(face);
}

bool WireframeObject::ValidateCounters() const {
  if (count_.v <= 0 || count_.v > kMaxVertices) {
    LogError("ValidateCounters", "Invalid vertex count_");
    return false;
  }
  if (count_.f <= 0 || count_.f > kMaxFaces) {
    LogError("ValidateCounters", "Invalid face count_");
    return false;
  }
  return true;
}
}  // namespace s21

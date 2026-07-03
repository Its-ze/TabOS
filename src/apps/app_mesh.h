#pragma once

#include "../core/mesh_manager.h"
#include "app_base.h"

namespace tabos {

class MeshApp : public AppBase {
 public:
  explicit MeshApp(MeshManager& mesh);

  const char* id() const override { return "mesh"; }
  const char* name() const override { return "Mesh"; }
  const char* icon() const override { return "MS"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  MeshManager& _mesh;
};

}  // namespace tabos

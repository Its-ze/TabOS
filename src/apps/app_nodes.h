#pragma once

#include "../core/mesh_manager.h"
#include "app_base.h"

namespace tabos {

class NodesApp : public AppBase {
 public:
  explicit NodesApp(MeshManager& mesh);

  const char* id() const override { return "nodes"; }
  const char* name() const override { return "Nodes"; }
  const char* icon() const override { return "ND"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  MeshManager& _mesh;
};

}  // namespace tabos

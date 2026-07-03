#pragma once

#include "../core/mesh_manager.h"
#include "app_base.h"

namespace tabos {

class MessagesApp : public AppBase {
 public:
  explicit MessagesApp(MeshManager& mesh);

  const char* id() const override { return "messages"; }
  const char* name() const override { return "Messages"; }
  const char* icon() const override { return "MG"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  MeshManager& _mesh;
};

}  // namespace tabos

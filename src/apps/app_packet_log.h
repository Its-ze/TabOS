#pragma once

#include "../core/mesh_manager.h"
#include "app_base.h"

namespace tabos {

class PacketLogApp : public AppBase {
 public:
  explicit PacketLogApp(MeshManager& mesh);

  const char* id() const override { return "packetlog"; }
  const char* name() const override { return "Packet Log"; }
  const char* icon() const override { return "PK"; }
  void draw() override;
  void handleInput(const InputEvent& event) override;

 private:
  MeshManager& _mesh;
};

}  // namespace tabos

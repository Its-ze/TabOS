#pragma once

#include "app_base.h"

namespace tabos {

class PlaceholderApp : public AppBase {
 public:
  PlaceholderApp(const char* id, const char* name, const char* icon,
                 const char* detail);

  const char* id() const override { return _id; }
  const char* name() const override { return _name; }
  const char* icon() const override { return _icon; }
  void draw() override;

 private:
  const char* _id;
  const char* _name;
  const char* _icon;
  const char* _detail;
};

}  // namespace tabos

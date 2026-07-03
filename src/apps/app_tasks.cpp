#include "app_tasks.h"

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

TasksApp::TasksApp(UtilityManager& utilities) : _utilities(utilities) {}

void TasksApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t x = 30;

  d.fillRect(0, top, w, d.height() - top - config::BottomNavHeight,
             theme::Background);
  d.drawText("Tasks", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  d.fillRoundRect(x, top + 82, 170, 50, 7, theme::PanelAlt);
  d.drawRoundRect(x, top + 82, 170, 50, 7, theme::Border);
  d.drawText("Add Task", x + 85, top + 107, 2, theme::Text, theme::PanelAlt,
             TextAlign::MiddleCenter);
  d.fillRoundRect(x + 190, top + 82, 170, 50, 7, theme::PanelAlt);
  d.drawRoundRect(x + 190, top + 82, 170, 50, 7, theme::Border);
  d.drawText("Clear Done", x + 275, top + 107, 2, theme::Text, theme::PanelAlt,
             TextAlign::MiddleCenter);

  int16_t y = top + 152;
  for (uint8_t i = 0; i < _utilities.taskCount(); ++i) {
    const TaskItem& task = _utilities.task(i);
    const uint32_t fill = task.done ? theme::PanelAlt : theme::Panel;
    d.fillRoundRect(x, y, w - 60, 48, 6, fill);
    d.drawText(task.done ? "[x]" : "[ ]", x + 16, y + 24, 2,
               task.done ? theme::Accent : theme::MutedText, fill,
               TextAlign::MiddleLeft);
    d.drawText(task.text, x + 70, y + 24, 2, theme::Text, fill,
               TextAlign::MiddleLeft);
    y += 56;
  }

  if (_utilities.taskCount() == 0) {
    d.fillRoundRect(x, y, w - 60, 100, 8, theme::Panel);
    d.drawText("No tasks yet", x + 22, y + 34, 2, theme::Text, theme::Panel,
               TextAlign::MiddleLeft);
    d.drawText("Tap Add Task to create a local saved task.", x + 22, y + 72, 2,
               theme::MutedText, theme::Panel, TextAlign::MiddleLeft);
  }
}

void TasksApp::handleInput(const InputEvent& event) {
  if (event.type != InputEventType::TouchTap) {
    return;
  }
  const int16_t top = config::StatusBarHeight;
  const int16_t x = 30;
  if (event.y >= top + 82 && event.y < top + 132) {
    if (event.x >= x && event.x < x + 170) {
      _utilities.addTask();
    } else if (event.x >= x + 190 && event.x < x + 360) {
      _utilities.clearDoneTasks();
    }
    return;
  }

  const int16_t listY = top + 152;
  if (event.y >= listY) {
    const uint8_t row = (event.y - listY) / 56;
    _utilities.toggleTask(row);
  }
}

}  // namespace tabos

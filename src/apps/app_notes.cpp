#include "app_notes.h"

#include <stdio.h>

#include "../config.h"
#include "../hal/hal_display.h"
#include "../ui/theme.h"

namespace tabos {

NotesApp::NotesApp(UtilityManager& utilities) : _utilities(utilities) {}

void NotesApp::draw() {
  HalDisplay& d = display();
  const int16_t w = d.width();
  const int16_t top = config::StatusBarHeight;
  const int16_t bottom = d.height() - config::BottomNavHeight;
  const int16_t x = 30;

  d.fillRect(0, top, w, bottom - top, theme::Background);
  d.drawText("Notes", 34, top + 44, 4, theme::Text, theme::Background,
             TextAlign::MiddleLeft);

  d.fillRoundRect(x, top + 82, 170, 50, 7, theme::PanelAlt);
  d.drawRoundRect(x, top + 82, 170, 50, 7, theme::Border);
  d.drawText("New Note", x + 85, top + 107, 2, theme::Text, theme::PanelAlt,
             TextAlign::MiddleCenter);
  d.fillRoundRect(x + 190, top + 82, 170, 50, 7, theme::PanelAlt);
  d.drawRoundRect(x + 190, top + 82, 170, 50, 7, theme::Border);
  d.drawText("Delete Last", x + 275, top + 107, 2, theme::Text, theme::PanelAlt,
             TextAlign::MiddleCenter);

  int16_t y = top + 152;
  const uint8_t visible = min<uint8_t>(_utilities.noteCount(), 5);
  for (uint8_t i = 0; i < visible; ++i) {
    const NoteItem& note = _utilities.note(i);
    const uint32_t fill = (i % 2 == 0) ? theme::Panel : theme::PanelAlt;
    d.fillRoundRect(x, y, w - 60, 64, 6, fill);
    d.drawText(note.title, x + 16, y + 22, 2, theme::Text, fill,
               TextAlign::MiddleLeft);
    d.drawText(note.body, x + 16, y + 48, 1, theme::MutedText, fill,
               TextAlign::MiddleLeft);
    y += 72;
  }

  if (_utilities.noteCount() == 0) {
    d.fillRoundRect(x, y, w - 60, 100, 8, theme::Panel);
    d.drawText("No notes yet", x + 22, y + 34, 2, theme::Text, theme::Panel,
               TextAlign::MiddleLeft);
    d.drawText("Tap New Note to create a local saved note.", x + 22, y + 72, 2,
               theme::MutedText, theme::Panel, TextAlign::MiddleLeft);
  }
}

void NotesApp::handleInput(const InputEvent& event) {
  if (event.type != InputEventType::TouchTap) {
    return;
  }
  const int16_t top = config::StatusBarHeight;
  const int16_t x = 30;
  const int16_t y = top + 82;
  if (event.y >= y && event.y < y + 50) {
    if (event.x >= x && event.x < x + 170) {
      _utilities.addNote();
    } else if (event.x >= x + 190 && event.x < x + 360) {
      _utilities.deleteLastNote();
    }
  }
}

}  // namespace tabos

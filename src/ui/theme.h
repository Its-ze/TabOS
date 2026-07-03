#pragma once

#include <stdint.h>

namespace tabos {
namespace theme {

constexpr uint32_t Black = 0x000000;
constexpr uint32_t Background = 0x07100D;
constexpr uint32_t Panel = 0x10231E;
constexpr uint32_t PanelAlt = 0x17332C;
constexpr uint32_t Bar = 0x050A09;
constexpr uint32_t Border = 0x2B5C50;
constexpr uint32_t Text = 0xE9FFF7;
constexpr uint32_t MutedText = 0x8DB7AB;
constexpr uint32_t Accent = 0x20D6A2;
constexpr uint32_t Accent2 = 0x5EC8FF;
constexpr uint32_t Warning = 0xFFCA5C;
constexpr uint32_t Error = 0xFF6B6B;
constexpr uint32_t Track = 0x1C2926;

const char* pageTitle(uint8_t page);

}  // namespace theme
}  // namespace tabos

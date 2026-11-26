#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk {

Night ringNight(const std::vector<Peal>& parts) {
  Night night;
  
  night.fates.reserve(parts.size());

  const Peal* last_non_empty = nullptr;

  for (const Peal& part : parts) {
    if (part.size() == 0) {
      night.fates.push_back(SheetFate::Empty);
      continue;
    }

    if(last_non_empty != nullptr && part == *last_non_empty) {
      night.fates.push_back(SheetFate::Repeat);
      continue;
    }

    last_non_empty = &part;

    Voice v = voiceOf(part);
    if (!v.accepted) {
      night.fates.push_back(SheetFate::Unfit);
    }else{
      night.played += v.peal;
      night.fates.push_back(SheetFate::Played);
    }
  }

  return night;
}
} // namespace rybinsk
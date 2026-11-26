#include "rybinsk/night.hpp"
#include "rybinsk/voice.hpp"

namespace rybinsk
{

    Night ringNight(const std::vector<Peal> &parts)
    {
        Night result;
        Peal previous;
        bool has_previous = false;

        for (const Peal &sheet : parts)
        {

            if (sheet.size() == 0)
            {
                result.fates.push_back(SheetFate::Empty);
                continue;
            }

            if (has_previous && sheet == previous)
            {
                result.fates.push_back(SheetFate::Repeat);
                continue;
            }

            previous = sheet;
            has_previous = true;

            Voice voice = voiceOf(sheet);

            if (!voice.accepted)
            {
                result.fates.push_back(SheetFate::Unfit);
                continue;
            }

            result.played += voice.peal;
            result.fates.push_back(SheetFate::Played);
        }

        return result;
    }

} // namespace rybinsk

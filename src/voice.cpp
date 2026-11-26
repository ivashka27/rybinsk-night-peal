#include "rybinsk/voice.hpp"
#include <vector>

namespace rybinsk
{

    Voice voiceOf(const Peal &sheet)
    {
        bool seen[9]{};
        std::vector<Stroke> result;

        for (int i = 0; i < sheet.size(); ++i)
        {
            const Stroke &stroke = sheet[i];

            int bell = stroke.bell;
            int beats = stroke.beats;

            if (bell < 0 || bell > 8 ||
                beats < 1 || (bell == 0 && beats != 1))
            {
                return Voice{false, Peal()};
            }

            if (bell == 0)
            {
                result.push_back(stroke);

                for (int j = 0; j < 9; ++j)
                {
                    seen[j] = false;
                }
            }
            else if (!seen[bell])
            {
                result.push_back(stroke);
                seen[bell] = true;
            }
        }

        return Voice{
            true,
            Peal(result.data(), static_cast<int>(result.size()))};
    }

} // namespace rybinsk

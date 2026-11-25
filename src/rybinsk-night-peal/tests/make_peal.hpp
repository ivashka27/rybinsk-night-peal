#pragma once

#include "harness.hpp"

#include "rybinsk/peal.hpp"

#include <initializer_list>
#include <vector>

inline rybinsk::Peal makePeal(std::initializer_list<rybinsk::Stroke> strokes) {
    const std::vector<rybinsk::Stroke> data(strokes);
    return rybinsk::Peal(data.data(), static_cast<int>(data.size()));
}

inline void expectStrokes(const rybinsk::Peal& peal, std::initializer_list<rybinsk::Stroke> strokes) {
    EXPECT_EQ(peal.size(), static_cast<int>(strokes.size()));
    int index = 0;
    for (const rybinsk::Stroke& stroke : strokes) {
        EXPECT_EQ(peal[index].bell, stroke.bell);
        EXPECT_EQ(peal[index].beats, stroke.beats);
        ++index;
    }
}

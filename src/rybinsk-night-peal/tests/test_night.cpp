#include "harness.hpp"
#include "make_peal.hpp"

#include "rybinsk/night.hpp"

#include <initializer_list>
#include <string>
#include <vector>

using rybinsk::Night;
using rybinsk::Peal;
using rybinsk::SheetFate;
using rybinsk::Stroke;
using rybinsk::ringNight;

namespace {

const char* fateName(SheetFate fate) {
    switch (fate) {
    case SheetFate::Played:
        return "played";
    case SheetFate::Empty:
        return "empty";
    case SheetFate::Repeat:
        return "repeat";
    case SheetFate::Unfit:
        return "unfit";
    }
    return "?";
}

void expectFates(const std::vector<SheetFate>& actual, std::initializer_list<SheetFate> expected) {
    EXPECT_EQ(static_cast<int>(actual.size()), static_cast<int>(expected.size()));
    std::string got;
    std::string want;
    std::size_t index = 0;
    for (SheetFate fate : expected) {
        if (!got.empty()) {
            got += " ";
            want += " ";
        }
        got += fateName(actual[index]);
        want += fateName(fate);
        ++index;
    }
    EXPECT_EQ(got, want);
}

void sameVoiceIsNotARepeatOfAnotherSheet() {
    const Peal echoed = makePeal({Stroke{1, 2}, Stroke{1, 9}});
    const Peal clean = makePeal({Stroke{1, 2}});
    const Night night = ringNight({echoed, clean});
    expectFates(night.fates, {SheetFate::Played, SheetFate::Played});
    expectStrokes(night.played, {Stroke{1, 2}});
}

void emptySheetDoesNotForgetThePreviousOne() {
    const Peal sheet = makePeal({Stroke{4, 1}});
    const Night night = ringNight({sheet, Peal(), sheet});
    expectFates(night.fates, {SheetFate::Played, SheetFate::Empty, SheetFate::Repeat});
    expectStrokes(night.played, {Stroke{4, 1}});
}

void secondCopyOfAnUnfitSheetIsARepeat() {
    const Peal bad = makePeal({Stroke{1, 1}, Stroke{9, 1}});
    const Peal again = makePeal({Stroke{1, 1}, Stroke{9, 1}});
    const Night night = ringNight({bad, Peal(), again});
    expectFates(night.fates, {SheetFate::Unfit, SheetFate::Empty, SheetFate::Repeat});
    EXPECT_EQ(night.played.size(), 0);
}

void playedSheetsJoinByPairsNotByAppending() {
    const Peal high = makePeal({Stroke{5, 1}});
    const Peal low = makePeal({Stroke{2, 4}});
    const Night night = ringNight({high, low});
    expectFates(night.fates, {SheetFate::Played, SheetFate::Played});
    expectStrokes(night.played, {Stroke{2, 4}, Stroke{5, 1}});
}

void wholeNightUsesEveryRule() {
    const Peal first = makePeal({Stroke{5, 1}, Stroke{5, 4}, Stroke{0, 1}, Stroke{5, 2}});
    const Peal same = makePeal({Stroke{5, 1}, Stroke{5, 4}, Stroke{0, 1}, Stroke{5, 2}});
    const Peal already_clean = makePeal({Stroke{5, 1}, Stroke{0, 1}, Stroke{5, 2}});
    const Peal bad = makePeal({Stroke{9, 1}});
    const Peal last = makePeal({Stroke{1, 3}, Stroke{1, 1}});
    const std::vector<Peal> parts{first, Peal(), same, already_clean, bad, bad, last};

    const Night night = ringNight(parts);
    expectFates(
        night.fates,
        {SheetFate::Played, SheetFate::Empty, SheetFate::Repeat, SheetFate::Played, SheetFate::Unfit,
         SheetFate::Repeat, SheetFate::Played}
    );
    expectStrokes(night.played, {Stroke{1, 3}, Stroke{5, 1}, Stroke{0, 1}, Stroke{5, 2}});
    EXPECT_EQ(static_cast<int>(night.played), 7);

    EXPECT(parts.size() == 7);
    expectStrokes(parts[0], {Stroke{5, 1}, Stroke{5, 4}, Stroke{0, 1}, Stroke{5, 2}});
    EXPECT_EQ(parts[1].size(), 0);
    expectStrokes(parts[3], {Stroke{5, 1}, Stroke{0, 1}, Stroke{5, 2}});
    expectStrokes(parts[6], {Stroke{1, 3}, Stroke{1, 1}});
}

void nightOfEmptiesIsEmpty() {
    const Night night = ringNight({Peal(), Peal(), Peal()});
    expectFates(night.fates, {SheetFate::Empty, SheetFate::Empty, SheetFate::Empty});
    EXPECT_EQ(night.played.size(), 0);

    const Night nothing = ringNight({});
    EXPECT(nothing.fates.empty());
    EXPECT_EQ(nothing.played.size(), 0);
}

}  // namespace

void registerNightTests(std::vector<rybinsk_test::TestCase>& tests) {
    tests.push_back({"night / тот же голос ещё не повтор листка", sameVoiceIsNotARepeatOfAnotherSheet});
    tests.push_back({"night / пустой листок не стирает предыдущий", emptySheetDoesNotForgetThePreviousOne});
    tests.push_back({"night / вторая копия негодного листка — повтор", secondCopyOfAnUnfitSheetIsARepeat});
    tests.push_back({"night / сыгранные листки складываются вперебор", playedSheetsJoinByPairsNotByAppending});
    tests.push_back({"night / вся ночь", wholeNightUsesEveryRule});
    tests.push_back({"night / стопка пустых листков пустая", nightOfEmptiesIsEmpty});
}

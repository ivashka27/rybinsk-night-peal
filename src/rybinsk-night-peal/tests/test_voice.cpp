#include "harness.hpp"
#include "make_peal.hpp"

#include "rybinsk/voice.hpp"

using rybinsk::Peal;
using rybinsk::Stroke;
using rybinsk::voiceOf;

namespace {

void badStrokeRejectsTheWholeSheet() {
    const Peal late = makePeal({Stroke{1, 2}, Stroke{9, 1}});
    const auto rejected = voiceOf(late);
    EXPECT(!rejected.accepted);
    EXPECT_EQ(rejected.peal.size(), 0);
    expectStrokes(late, {Stroke{1, 2}, Stroke{9, 1}});

    EXPECT(!voiceOf(makePeal({Stroke{0, 2}})).accepted);
    EXPECT(!voiceOf(makePeal({Stroke{1, 0}})).accepted);
    EXPECT(!voiceOf(makePeal({Stroke{-1, 3}})).accepted);
    EXPECT(!voiceOf(makePeal({Stroke{8, 1}, Stroke{1, -4}})).accepted);
    EXPECT(!voiceOf(Peal(2)).accepted);
}

void echoIsCutAndBeatsAreNotAdded() {
    const Peal sheet = makePeal({Stroke{1, 2}, Stroke{1, 9}, Stroke{3, 1}});
    const auto voice = voiceOf(sheet);
    EXPECT(voice.accepted);
    expectStrokes(voice.peal, {Stroke{1, 2}, Stroke{3, 1}});
    expectStrokes(sheet, {Stroke{1, 2}, Stroke{1, 9}, Stroke{3, 1}});
    EXPECT_EQ(static_cast<int>(voice.peal), 3);
}

void breathLetsTheSameBellSpeakAgain() {
    const Peal sheet = makePeal({Stroke{2, 1}, Stroke{0, 1}, Stroke{2, 4}, Stroke{0, 1}, Stroke{0, 1}});
    const auto voice = voiceOf(sheet);
    EXPECT(voice.accepted);
    expectStrokes(voice.peal, {Stroke{2, 1}, Stroke{0, 1}, Stroke{2, 4}, Stroke{0, 1}, Stroke{0, 1}});
}

void emptySheetIsAcceptedAndSilent() {
    const auto voice = voiceOf(Peal());
    EXPECT(voice.accepted);
    EXPECT_EQ(voice.peal.size(), 0);
}

void bellEightIsAllowedAndNineIsNot() {
    EXPECT(voiceOf(makePeal({Stroke{8, 1}})).accepted);
    EXPECT(!voiceOf(makePeal({Stroke{9, 1}})).accepted);
}

}  // namespace

void registerVoiceTests(std::vector<rybinsk_test::TestCase>& tests) {
    tests.push_back({"voice / плохой удар портит листок целиком", badStrokeRejectsTheWholeSheet});
    tests.push_back({"voice / эхо снимается, доли не складываются", echoIsCutAndBeatsAreNotAdded});
    tests.push_back({"voice / передышка снова пускает тот же колокол", breathLetsTheSameBellSpeakAgain});
    tests.push_back({"voice / пустой листок принят и молчит", emptySheetIsAcceptedAndSilent});
    tests.push_back({"voice / восьмой колокол годится, девятый нет", bellEightIsAllowedAndNineIsNot});
}

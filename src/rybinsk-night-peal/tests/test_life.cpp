#include "harness.hpp"
#include "make_peal.hpp"

#include "rybinsk/peal.hpp"

#include <utility>

using rybinsk::Peal;
using rybinsk::PealError;
using rybinsk::Stroke;

namespace {

void emptyPealTakesNothing() {
    const Peal peal;
    EXPECT_EQ(peal.size(), 0);
    EXPECT_EQ(static_cast<int>(peal), 0);
}

void countedPealIsZeroFilled() {
    Peal peal(3);
    EXPECT_EQ(peal.size(), 3);
    EXPECT_EQ(peal[0].bell, 0);
    EXPECT_EQ(peal[0].beats, 0);
    EXPECT_EQ(peal[2].bell, 0);
    EXPECT_EQ(peal[2].beats, 0);
    peal[1] = Stroke{5, 6};
    EXPECT_EQ(static_cast<int>(peal), 6);

    const Peal zero(0);
    EXPECT_EQ(zero.size(), 0);
}

void negativeCountThrowsBeforeAllocation() {
    EXPECT_THROW(Peal(-1), PealError);
    const Stroke stroke{1, 1};
    EXPECT_THROW(Peal(&stroke, -3), PealError);
}

void arrayIsCopiedAndSourceMayChange() {
    Stroke raw[] = {Stroke{1, 2}, Stroke{3, 4}};
    const Peal peal(raw, 2);
    raw[0].bell = 9;
    raw[0].beats = 9;
    EXPECT_EQ(peal.size(), 2);
    EXPECT_EQ(peal[0].bell, 1);
    EXPECT_EQ(peal[0].beats, 2);
    EXPECT_EQ(peal[1].bell, 3);
    EXPECT_EQ(static_cast<int>(peal), 6);
}

void nullPointerOnlyAllowedForEmpty() {
    const Peal empty(nullptr, 0);
    EXPECT_EQ(empty.size(), 0);
    EXPECT_THROW(Peal(nullptr, 2), PealError);
}

void copyKeepsItsOwnMemory() {
    Peal original = makePeal({Stroke{1, 2}, Stroke{3, 4}});
    {
        Peal copy = original;
        EXPECT(copy == original);
        copy[0] = Stroke{9, 9};
        EXPECT_EQ(original[0].bell, 1);
        EXPECT_EQ(original[0].beats, 2);
        EXPECT_EQ(copy[0].bell, 9);
    }
    EXPECT_EQ(original[1].bell, 3);
    EXPECT_EQ(static_cast<int>(original), 6);

    const Peal empty;
    const Peal empty_copy = empty;
    EXPECT_EQ(empty_copy.size(), 0);
    EXPECT(empty_copy == empty);
}

void moveTakesMemoryAndLeavesSourceEmpty() {
    Peal source = makePeal({Stroke{4, 7}, Stroke{2, 1}});
    Peal taken(std::move(source));
    EXPECT_EQ(source.size(), 0);
    EXPECT_EQ(static_cast<int>(source), 0);
    EXPECT_EQ(taken.size(), 2);
    EXPECT_EQ(taken[0].bell, 4);
    EXPECT_EQ(taken[0].beats, 7);
    EXPECT_EQ(taken[1].beats, 1);

    Peal from_empty(std::move(source));
    EXPECT_EQ(from_empty.size(), 0);
    EXPECT_EQ(source.size(), 0);
    EXPECT_EQ(taken[0].bell, 4);
}

}  // namespace

void registerLifeTests(std::vector<rybinsk_test::TestCase>& tests) {
    tests.push_back({"life / пустой перезвон ничего не занимает", emptyPealTakesNothing});
    tests.push_back({"life / Peal(n) заполнен нулями", countedPealIsZeroFilled});
    tests.push_back({"life / отрицательная длина — ошибка", negativeCountThrowsBeforeAllocation});
    tests.push_back({"life / массив копируется, исходник можно менять", arrayIsCopiedAndSourceMayChange});
    tests.push_back({"life / пустой указатель годится только при длине 0", nullPointerOnlyAllowedForEmpty});
    tests.push_back({"life / копия живёт своей памятью", copyKeepsItsOwnMemory});
    tests.push_back({"life / перемещение забирает память", moveTakesMemoryAndLeavesSourceEmpty});
}

#include "harness.hpp"
#include "make_peal.hpp"

#include "rybinsk/peal.hpp"

#include <utility>

using rybinsk::Peal;
using rybinsk::Stroke;

namespace {

void copyAssignDoesNotShareMemory() {
    Peal source = makePeal({Stroke{1, 2}, Stroke{3, 4}});
    Peal target = makePeal({Stroke{8, 8}});
    target = source;
    EXPECT_EQ(target.size(), 2);
    EXPECT_EQ(target[1].bell, 3);
    source[0] = Stroke{9, 9};
    EXPECT_EQ(target[0].bell, 1);
    EXPECT_EQ(target[0].beats, 2);

    target = Peal();
    EXPECT_EQ(target.size(), 0);
    EXPECT_EQ(static_cast<int>(source), 13);
}

void selfCopyAssignKeepsThePeal() {
    Peal peal = makePeal({Stroke{1, 5}, Stroke{2, 6}});
    Peal& same = peal;
    peal = peal;
    peal = same;
    EXPECT_EQ(peal.size(), 2);
    EXPECT_EQ(peal[0].bell, 1);
    EXPECT_EQ(peal[0].beats, 5);
    EXPECT_EQ(peal[1].bell, 2);
    EXPECT_EQ(static_cast<int>(peal), 11);
}

void moveAssignEmptiesSource() {
    Peal source = makePeal({Stroke{2, 3}, Stroke{4, 5}});
    Peal target = makePeal({Stroke{9, 9}});
    target = std::move(source);
    EXPECT_EQ(source.size(), 0);
    EXPECT_EQ(static_cast<int>(source), 0);
    EXPECT_EQ(target.size(), 2);
    EXPECT_EQ(target[0].bell, 2);
    EXPECT_EQ(target[1].bell, 4);
    EXPECT_EQ(static_cast<int>(target), 8);
    EXPECT_EQ(target[0].beats, 3);
}

void selfMoveKeepsThePeal() {
    Peal peal = makePeal({Stroke{1, 5}, Stroke{2, 6}});
    peal = std::move(peal);
    EXPECT_EQ(peal.size(), 2);
    EXPECT_EQ(peal[0].bell, 1);
    EXPECT_EQ(peal[1].beats, 6);
    EXPECT_EQ(static_cast<int>(peal), 11);
}

void movedFromCanBeAssignedAgain() {
    Peal source = makePeal({Stroke{4, 7}});
    Peal taken(std::move(source));
    source = makePeal({Stroke{8, 1}, Stroke{1, 1}});
    EXPECT_EQ(source.size(), 2);
    EXPECT_EQ(source[0].bell, 8);
    EXPECT_EQ(taken.size(), 1);
    EXPECT_EQ(taken[0].bell, 4);
    EXPECT_EQ(taken[0].beats, 7);

    taken = std::move(source);
    EXPECT_EQ(source.size(), 0);
    EXPECT_EQ(taken.size(), 2);
    EXPECT_EQ(taken[1].bell, 1);
}

}  // namespace

void registerAssignTests(std::vector<rybinsk_test::TestCase>& tests) {
    tests.push_back({"assign / присваивание не делит память", copyAssignDoesNotShareMemory});
    tests.push_back({"assign / присваивание самому себе", selfCopyAssignKeepsThePeal});
    tests.push_back({"assign / перемещение опустошает источник", moveAssignEmptiesSource});
    tests.push_back({"assign / перемещение в себя оставляет партию", selfMoveKeepsThePeal});
    tests.push_back({"assign / в опустошённый перезвон можно присвоить снова", movedFromCanBeAssignedAgain});
}

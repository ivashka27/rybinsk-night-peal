#include "harness.hpp"
#include "make_peal.hpp"

#include "rybinsk/peal.hpp"

using rybinsk::Peal;
using rybinsk::PealError;
using rybinsk::Stroke;

namespace {

const Stroke& readAt(const Peal& peal, int index) {
    return peal[index];
}

void indexReadsAndWrites() {
    Peal peal(2);
    peal[0] = Stroke{1, 2};
    peal[1] = Stroke{3, 4};
    EXPECT_EQ(peal[0].bell, 1);
    EXPECT_EQ(peal[1].beats, 4);
    EXPECT_EQ(static_cast<int>(peal), 6);
}

void badIndexDoesNotChangePeal() {
    Peal peal = makePeal({Stroke{1, 2}, Stroke{3, 4}});
    EXPECT_THROW(peal[2], PealError);
    EXPECT_THROW(peal[-1], PealError);
    EXPECT_THROW((peal[5] = Stroke{9, 9}), PealError);
    EXPECT_EQ(peal.size(), 2);
    EXPECT_EQ(peal[0].bell, 1);
    EXPECT_EQ(peal[0].beats, 2);
    EXPECT_EQ(peal[1].bell, 3);
    EXPECT_EQ(static_cast<int>(peal), 6);

    const Peal empty;
    EXPECT_THROW(empty[0], PealError);
}

void constPealIsReadable() {
    const Peal peal = makePeal({Stroke{6, 1}, Stroke{7, 2}});
    EXPECT_EQ(readAt(peal, 1).bell, 7);
    EXPECT_EQ(readAt(peal, 1).beats, 2);
    EXPECT_THROW(readAt(peal, -1), PealError);
    EXPECT_EQ(peal.size(), 2);
}

void equalityComparesStrokesNotAddresses() {
    const Peal left = makePeal({Stroke{1, 2}, Stroke{3, 4}});
    const Peal right = makePeal({Stroke{1, 2}, Stroke{3, 4}});
    const Peal different_beat = makePeal({Stroke{1, 2}, Stroke{3, 5}});
    const Peal shorter = makePeal({Stroke{1, 2}});
    const Peal empty_left;
    const Peal empty_right;
    EXPECT(left == right);
    EXPECT(!(left != right));
    EXPECT(left != different_beat);
    EXPECT(left != shorter);
    EXPECT(empty_left == empty_right);
    EXPECT(left != empty_left);
    EXPECT(!(shorter == left));
}

void plusPlaysTheSmallerBellFirst() {
    const Peal left = makePeal({Stroke{5, 1}, Stroke{1, 4}});
    const Peal right = makePeal({Stroke{3, 2}});
    const Peal sum = left + right;
    expectStrokes(left, {Stroke{5, 1}, Stroke{1, 4}});
    expectStrokes(right, {Stroke{3, 2}});
    // Пара решается на своём месте. Хвост (1, 4) не уезжает в начало.
    expectStrokes(sum, {Stroke{3, 2}, Stroke{5, 1}, Stroke{1, 4}});
    EXPECT_EQ(static_cast<int>(sum), 7);
}

void plusKeepsTheLongerStrokeOfTheSameBell() {
    const Peal left = makePeal({Stroke{4, 2}, Stroke{6, 8}});
    const Peal right = makePeal({Stroke{4, 7}, Stroke{6, 8}});
    const Peal sum = left + right;
    expectStrokes(sum, {Stroke{4, 7}, Stroke{6, 8}});
    expectStrokes(left, {Stroke{4, 2}, Stroke{6, 8}});
}

void plusKeepsTheTailAndDoesNotJudgeBells() {
    const Peal left = makePeal({Stroke{9, 1}});
    const Peal right = makePeal({Stroke{1, 2}, Stroke{3, 4}});
    const Peal sum = left + right;
    expectStrokes(sum, {Stroke{1, 2}, Stroke{9, 1}, Stroke{3, 4}});

    const Peal empty;
    EXPECT(left + empty == left);
    EXPECT(empty + right == right);
    EXPECT(empty + empty == empty);
}

void joiningAPealWithItselfDoesNotGrow() {
    Peal peal = makePeal({Stroke{2, 5}, Stroke{1, 3}, Stroke{2, 5}});
    const Peal joined = peal + peal;
    EXPECT(joined == peal);
    peal += peal;
    expectStrokes(peal, {Stroke{2, 5}, Stroke{1, 3}, Stroke{2, 5}});
    Peal& same = peal;
    peal += same;
    EXPECT_EQ(peal.size(), 3);
    EXPECT_EQ(static_cast<int>(peal), 13);
}

void compoundAssignChains() {
    Peal first = makePeal({Stroke{5, 1}});
    Peal second = makePeal({Stroke{4, 2}});
    Peal third = makePeal({Stroke{4, 9}});
    Peal& returned = (first += second += third);
    EXPECT(&returned == &first);
    expectStrokes(third, {Stroke{4, 9}});
    expectStrokes(second, {Stroke{4, 9}});
    expectStrokes(first, {Stroke{4, 9}, Stroke{5, 1}});

    const int before = static_cast<int>(first);
    first += Peal();
    EXPECT_EQ(static_cast<int>(first), before);
    EXPECT_EQ(first.size(), 2);
}

}  // namespace

void registerOpsTests(std::vector<rybinsk_test::TestCase>& tests) {
    tests.push_back({"ops / индекс читает и пишет удар", indexReadsAndWrites});
    tests.push_back({"ops / плохой индекс не меняет партию", badIndexDoesNotChangePeal});
    tests.push_back({"ops / константный перезвон читается", constPealIsReadable});
    tests.push_back({"ops / равны удары, а не адреса", equalityComparesStrokesNotAddresses});
    tests.push_back({"ops / в паре сначала меньший колокол, хвост не сортируется", plusPlaysTheSmallerBellFirst});
    tests.push_back({"ops / один колокол оставляет длинный удар", plusKeepsTheLongerStrokeOfTheSameBell});
    tests.push_back({"ops / хвост дописывается, чужой номер не бракуется", plusKeepsTheTailAndDoesNotJudgeBells});
    tests.push_back({"ops / листок, сложенный сам с собой, не растёт", joiningAPealWithItselfDoesNotGrow});
    tests.push_back({"ops / цепочка += собирается справа и возвращает левый листок", compoundAssignChains});
}

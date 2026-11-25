#include "harness.hpp"

#include <vector>

void registerLifeTests(std::vector<rybinsk_test::TestCase>& tests);
void registerAssignTests(std::vector<rybinsk_test::TestCase>& tests);
void registerOpsTests(std::vector<rybinsk_test::TestCase>& tests);
void registerVoiceTests(std::vector<rybinsk_test::TestCase>& tests);
void registerNightTests(std::vector<rybinsk_test::TestCase>& tests);

int main() {
    std::vector<rybinsk_test::TestCase> tests;
    registerLifeTests(tests);
    registerAssignTests(tests);
    registerOpsTests(tests);
    registerVoiceTests(tests);
    registerNightTests(tests);
    return rybinsk_test::runAll(tests);
}

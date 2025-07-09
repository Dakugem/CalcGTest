#include "SimpleCalculator.h"
#include "InMemoryHistory.h"

#include <gtest/gtest.h>
#include <iostream>

using namespace calc;

class CalculatorTest : public ::testing::Test
{
public:
    static void SetUpTestSuite() {
        std::cerr << "Prepairing test environment..." << std::endl;
    }

    void SetUp() override{
        auto hist = std::make_shared<InMemoryHistory>();
        calc = std::make_unique<SimpleCalculator>(hist);
    }

    void TearDown() override {
        calc.reset();
    }

    static void TearDownTestSuite() {
        std::cout << "Cleaning up..." << std::endl;
    }

    std::unique_ptr<SimpleCalculator> calc;
};

GTEST_TEST_F(CalculatorTest, Case1) {
    ASSERT_EQ(calc->Add(2, 3), 5);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
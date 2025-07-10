#include "SimpleCalculator.h"
#include "InMemoryHistory.h"

#include <gtest/gtest.h>
//#include <gmock/gmock.h>
#include <iostream>
#include <limits>

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

struct TestParams{
    int in_a;
    int in_b;
    int out;
    std::string message = "";
};

class ParametrizedAddOverflowTest : public CalculatorTest
    , public ::testing::WithParamInterface<TestParams>{};
class ParametrizedSubOverflowTest : public CalculatorTest
    , public ::testing::WithParamInterface<TestParams>{};
class ParametrizedMulOverflowTest : public CalculatorTest
    , public ::testing::WithParamInterface<TestParams>{};


INSTANTIATE_TEST_SUITE_P(
    Default,
    ParametrizedAddOverflowTest,
    ::testing::Values(
        TestParams{INT_MAX, 1, INT_MIN, "Лучше выкидывать исключение"},
        TestParams{INT_MIN,-1, INT_MAX, "Лучше выкидывать исключение"},
        TestParams{INT_MAX, INT_MAX,-2, "Лучше выкидывать исключение"},
        TestParams{INT_MIN, INT_MIN, 0, "Лучше выкидывать исключение"}
    )
);

INSTANTIATE_TEST_SUITE_P(
    Default,
    ParametrizedSubOverflowTest,
    ::testing::Values(
        TestParams{INT_MAX,-1, INT_MIN, "Лучше выкидывать исключение"},
        TestParams{INT_MIN, 1, INT_MAX, "Лучше выкидывать исключение"},
        TestParams{INT_MAX, INT_MAX, 0, "Лучше выкидывать исключение"},
        TestParams{INT_MIN, INT_MIN, 0, "Лучше выкидывать исключение"}
    )
);

INSTANTIATE_TEST_SUITE_P(
    Default,
    ParametrizedMulOverflowTest,
    ::testing::Values(
        TestParams{INT_MAX, 2, -2, "Лучше выкидывать исключение"},
        TestParams{INT_MIN, 2,  0, "Лучше выкидывать исключение"},
        TestParams{INT_MIN, -1, INT_MIN, "Лучше выкидывать исключение"}
    )
);

TEST_F(CalculatorTest, Add) {
    ASSERT_EQ(calc->Add(0, 0), 0);

    EXPECT_EQ(calc->Add(21, 5), 26);
    EXPECT_EQ(calc->Add(2, -5), -3);

    EXPECT_EQ(calc->Add(INT_MIN, 0),INT_MIN);
    EXPECT_EQ(calc->Add(INT_MAX, 0),INT_MAX);

    EXPECT_EQ(calc->Add(INT_MAX, INT_MIN), -1);
}

TEST_F(CalculatorTest, Sub) {
    ASSERT_EQ(calc->Subtract(0, 0), 0);

    EXPECT_EQ(calc->Subtract(15, 3), 12);
    EXPECT_EQ(calc->Subtract(2, -5), 7);

    EXPECT_EQ(calc->Subtract(INT_MIN, 0),INT_MIN);
    EXPECT_EQ(calc->Subtract(INT_MAX, 0),INT_MAX);

    EXPECT_EQ(calc->Subtract(INT_MAX, INT_MAX), 0);
}

TEST_F(CalculatorTest, Mul) {
    ASSERT_EQ(calc->Multiply(0, 0), 0);
    ASSERT_EQ(calc->Multiply(5, 1), 5);

    EXPECT_EQ(calc->Multiply(2, 5), 10);
    EXPECT_EQ(calc->Multiply(2, -5), -10);

    EXPECT_EQ(calc->Multiply(INT_MIN, 0), 0);
    EXPECT_EQ(calc->Multiply(INT_MAX, 0), 0);
}

TEST_F(CalculatorTest, Div) {
    ASSERT_EQ(calc->Divide(5, 1), 5);
    ASSERT_EQ(calc->Divide(1, 2), 0);

    EXPECT_EQ(calc->Divide(2, 5), 0);
    EXPECT_EQ(calc->Divide(5, 2), 2);
}

TEST_F(CalculatorTest, Div_By_Zero){
    ASSERT_THROW(calc->Divide(7, 0), std::invalid_argument) << "Деление на ноль должно выкидывать ошибку";
}

TEST_F(CalculatorTest, HistoryLogs){
    std::shared_ptr<InMemoryHistory> hist = std::make_shared<InMemoryHistory>();
    calc->SetHistory(hist);

    calc->Add(2, 5);
    calc->Subtract(2, 5);
    calc->Multiply(2, 5);
    calc->Divide(2, 5);

    std::vector<std::string> log = hist->GetLastOperations(4);
    ASSERT_EQ(log[0], "2 + 5 = 7");
    EXPECT_EQ(log[1], "2 - 5 = -3");
    EXPECT_EQ(log[2], "2 * 5 = 10");
    EXPECT_EQ(log[3], "2 / 5 = 0");
}

TEST_F(CalculatorTest, DISABLED_HistoryLog_ReadNotExistedLogs){
    std::shared_ptr<InMemoryHistory> hist = std::make_shared<InMemoryHistory>();
    calc->SetHistory(hist);

    calc->Add(2, 5);
    calc->Subtract(2, 5);
    calc->Multiply(2, 5);
    calc->Divide(2, 5);

    EXPECT_ANY_THROW(hist->GetLastOperations(5)) << "Попытка прочитать больше логов, чем есть, должна сообщать о себе, а не молча обрабатываться, т.к. заранее точно не известно, какой длины история и возможна попытка прочитать то, чего нет";
    EXPECT_ANY_THROW(hist->GetLastOperations(5)[4]);
}

TEST_P(ParametrizedAddOverflowTest, LimitsAddWithOverflow){
    auto params = GetParam();
    EXPECT_THROW(calc->Add(params.in_a, params.in_b), std::overflow_error) << params.message;
}

TEST_P(ParametrizedSubOverflowTest, LimitsSubWithOverflow){
    auto params = GetParam();
    EXPECT_THROW(calc->Subtract(params.in_a, params.in_b), std::overflow_error) << params.message;
}

TEST_P(ParametrizedMulOverflowTest, LimitsMulWithOverflow){
    auto params = GetParam();
    EXPECT_THROW(calc->Multiply(params.in_a, params.in_b), std::overflow_error) << params.message;
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
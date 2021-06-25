#include "../Client.h"

#include <gtest/gtest.h>

typedef std::vector<std::string> StrVec;

TEST(ClientTest, TestParse) {
    Client client;
    StrVec strs;

    auto args = client.Parse("signup aaron pass");
    ASSERT_EQ(args.size(), 3);
    strs = {"signup", "aaron", "pass"};
    for (int i = 0; i < 3; ++i) {
        EXPECT_EQ(args[i], strs[i]);
    }

    args = client.Parse(" signup    aaron pass   ");
    ASSERT_EQ(args.size(), 3);
    for (int i = 0; i < 3; ++i) {
        EXPECT_EQ(args[i], strs[i]);
    }

    args = client.Parse("     ");
    ASSERT_EQ(args.size(), 0);
}

int main() {
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
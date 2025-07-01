#include <gtest/gtest.h>
#include "../include/stock.h"

TEST(stockTest, testCreatingStock)
{
    stock stk('A', "CNY", 1000, 25);
    ASSERT_EQ(stk.getCategory(), 'A');
    ASSERT_STREQ((stk.getCurrency()).c_str(), "CNY");
    ASSERT_EQ(stk.getAmount(), 1000);
    ASSERT_EQ(stk.getPrice(), 25);
}
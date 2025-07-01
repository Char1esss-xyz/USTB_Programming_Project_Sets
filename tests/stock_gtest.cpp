#include <gtest/gtest.h>
#include <string>
#include "../include/stock.h"
#include "../include/cer.h"


TEST(stockTest, testCreatingStock)
{
    stock stk('A', "CNY", 1000, 25);
    ASSERT_EQ(stk.getCategory(), 'A');
    ASSERT_STREQ((stk.getCurrency()).c_str(), "CNY");
    ASSERT_EQ(stk.getAmount(), 1000);
    ASSERT_EQ(stk.getPrice(), 25);
}

TEST(cerTest, testCreatingCer)
{
    std::string cny = "CNY";
    std::string jpy = "JPY";
    cer CTJ(cny, jpy, 22);

    ASSERT_STREQ((CTJ.getSrc()).c_str(), "CNY");  
    ASSERT_STREQ((CTJ.getDes()).c_str(), "JPY");
    ASSERT_EQ(CTJ.getRate(), 22);
}

TEST(cerTest, testExchange)
{
    std::string cny = "CNY";
    std::string jpy = "JPY";
    cer CTJ(cny, jpy, 22);

    ASSERT_EQ(CTJ.exchange(jpy, cny, 220), 10);
}
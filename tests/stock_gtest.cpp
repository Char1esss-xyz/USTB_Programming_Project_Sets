#include <gtest/gtest.h>
#include <string>
#include "../include/stock.h"
#include "../include/cer.h"
#include "../include/person.h"


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
    cer CTJ("CNY", "JPY", 22);

    ASSERT_STREQ((CTJ.getSrc()).c_str(), "CNY");  
    ASSERT_STREQ((CTJ.getDes()).c_str(), "JPY");
    ASSERT_EQ(CTJ.getRate(), 22);
}

TEST(cerTest, testExchange)
{
    cer CTJ("CNY", "JPY", 22);
    ASSERT_EQ(CTJ.exchange("JPY", "CNY", 220), 10);
}

TEST(personTest, testQueryPerson)
{
    std::string name = "ZhangSan";
    person zs(name);
    zs.createStock('A' , "CNY", 100.0, 10.0);
    zs.createStock('B', "JPY", 220.0, 10.0);
    zs.pushCer("CNY", "JPY", 22);
    ASSERT_EQ(zs.queryPerson("CNY"), 1100.0);
}
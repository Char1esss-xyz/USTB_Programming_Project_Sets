#include <gtest/gtest.h>
#include <string>
#include "../include/stock.h"
#include "../include/cer.h"
#include "../include/person.h"

TEST(stockTest, testCreateStock)
{
    stock stk('A', "CNY", 1000, 25);
    ASSERT_EQ(stk.getCategory(), 'A');
    ASSERT_STREQ((stk.getCurrency()).c_str(), "CNY");
    ASSERT_EQ(stk.getAmount(), 1000);
    ASSERT_EQ(stk.getPrice(), 25);
}

TEST(stockTest, testAddAmount)
{
    person zs("ZhangSan");
    zs.createStock('A', "CNY", 100.0, 10.0);
    zs.createStock('A', "CNY", 100.0, 10.0);
    ASSERT_EQ(zs.queryPerson("CNY"), 2000.0);
}

TEST(cerTest, testExchange)
{
    cer CTJ;
    CTJ.add_rate("CNY", "JPY", 22);
    ASSERT_EQ(CTJ.convert("JPY", "CNY", 220), 10);
}

TEST(personTest, testQueryPerson)
{
    person zs("ZhangSan");
    zs.createStock('A', "CNY", 100.0, 10.0);
    zs.createStock('B', "JPY", 220.0, 10.0);
    zs.pushCer("CNY", "JPY", 22.0);
    ASSERT_EQ(zs.queryPerson("CNY"), 1100.0);
}

TEST(personTest, testAddPersonStock)
{
    person zs("ZhangSan");
    zs.createStock('A', "CNY", 100.0, 10.0);
    zs.createStock('A', "CNY", 100.0, 10.0);
    ASSERT_EQ(zs.queryPerson("CNY"), 2000.0);
}

TEST(personTest, testQueryStockWithDifferentCurrency)
{
    person zs("ZhangSan");
    zs.createStock('A', "CNY", 100.0, 10.0);
    zs.createStock('A', "JPY", 220.0, 10.0);
    zs.pushCer("CNY", "JPY", 22.0);
    ASSERT_EQ(zs.queryStock('A', "CNY"), 1100.0);
}

TEST(personTest, testQueryMutipleStocks)
{
    person zs("ZhangSan");
    zs.createStock('A', "CNY", 100.0, 10.0);
    zs.createStock('A', "JPY", 220.0, 10.0);
    zs.createStock('B', "JPY", 220.0, 200.0);
    zs.pushCer("CNY", "JPY", 22.0);
    ASSERT_EQ(zs.queryStock('A', "CNY"), 1100.0);
    ASSERT_EQ(zs.queryStock('B', "CNY"), 2000.0);
}
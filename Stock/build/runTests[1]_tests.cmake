add_test([=[stockTest.testCreateStock]=]  D:/Files/Code/Cpp/25_Summer/stock/build/runTests.exe [==[--gtest_filter=stockTest.testCreateStock]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[stockTest.testCreateStock]=]  PROPERTIES WORKING_DIRECTORY D:/Files/Code/Cpp/25_Summer/stock/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test([=[stockTest.testAddAmount]=]  D:/Files/Code/Cpp/25_Summer/stock/build/runTests.exe [==[--gtest_filter=stockTest.testAddAmount]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[stockTest.testAddAmount]=]  PROPERTIES WORKING_DIRECTORY D:/Files/Code/Cpp/25_Summer/stock/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test([=[cerTest.testExchange]=]  D:/Files/Code/Cpp/25_Summer/stock/build/runTests.exe [==[--gtest_filter=cerTest.testExchange]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[cerTest.testExchange]=]  PROPERTIES WORKING_DIRECTORY D:/Files/Code/Cpp/25_Summer/stock/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test([=[personTest.testQueryPerson]=]  D:/Files/Code/Cpp/25_Summer/stock/build/runTests.exe [==[--gtest_filter=personTest.testQueryPerson]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[personTest.testQueryPerson]=]  PROPERTIES WORKING_DIRECTORY D:/Files/Code/Cpp/25_Summer/stock/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test([=[personTest.testAddPersonStock]=]  D:/Files/Code/Cpp/25_Summer/stock/build/runTests.exe [==[--gtest_filter=personTest.testAddPersonStock]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[personTest.testAddPersonStock]=]  PROPERTIES WORKING_DIRECTORY D:/Files/Code/Cpp/25_Summer/stock/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test([=[personTest.testQueryStockWithDifferentCurrency]=]  D:/Files/Code/Cpp/25_Summer/stock/build/runTests.exe [==[--gtest_filter=personTest.testQueryStockWithDifferentCurrency]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[personTest.testQueryStockWithDifferentCurrency]=]  PROPERTIES WORKING_DIRECTORY D:/Files/Code/Cpp/25_Summer/stock/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
add_test([=[personTest.testQueryMutipleStocks]=]  D:/Files/Code/Cpp/25_Summer/stock/build/runTests.exe [==[--gtest_filter=personTest.testQueryMutipleStocks]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[personTest.testQueryMutipleStocks]=]  PROPERTIES WORKING_DIRECTORY D:/Files/Code/Cpp/25_Summer/stock/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  runTests_TESTS stockTest.testCreateStock stockTest.testAddAmount cerTest.testExchange personTest.testQueryPerson personTest.testAddPersonStock personTest.testQueryStockWithDifferentCurrency personTest.testQueryMutipleStocks)

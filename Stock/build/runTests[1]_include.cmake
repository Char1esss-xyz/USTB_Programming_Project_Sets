if(EXISTS "D:/Files/Code/Cpp/25_Summer/stock/build/runTests[1]_tests.cmake")
  include("D:/Files/Code/Cpp/25_Summer/stock/build/runTests[1]_tests.cmake")
else()
  add_test(runTests_NOT_BUILT runTests_NOT_BUILT)
endif()

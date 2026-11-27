include(GoogleTest)

# Tests unitarios: verifican que cada algoritmo produce resultados correctos
add_executable(unit_tests test_mergesort.cpp)
target_link_libraries(unit_tests PRIVATE algorithms GTest::gtest_main)
gtest_discover_tests(unit_tests PROPERTIES LABELS unit)

# Tests de integración: levantan el servidor HTTP real y llaman a sus endpoints
add_executable(integration_tests test_api.cpp)
target_link_libraries(integration_tests PRIVATE api GTest::gtest_main)
gtest_discover_tests(integration_tests PROPERTIES LABELS integration)

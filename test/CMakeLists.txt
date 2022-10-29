add_executable(test-is_valid_number units/is_valid_number.cpp ../compiler/util.cpp)

add_test(NAME test-is_valid_number COMMAND test-is_valid_number)

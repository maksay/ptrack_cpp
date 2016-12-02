/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_TEST_PARAMETERSTEST_H_
#define CODE_INCLUDE_TEST_PARAMETERSTEST_H_
#include <cassert>
#include <cmath>
#include <cstdio>
#include <iostream>
#include "Parameters.h"
class ParametersTest {
 public:
  void testAll() {
    std::cerr << "Testing Parameters...";
    testReadConfigAndChangeParams();
    std::cerr << "OK\n";
  }

  void testReadConfigAndChangeParams() {
    FILE *fp = fopen("input.txt", "w");
    fprintf(fp, "     \n");
    fprintf(fp, "# Line with a comment\n");
    fprintf(fp, "int intParam 3\n");
    fprintf(fp, "float floatParam 2.9\n");
    fprintf(fp, "string stringParam somestring\n");
    fprintf(fp, "int intParamList 3 4 5\n");
    fprintf(fp, "float floatParamList 2.9 3.0 3.1\n");
    fprintf(fp, "string stringParamList somestring otherstring laststring\n");
    fprintf(fp, "string stringParamEmptyList \n");
    fclose(fp);

    Parameters::read_config("input.txt");
    assert(Parameters::get_int("intParam") == 3);
    assert(fabs(Parameters::get_float("floatParam") - 2.9) < 1e-6);
    assert(Parameters::get_string("stringParam") == "somestring");

    std::vector<int> expected_int_list(0);
    expected_int_list.push_back(3);
    expected_int_list.push_back(4);
    expected_int_list.push_back(5);
    assert(Parameters::get_int_list("intParamList") == expected_int_list);

    std::vector<float> expected_float_list(0);
    expected_float_list.push_back(2.9);
    expected_float_list.push_back(3.0);
    expected_float_list.push_back(3.1);
    assert(Parameters::get_float_list("floatParamList") == expected_float_list);

    std::vector<std::string> expected_string_list(0);
    expected_string_list.push_back("somestring");
    expected_string_list.push_back("otherstring");
    expected_string_list.push_back("laststring");
    assert(Parameters::get_string_list("stringParamList") ==
           expected_string_list);

    expected_string_list.clear();
    assert(Parameters::get_string_list("stringParamEmptyList") ==
           expected_string_list);

    /*
    Parameters::get_int("intParam") = 4;
    assert(Parameters::get_int("intParam") == 4);
    Parameters::get_float("floatParam") = 2.2;
    assert(fabs(Parameters::get_float("floatParam") - 2.2) < 1e-6);
    Parameters::get_string("stringParam") = "blah";
    assert(Parameters::get_string("stringParam") == "blah");

    Parameters::get_int_list("intParamList").pop_back();
    expected_int_list.pop_back();
    assert(Parameters::get_int_list("intParamList") == expected_int_list);

    Parameters::get_float_list("floatParamList").push_back(5.5);
    expected_float_list.push_back(5.5);
    assert(Parameters::get_float_list("floatParamList") == expected_float_list);

    expected_string_list.clear();
    expected_string_list.push_back("one");
    expected_string_list.push_back("two");
    Parameters::get_string_list("stringParamList") = expected_string_list;
    assert(Parameters::get_string_list("stringParamList") ==
           expected_string_list);
           */
    system("rm input.txt");
  }
};
#endif  // CODE_INCLUDE_TEST_PARAMETERSTEST_H_

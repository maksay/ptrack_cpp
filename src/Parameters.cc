/*
 * Copyright 2016 Andrii Maksai
 */
#include "Parameters.h"
#include <cassert>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <sstream>
std::map<std::string, std::vector<int> > Parameters::intParams =
    std::map<std::string, std::vector<int> >();
std::map<std::string, std::vector<std::string> > Parameters::stringParams =
    std::map<std::string, std::vector<std::string> >();
std::map<std::string, std::vector<float> > Parameters::floatParams =
    std::map<std::string, std::vector<float> >();

int Parameters::get_int(std::string name) {
  assert(intParams.count(name) && intParams[name].size() == 1);
  return intParams[name][0];
}
std::string Parameters::get_string(std::string name) {
  assert(stringParams.count(name) && stringParams[name].size() == 1);
  return stringParams[name][0];
}
float Parameters::get_float(std::string name) {
  assert(floatParams.count(name) && floatParams[name].size() == 1);
  return floatParams[name][0];
}
std::vector<int> Parameters::get_int_list(std::string name) {
  assert(intParams.count(name));
  return intParams[name];
}
std::vector<std::string> Parameters::get_string_list(std::string name) {
  assert(stringParams.count(name));
  return stringParams[name];
}
std::vector<float> Parameters::get_float_list(std::string name) {
  assert(floatParams.count(name));
  return floatParams[name];
}

void Parameters::read_config(std::string fpath) {
  stringParams.clear();
  intParams.clear();
  floatParams.clear();
  char buff[1111];
  FILE* fp = fopen(fpath.c_str(), "r");
  std::string type_str, name_str, val_str;
  while (fgets(buff, 1111, fp)) {
    std::istringstream iss(buff);
    if (strlen(buff) < 1 || isspace(buff[0]) || buff[0] == '#') continue;
    iss >> type_str >> name_str;
    if (type_str.size() < 1) continue;
    if (type_str[0] == '#') continue;
    assert(type_str == "int" || type_str == "float" || type_str == "string");
    if (type_str == "int") {
      intParams[name_str] = std::vector<int>(0);
      while (iss >> val_str) {
        intParams[name_str].push_back(atoi(val_str.c_str()));
      }
    }
    if (type_str == "float") {
      floatParams[name_str] = std::vector<float>(0);
      while (iss >> val_str) {
        floatParams[name_str].push_back(atof(val_str.c_str()));
      }
    }
    if (type_str == "string") {
      stringParams[name_str] = std::vector<std::string>(0);
      while (iss >> val_str) {
        stringParams[name_str].push_back(val_str);
      }
    }
  }
  fclose(fp);
}

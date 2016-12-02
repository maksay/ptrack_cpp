/*
 * Copyright 2016 Andrii Maksai
 */
#ifndef CODE_INCLUDE_PARAMETERS_H_
#define CODE_INCLUDE_PARAMETERS_H_
#include <map>
#include <string>
#include <vector>
class Parameters {
  static std::map<std::string, std::vector<int> > intParams;
  static std::map<std::string, std::vector<std::string> > stringParams;
  static std::map<std::string, std::vector<float> > floatParams;
  Parameters() {}

 public:
  static int get_int(std::string name);
  static std::string get_string(std::string name);
  static float get_float(std::string name);
  static std::vector<int> get_int_list(std::string name);
  static std::vector<std::string> get_string_list(std::string name);
  static std::vector<float> get_float_list(std::string name);
  static void read_config(std::string fpath);
};
#endif  // CODE_INCLUDE_PARAMETERS_H_

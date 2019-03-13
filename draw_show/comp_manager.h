// Copyright 2018 Shanghai Fangling Software Co., Ltd. All Rights Reserved.
// Author: Yalong Ye

#include "component.h"
#include "index.h"

#include <iostream>

class CompManager
{
 public:
  CompManager();
  ~CompManager();
  bool Add(std::string path);
  bool Delete(std::string path, int id);
  bool Rotate(std::string path, int id, double R);

  bool SetSelect(std::string path, int id);
  Index GetSelect(std::string path, int id);

 private:
  map<string, vector<Index> > IndexS;
  //std::string select_path_;
  //int select_id_;
  Index comp_cur_index;
  Component component;
  /*bool is_first_add_one_type;*/
};
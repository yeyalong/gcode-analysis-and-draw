// Copyright 2018 Shanghai Fangling Software Co., Ltd. All Rights Reserved.
// Author: Yalong Ye

#include <iostream>

class Index
{
 public:
  Index();
  Index(std::string path, int id);
  ~Index();

  std::string select_path_;
  int select_id_;
 private:

};
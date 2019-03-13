// Copyright 2018 Shanghai Fangling Software Co., Ltd. All Rights Reserved.
// Author: Yalong Ye

#include "component.h"
#include "comp_manager.h"

#include <iostream>

using namespace std;

//Give parameter and task.
class Task{
 public:
  void Run();

 private:
  std::vector<Component *> component_vec_;
};

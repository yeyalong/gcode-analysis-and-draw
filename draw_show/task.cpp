// Copyright 2018 Shanghai Fangling Software Co., Ltd. All Rights Reserved.
// Author: Yalong Ye

#include "task.h"

#include <iostream>

using namespace std;

void Task::Run() {
  Component component;
  CompManager comp_manager;
  comp_manager.Add("71.TXT");
  //std::cout << "path:  " << p_cur_component->type_name_ << "id:  " << p_cur_component->id_ << std::endl;
  comp_manager.Add("71.TXT");
  comp_manager.Delete("71.TXT",1);
  //comp_manager.Delete("71.TXT",2);
  comp_manager.Add("71.TXT");
  comp_manager.Add("1.TXT");
  comp_manager.Add("1.TXT");
  comp_manager.Delete("1.TXT",1);

  comp_manager.Rotate("71.TXT",2,30);
  //component.SetUnit(1.0);
  //component.LoadComponent("../²âÊÔG´úÂë/N154·½ÅÌ.TXT");
  //component.SetR(30.0);
  //component.SetOffset(0.0, 0.0);
  //component.GetMatOriginal();
  //component.GetMatRotate();
  //component_vec_.push_back(&component);
}

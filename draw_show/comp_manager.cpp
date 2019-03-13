// Copyright 2018 Shanghai Fangling Software Co., Ltd. All Rights Reserved.
// Author: Yalong Ye

#include "comp_manager.h"

#include <iostream>

using namespace std;

CompManager::CompManager(){}

CompManager::~CompManager(){}

bool CompManager::Add(std::string path)
{
  vector<Index>& indexs_one_type = IndexS[path];
  //if (piece_one_type.size() == 0)
  //  is_first_add_one_type = true;
  //else
  //  is_first_add_one_type = false;
  int max_id_oneType(0);
  for (auto iter = indexs_one_type.begin(); iter != indexs_one_type.end(); iter++) {
    max_id_oneType = max_id_oneType < iter->select_id_ ? iter->select_id_ : max_id_oneType;
  }
  indexs_one_type.push_back(Index(path, ++max_id_oneType));
  comp_cur_index = *indexs_one_type.rbegin();
  std::cout << "path:  " << comp_cur_index.select_path_ << "    id:  " << comp_cur_index.select_id_ << std::endl;
  //Component *component = new Component();
  component.LoadComponent(path);
  //delete component;
  return true;
}

bool CompManager::Delete(std::string path, int id)
{
  vector<Index>& indexs_one_type = IndexS[path];
  if (indexs_one_type.size() == 0) {
    // 删除类型没有具体零件
    return false;
  }
  for (auto iter = indexs_one_type.begin(); iter != indexs_one_type.end(); iter++) {
    if (id == iter->select_id_) {
      indexs_one_type.erase(iter);  // 注意删除导致指针位置变化，具体测试而定
      return true;
    }
  }
  return false;
}

bool CompManager::Rotate(std::string path, int id, double R)
{
  vector<Index>& indexs_one_type = IndexS[path];
  if (indexs_one_type.size() == 0) {
    // 操作类型没有具体零件
    return false;
  }
  for (auto iter = indexs_one_type.begin(); iter != indexs_one_type.end(); iter++) {
    if (id == iter->select_id_) {
      //Component *component = new Component();
      component.LoadComponent(path);
      component.SetR(R);
      //delete component;
      return true;
    }
  }
  return false;
}

bool CompManager::SetSelect(std::string path, int id)
{
  comp_cur_index.select_path_ = path;
  comp_cur_index.select_id_ = id;
  return true;
}

Index CompManager::GetSelect(std::string path, int id)
{
  return comp_cur_index;
}

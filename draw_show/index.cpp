// Copyright 2018 Shanghai Fangling Software Co., Ltd. All Rights Reserved.
// Author: Yalong Ye

#include "index.h"

#include <iostream>

Index::Index(){}

Index::Index(std::string path, int id)
{
  select_path_ = path;
  select_id_ = id;
}

Index::~Index(){}
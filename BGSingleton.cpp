#include "stdafx.h"
#include "BGSingleton.h"

std::once_flag BGSingleton::onceflag_;
std::unique_ptr<BGSingleton> BGSingleton::instance_{ nullptr };
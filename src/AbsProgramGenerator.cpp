// -*- mode: C++ -*-
//
// Copyright (c) 2007, 2008, 2009, 2010, 2011, 2017 The University of Utah
// All rights reserved.
//
// 本文件实现了抽象程序生成器 AbsProgramGenerator 的相关方法，
// 负责生成器的实例管理、不同生成策略的选择与初始化、概率配置等。
//
// 主要内容：
//   - 单例生成器的创建与获取
//   - 生成器初始化流程
//   - 概率配置文件的处理
//   - 输出管理器的获取
//
#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "AbsProgramGenerator.h"
#include <iostream>
#include <cassert>
#include <string>
#include "CGOptions.h"
#include "DefaultProgramGenerator.h"
#include "DFSProgramGenerator.h"
#include "Probabilities.h"

using namespace std;

// 静态成员变量，当前生成器单例指针
AbsProgramGenerator *AbsProgramGenerator::current_generator_ = NULL;

// 获取当前生成器的输出管理器
OutputMgr *
AbsProgramGenerator::GetOutputMgr()
{
	assert(current_generator_);
	return current_generator_->getOutputMgr();
}

// 创建生成器实例（根据配置选择不同子类），并初始化
AbsProgramGenerator *
AbsProgramGenerator::CreateInstance(int argc, char *argv[], unsigned long seed)
{
	// 根据选项选择 DFS/exhaustive 还是默认生成器
	if (CGOptions::dfs_exhaustive()) {
		AbsProgramGenerator::current_generator_ = new DFSProgramGenerator(argc, argv, seed);
	}
	else
	{
		AbsProgramGenerator::current_generator_ = new DefaultProgramGenerator(argc, argv, seed);
	}

	// 初始化生成器
	AbsProgramGenerator::current_generator_->initialize();

	// 如果指定了概率配置的导出，直接导出并退出
	if (!(CGOptions::dump_default_probabilities().empty())) {
		CGOptions::random_random(false);
		Probabilities *prob = Probabilities::GetInstance();
		prob->dump_default_probabilities(CGOptions::dump_default_probabilities());
		exit(0);
	}
	// 如果指定了随机概率的导出，导出并退出
	else if (!(CGOptions::dump_random_probabilities().empty())) {
		CGOptions::random_random(true);
		Probabilities *prob = Probabilities::GetInstance();
		prob->dump_actual_probabilities(CGOptions::dump_random_probabilities(), seed);
		exit(0);
	}

	// 如果指定了概率配置文件，进行解析
	Probabilities *prob = Probabilities::GetInstance();
	string msg;
	if (!(CGOptions::probability_configuration().empty())) {
		if (!prob->parse_configuration(msg, CGOptions::probability_configuration())) {
			cout << "parsing configuration file error:" << msg << endl;
			return NULL;
		}
	}

	return AbsProgramGenerator::current_generator_;
}

// 获取当前生成器实例（单例）
AbsProgramGenerator *
AbsProgramGenerator::GetInstance()
{
	assert(AbsProgramGenerator::current_generator_);
	return AbsProgramGenerator::current_generator_;
}

// 构造函数
AbsProgramGenerator::AbsProgramGenerator()
{

}

// 虚析构函数
AbsProgramGenerator::~AbsProgramGenerator()
{

}

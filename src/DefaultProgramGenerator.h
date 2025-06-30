// -*- mode: C++ -*-
//
// Copyright (c) 2007, 2008, 2009, 2010, 2011 The University of Utah
// All rights reserved.
//
// 本文件定义了默认的程序生成器 DefaultProgramGenerator，
// 该类继承自 AbsProgramGenerator，实现了常规随机 C 程序的生成主流程。
//
// 主要职责：
//   - 初始化随机数生成器和输出管理器
//   - 生成所有类型、函数、主流程
//   - 管理生成过程中的全局状态
//   - 支持扩展机制
//
#ifndef DEFAULT_PROGRAM_GENERATOR_H
#define DEFAULT_PROGRAM_GENERATOR_H

#include "AbsProgramGenerator.h"

class OutputMgr;

// 默认程序生成器，实现常规的随机 C 程序生成主流程
class DefaultProgramGenerator : public AbsProgramGenerator {
public:
	// 构造函数，保存命令行参数和种子
	DefaultProgramGenerator(int argc, char *argv[], unsigned long seed);

	// 析构函数，负责清理资源
	virtual ~DefaultProgramGenerator();

	// 获取输出管理器
	virtual OutputMgr* getOutputMgr() { return output_mgr_; }

	// 主生成流程，生成类型、函数、输出代码
	virtual void goGenerator();

	// 初始化生成器，包括随机数、输出管理器、扩展机制
	virtual void initialize();

	// 获取计数前缀（本实现未用到，直接断言）
	virtual std::string get_count_prefix(const std::string &name);

private:
	// 命令行参数个数
	int argc_;

	// 命令行参数数组
	char **argv_;

	// 随机种子
	unsigned long seed_;

	// 输出管理器指针
	OutputMgr *output_mgr_;
};

#endif

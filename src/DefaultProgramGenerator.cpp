// -*- mode: C++ -*-
//
// Copyright (c) 2007, 2008, 2009, 2010, 2011, 2017 The University of Utah
// All rights reserved.
//
// 本文件实现了默认程序生成器 DefaultProgramGenerator 的相关方法，
// 负责常规随机 C 程序的生成主流程。
//
// 主要内容：
//   - 构造与析构
//   - 初始化流程（随机数、输出管理器、扩展）
//   - 主生成流程（类型、函数、输出）
//   - 计数前缀接口
//
#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "DefaultProgramGenerator.h"
#include <cassert>
#include <sstream>
#include "RandomNumber.h"
#include "AbsRndNumGenerator.h"
#include "DefaultOutputMgr.h"
#include "Finalization.h"
#include "Function.h"
#include "Type.h"
#include "CGOptions.h"
#include "SafeOpFlags.h"
#include "ExtensionMgr.h"

// 构造函数，保存命令行参数和种子
DefaultProgramGenerator::DefaultProgramGenerator(int argc, char *argv[], unsigned long seed)
	: argc_(argc),
	  argv_(argv),
	  seed_(seed),
	  output_mgr_(NULL)
{

}

// 析构函数，清理输出管理器和全局状态
DefaultProgramGenerator::~DefaultProgramGenerator()
{
	Finalization::doFinalization(); // 清理全局资源
	delete output_mgr_;
}

// 初始化生成器，包括随机数生成器、输出管理器、扩展机制
void
DefaultProgramGenerator::initialize()
{
	// 创建随机数生成器实例（使用默认策略和指定种子）
	RandomNumber::CreateInstance(rDefaultRndNumGenerator, seed_);
	// 创建输出管理器实例
	output_mgr_ = DefaultOutputMgr::CreateInstance();
	assert(output_mgr_);
	// 初始化扩展机制（如符号执行、覆盖测试等）
	ExtensionMgr::CreateExtension();
}

// 获取计数前缀（本实现未用到，直接断言）
std::string
DefaultProgramGenerator::get_count_prefix(const std::string &)
{
	assert(0);
	return "";
}

// 主生成流程，依次生成头部、类型、函数、输出代码
void
DefaultProgramGenerator::goGenerator()
{
	// 输出头部信息（包含命令行参数、种子等）
	output_mgr_->OutputHeader(argc_, argv_, seed_);

	// 生成所有类型（结构体、联合体、基本类型等）
	GenerateAllTypes();
	// 生成所有函数（包括 main 和随机生成的辅助函数）
	GenerateFunctions();
	// 输出完整的 C 程序代码
	output_mgr_->Output();
	// 如果启用了安全算子包装器统计，输出包装器数量
	if (CGOptions::identify_wrappers()) {
		ofstream ofile;
		ofile.open("wrapper.h");
		ofile << "#define N_WRAP " << SafeOpFlags::wrapper_names.size() << std::endl;
		ofile.close();
	}
}


// -*- mode: C++ -*-
//
// Copyright (c) 2007, 2008, 2009, 2010, 2011 The University of Utah
// All rights reserved.
//
// 本文件定义了抽象的程序生成器基类 AbsProgramGenerator，
// 该类为不同风格的程序生成器（如默认/DFS/exhaustive）提供统一接口。
//
// 主要职责：
//   - 统一管理生成器实例（单例模式）
//   - 提供生成主流程的虚接口
//   - 管理输出管理器
//   - 支持不同生成策略的扩展
//
#ifndef ABS_PROGRAM_GENERATOR_H
#define ABS_PROGRAM_GENERATOR_H

#include <string>

class OutputMgr;

// 抽象程序生成器基类，定义了生成器的统一接口
class AbsProgramGenerator {
public:
	// 创建生成器实例（根据配置选择不同子类），并初始化
	static AbsProgramGenerator* CreateInstance(int argc, char *argv[], unsigned long seed);

	// 获取当前生成器实例（单例）
	static AbsProgramGenerator* GetInstance();

	// 构造函数
	AbsProgramGenerator();

	// 虚析构函数，保证子类正确析构
	virtual ~AbsProgramGenerator();

	// 获取输出管理器（静态接口，实际由子类实现）
	static OutputMgr* GetOutputMgr();

	// 主生成流程（纯虚函数，需子类实现）
	virtual void goGenerator() = 0;

	// 获取输出管理器（纯虚函数，需子类实现）
	virtual OutputMgr* getOutputMgr() = 0;

	// 获取计数前缀（如用于命名变量/函数等，纯虚函数）
	virtual std::string get_count_prefix(const std::string &name) = 0;

protected:
	// 初始化生成器（纯虚函数，需子类实现）
	virtual void initialize() = 0;

private:
	// 当前生成器单例指针
	static AbsProgramGenerator *current_generator_;

	// 获取输出管理器的静态辅助函数
	static OutputMgr *getmgr(AbsProgramGenerator *gen);
};

#endif

// -*- mode: C++ -*-
//
// Copyright (c) 2007, 2008, 2009, 2010, 2011 The University of Utah
// All rights reserved.
//
// 本文件定义了csmith的类型系统Type类，支持简单类型、指针、结构体、联合体等，
// 并提供了类型生成、选择、管理等核心接口。
//
#ifndef TYPE_H
#define TYPE_H

///////////////////////////////////////////////////////////////////////////////

#include <string>
#include <ostream>
#include <vector>
#include "CommonMacros.h"
#include "StatementAssign.h"
#include "CVQualifiers.h"

template <class Name> class Enumerator;
using namespace std;

#define SIZE_UNKNOWN 0xFFFF

// -------------------- 类型描述枚举 --------------------
// 用于区分类型的种类：简单类型、指针、结构体、联合体
/*
 *
 */
enum eTypeDesc
{
	eSimple,   // 基本类型
	ePointer,  // 指针类型
	eUnion,    // 联合体
    eStruct,   // 结构体
};
#define MAX_TYPE_DESC ((eTypeDesc) (eStruct+1))

// -------------------- 简单类型枚举 --------------------
// 支持的所有基本类型
/*
 *
 */
enum eSimpleType
{
	eVoid,
	eChar,
	eInt,
	eShort,
	eLong,
	eLongLong,
	eUChar,
	eUInt,
	eUShort,
	eULong,
	eFloat,
	// eDouble,
	eULongLong,
	eInt128,
	eUInt128,
};
#define MAX_SIMPLE_TYPES ((eSimpleType) (eUInt128+1))

// -------------------- 类型匹配方式枚举 --------------------
enum eMatchType
{
	eExact,
	eConvert,
	eDereference,
	eDerefExact,
	eFlexible,
};

// -------------------- 类型系统主类 --------------------
/*
 * Type类：表示C语言中的各种类型，支持简单类型、指针、结构体、联合体等。
 * 提供类型生成、选择、管理等静态和成员方法。
 */
class Type
{
public:
	// 随机选择一个已生成的类型（工厂方法，非新建）
	static const Type *choose_random();

	// 随机选择一个非void类型
	static const Type *choose_random_nonvoid();

	// 随机选择一个整数类型
	static const Type *choose_random_simple(void);

	// 随机选择一个指针类型
	static const Type* choose_random_pointer_type(void);

	// 从给定类型中随机选择一个结构体类型
	static const Type *choose_random_struct_from_type(const Type* type, bool no_volatile);

	// 获取所有可用的结构体/联合体类型
	static void get_all_ok_struct_union_types(vector<Type *> &ok_types, bool no_const, bool no_volatile, bool need_int_field, bool bStruct);

	// 随机决定结构体/联合体是否有赋值操作符（C++）
	static bool if_struct_will_have_assign_ops();
	static bool if_union_will_have_assign_ops();

	// 判断类型是否包含整数字段
	bool has_int_field() const;

	// 判断类型是否可能有符号溢出
	bool signed_overflow_possible() const;

	// 从可用类型中随机选择一个结构体/联合体类型
	static const Type* choose_random_struct_union_type(vector<Type *> &ok_types);

	// 随机选择一个非void且非volatile类型
	static const Type * choose_random_nonvoid_nonvolatile(void);

	// 判断是否有指针类型
	static bool has_pointer_type(void);

	// 从给定类型递归随机选择一个类型
	static const Type* random_type_from_type(const Type* type, bool no_volatile=false, bool strict_simple_type=false);

	// 拷贝所有字段类型
	static void copy_all_fields_types(vector<const Type*> &dest_types, vector<const Type*> &src_types);

	// 重置累积类型集合
	static void reset_accum_types(vector<const Type*> &accum_types);

	// 删除无用结构体
	static void delete_useless_structs(vector<const Type*> &all_types, vector<const Type*> &accum_types);

	// 初始化位域枚举器
	static void init_is_bitfield_enumerator(Enumerator<string> &enumerator, int bitfield_prob);

	// 初始化字段枚举器
	static void init_fields_enumerator(Enumerator<std::string> &enumerator,
				Enumerator<string> &bitfield_enumerator,
				int type_bound, int qual_bound, int bitfield_qual_bound);

	// 枚举生成结构体字段
	static bool make_one_normal_field_by_enum(Enumerator<string> &enumerator, vector<const Type*> &all_types,
				vector<CVQualifiers> &all_quals, vector<const Type*> &fields,
				vector<CVQualifiers> &quals, vector<int> &fields_length, int i);

	// 枚举生成位域字段
	static bool make_one_bitfield_by_enum(Enumerator<string> &enumerator,
				vector<CVQualifiers> &all_bitfield_quals,
				vector<const Type*> &random_fields,
				vector<CVQualifiers> &qualifiers,
				vector<int> &fields_length,
				int index, bool &last_is_zero);

	// 获取位域长度
	static int get_bitfield_length(int length_flag);

	// 递归生成所有结构体类型（穷举模式）
	static void make_all_struct_types_(Enumerator<string> &bitfields_enumerator, vector<const Type*> &accum_types,
				vector<const Type*> &all_types, vector<CVQualifiers> &all_quals,
				vector<CVQualifiers> &all_bitfield_quals);

	// 生成带位域的结构体类型
	static void make_all_struct_types_with_bitfields(Enumerator<string> &enumerator,
					Enumerator<string> &bitfields_enumerator,
					vector<const Type*> &accum_types, vector<const Type*> &all_types,
					vector<CVQualifiers> &all_quals, vector<CVQualifiers> &all_bitfield_quals);

	// 递归生成所有结构体类型（穷举模式）
	static void make_all_struct_types(int level, vector<const Type*> &accum_types);

	// 生成所有结构体和联合体类型（穷举模式）
	static void make_all_struct_union_types();

	// 随机生成一个结构体或联合体类型
	static Type* make_random_struct_type(void);
	static Type* make_random_union_type(void);

	// 随机生成一个位域字段
	static void make_one_bitfield(vector<const Type*> &random_fields,
				vector<CVQualifiers> &qualifiers,
				vector<int> &fields_length);

	// 随机生成一个结构体字段
	static void make_one_struct_field(vector<const Type*> &random_fields,
				vector<CVQualifiers> &qualifiers,
				vector<int> &fields_length, 
				bool structHasAssignOps);
	// 随机生成一个联合体字段
	static void make_one_union_field(vector<const Type*> &fields, vector<CVQualifiers> &qfers, vector<int> &lens);

	// 随机生成一个全为位域的结构体
	static void make_full_bitfields_struct_fields(size_t field_cnt, vector<const Type*> &random_fields,
				vector<CVQualifiers> &qualifiers,
				vector<int> &fields_length, 
				bool structHasAssignOps);

	// 随机生成一个普通结构体
	static void make_normal_struct_fields(size_t field_cnt, vector<const Type*> &random_fields,
				vector<CVQualifiers> &qualifiers,
				vector<int> &fields_length, 
				bool structHasAssignOps);

	// 随机生成一个指针类型
	static Type* make_random_pointer_type(void);

	// 通过字符串获取类型
	static const Type *get_type_from_string(const string &type_string);

	// 生成所有简单类型（除void和不允许的long long）
	static void GenerateSimpleTypes(void);

	void get_type_sizeof_string(std::string &s) const;

	// 随机选择一个非void简单类型
	static eSimpleType choose_random_nonvoid_simple(void);

	// 选择LHS类型（赋值左值）
	static const Type *SelectLType(bool no_volatile, eAssignOps op);

	// 判断字段中是否有聚合类型
	static bool has_aggregate_field(const vector<const Type *> &fields);

	// 判断字段中是否有longlong类型
	static bool has_longlong_field(const vector<const Type *> &fields);

	// 构造函数：简单类型
	explicit Type(eSimpleType simple_type);
	// 构造函数：结构体/联合体类型
	Type(vector<const Type*>& fields, bool isStruct, bool packed,
			vector<CVQualifiers> &qfers, vector<int> &fields_length, bool hasAssignOps, bool hasImplicitNontrivialAssignOps);
	Type(vector<unsigned int>& array_dims, eSimpleType st);
	explicit Type(const Type* t);
	~Type(void);

	static const Type &get_simple_type(eSimpleType simple_type);

	static void doFinalization(void);

	const Type* get_base_type(void) const;
	int get_indirect_level(void) const;
	int get_struct_depth(void) const;
	void get_int_subfield_names(string prefix, vector<string>& names,
		vector<const Type *>& types, const vector<int>& excluded_fields) const;
	bool is_signed(void) const;
	bool is_long_long(void) const {
		return ((eType == eSimple) && (simple_type == eLongLong || simple_type == eULongLong));
	}
	const Type* to_unsigned(void) const;
	bool is_pointer_to_char(void) const { return ptr_type && ptr_type->eType == eSimple && (ptr_type->simple_type==eChar || ptr_type->simple_type==eUChar);}
	bool is_signed_char() const {
		return ((eType == eSimple) && (simple_type == eChar));
	}
	bool is_float() const {
		return ((eType == eSimple) && (simple_type == eFloat));
	}
	bool is_promotable(const Type* t) const;
	bool is_convertable(const Type* t) const;
	bool is_derivable(const Type* t) const;
	bool is_dereferenced_from(const Type* t) const;
	bool is_equivalent(const Type* t) const;
	bool needs_cast(const Type* t) const;
	bool is_unamed_padding(size_t index) const;
	bool is_full_bitfields_struct() const;
	bool is_bitfield(size_t index) const ;
	bool has_bitfields() const;
	bool has_padding(void) const;
	bool contain_pointer_field(void) const;
	bool is_const_struct_union() const;
	bool is_volatile_struct_union() const;
	bool has_assign_ops() const  { return has_assign_ops_; }
    bool has_implicit_nontrivial_assign_ops() const {
        return has_implicit_nontrivial_assign_ops_;
    }
	bool is_int(void) const { return eType == eSimple && simple_type != eVoid;}
	bool is_aggregate(void) const { return eType == eStruct || eType == eUnion;}
	bool match(const Type* t, enum eMatchType mt) const;
	unsigned long SizeInBytes(void) const;
	void Output(std::ostream &) const;
	std::string printf_directive(void) const;
	static Type* find_pointer_type(const Type* t, bool add);
	static Type* find_type(const Type* t);

// private:
	eTypeDesc eType;                 // 类型描述
	const Type *ptr_type;            // 指针类型的指向类型
	eSimpleType simple_type;         // 简单类型枚举
	vector<unsigned int> dimensions; // 数组维度
	vector<const Type*> fields;      // 结构体/联合体字段
	unsigned int sid;                // 结构体/联合体序号

	bool used;                       // 是否被使用
	bool printed;                    // 是否已输出
	const bool packed_;              // 是否为packed结构体
	bool has_assign_ops_;            // 是否有赋值操作符
    bool has_implicit_nontrivial_assign_ops_;   // 是否有隐式复杂赋值操作符
	vector<CVQualifiers> qfers_;     // 字段的限定符
	vector<int> bitfields_length_;   // 位域长度，-1为普通字段

	static Type *void_type;
private:
	DISALLOW_COPY_AND_ASSIGN(Type);

	static const Type *simple_types[MAX_SIMPLE_TYPES];

	// Package init.
	friend void GenerateAllTypes(void);
};

// -------------------- 类型生成主流程 --------------------
// 生成所有类型（简单类型、结构体、联合体等）
void GenerateAllTypes(void);
// 获取int类型
const Type * get_int_type(void);
// 输出所有结构体/联合体声明
void OutputStructUnionDeclarations(std::ostream &);
// 输出结构体赋值操作符
void OutputStructAssignOps(Type* type, std::ostream &out, bool vol);
// 输出结构体/联合体定义
void OutputStructUnion(Type* type, std::ostream &out);

///////////////////////////////////////////////////////////////////////////////

#endif // TYPE_H

// Local Variables:
// c-basic-offset: 4
// tab-width: 4
// End:

// End of file.

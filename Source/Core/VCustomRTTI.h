#pragma once

#include <map>

// 用于基类
#define DECLARE_DYNAMIC_CREATE_BASE(Base) \
public: \
	typedef Base*( *ClassGenerate )( ); \
	static void Register( const char* _ClassName , ClassGenerate _ClassGenerate ) \
	{ \
		std::map<std::string , ClassGenerate>& ClassRepository = Base::GetRepository();\
		ClassRepository.insert( std::map<std::string  , ClassGenerate>::value_type( std::string(_ClassName) , _ClassGenerate ) );\
	};\
public:\
	static Base* Create( const char* _ClassName ) \
	{ \
		std::map<std::string , ClassGenerate>& ClassRepository = Base::GetRepository();\
		std::map<std::string , ClassGenerate>::iterator iter; \
		if( ( iter = ClassRepository.find( std::string (_ClassName) ) ) != ClassRepository.end() ) \
		{ \
			return ( ( *iter ).second )( ); \
		}\
		return nullptr;\
	};\
	static std::map<std::string , ClassGenerate>& GetRepository()\
	{\
		static std::map<std::string , ClassGenerate> ClassRepository;\
		return ClassRepository;\
	}\

#define DECLARE_DYNAMIC_CREATE_DERIVED(Derived , Base)\
public:\
	struct Derived##Register\
	{\
		Derived##Register()\
		{\
			static bool bRegistered = false;\
			if(!bRegistered)\
			{\
				Base::Register(#Derived , Create);\
				bRegistered = true;\
			}\
		}\
	};\
	static Base* Create()\
	{\
		return new Derived;\
	};

// 自动实现注册功能
#define IMPLEMENT_DYNAMIC_CREATE_DERIVED(Derived)\
	static Derived::Derived##Register Derived##Instance;
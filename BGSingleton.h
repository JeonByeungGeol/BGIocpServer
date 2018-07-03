#pragma once
#include "BGConfigManager.h"
#include "BGLogManager.h"

/**
 * std::once_flag는 초기화 하지 않으며 std::call_once와 연동되어 멀티스레드 환경에서
 * 단 한번만 실행되는 것을 운영체제 차원에서 보장해 주도록 구현되었다.
 * 컴파일러가 만들어 주는 클래스의 복사 생성자나 할당 연산자를 사용시 깊은 복사(Deep copy)를
 * 수행하는 복사 시맨틱(Copy Semantics)이 일어난다. 하지만 벡터와 같은 컨테이너에 작업을
 * 수행하면 원본 객체는 사용하지 않게 되어 낭비가 발생한다. 소유권 이전은 얕은 복사(Shallow copy)이후
 * 원본을 더미 객체로 만들어 소유권을 이전시키는 이동 시맨틱(Move Semantics)이 일어나는데,
 * 대표적으로 이동 생성자나 unique_ptr이 있다. unique_ptr에는 복사 생성자나 할당 연산자가 없기 때문에
 * std::move()를 사용하거나 reset() 메소드를 이용해 객체를 할당하며 스마트 포인터기 때문에 자원해제에 안전하다.
*/

/**
 * <pre>
 * 싱글톤으로 만들어야 할 필요가 있는 객체들은
 * 이 객체안에 넣는다.
 * 많은 싱글톤 객체가 만들어 지는 것을 방지하기 위함
 * 크기가 너무 큰 객체들은 넣지 않는다...
 * </pre>
*/

/** define으로 정의*/
#define g_Config BGSingleton::Instance()->GetConfigManager()
#define g_LogManager BGSingleton::Instance()->GetLogManager()

/** 사용자 정의*/

class BGSingleton
{
//////////////////////////////////////////////////
	/** 싱글톤 구현 부*/
private:
	static std::once_flag onceflag_;
	static std::unique_ptr<BGSingleton> instance_;
	
protected:
	BGSingleton() = default;
	BGSingleton(const BGSingleton&) = delete;
	BGSingleton& operator=(const BGSingleton&) = delete;

public:
	static BGSingleton* Instance() {
		std::call_once(onceflag_, []() {
			instance_.reset(new BGSingleton);
		});
		return instance_.get();
	}
//////////////////////////////////////////////////


	/** 싱글톤으로 생성할 객체*/
private:
	BGConfigManager m_ConfigManager;
	BGLogManager m_LogManager;


public:
	BGConfigManager& GetConfigManager() { return m_ConfigManager; }
	BGLogManager& GetLogManager() { return m_LogManager; }
};


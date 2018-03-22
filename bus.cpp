#pragma once
#include <iostream>
#include <string>
#include <windows.h>
#include "FIRMSYS_API.h" //firmsys에서 제공하는 헤더
#include <ctime>
using namespace std;

#define com1 0 //COM1 PORT
#define com2 1 //COM2 PORT

#define com3 2 //COM3 PORT

string CurrentHour();	// 현재 시간(시)
string CurrentMinute();	// 현재 시간(분)
string CurrentSecond();	// 현재 시간(초)
int sell_check(string st_time, string _hour, string _minute, string _second);	// 승차시간과 하차시간을 계산 후 추가요금을 리턴.

void main()
{
	int bool_ary[2] = {0,0};		// 두명 구분(0 = 타지 않은상태, 1 = 탄상태(대기상태))
	string start_time[2];			// 승차 시간
	string end_time[2];				// 하차 시간
	int plus_money = 0;				// 추가 요금

	BOOL m_Flag = FALSE;			// 리더기 인식
	BYTE result[8];
	while(m_Flag==FALSE)			// 리더기가 인식되지 않았을
	{
		m_Flag = FS_Open(com3,CBR_38400); //RFID Connection
		if(!m_Flag)	// 연결 시도 후에도 리더기가 없다면
		{
			cout<<"RFID Connection Failed!"<<endl;
			system("pause");
		}
	}
	
	FS_SYSTEMINFORMATION pInfo;

	//RFID 값을 대면 바로 값을 읽어온다.
	while(true)
	{
		
		//FS_GetSystemInformation(&pInfo)// Tag의 모든 정보를 읽어온다.
		
		cout << FS_Inventory(&pInfo) << endl;	// 출력하면 에러코드인 1이 출력되는것을 볼 수 있습니다(0 = 정상작동, 1 = 에러코드)
		if((FS_Inventory(&pInfo))==0) //RFID Tag값을 읽어왔다면
		{
			/* 
			시현을 두가지 태그로 하려해서, 예시를 두가지밖에 만들지 않았습니다. 더 하려면 리더기 태그의 갯수는 제한이 없으므로 리스트
			또는 벡터를 사용하면 될 것이라고 생각됩니다. 또한 갯수가 많아지면 많아질 수록 검색하는데 많은 시간이 걸리므로,
			해쉬구조를 사용하면 될 것이라고 생각합니다. 해쉬는 빅오가 O(1)이므로 바로 찾을 수있습니다. 이것을 구현하면서 보면 어차피 일련번호가 같으므로
			일련번호를 해쉬값으로 사용해서 하면 충돌이 일어나지 않을것이라고 생각됩니다. 또는 만약 이것을 추가할때 소팅을 해서 추가한다고 가정 했을 경우
			빅오가 O(logN) - 및이 2인 로그N 을 사용해도 괜찮을 것이라고 생각합니다.
			*/

			if(pInfo.uid.digit[3] == 0)	//'6D 57 5D 4E ' 이 되도록 해야하지만, 리더기 인식이 되지 않아 방법을 찾지못함.
			{
				switch(bool_ary[0])	//'6D 57 5D 4E 에 관하여 정보처리
				{
				case 0:		// 안탄 상태
					//탄 시간 저장
					start_time[0] = CurrentHour() + CurrentMinute() + CurrentSecond();
					//내역 출력
					cout<<"유저 코드 : "<<"6D 57 5D 4E "<<endl;
					cout<<"승차시간 : "<<CurrentHour()<<"시 "<<CurrentMinute()<<"분 "<<CurrentSecond()<<"초"<<endl;
					bool_ary[0] = 1;	// 탄상태(대기상태) 설정
					system("pause");
					break;

				case 1:		// 탄상태(대기 상태)
					//하차 시간 저장
					end_time[0] = CurrentHour() + CurrentMinute() + CurrentSecond();
					//시간에 비례해서 추가요금이 올라감.
					plus_money = sell_check(start_time[0],CurrentHour(), CurrentMinute(), CurrentSecond());
					//내역 출력
					cout<<"유저 코드 : "<<"6D 57 5D 4E "<<endl;
					cout<<"승차시간 : "<<start_time[0]<<endl;
					cout<<"하자시간 : "<<CurrentHour()<<"시 "<<CurrentMinute()<<"분 "<<CurrentSecond()<<"초"<<endl;
					cout<<"요금 : "<< 1000 + 100*plus_money<<"원"<<endl;
					plus_money = 0;		//추가요금 초기화
					bool_ary[0] = 0;	//안탄상태로 설정
					system("pause");
					break;
			
				default:	// 다른값
					break;	//작동안함.
				}
			}

			else if(pInfo.uid.digit[3] == 0)	//'CD B5 5C 4E  ' 이 되도록 해야하지만, 리더기 인식이 되지 않아 방법을 찾지못함. 
			{
				switch(bool_ary[1])
				{
				case 0:		// 안탄 상태
					start_time[1] =  CurrentHour() + CurrentMinute() + CurrentSecond();
					//내역 출력
					cout<<"유저 코드 : "<<"CD B5 5C 4E"<<endl;
					cout<<"승차시간 : "<<CurrentHour()<<"시 "<<CurrentMinute()<<"분 "<<CurrentSecond()<<"초"<<endl;
					bool_ary[1] = 1;
					break;

				case 1:		// 탄상태(대기 상태)
					//하차시간 저장
					end_time[1] = CurrentHour() + CurrentMinute() + CurrentSecond();
					//시간에 비례하여 추가요금 올라감.
					plus_money = sell_check(start_time[1],CurrentHour(), CurrentMinute(), CurrentSecond());
					//내역 출력
					cout<<"유저 코드 : "<<"CD B5 5C 4E"<<endl;
					cout<<"승차시간 : "<<start_time[1]<<endl;
					cout<<"하자시간 : "<<CurrentHour()<<"시 "<<CurrentMinute()<<"분 "<<CurrentSecond()<<"초"<<endl;
					cout<<"요금 : "<<1000 + 100*plus_money<<"원"<<endl;
					bool_ary[1] = 0;	//안탄상태로 설정
					plus_money = 0;		//추가요금 초기화
					break;

				default:	// 다른값
					break;	//작동안함.
				}
			}
			

		}
	}

	if(m_Flag)
		FS_Close(); //RFID Disconnection
}

// 현재 시간(분)
string CurrentHour()
{
	time_t timer;
	struct tm t;

	timer = time(NULL);
	localtime_s(&t,&timer); 

	string currentTime;
	int buf;
	char target[10];

	//시
	buf = t.tm_hour;
	if(buf < 10)
	{
		//시간이 10보다 작을떄(한자리수) 일경우 0을추가해준다.
		currentTime.push_back('0');
		itoa(buf,target,10);
		currentTime.append(target);
	}
	
	else
	{
		itoa(buf,target,10);
		currentTime.append(target);
	}

	return currentTime;
}

// 현재 시간(시)
string CurrentMinute()
{
	time_t timer;
	struct tm t;

	timer = time(NULL);
	localtime_s(&t,&timer); 

	string currentTime;
	int buf;
	char target[10];

	//분
	buf = t.tm_min;
	if(buf < 10)
	{
		//시간이 10보다 작을떄(한자리수) 일경우 0을추가해준다.
		currentTime.push_back('0');
		itoa(buf,target,10);
		currentTime.append(target);
	}
	
	else
	{
		itoa(buf,target,10);
		currentTime.append(target);
	}

	return currentTime;
}

// 현재 시간(초)
string CurrentSecond()
{
	time_t timer;
	struct tm t;

	timer = time(NULL);
	localtime_s(&t,&timer); 

	string currentTime;
	int buf;
	char target[10];

	//초
	buf = t.tm_sec;
	if(buf < 10)
	{
		//시간이 10보다 작을떄(한자리수) 일경우 0을추가해준다.
		currentTime.push_back('0');
		itoa(buf,target,10);
		currentTime.append(target);
	}

	else
	{
		itoa(buf,target,10);
		currentTime.append(target);
	}

	return currentTime;
}

// 승차시간과 하차시간을 계산 후 추가요금을 리턴.
int sell_check(string st_time, string _hour, string _minute, string _second)
{
	int s_time = atoi(st_time.c_str());		// string 형으로 받은것을 int형으로바꿈
	int h, m, s;
	h = s_time / 10000;					// 탄시간의 [시]만 따로 저장
	m = (s_time / 100) % 100;			// 탄시간의 [분]만 따로 저장
	s = s_time % 100;					// 탄시간의 [초]만 따로 저장
	int start_time = h * 3600 +  m * 60 + s;	// 탄시간 시 + 분 + 초 계산
	int end_time = atoi(_hour.c_str()) * 3600 +  atoi(_minute.c_str()) * 60 + atoi(_second.c_str());	// 하차시간의 시+분+초 계산
	int count = 0;

	while(end_time>=start_time)
	{
		if(end_time - start_time >= 300)	//5분에 100원씩 추가
		{
			end_time - 300;
			count++;
		}
	}
	//계산할땐 count*100을해서 계산하면됨. 그것이 불편하다면 그냥 리턴값을count*100으로 하고, 위에 요금 부분에서 plus_money*100부분에서
	//그냥 plus_money만 해도 상관은 없음.
	return count;
}
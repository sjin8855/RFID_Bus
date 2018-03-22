#pragma once
#include <iostream>
#include <string>
#include <windows.h>
#include "FIRMSYS_API.h" //firmsys���� �����ϴ� ���
#include <ctime>
using namespace std;

#define com1 0 //COM1 PORT
#define com2 1 //COM2 PORT

#define com3 2 //COM3 PORT

string CurrentHour();	// ���� �ð�(��)
string CurrentMinute();	// ���� �ð�(��)
string CurrentSecond();	// ���� �ð�(��)
int sell_check(string st_time, string _hour, string _minute, string _second);	// �����ð��� �����ð��� ��� �� �߰������ ����.

void main()
{
	int bool_ary[2] = {0,0};		// �θ� ����(0 = Ÿ�� ��������, 1 = ź����(������))
	string start_time[2];			// ���� �ð�
	string end_time[2];				// ���� �ð�
	int plus_money = 0;				// �߰� ���

	BOOL m_Flag = FALSE;			// ������ �ν�
	BYTE result[8];
	while(m_Flag==FALSE)			// �����Ⱑ �νĵ��� �ʾ���
	{
		m_Flag = FS_Open(com3,CBR_38400); //RFID Connection
		if(!m_Flag)	// ���� �õ� �Ŀ��� �����Ⱑ ���ٸ�
		{
			cout<<"RFID Connection Failed!"<<endl;
			system("pause");
		}
	}
	
	FS_SYSTEMINFORMATION pInfo;

	//RFID ���� ��� �ٷ� ���� �о�´�.
	while(true)
	{
		
		//FS_GetSystemInformation(&pInfo)// Tag�� ��� ������ �о�´�.
		
		cout << FS_Inventory(&pInfo) << endl;	// ����ϸ� �����ڵ��� 1�� ��µǴ°��� �� �� �ֽ��ϴ�(0 = �����۵�, 1 = �����ڵ�)
		if((FS_Inventory(&pInfo))==0) //RFID Tag���� �о�Դٸ�
		{
			/* 
			������ �ΰ��� �±׷� �Ϸ��ؼ�, ���ø� �ΰ����ۿ� ������ �ʾҽ��ϴ�. �� �Ϸ��� ������ �±��� ������ ������ �����Ƿ� ����Ʈ
			�Ǵ� ���͸� ����ϸ� �� ���̶�� �����˴ϴ�. ���� ������ �������� ������ ���� �˻��ϴµ� ���� �ð��� �ɸ��Ƿ�,
			�ؽ������� ����ϸ� �� ���̶�� �����մϴ�. �ؽ��� ����� O(1)�̹Ƿ� �ٷ� ã�� ���ֽ��ϴ�. �̰��� �����ϸ鼭 ���� ������ �Ϸù�ȣ�� �����Ƿ�
			�Ϸù�ȣ�� �ؽ������� ����ؼ� �ϸ� �浹�� �Ͼ�� �������̶�� �����˴ϴ�. �Ǵ� ���� �̰��� �߰��Ҷ� ������ �ؼ� �߰��Ѵٰ� ���� ���� ���
			����� O(logN) - ���� 2�� �α�N �� ����ص� ������ ���̶�� �����մϴ�.
			*/

			if(pInfo.uid.digit[3] == 0)	//'6D 57 5D 4E ' �� �ǵ��� �ؾ�������, ������ �ν��� ���� �ʾ� ����� ã������.
			{
				switch(bool_ary[0])	//'6D 57 5D 4E �� ���Ͽ� ����ó��
				{
				case 0:		// ��ź ����
					//ź �ð� ����
					start_time[0] = CurrentHour() + CurrentMinute() + CurrentSecond();
					//���� ���
					cout<<"���� �ڵ� : "<<"6D 57 5D 4E "<<endl;
					cout<<"�����ð� : "<<CurrentHour()<<"�� "<<CurrentMinute()<<"�� "<<CurrentSecond()<<"��"<<endl;
					bool_ary[0] = 1;	// ź����(������) ����
					system("pause");
					break;

				case 1:		// ź����(��� ����)
					//���� �ð� ����
					end_time[0] = CurrentHour() + CurrentMinute() + CurrentSecond();
					//�ð��� ����ؼ� �߰������ �ö�.
					plus_money = sell_check(start_time[0],CurrentHour(), CurrentMinute(), CurrentSecond());
					//���� ���
					cout<<"���� �ڵ� : "<<"6D 57 5D 4E "<<endl;
					cout<<"�����ð� : "<<start_time[0]<<endl;
					cout<<"���ڽð� : "<<CurrentHour()<<"�� "<<CurrentMinute()<<"�� "<<CurrentSecond()<<"��"<<endl;
					cout<<"��� : "<< 1000 + 100*plus_money<<"��"<<endl;
					plus_money = 0;		//�߰���� �ʱ�ȭ
					bool_ary[0] = 0;	//��ź���·� ����
					system("pause");
					break;
			
				default:	// �ٸ���
					break;	//�۵�����.
				}
			}

			else if(pInfo.uid.digit[3] == 0)	//'CD B5 5C 4E  ' �� �ǵ��� �ؾ�������, ������ �ν��� ���� �ʾ� ����� ã������. 
			{
				switch(bool_ary[1])
				{
				case 0:		// ��ź ����
					start_time[1] =  CurrentHour() + CurrentMinute() + CurrentSecond();
					//���� ���
					cout<<"���� �ڵ� : "<<"CD B5 5C 4E"<<endl;
					cout<<"�����ð� : "<<CurrentHour()<<"�� "<<CurrentMinute()<<"�� "<<CurrentSecond()<<"��"<<endl;
					bool_ary[1] = 1;
					break;

				case 1:		// ź����(��� ����)
					//�����ð� ����
					end_time[1] = CurrentHour() + CurrentMinute() + CurrentSecond();
					//�ð��� ����Ͽ� �߰���� �ö�.
					plus_money = sell_check(start_time[1],CurrentHour(), CurrentMinute(), CurrentSecond());
					//���� ���
					cout<<"���� �ڵ� : "<<"CD B5 5C 4E"<<endl;
					cout<<"�����ð� : "<<start_time[1]<<endl;
					cout<<"���ڽð� : "<<CurrentHour()<<"�� "<<CurrentMinute()<<"�� "<<CurrentSecond()<<"��"<<endl;
					cout<<"��� : "<<1000 + 100*plus_money<<"��"<<endl;
					bool_ary[1] = 0;	//��ź���·� ����
					plus_money = 0;		//�߰���� �ʱ�ȭ
					break;

				default:	// �ٸ���
					break;	//�۵�����.
				}
			}
			

		}
	}

	if(m_Flag)
		FS_Close(); //RFID Disconnection
}

// ���� �ð�(��)
string CurrentHour()
{
	time_t timer;
	struct tm t;

	timer = time(NULL);
	localtime_s(&t,&timer); 

	string currentTime;
	int buf;
	char target[10];

	//��
	buf = t.tm_hour;
	if(buf < 10)
	{
		//�ð��� 10���� ������(���ڸ���) �ϰ�� 0���߰����ش�.
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

// ���� �ð�(��)
string CurrentMinute()
{
	time_t timer;
	struct tm t;

	timer = time(NULL);
	localtime_s(&t,&timer); 

	string currentTime;
	int buf;
	char target[10];

	//��
	buf = t.tm_min;
	if(buf < 10)
	{
		//�ð��� 10���� ������(���ڸ���) �ϰ�� 0���߰����ش�.
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

// ���� �ð�(��)
string CurrentSecond()
{
	time_t timer;
	struct tm t;

	timer = time(NULL);
	localtime_s(&t,&timer); 

	string currentTime;
	int buf;
	char target[10];

	//��
	buf = t.tm_sec;
	if(buf < 10)
	{
		//�ð��� 10���� ������(���ڸ���) �ϰ�� 0���߰����ش�.
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

// �����ð��� �����ð��� ��� �� �߰������ ����.
int sell_check(string st_time, string _hour, string _minute, string _second)
{
	int s_time = atoi(st_time.c_str());		// string ������ �������� int�����ιٲ�
	int h, m, s;
	h = s_time / 10000;					// ź�ð��� [��]�� ���� ����
	m = (s_time / 100) % 100;			// ź�ð��� [��]�� ���� ����
	s = s_time % 100;					// ź�ð��� [��]�� ���� ����
	int start_time = h * 3600 +  m * 60 + s;	// ź�ð� �� + �� + �� ���
	int end_time = atoi(_hour.c_str()) * 3600 +  atoi(_minute.c_str()) * 60 + atoi(_second.c_str());	// �����ð��� ��+��+�� ���
	int count = 0;

	while(end_time>=start_time)
	{
		if(end_time - start_time >= 300)	//5�п� 100���� �߰�
		{
			end_time - 300;
			count++;
		}
	}
	//����Ҷ� count*100���ؼ� ����ϸ��. �װ��� �����ϴٸ� �׳� ���ϰ���count*100���� �ϰ�, ���� ��� �κп��� plus_money*100�κп���
	//�׳� plus_money�� �ص� ����� ����.
	return count;
}
//
//Attribute.h ���������� 
//
//by �ű�� on 16/5/23
//

#ifndef minisql_Attribute_h
#define minisql_Attribute_h

#include<string>
#include<iostream>
using namespace std;

class Attribute //������ 
{
	public:
		string name; //��������
		int type; //�������ͣ�-1����float, 0����int,������n����char�ͣ�n��ʾ���ٸ�char
		bool ifUnique;
		string index;  //ȱʡΪ�������ַ�������ʾû������
		Attribute(string n,int t, bool i);//���캯����name=n,type=t,ifUnique=i
		
	    int static const TYPE_FLOAT = -1;
	    int static const TYPE_INT = 0;
	    string indexNameGet(){return index;} //�ж��Ƿ������������ؿ��ַ�����������
		
		void print()//���������Ϣ�����֣����ͣ��Ƿ�unique 
		{
			cout << "name:" << name << ";type:" << ";ifUnique: " << ifUnique << ";index:" << index << endl;	
		}
	 	    
};
 
#endif
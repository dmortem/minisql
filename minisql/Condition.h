#ifndef minisql_Condition_h
#define minisql_Condition_h 
#include<string>
#include<sstream>
using namespace std; 

//Condition�� ,�ṩwhere���������ȽϺ��� 
class Condition 
{
	public:
		const static int OPERATOR_EQUAL = 0; // 0����"="
		const static int OPERATOR_NOT_EQUAL = 1; //1����"<>"�������ڣ� 
		const static int OPERATOR_LESS = 2; //2����"<"
		const static int OPERATOR_MORE = 3; //3����">"
		const static int OPERATOR_LESS_EQUAL = 4; //4����"<="
		const static int OPERATOR_MORE_EQUAL = 5; //5����">="
		
		Condition(string a, string v,int o); //���캯��
		
		string attributeName; //�����ĸ����Ե�����
		string value; //���Ƚϵļ�ֵ���жϱ�׼
		int operate;  //���Ƚϵ����ͣ�-1����float,0����int,����������string
		
		//�������͵ıȽ� 
		bool ifRight(int content); //��string��value���Ƚϼ�ֵת���� int���봫��Ĳ���content�Ƚ� 
		bool ifRight(float content); //ͬ���� 
		bool ifRight(string content); //ͬ���� 	
	
}; 

#endif

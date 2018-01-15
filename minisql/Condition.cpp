#include"Condition.h" 
#include<stdio.h> 

Condition::Condition(string a,string v,int o){//���캯��ʵ�� 
	attributeName = a;
	value = v;
	operate = o;
} 

bool Condition::ifRight(int content)
{
	stringstream ss; //stringstream������������������� 
	ss << value; //���Ƚ�ֵ�����ss
	int myContent;
	ss >> myContent; //���Ƚ�ֵ�����int�� myContent
	
	switch(operate)//���ݱ��Ƚ�ֵ��������ѡ���֧ 
	{
		case Condition::OPERATOR_EQUAL:
		     return content == myContent; //��ȷ���1������ȷ���0 
			 break;
		case Condition::OPERATOR_NOT_EQUAL:
		     return content != myContent; //����ȷ���1����ȷ���0 
			 break; 
		case Condition::OPERATOR_LESS:
		     return content < myContent; //С�ڷ���1����С�ڷ���0 
			 break;
		case Condition::OPERATOR_MORE:
		     return content > myContent; //���ڷ���1�������ڷ���0 
			 break;
		case Condition::OPERATOR_LESS_EQUAL:
		     return content <= myContent; //С�ڵ��ڷ���1����С�ڵ��ڷ���0 
			 break;
		case Condition::OPERATOR_MORE_EQUAL:
		     return content >= myContent; //С�ڷ���1����С�ڷ���0 
			 break;
		default:
		     printf("û����ԸñȽϷ�����!\n");
		     return false;
		     break;
	}
}
	
	
bool Condition::ifRight(float content)
{
	stringstream ss; //stringstream������������������� 
	ss << value; //���Ƚ�ֵ�����ss
	float myContent;
	ss >> myContent; //���Ƚ�ֵ�����int�� myContent
	
	switch(operate)//���ݱ��Ƚ�ֵ��������ѡ���֧ 
	{
		case Condition::OPERATOR_EQUAL:
		     return content == myContent; //��ȷ���1������ȷ���0 
			 break;
		case Condition::OPERATOR_NOT_EQUAL:
		     return content != myContent; //����ȷ���1����ȷ���0 
			 break; 
		case Condition::OPERATOR_LESS:
		     return content < myContent; //С�ڷ���1����С�ڷ���0 
			 break;
		case Condition::OPERATOR_MORE:
		     return content > myContent; //���ڷ���1�������ڷ���0 
			 break;
		case Condition::OPERATOR_LESS_EQUAL:
		     return content <= myContent; //С�ڵ��ڷ���1����С�ڵ��ڷ���0 
			 break;
		case Condition::OPERATOR_MORE_EQUAL:
		     return content >= myContent; //С�ڷ���1����С�ڷ���0 
			 break;
		default:
		     printf("û����ԸñȽϷ�����!\n");
		     return false;
		     break;
	} 
}
	
	
bool Condition::ifRight(string content)
{	
	string myContent = value; //����Ҫת������ֱ�ӱȽ� 
		
	switch(operate)//���ݱ��Ƚ�ֵ��������ѡ���֧ 
	{
		case Condition::OPERATOR_EQUAL:
		     return content == myContent; //��ȷ���1������ȷ���0 
			 break;
		case Condition::OPERATOR_NOT_EQUAL:
		     return content != myContent; //����ȷ���1����ȷ���0 
			 break; 
		case Condition::OPERATOR_LESS:
		     return content < myContent; //С�ڷ���1����С�ڷ���0 
			 break;
		case Condition::OPERATOR_MORE:
		     return content > myContent; //���ڷ���1�������ڷ���0 
			 break;
		case Condition::OPERATOR_LESS_EQUAL:
		     return content <= myContent; //С�ڵ��ڷ���1����С�ڵ��ڷ���0 
			 break;
		case Condition::OPERATOR_MORE_EQUAL:
		     return content >= myContent; //С�ڷ���1����С�ڷ���0 
			 break;
		default:
		     printf("û����ԸñȽϷ�����!\n");
		     return false;
		     break;
	}
}

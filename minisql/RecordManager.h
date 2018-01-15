#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H
#include "Condition.h"
#include "Attribute.h" 
#include "BufferManager.h" 
#include "Minisql.h" 
#include <string>
#include <vector>
using namespace std;
class API;
class RecordManager
{
	public:
		RecordManager(){} 
		BufferManager bm;
		API *api;
		
		int tableCreate(string tableName); //�������ļ�,�ɹ�����1�����򷵻�0 
		int tableDrop(string tableName); //ɾ�����ļ����ɹ�����1�����򷵻�0 
		
		int indexCreate(string indexName); //���������ļ����ɹ�����1�����򷵻�0 
		int indexDrop(string indexName); //ɾ�������ļ����ɹ�����1�����򷵻�0 
		
		int recordInsert(string tableName,char*record,int recordSize);//��������һ���̶����ȵļ�¼ 
		
	    int recordAllShow(string tableName,vector<string>*attributeNameVector,vector<Condition>*conditionVector);//��ʾ������п�ڵ����������ļ�¼ �����ؼ�¼����count 
		int recordBlockShow(string tableName,vector<string>*attributeNameVector,vector<Condition>*conditionVector,int blockOffset);//��ʾ���ָ��ƫ�����Ŀ�ڵ����������ļ�¼�����ؼ�¼����count  
		
		int recordAllFind(string tableName, vector<Condition>* conditionVector); //�������п�ڵ�����������¼�����ؼ�¼��Ŀ 
				
		int recordAllDelete(string tableName, vector<Condition>* conditionVector); //ɾ�����п�ڵ����������ļ�¼������ɾ���ļ�¼��Ŀ��-1 ������� 
        int recordBlockDelete(string tableName,  vector<Condition>* conditionVector, int blockOffset);  //ɾ��ָ��ƫ������ڵ����������ļ�¼������ɾ���ļ�¼��Ŀ��-1 ������� 
		
		int indexRecordAllAlreadyInsert(string tableName,string indexName);  //��Ա�����м�¼����ָ������������������������Ҫ��ļ�¼��Ŀ��-1������� 
		
		string tableFileNameGet(string tableName);//���ش��ָ���������ļ��� 
        string indexFileNameGet(string indexName);//���ش��ָ�����������ļ��� 
        
	private:
		int recordBlockFind(string tableName, vector<Condition>* conditionVector, blockNode* block);//����ĳ��ڵ�����������¼�����ؼ�¼��Ŀ �����ڲ�������
	    int recordBlockShow(string tableName, vector<string>*attributeNameVector, vector<Condition>* conditionVector, blockNode* block);//��ʾ����ĳ��ڵ�����������¼�����ؼ�¼��Ŀ �������������
        int recordBlockDelete(string tableName,  vector<Condition>* conditionVector, blockNode* block);//ɾ��ĳ��ڵ�����������¼�����ؼ�¼��Ŀ ������ɾ������
        int indexRecordBlockAlreadyInsert(string tableName,string indexName, blockNode* block); //��ĳ��ڵ��¼����ָ�����������ؼ�¼��Ŀ �����ڶ����м�¼������������������ 
		 
		bool recordConditionFit(char* recordBegin,int recordSize, vector<Attribute>* attributeVector,vector<Condition>* conditionVector); //�жϴ�ĳ����ʼ��ָ�����ȼ�¼�Ƿ�����������true-���㣬false-������ 
		void recordPrint(char* recordBegin, int recordSize, vector<Attribute>* attributeVector, vector<string> *attributeNameVector); //�����ĳ����ʼ������������attributeVector��һ�����������������ѡattributeNameVector���������е��������� 
        bool contentConditionFit(char* content, int type, Condition* condition); //���嵽�ж�ÿһ�����Ե������Ƿ�������������recordConditionFit���Ӳ��� 
        void contentPrint(char * content, int type);//���ַ�����ʽ������ֵת��Ϊ�������͸�ʽ����� 
};

#endif 

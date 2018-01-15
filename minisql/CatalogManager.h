#ifndef CATALOGMANAGER_H
#define CATALOGMANAGER_H

#include<string> 
#include<vector>
#include"Attribute.h"
#include"BufferManager.h"
#include"IndexInfo.h"
using namespace std;

//�ඨ��
class CatalogManager{
	public:
		BufferManager bm;
		CatalogManager();//���캯�� 
		virtual ~CatalogManager();//��������
		
		int addIndex(string indexName,string tableName,string attributeName,int type);
		//������������Indexs�ļ�������һ��������Ϣ������table�ı���Ϣ����Ӧ����������������;����������������������Ӧ���ԣ��������� ;�ɹ�����1��ʧ�ܷ���0 
		
		int revokeIndexOnAttribute(string tableName,string AttributeName,string indexName);
		//���ñ���Ϣ��Ӧ����������ϢΪ���ַ���;����������������������ԣ��������֣�;�ҵ��������Է���1�����򷵻�0 
		
		int findTable(string tableName);
		//�����Ƿ��иñ����ı������������;û�з����㣬�з���TABLE_FILE ֵΪ9
		
		int findIndex(string indexName);
		//�����Ƿ��и�����������������;�������������;�Ҳ�������UNKNOWN_FILEֵΪ8���ҵ�����INDEX_FILE10
		
		int dropTable(string tableName);
		// ɾ�����ԭ�����ļ���������Ϣ�ļ����ļ������ڱ�������������������ɹ�����1�����򷵻�0 
		
		int dropIndex(string indexName);
		//ɾ��������ɾ��Indexs�ļ���Ӧ��������Ϣ�����ñ���Ϣ��Ӧ����������ϢΪ���ַ�����;����������������֣�;�ɹ�����1��ʧ�ܷ���0��
		
		int deleteValue(string tableName,int deleteNum);
		//ɾ����¼ʱ������Ϣ��¼��Ŀ��ȥɾ���ļ�¼��Ŀ;�������������ɾ����¼��Ŀ;�ɹ����ظñ�ɾ����ļ�¼��Ŀ�����򷵻�0 
		
		int insertRecord(string tableName,int addNum);
		//���Ӽ�¼ʱ������Ϣ��¼��Ŀ�������ӵļ�¼��Ŀ;����������������ӵļ�¼��Ŀ;�ɹ����ظñ����Ӻ�ļ�¼��Ŀ�����򷵻�0 
		
		int getRecordNum(string tableName);
		//��ȡĳ��ļ�¼��Ŀ;�������;�ɹ����ؼ�¼��Ŀ��ʧ�ܷ����� 
		
		int indexNameListGet(string tableName,vector<string>*indexVector);
		//��ȡָ��ĳ�������е�����(��Indexs�ļ���һ���������������������ҵ�����Ϊָ��������������);�������������������������Ž��);�ɹ�����1��ʧ�ܷ���0��
		
		int getAllIndex(vector<IndexInfo>*indexs);
		//��ȡIndexs�ļ�������������Ϣ������һ�����������������������������������ڴ�Ž��������1����ʾ����  
		
		int setIndexOnAttribute(string tableName,string AttributeName,string indexName);
		//���ñ���Ϣ����Ӧ����������;���������������������������;�ɹ��ҵ������������������� 1��ʧ�ܷ���0
		
		int addTable(string tableName,vector<Attribute>*attributeVector,string primaryKeyName,int primaryKeyLocation);
		//�½�һ������Ϣ�ļ��������Ϣ�����¼��Ŀ��ʼ��Ϊ0������λ�ã�������Ŀ�����Զ�����Ϣ ;������������������б�����������������ʼ��Ϊ���ַ�����������λ�ã���ʼ��Ϊ0;�ɹ�����1���񷵻�0
		
		int getIndexType(string indexName);
		//��ȡĳһ�����������ͣ���������������������������� ��-1,0����������-2��ʾ���� 
		
		int attributeGet(string tableName,vector<Attribute>*attributeVector);
		//���ĳһ��������������;���������������������(������ԣ�;�ɹ�����1��ʧ�ܷ���0
		
		int calcuteLenth(string tableName);
		//������һ����¼�Ĵ�С���ӱ���Ϣ�ļ�������;�����������;���ؼ�¼��С��ʧ�ܷ����� 
		
		int calcuteLenth2(int type);
		//�������ʹ�С;����������͵�����ֵ;������ʹ�С
		
		void recordStringGet(string tableName,vector<string>*recordContent,char*recordResult);
		//�Ѿ����һ�ȡ��¼���ݣ������recordContent���������¼��������һ���ַ��������ڽ���ַ�����;���������������¼����ָ����������һ��������ֵ��ɣ�������ַ���ָ��;�޷���
}; 

#endif

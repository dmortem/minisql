#ifndef minisql_IndexInfo_h
#define minisql_IndexInfo_h

#include<string>
using namespace std;

class IndexInfo//index��Ϣ�࣬��������������������������
{
	public:
		IndexInfo(string i,string t,string a,int ty)//���캯�� 
		{
			indexName = i;tableName = t;Attribute = a;type = ty;
		}
		string indexName;
        string tableName;
        string Attribute;
        int type;
};

#endif 

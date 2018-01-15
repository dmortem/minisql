#include"CatalogManager.h" 
#include"BufferManager.h"
#include"IndexInfo.h"
#include<iostream>
#include<vector>
#include<string>
#include<cstring>
#include<sstream>
#include"Attribute.h"
#define UNKNOWN_FILE 8
#define TABLE_FILE 9
#define INDEX_FILE 10

//���캯�� ����������ϵͳ�Զ�����
CatalogManager::CatalogManager()
{

}

CatalogManager::~CatalogManager()
{

}


// ɾ�����ԭ�����ļ����ļ������ڱ��� 
// �����������
//�ɹ�����1�����򷵻�0
int CatalogManager::dropTable(string tableName) 
{
	bm.delete_fileNode(tableName.c_str());//����BufferManagerɾ�����ļ��ڵ�
	if(remove(tableName.c_str()))//removeʧ�ܷ���-1 
	{
		return 0; 
	}
	return 1;
}


//��ȡĳһ������������
//�������������
//������������ ��-1,0��������;-2��ʾ����
int CatalogManager::getIndexType(string indexName) 
{
	fileNode*ftmp=bm.getFile("Indexs");//���Ҵ������������Ϣ���ļ�Indexs 
	blockNode*btmp=bm.getBlockHead(ftmp);//��ȡ�ļ��ڵ�ĵ�һ���ڵ�
	if(btmp){
		char*addressBegin;
		addressBegin=bm.get_content(*btmp);//ָ���ڵ����ݿ�ʼ�ĵط� 
		IndexInfo*i=(IndexInfo*)addressBegin;//��ʼ��ַת��ΪIndexInfo��ָ�븳ֵ 
		for(int j=0;j<(bm.get_usingSize(*btmp)/sizeof(IndexInfo));j++)//(bm.get_usingSize(*btmp)/sizeof(IndexInfo))��������
		{
			if((*i).indexName==indexName){
				return i->type;
			}
			i++;//ָ����һ��������Ϣ���� 
		} 
		return -2;//û�ҵ������� 
	}
	return -2;//btmpΪnull 
}


//��ȡIndexs�ļ�������������Ϣ������һ������������
//������������������������ڴ�Ž��
//����1����ʾ���� 
int CatalogManager::getAllIndex(vector<IndexInfo>*indexs) 
{
	fileNode*ftmp=bm.getFile("Indexs");
	blockNode*btmp=bm.getBlockHead(ftmp);
	if(btmp)
	{
		char*addressBegin;
		addressBegin=bm.get_content(*btmp);
		IndexInfo*i=(IndexInfo*)addressBegin;
		for(int j=0;j<(bm.get_usingSize(*btmp)/sizeof(IndexInfo));j++)
		{
			indexs->push_back((*i));//��vector��������Ϊ��vectorβ������һ�����ݡ�
			i++;//�Ƶ���һ�����������ȡ 			
		}
	} 
	return 1;
}


//������������Indexs�ļ�������һ��������Ϣ������table�ı���Ϣ����Ӧ����������������
//����������������������Ӧ���ԣ��������� 
//�ɹ�����1��ʧ�ܷ���0
int CatalogManager::addIndex(string indexName,string tableName,string Attribute,int type)
{
	fileNode*ftmp=bm.getFile("Indexs");
	blockNode*btmp=bm.getBlockHead(ftmp);
	IndexInfo i(indexName,tableName,Attribute,type);//��ʼ����ֵһ���������� 
	while(true)
	{
		if(btmp==NULL)
		{
			return 0;
		}
		if(bm.get_usingSize(*btmp)<=bm.getBlockSize()-sizeof(IndexInfo))
		{
			char*addressBegin;
			addressBegin=bm.get_content(*btmp)+bm.get_usingSize(*btmp);
			memcpy(addressBegin,&i,sizeof(IndexInfo));//����������������Ϣ��Indexs�ļ���λ�� 
			bm.set_usingSize(*btmp,bm.get_usingSize(*btmp)+sizeof(IndexInfo));
			bm.set_dirty(*btmp);
			
			return this->setIndexOnAttribute(tableName, Attribute, indexName);//��table�ı���Ϣ����Ӧ���������������� 
		}
		else//��һ���ڵ�ռ䲻��,�Ƶ���һ�� 
		{
			btmp=bm.getNextBlock(ftmp,btmp);			
		} 
	}
	return 0;
} 


//�����Ƿ��иñ����ı�;
//����������� 
//û�з����㣬�з���TABLE_FILE ֵΪ9
int CatalogManager::findTable(string tableName)
{
	FILE*fp;
	fp=fopen(tableName.c_str(),"r");//��ͼֻ�����ļ���Ϊ�������ļ� 
	if(fp==NULL)//�򲻿����ļ���˵��û�иñ�
	{
		return 0; 
	} 
	else//�ɹ��򿪸��ļ���˵������ 
	{
		fclose(fp);
		return TABLE_FILE;		
	} 
}



//�����Ƿ��и�����������������
//�������������
//�Ҳ�������UNKNOWN_FILEֵΪ8���ҵ�����INDEX_FILE10
int CatalogManager::findIndex(string fileName) 
{
	fileNode*ftmp=bm.getFile("Indexs");
	blockNode*btmp=bm.getBlockHead(ftmp);
	if(btmp)
	{
		char*addressBegin;
		addressBegin=bm.get_content(*btmp);
		IndexInfo*i=(IndexInfo*)addressBegin;
		int flag=UNKNOWN_FILE;//��־�ܷ��ҵ�������
		for(int j=0;j<(bm.get_usingSize(*btmp)/sizeof(IndexInfo));j++)
		{
			if((*i).indexName==fileName)//�ҵ������� 
			{
				flag=INDEX_FILE;
				break; 
			}
			i++; //�Ƶ���һ�������ļ����������� 
		}
		return flag; 
	}
}



//ɾ��������ɾ��Indexs�ļ���Ӧ��������Ϣ�����ñ���Ϣ��Ӧ����������ϢΪ���ַ�����
//����������������֣� 
//�ɹ�����1��ʧ�ܷ���0;
int CatalogManager::dropIndex(string indexName) 
{
	fileNode*ftmp=bm.getFile("Indexs");
	blockNode*btmp=bm.getBlockHead(ftmp);
	if(btmp)
	{
		char*addressBegin;
		addressBegin=bm.get_content(*btmp);
		IndexInfo*i=(IndexInfo*) addressBegin;
		int j;
		for(j=0;j<(bm.get_usingSize(*btmp)/sizeof(IndexInfo));j++)
		{
			if((*i).indexName==indexName)//�ҵ����������� 
			{
				break;
			}
			i++; 
		}
		this->revokeIndexOnAttribute((*i).tableName,(*i).Attribute,(*i).indexName);//���ñ���Ϣ��Ӧ����������ϢΪ���ַ���
		for(;j<(bm.get_usingSize(*btmp)/sizeof(IndexInfo)-1);j++)//��ɾ������������������������ǰ��һλ
		{
			//(*i)=*(i+sizeof(IndexInfo));
			(*i) = *(i + 1);
			i++;
		}
		bm.set_usingSize(*btmp, bm.get_usingSize(*btmp) - sizeof(IndexInfo));//��ʹ�ÿռ��ȥһ����������ռ�
        bm.set_dirty(*btmp);//��־Ϊ���

        return 1;		
	}
	return 0;
}


//���ñ���Ϣ��Ӧ����������ϢΪ���ַ���
//����������������������ԣ��������֣� 
//�ҵ��������Է���1�����򷵻�0 
int CatalogManager::revokeIndexOnAttribute(string tableName,string AttributeName,string indexName) 
{
	fileNode*ftmp=bm.getFile(tableName.c_str());
	blockNode*btmp=bm.getBlockHead(ftmp);
	if(btmp)//table��Ϣ��ͷ��size(int�ͣ���¼��Ŀ��������λ�� ��������Ŀ 
	{
		char*addressBegin=bm.get_content(*btmp);
		addressBegin+=(1+sizeof(int));//�Թ���¼��Ŀ��������Ŀ����
		int size = *addressBegin;//��ȡ������Ŀ
		addressBegin++;
		Attribute*a=(Attribute*)addressBegin;//ת��Ϊ����ָ��
		int i;
		for(i=0;i<size;i++)
		{
			if(a->name==AttributeName)
			{
				if(a->index==indexName)
				{
					a->index="";//������������Ϊ���ַ��� 
					bm.set_dirty(*btmp);//�޸ĺ�����Ϊ��� 
				}
				else//��������
				{
					cout<<"Revoke unknown index:"<<indexName<<" on "<<tableName<<"!"<<endl;
					cout<<"Attribute: "<<AttributeName<<" on table "<<tableName<<" has index: "<<a->index<<"!"<<endl; 
				} 
				break;
			}
			a++;//�Ƶ���һ������ 
		}
	if(i<size)//�ҵ�������
	   return 1; 
	else
	   return 0; 	 
	}
	return 0;
}


//��ȡָ��ĳ�������е�����(��Indexs�ļ���һ���������������������ҵ�����Ϊָ��������������) 
//�������������������������Ž��
//�ɹ�����1��ʧ�ܷ���0; 
int CatalogManager::indexNameListGet(string tableName,vector<string>*indexNameVector)
{
	fileNode*ftmp=bm.getFile("Indexs");
	blockNode*btmp=bm.getBlockHead(ftmp);
	if(btmp)
	{
		char*addressBegin;
		addressBegin=bm.get_content(*btmp);
		IndexInfo*i=(IndexInfo*)addressBegin;
		for(int j=0;j<(bm.get_usingSize(*btmp)/sizeof(IndexInfo));j++)
		{
			if((*i).tableName==tableName)
			{
				(*indexNameVector).push_back((*i).indexName);
			} 
			i++;//����һ���������� 
		}
		return 1;
	}
	return 0;
}



//ɾ����¼ʱ������Ϣ��¼��Ŀ��ȥɾ���ļ�¼��Ŀ
//�������������ɾ����¼��Ŀ
//�ɹ����ظñ�ɾ����ļ�¼��Ŀ�����򷵻�0 
int CatalogManager::deleteValue(string tableName,int deleteNum)
{
	fileNode*ftmp=bm.getFile(tableName.c_str());
	blockNode*btmp=bm.getBlockHead(ftmp);
	if(btmp)
	{
		char*addressBegin=bm.get_content(*btmp);
		int*recordNum=(int*)addressBegin;//��ȡ����Ϣ��һ�����ݼ�¼��Ŀ
		if((*recordNum)<deleteNum)//����� 
		{
			cout<<"Error in CatalogManager::deleteValue"<<endl;
			return 0;
		}
		else
		{
			(*recordNum)-=deleteNum; 
		} 
		bm.set_dirty(*btmp);//��־Ϊ��� 
		return *recordNum;
	}
	return 0; 
} 


//���Ӽ�¼ʱ������Ϣ��¼��Ŀ�������ӵļ�¼��Ŀ
//����������������ӵļ�¼��Ŀ
//�ɹ����ظñ����Ӻ�ļ�¼��Ŀ�����򷵻�0 
int CatalogManager::insertRecord(string tableName, int addNum)//���Ƽ��ټ�¼��Ŀ 
{
    fileNode *ftmp = bm.getFile(tableName.c_str());
    blockNode *btmp = bm.getBlockHead(ftmp);

    if (btmp)
    {

        char* addressBegin = bm.get_content(*btmp) ;
        int * RecordNum = (int*)addressBegin;
        *RecordNum += addNum;
        bm.set_dirty(*btmp);
        return *RecordNum;
    }
    return 0;
}


//��ȡĳ��ļ�¼��Ŀ
//�������
//�ɹ����ؼ�¼��Ŀ��ʧ�ܷ����� 
int CatalogManager::getRecordNum(string tableName)
{
    fileNode *ftmp = bm.getFile(tableName.c_str());
    blockNode *btmp = bm.getBlockHead(ftmp);

    if (btmp)
    {
        char* addressBegin = bm.get_content(*btmp) ;
        int * recordNum = (int*)addressBegin;//��ȡ����Ϣ��һ�����ݼ�¼��Ŀ
        return *recordNum;
    }
    return 0;
}


//�½�һ������Ϣ�ļ��������Ϣ�����¼��Ŀ��ʼ��Ϊ0������λ�ã�������Ŀ�����Զ�����Ϣ 
//������������������б�����������������ʼ��Ϊ���ַ�����������λ�ã���ʼ��Ϊ0��
//�ɹ�����1���񷵻�0
int CatalogManager::addTable(string tableName,vector<Attribute>*attributeVector,string primaryKeyName="",int primaryKeyLocation=0)
{
	FILE *fp;
    fp = fopen(tableName.c_str(), "w+");
    if (fp == NULL)
    {
        return 0;
    }
    fclose(fp);
    fileNode *ftmp = bm.getFile(tableName.c_str());
    blockNode *btmp = bm.getBlockHead(ftmp);
    if(btmp){
	   char*addressBegin=bm.get_content(*btmp);
	   int* size=(int*)addressBegin;
	   *size=0;// ���¼��Ŀ��ʼ��Ϊ0
	   addressBegin+=sizeof(int);
	   *addressBegin = primaryKeyLocation;
     	addressBegin++;
       *addressBegin = (*attributeVector).size();//��ȡ�����б��С ��������Ŀ
	   addressBegin++;
	   for(int i= 0;i<(*attributeVector).size();i++)//����������Ϣ 
        {
            memcpy(addressBegin, &((*attributeVector)[i]), sizeof(Attribute));
            addressBegin += sizeof(Attribute);
        }
        bm.set_usingSize(*btmp, bm.get_usingSize(*btmp) + (*attributeVector).size()*sizeof(Attribute)+2+sizeof(int));//������ʹ�ÿ�ڵ�ռ� 
        bm.set_dirty(*btmp);//����Ϊ��� 
        return 1;
   }
   return 0;
} 


//���ñ���Ϣ����Ӧ����������
//���������������������������
//�ɹ��ҵ������������������� 1��ʧ�ܷ���0
int CatalogManager::setIndexOnAttribute(string tableName,string AttributeName,string indexName) 
{
	fileNode*ftmp=bm.getFile(tableName.c_str());
	blockNode*btmp=bm.getBlockHead(ftmp);
	if(btmp)
	{
		char*addressBegin=bm.get_content(*btmp);
		addressBegin+=1+sizeof(int);
		int size =*addressBegin;//����λ��
		addressBegin++;
		Attribute*a=(Attribute*)addressBegin;
		int i;
		for(i=0;i<size;i++)
		{
			if(a->name==AttributeName)
			{
				a->index=indexName;
				bm.set_dirty(*btmp);
				break;
			}
			a++;
		}
		if(i<size)
		   return 1;
		else
		   return 0;
	}
	return 0;
}



//���ĳһ��������������
//���������������������(������ԣ�
//�ɹ�����1��ʧ�ܷ���0
int CatalogManager::attributeGet(string tableName,vector<Attribute>*attributeVector)
{
	fileNode*ftmp=bm.getFile(tableName.c_str());
	blockNode*btmp=bm.getBlockHead(ftmp);
	if(btmp)
	{
		char*addressBegin=bm.get_content(*btmp);
		addressBegin+=1+sizeof(int);
		int size=*addressBegin;//���������Ŀ 
		addressBegin++;
		Attribute*a=(Attribute*)addressBegin;
		for(int i=0;i<size;i++) 
		{
			attributeVector->push_back(*a);
			a++;
		}
		return 1; 
	}
	return 0; 
} 


//������һ����¼�Ĵ�С���ӱ���Ϣ�ļ������ã� 
//�����������
//���ؼ�¼��С��ʧ�ܷ����� 
int CatalogManager::calcuteLenth(string tableName)
{
	fileNode*ftmp=bm.getFile(tableName.c_str());
	blockNode*btmp=bm.getBlockHead(ftmp);
	if(btmp)
	{
		int singleRecordSize=0;//��ż�¼��С����ʼ��Ϊ0
		char*addressBegin=bm.get_content(*btmp);
		addressBegin+=1+sizeof(int) ;
		int size=*addressBegin;
		addressBegin++;
		Attribute*a=(Attribute*)addressBegin;
		for(int i=0;i<size;i++)
		{
			if((*a).type==-1)
		    {
		    	singleRecordSize+=sizeof(float);
		    }
		    else if((*a).type==0)
		    {
		    	singleRecordSize+=sizeof(int);
		    }
		    else if((*a).type>0)
		    {
		    	singleRecordSize+=(*a).type*sizeof(char);
		    }
		    else{//���� 
		    	cout<<"Catalog data damaged!"<<endl; 
		    	return 0;
		    }
		    a++; 
		}
		return singleRecordSize;
	}
	return 0;
}


//�������ʹ�С
//����������͵�����ֵ
//������ʹ�С
int CatalogManager::calcuteLenth2(int type)
{
	if(type==Attribute::TYPE_INT)
	{
		return sizeof(int);
	}
	else if(type==Attribute::TYPE_FLOAT)
	{
		return sizeof(float);
	}
	else if(type>0)
	{
		return (int)sizeof(char)*type;
	}
	else
	{
		cout<<"No this type"<<endl;
	}
}


//�Ѿ����һ�ȡ��¼���ݣ������recordContent���������¼��������һ���ַ��������ڽ���ַ����� 
//���������������¼����ָ����������һ��������ֵ��ɣ�������ַ���ָ��
//�޷���
void CatalogManager::recordStringGet(string tableName,vector<string>*recordContent,char*recordResult) 
{
	vector<Attribute>attributeVector;
	attributeGet(tableName,&attributeVector);//��ȡһ��������������б�����
	char* contentBegin=recordResult;//��֤recordResultһֱָ������ʼ����contentBegin�ƶ�
	for(int i=0;i<attributeVector.size();i++) //Ĭ��������Ŀ���¼������Ŀ��� 
	{
		Attribute attribute=attributeVector[i];
		string content =(*recordContent)[i];//��ȡһ������
		int type=attribute.type;
		int typeSize=calcuteLenth2(type);//�������ʹ�С�����ڿ�������ʱ 
		stringstream ss;
		ss<<content;
		if(type==Attribute::TYPE_INT)
		{
			int intTmp;
			ss>>intTmp;//����ֵ
			memcpy(contentBegin,((char*)&intTmp),typeSize);
		}
		else if (type == Attribute::TYPE_FLOAT)
        {
            float floatTmp;
            ss >> floatTmp;
            memcpy(contentBegin, ((char*)&floatTmp), typeSize);
        }
        else//Ϊ�ַ����Ϳ�ֱ�ӿ��� 
        {
            memcpy(contentBegin, content.c_str(), typeSize);
        }
		contentBegin+=typeSize;//�ƶ�����ַ�������һ�����Կ������ݵ�λ�� 
	}
	return; 	
}

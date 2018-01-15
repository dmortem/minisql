#include<iostream>
#include"RecordManager.h"
#include<cstring>
#include"API.h"

//����һ���������������
//��table�Ŀ��ļ�������1��ʾ�ɹ���0��ʾʧ��
int RecordManager::tableCreate(string tableName) 
{
	string tableFileName=tableFileNameGet(tableName);//���ô������table��table's file name������TABLE_FILE_**" ��**��tableName
	
	FILE *fp;
	fp=fopen(tableFileName.c_str(),"w+");//����һ���ű���ļ�
	//tableFileName��string���ͣ�tableFileName.c_str()ָ��tableFileName�ַ�������
	
	if(fp==NULL)
	{
		return 0;
	}
	fclose(fp);
	return 1;
  	
}


//ɾ��һ���������������
////��table���ļ�ɾ����1��ʾ�ɹ���0��ʾʧ��
int RecordManager::tableDrop(string tableName) //����BufferManager
{
	string tableFileName=tableFileNameGet(tableName);//��ȡ�ļ���
	bm.delete_fileNode(tableFileName.c_str());//BufferManager bm��delete the file node and its block node 
	if (remove(tableFileName.c_str()))	//remove����ɾ��ָ���ļ�,int  remove(const char *_Filename);����FilenameΪָ����Ҫɾ�����ļ������ɹ��򷵻�0��ʧ���򷵻�-1
    {
        return 0;
    }
	return 1;
} 


//����һ��������������������� 
//��index�Ŀ��ļ�������1��ʾ�ɹ���0��ʾʧ��
int RecordManager::indexCreate(string indexName)//���ƴ����� 
{
	string indexFileName = indexFileNameGet(indexName);//INDEX_FILE_"+indexName
	FILE *fp;
    fp = fopen(indexFileName.c_str(), "w+");
    if (fp == NULL)
    {
        return 0;
    }
    fclose(fp);
    return 1;
}


//ɾ��һ��������������������� 
//��index���ļ�ɾ����1��ʾ�ɹ���0��ʾʧ�� 
int RecordManager::indexDrop(string indexName)//����BufferManager 
{
	string indexFileName = indexFileNameGet(indexName);//��ȡ�ļ��� 
    bm.delete_fileNode(indexFileName.c_str()); //����BufferManagerɾ���ļ��ڵ� 
    if (remove(indexFileName.c_str()))//�����ļ� 
    {
        return 0;
    }
    return 1;
}


//���������һ����¼���������������ָ���¼���ݵ��ַ�ָ�룬��¼��С
//���ز����¼�Ŀ�ڵ��λ�á���ƫ����,-1��ʾ���� 
int RecordManager::recordInsert(string tableName,char*record,int recordSize)//����BufferManager 
{
	fileNode *ftmp = bm.getFile(tableFileNameGet(tableName).c_str());//�������tableName���ļ��ڵ�
    blockNode *btmp = bm.getBlockHead(ftmp);//��ȡ���ļ��ڵ��ͷ��� 
    while (true)
    {
        if (btmp == NULL)
        {
            return -1;
        }
        if (bm.get_usingSize(*btmp) <= bm.getBlockSize() - recordSize)//�㹻����һ����¼ 
        {
            
            char* addressBegin;
            addressBegin = bm.get_content(*btmp) + bm.get_usingSize(*btmp);//��ڵ㿪ʼָ��+��ʹ�õ�
            memcpy(addressBegin, record, recordSize);//������¼���� 
            bm.set_usingSize(*btmp, bm.get_usingSize(*btmp) + recordSize);//���¸ÿ�ڵ���ʹ�õĿռ�
            bm.set_dirty(*btmp);//���Ϊ��ڵ㣬Ҫ����д���ļ�
            return btmp->offsetNum;//���ظýڵ�λ�� 
        }
        else
        {
            btmp = bm.getNextBlock(ftmp, btmp); //����һ�����¼����������һ����ڵ� 
        }
    }
    
    return -1;
} 

//��ʾ����������������ļ�¼ �������������б�������Ӧ��������
//�������������������ʾ�����б� �������б����� 
//�������������ļ�¼��Ŀ,-1��ʾ���� 
int RecordManager::recordAllShow(string tableName, vector<string>* attributeNameVector,  vector<Condition>* conditionVector)//����BufferManager����һ����ڵ� 
{
	fileNode *ftmp = bm.getFile(tableFileNameGet(tableName).c_str());
    blockNode *btmp = bm.getBlockHead(ftmp);
    int count = 0;//countΪ���п��recordblocknum֮��
    while (true)
    {
        if (btmp == NULL)
        {
            return -1;
        }
        if (btmp->ifbottom)//����ļ����һ��
        {
            int recordBlockNum = recordBlockShow(tableName,attributeNameVector, conditionVector, btmp);//btmp-��ڵ㣬����ڵ���ʾ 
            count += recordBlockNum;
            return count;
        }
        else//�����ڵ�Ҫ�Ƶ���һ��
        {
            int recordBlockNum = recordBlockShow(tableName, attributeNameVector, conditionVector, btmp);
            count += recordBlockNum;
            btmp = bm.getNextBlock(ftmp, btmp);
        }
    }
    
    return -1;
} 

//ָ��ƫ�����Ŀ�ڵ����������ļ�¼��Ӧ��ʾ
//�������������������ʾ�����б� �������б����� ����ڵ�λ��ƫ����
//�������������ļ�¼��Ŀ,-1��ʾ���� 
int RecordManager::recordBlockShow(string tableName, vector<string>* attributeNameVector, vector<Condition>* conditionVector, int blockOffset)//��ʾ�ض�ƫ�ƵĿ�ڵ�����з��ϼ�¼
{
    fileNode *ftmp = bm.getFile(tableFileNameGet(tableName).c_str());
    blockNode* block = bm.getBlockByOffset(ftmp, blockOffset);//���ƫ��ΪblockOffset�Ŀ�ڵ�
    if (block == NULL)
    {
        return -1;
    }
    else
    {
        return  recordBlockShow(tableName, attributeNameVector, conditionVector, block);//����˽�к��� 
    }
}


//�����ڵ����������ļ�¼��Ӧ��ʾ
//�������������������ʾ�����б� �������б����� ����ڵ�ָ�� 
//�������������ļ�¼��Ŀ,-1��ʾ����  
int RecordManager::recordBlockShow(string tableName, vector<string>* attributeNameVector, vector<Condition>* conditionVector, blockNode* block)
{
	//if block is null, return -1
    if (block == NULL)
    {
        return -1;
    }
    
    int count = 0;
    
    char* recordBegin = bm.get_content(*block);//��ʼ����blockBegin
    vector<Attribute> attributeVector;
    int recordSize = api->recordSizeGet(tableName);//���һ����¼�Ĵ�С

    api->attributeGet(tableName, &attributeVector);//���������︳ֵ������������� 
    char* blockBegin = bm.get_content(*block);//ָ��ָ��block�����ݿ�ʼ�ĵط� 
    size_t usingSize = bm.get_usingSize(*block);
    
    while (recordBegin - blockBegin  < usingSize)//ÿ��recordBegin���Ƶ���һ����¼��ʼ 
    {
        //if the recordBegin point to a record
        
        if(recordConditionFit(recordBegin, recordSize, &attributeVector, conditionVector))//�Ƿ���������
        {
            count ++;
            recordPrint(recordBegin, recordSize, &attributeVector, attributeNameVector);//attributeVector��һ������������ԣ�attributeNameVector�����Ƚϵ�������
            printf("\n");
        }
        
        recordBegin += recordSize;
    }
    
    return count;
	
} 


//������������������ļ�¼���������ʾ
//������������������б����� 
//�������������ļ�¼��Ŀ��-1�������
int RecordManager::recordAllFind(string tableName, vector<Condition>* conditionVector)//���������ʾ 
{
    fileNode *ftmp = bm.getFile(tableFileNameGet(tableName).c_str());
    blockNode *btmp = bm.getBlockHead(ftmp);
    int count = 0;
    while (true)
    {
        if (btmp == NULL)
        {
            return -1;
        }
        if (btmp->ifbottom)
        {
            int recordBlockNum = recordBlockFind(tableName, conditionVector, btmp);
            count += recordBlockNum;
            return count;
        }
        else
        {
            int recordBlockNum = recordBlockFind(tableName, conditionVector, btmp);
            count += recordBlockNum;
            btmp = bm.getNextBlock(ftmp, btmp);
        }
    }
    
    return -1;
}


//���ҵ����ڵ�����������ļ�¼���������ʾ
//������������������б���������ڵ�ָ�� 
//�������������ļ�¼��Ŀ��-1�������
int RecordManager::recordBlockFind(string tableName, vector<Condition>* conditionVector, blockNode* block)
{
    //if block is null, return -1
    if (block == NULL)
    {
        return -1;
    }
    int count = 0;
    
    char* recordBegin = bm.get_content(*block);
    vector<Attribute> attributeVector;
    int recordSize = api->recordSizeGet(tableName);
    
    api->attributeGet(tableName, &attributeVector);
    
    while (recordBegin - bm.get_content(*block)  < bm.get_usingSize(*block))
    {
        //if the recordBegin point to a record
        
        if(recordConditionFit(recordBegin, recordSize, &attributeVector, conditionVector))
        {
            count++;
        }
        
        recordBegin += recordSize;
        
    }
    
    return count;
}


//ɾ���������������ļ�¼
//������������������б����� 
//������������ļ�¼������-1�������
int RecordManager::recordAllDelete(string tableName, vector<Condition>* conditionVector)
{
    fileNode *ftmp = bm.getFile(tableFileNameGet(tableName).c_str());
    blockNode *btmp = bm.getBlockHead(ftmp);

    int count = 0;
    while (true)
    {
        if (btmp == NULL)
        {
            return -1;
        }
        if (btmp->ifbottom)
        {
            int recordBlockNum = recordBlockDelete(tableName, conditionVector, btmp);//�ֽ��ɾ�������ڵ����������ļ�¼ 
            count += recordBlockNum;
            return count;
        }
        else
        {
            int recordBlockNum = recordBlockDelete(tableName, conditionVector, btmp);
            count += recordBlockNum;
            btmp = bm.getNextBlock(ftmp, btmp);
        }
    }
    
    return -1;
}


//ɾ��ָ��ƫ������ڵ����������ļ�¼
//������������������б����� ����ڵ�ƫ���� 
//������������ļ�¼������-1�������
int RecordManager::recordBlockDelete(string tableName,  vector<Condition>* conditionVector, int blockOffset)
{
    fileNode *ftmp = bm.getFile(tableFileNameGet(tableName).c_str());
    blockNode* block = bm.getBlockByOffset(ftmp, blockOffset);
    if (block == NULL)
    {
        return -1;
    }
    else
    {
        return  recordBlockDelete(tableName, conditionVector, block);
    }
}


//ɾ��ĳ��ڵ����������ļ�¼
//������������������б����� ����ڵ�ָ��
//������������ļ�¼������-1�������
int RecordManager::recordBlockDelete(string tableName,  vector<Condition>* conditionVector, blockNode* block)//����ɾ����¼�����Ļ��� 
{
    //if block is null, return -1
    if (block == NULL)
    {
        return -1;
    }
    int count = 0;
    
    char* recordBegin = bm.get_content(*block);
    vector<Attribute> attributeVector;
    int recordSize = api->recordSizeGet(tableName);
    
    api->attributeGet(tableName, &attributeVector);
    
    while (recordBegin - bm.get_content(*block) < bm.get_usingSize(*block))
    {
        //if the recordBegin point to a record
        
        if(recordConditionFit(recordBegin, recordSize, &attributeVector, conditionVector))
        {
            count ++;
            
            api->recordIndexDelete(recordBegin, recordSize, &attributeVector, block->offsetNum);//ɾ����Ӧ����ֵ
            int i = 0;
            for (i = 0; i + recordSize + recordBegin - bm.get_content(*block) < bm.get_usingSize(*block); i++)
            {
                recordBegin[i] = recordBegin[i + recordSize];//ɾ�����ʱrecordbegin������м�¼��ǰŲһ��
            }
            memset(recordBegin + i, 0, recordSize);//�����һ����Ϊ��
			//void *memset(void *s,int c,size_t n)���ѿ����ڴ�ռ� s ���� n ���ֽڵ�ֵ��Ϊֵ c
            bm.set_usingSize(*block, bm.get_usingSize(*block) - recordSize);
            bm.set_dirty(*block);//���Ϊ���
        }
        else
        {
            recordBegin += recordSize;
        }
    }
    
    return count;
}


//��Ա�����м�¼����ĳһ����
//������������������� 
//������������ļ�¼������-1�������
int RecordManager::indexRecordAllAlreadyInsert(string tableName,string indexName)//����BufferManager 
{
    fileNode *ftmp = bm.getFile(tableFileNameGet(tableName).c_str());
    blockNode *btmp = bm.getBlockHead(ftmp);
    int count = 0;
    while (true)
    {
        if (btmp == NULL)
        {
            return -1;
        }
        if (btmp->ifbottom)
        {
            int recordBlockNum = indexRecordBlockAlreadyInsert(tableName, indexName, btmp);//������ڵ㽨������ 
            count += recordBlockNum;
            return count;
        }
        else
        {
            int recordBlockNum = indexRecordBlockAlreadyInsert(tableName, indexName, btmp);
            count += recordBlockNum;
            btmp = bm.getNextBlock(ftmp, btmp);
        }
    }
    
    return -1;
}



//���ĳһ��ڵ�ļ�¼����ĳһ����
//������������������� ����ڵ�ָ�� 
//������������ļ�¼������-1�������
int RecordManager::indexRecordBlockAlreadyInsert(string tableName,string indexName,  blockNode* block)//��һ����ڵ������Ϊ**��ÿ����¼����Ƿ���Ҫ����indexName������
{
    //if block is null, return -1
    if (block == NULL)
    {
        return -1;
    }
    int count = 0;
    
    char* recordBegin = bm.get_content(*block);
    vector<Attribute> attributeVector;
    int recordSize = api->recordSizeGet(tableName);
    
    api->attributeGet(tableName, &attributeVector);
    
    int type;
    int typeSize;
    char * contentBegin;
    
    while (recordBegin - bm.get_content(*block)  < bm.get_usingSize(*block))//һ����¼һ����¼����
    {
        contentBegin = recordBegin;
        //if the recordBegin point to a record
        for (int i = 0; i < attributeVector.size(); i++)//��һ����¼��ÿ�����Բ���
        {
            type = attributeVector[i].type;
            typeSize = api->typeSizeGet(type);
            
            //find the index  of the record, and insert it to index tree
            if (attributeVector[i].index == indexName)//default value is "", representing no index
            {
                api->indexInsert(indexName, contentBegin, type, block->offsetNum);
                count++;
            }
            
            contentBegin += typeSize;//��һ������
        }
        recordBegin += recordSize;//��һ����¼
    }
    
    return count;
}


//�ж�ĳ����¼�Ƿ���������
//���������¼��ʼ��ַ����¼��С ����¼���������������б������������б�
//���boolֵ��tru�������㣬false����������
bool RecordManager::recordConditionFit(char* recordBegin,int recordSize, vector<Attribute>* attributeVector,vector<Condition>* conditionVector)
{
    if (conditionVector == NULL) {
        return true;
    }
    int type;
    string attributeName;
    int typeSize;
    char content[255];
    
    char *contentBegin = recordBegin;
    for(int i = 0; i < attributeVector->size(); i++)//һ��һ�����Բ���
    {
        type = (*attributeVector)[i].type;//-1��0��������
        attributeName = (*attributeVector)[i].name;
        typeSize = api->typeSizeGet(type);
        
        //init content (when content is string , we can get a string easily)
        memset(content, 0, 255);
        memcpy(content, contentBegin, typeSize);//������contentBegin��ʼ�ĵ�ַ��һ������ֵ
        for(int j = 0; j < (*conditionVector).size(); j++)//��ÿһ������һ������һ�������ж�
        {
            if ((*conditionVector)[j].attributeName == attributeName)
            {
                //if this attribute need to deal about the condition
                if(!contentConditionFit(content, type, &(*conditionVector)[j]))//����condition->ifRight(tmp)�ж�
                {
                    //if this record is not fit the conditon
                    return false;
                }
            }
        }

        contentBegin += typeSize;
    }
    return true;
}



//���һ����¼��Ϣ
//���������¼��ʼ��ַ����¼��С����¼�����б�������������б�
// �޷���ֵ
void RecordManager::recordPrint(char* recordBegin, int recordSize, vector<Attribute>* attributeVector, vector<string> *attributeNameVector)
{
    int type;
    string attributeName;
    int typeSize;
    char content[255];
    
    char *contentBegin = recordBegin;
    for(int i = 0; i < attributeVector->size(); i++)//������Ա�����һ����������
    {
        type = (*attributeVector)[i].type;
        typeSize = api->typeSizeGet(type);//ÿ�����͹̶���С
        
        //init content (when content is string , we can get a string easily)
        memset(content, 0, 255);
        
        memcpy(content, contentBegin, typeSize);//��ȡ��ַ��contentBegin��ʼ��һ������

        for(int j = 0; j < (*attributeNameVector).size(); j++)//attributeVector��һ������������ԣ�attributeNameVector���û����ȥ��������
        {
            if ((*attributeNameVector)[j] == (*attributeVector)[i].name)
            {
                contentPrint(content, type);//type��һ����ͬ���ʱif�����ж�����
                break;
            }
        }
        
        contentBegin += typeSize;
    }
} 


//ĳһ����������� 
//�����������������ָ�룬��������
//�޷��� 
//�ַ�����Ӧ����ת�����ʽ�����
void RecordManager::contentPrint(char * content, int type)
{
    if (type == Attribute::TYPE_INT)
    {
        //if the content is a int
        int tmp = *((int *) content);   //get content value by point
        printf("%d ", tmp);
    }
    else if (type == Attribute::TYPE_FLOAT)
    {
        //if the content is a float
        float tmp = *((float *) content);   //get content value by point
        printf("%f ", tmp);
    }
    else
    {
        //if the content is a string
        string tmp = content;
        printf("%s ", tmp.c_str());
    }

}


//�ж�ĳһ���������Ƿ�����ĳһ����
//���������������ָ�룬�������� ����������ָ��
//����boolֵ��true�������㣬false����������
bool RecordManager::contentConditionFit(char* content,int type,Condition* condition)//����Condition��ifRight���� 
{
    if (type == Attribute::TYPE_INT)
    {
        //if the content is a int
        int tmp = *((int *) content);   //get content value by point
        return condition->ifRight(tmp);
    }
    else if (type == Attribute::TYPE_FLOAT)
    {
        //if the content is a float
        float tmp = *((float *) content);   //get content value by point
        return condition->ifRight(tmp);
    }
    else
    {
        //if the content is a string
        return condition->ifRight(content);
    }
    return true;
}



//���һ�������ļ����ļ��� 
//��������������� 
string RecordManager::indexFileNameGet(string indexName)
{
    return "INDEX_FILE_"+indexName;
} 



//���һ�����ļ����ļ��� 
//�������������
string RecordManager::tableFileNameGet(string tableName)
{
    return "TABLE_FILE_" + tableName;
}
 


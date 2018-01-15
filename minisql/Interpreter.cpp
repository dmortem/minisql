#include"Interpreter.h" 
#include"Condition.h"
#include"Attribute.h"
#include<string>
#include<string.h>
#include<iostream>
using namespace std;

class SyntaxException{};//�쳣��

//���ַ����������ɻ�����ʶ�����Ϣ,��������Ӧ�������д���
//����������ַ���
//������0,�ɹ�����1
int Interpreter::interpreter(string s)
{
	int tmp=0;//��¼�±� 
	string word;//���һ������
	
	word=getWord(s,&tmp);//���ַ����±�Ϊtmp��λ�ÿ�ʼ��ȡһ������,tmp�±���� 
	
	if(strcmp(word.c_str(),"create")==0)//�����create��� 
	{
		word=getWord(s,&tmp);//��ȡ��һ������ 
		
		if(strcmp(word.c_str(),"table")==0)//�����create table 
		{
		    string primaryKey="";//���� 
			string tableName="";//����
			word=getWord(s,&tmp);//��ȡ��һ������
			if(!word.empty())
				tableName=word;
			else
			{
				cout<<"Syntax Error for no table name"<<endl;//����,û�б��� 
				return 0;
			}
			
			word=getWord(s,&tmp);//��ȡ��һ������ 
			if(word.empty()||strcmp(word.c_str(),"(")!=0)//�﷨���� 
			{
				cout<<"Error in syntax!"<<endl;
				return 0;
			}
			else
			{
				word=getWord(s,&tmp);
				std::vector<Attribute> attributeVector;
				while(!word.empty()&&strcmp(word.c_str(),"primary")!=0&&strcmp(word.c_str(),")")!=0)
				{
				    string attributeName=word;//��ȡ������
					int type=0;//����Ĭ��Ϊint��
					bool ifUnique=false;//Ĭ��Ϊ����Unique 
					word=getWord(s,&tmp);
					if(strcmp(word.c_str(),"int")==0)
						type=0;
					else if(strcmp(word.c_str(),"float")==0)
						type=-1;
					else if(strcmp(word.c_str(),"char")==0)
					{
						word=getWord(s,&tmp);
						if(strcmp(word.c_str(),"("))
						{
							cout<<"Syntax Error:unknown data type"<<endl;
							return 0;
						}
						word=getWord(s,&tmp);
						istringstream convert(word);
						//�� istringstream �� string ����,�������½���ֵ�������һ�������ȡ istringstream �����Զ��ؽ���ֵ�����ݵ��ַ���ʾ��ʽת��Ϊ��Ӧ������ֵ
						if(!(convert>>type))//����type����,ʧ�ܱ��� 
						{
							cout<<"Syntax error:illegal number in char()"<<endl;
							return 0;
						}		
						word = getWord(s, &tmp);
						if (strcmp(word.c_str(), ")"))
						{
							cout << "Syntax Error: unknown data type" << endl;
							return 0;
						}
					}
					else//�������ͱ��� 
					{
						cout<<"Syntax Error:unknown or missing data type!"<<endl;
						return 0;
					} 
					word=getWord(s,&tmp);
					if(strcmp(word.c_str(),"unique")==0)//��Ϊunique
					{
						ifUnique=true;
						word=getWord(s,&tmp);//��ȡ��ĩ���� 
					}
					Attribute attr(attributeName,type,ifUnique);//��ʼ������һ�����Զ���
					attributeVector.push_back(attr);//�����Զ�������������� 
					if(strcmp(word.c_str(),",")!=0) 
					{
						if(strcmp(word.c_str(),")")!=0)
						{
							cout<<"Syntax Error for ,!"<<endl;
							return 0;
						}
						else break;			
					}
					word=getWord(s,&tmp);
				}
				int primaryKeyLocation = 0;
				if(strcmp(word.c_str(),"primary")==0)//����primaryKey
				{
					word=getWord(s,&tmp);
					if(strcmp(word.c_str(),"key")!=0)
					{
						cout<<"Error in syntax!"<<endl;
						return 0;
					}
					else
					{
						word=getWord(s,&tmp);
						if(strcmp(word.c_str(),"(")==0)
						{
							word=getWord(s,&tmp);
							primaryKey=word;
							int i=0;
							for(int i=0;i<attributeVector.size();i++)
							{
								if(primaryKey==attributeVector[i].name)//��������Ϊ����,������Ϊunique
								{
									attributeVector[i].ifUnique=true;
									break;//��¼����λ��
								}
							}
							if(i==attributeVector.size())//�Ҳ�����������
							{
								cout<<"Syntax Error: primaryKey does not exist in attributes"<<endl;
								return 0;
							}
							primaryKeyLocation=i;
							word=getWord(s,&tmp);
							if(strcmp(word.c_str(),")")!=0)
							{
								cout<<"Error in syntax!"<<endl;
								return 0;
							}
						}
						else
						{
							cout<<"Error in syntax!"<<endl;
							return 0;
						}
						word=getWord(s,&tmp);
						if(strcmp(word.c_str(),")")!=0)//primaryKey�Ļ�����
						{
							cout<<"Error in syntax!"<<endl;
							return 0;
						}
					}

				}
				else if(word.empty())
				{
					cout<<"Syntax Error:')' absent!"<<endl;
					return 0;
				}
				else if(primaryKeyLocation==-1)
				{
					cout<<"Syntax Error:no primary key!"<<endl;
					return 0;
				}

				ap->tableCreate(tableName,&attributeVector,primaryKey,primaryKeyLocation);//��ȡ��Ӧ��Ϣ�����API����������
				return 1;
			} 
		}
		else if(strcmp(word.c_str(),"index")==0)
		{
			string indexName="";
			string tableName="";
			string attributeName="";
			word=getWord(s,&tmp);
		    if(!word.empty())indexName=word;
			else
			{
				cout<<"Error in syntax!"<<endl;
				return 0;
			}
			word=getWord(s,&tmp);
			try//���쳣�����
			{
				if(strcmp(word.c_str(),"on")!=0)
					throw SyntaxException();//�׳��쳣
				word = getWord(s, &tmp);
				if(word.empty())
					throw SyntaxException();
				tableName=word;
				word=getWord(s,&tmp);
				if(strcmp(word.c_str(),"(")!=0)
					throw SyntaxException();
				word = getWord(s, &tmp);
				if(word.empty())
					throw SyntaxException();
				attributeName=word;
				word=getWord(s,&tmp);
				if(strcmp(word.c_str(),")")!=0)
					throw SyntaxException();
				ap->indexCreate(indexName,tableName,attributeName);//����API������������
				return 1;
			}
			catch(SyntaxException&)
			{
				cout<<"Syntax Error!"<<endl;
				return 0;
			}		
		}
		else//create������������
		{
			cout<<"Syntax Error for "<<word<<endl;
			return 0;
		}
		return 0;
	}//create��䲿��

	else if(strcmp(word.c_str(),"select")==0)//�����select���
	{
		vector<string>attrSelected;//��select������
		string tableName="";
		word=getWord(s,&tmp);
		if(strcmp(word.c_str(),"*")!=0)//һ��select���
		{
			while(strcmp(word.c_str(),"from")!=0&&strcmp(word.c_str(),";")!=0)
			{
				attrSelected.push_back(word);
				word=getWord(s,&tmp);
			}
			if ((word.c_str(),";")==0)
			{
				cout<<"Syntax Error for no 'from'!"<<endl;
			    return 0;
			}
		}
		else//select*���
		{
			word=getWord(s,&tmp);//��ȡ*����һ������
		}
		if(strcmp(word.c_str(),"from")!=0)
		{
			cout<<"Error in syntax!"<<endl;
			return 0;
		}
		word=getWord(s,&tmp);
		if(!word.empty())tableName=word;
		else
		{
			cout<<"Error in syntax!"<<endl;
			return 0;
		}
		word=getWord(s,&tmp);
		if (word.empty())//�󲻸�where����
		{
			if(attrSelected.size()==0){//select*�����ʾ��������Ϣ
				ap->recordShow(tableName);//����API������ʾ
			}
			else ap->recordShow(tableName,&attrSelected);//��ʾ���й����������������Ե���Ϣ
			return 1;
		}
		else if(strcmp(word.c_str(),"where")==0)//���where����
		{
			string attributeName="";
			string value="";//���Ƚ�ֵ
			int operate= Condition::OPERATOR_EQUAL;
			std::vector<Condition> conditionVector;//Condition�����б�
			word=getWord(s,&tmp);
			while(1)//֧��and���Ӷ�������ѯ
			{
				try
				{
					if(word.empty())
						throw SyntaxException();
					attributeName=word;
					word=getWord(s,&tmp);
					if (strcmp(word.c_str(),"<=") == 0)
						operate = Condition::OPERATOR_LESS_EQUAL;
					else if (strcmp(word.c_str(),">=") == 0)
						operate = Condition::OPERATOR_MORE_EQUAL;
					else if (strcmp(word.c_str(),"<") == 0)
						operate = Condition::OPERATOR_LESS;
					else if (strcmp(word.c_str(),">") == 0)
						operate = Condition::OPERATOR_MORE;
					else if (strcmp(word.c_str(),"=") == 0)
						operate = Condition::OPERATOR_EQUAL;
					else if (strcmp(word.c_str(),"<>") == 0)
						operate = Condition::OPERATOR_NOT_EQUAL;
					else
						throw SyntaxException();
					word = getWord(s,&tmp);
					if(word.empty())
						throw SyntaxException();
					value = word;//���Ƚ�ֵ
					Condition c(attributeName,value,operate);//��ʼ��Condition����
					conditionVector.push_back(c);//Condition����洢��Condition������
					word = getWord(s,&tmp);
					if(word.empty())//���ٸ�����,��������ѯ
						break;
                    if (strcmp(word.c_str(),"and") != 0)//ֻ֧��and��������������ѯ
						throw SyntaxException();
					word = getWord(s,&tmp);//��ȡ��һ������
				}
				catch(SyntaxException&)
				{
					cout<<"Syntax Error!"<<endl;
					return 0;
				}
			}
			if(attrSelected.size()==0)//select*������ѯ
				ap->recordShow(tableName,NULL,&conditionVector);
			else
				ap->recordShow(tableName, &attrSelected,&conditionVector);//�ض�����select������ѯ
			return 1;
		}
	}//select��䲿��

	else if(strcmp(word.c_str(), "drop")==0)//�����drop���
	{
		word = getWord(s,&tmp);
        if (strcmp(word.c_str(), "table") == 0)//drop table���
		{
			word = getWord(s,&tmp);
			if (!word.empty())
			{
				ap->tableDrop(word);//����API����ɾ����
				return 1;
			}
			else
			{
				cout<<"Syntax Error!"<<endl;
				return 0;
			}
		}

		else if(strcmp(word.c_str(), "index") == 0)//ɾ������
		{
			word = getWord(s,&tmp);
			if (!word.empty())
			{
				ap->indexDrop(word);//����API����ɾ������
				return 1;
			}
			else
			{
				cout<<"Error in syntax!"<<endl;
				return 1;
			}
		}
		else//drop���ֻ֧��drop�������
		{
			cout<<"Error in syntax!"<<endl;
			return 0;
		}       
	}//drop��䲿��

	else if(strcmp(word.c_str(), "delete")==0)
	{
		string tableName = "";
		word = getWord(s,&tmp);
		if (strcmp(word.c_str(), "from") != 0)//delete from...��ʽ
		{
			cout<<"Error in syntax!"<<endl;
			return 0;
		}
		word = getWord(s,&tmp);
		if (!word.empty())
			tableName = word;
		else
		{
			cout<<"Error in syntax!"<<endl;
			return 0;
		}

		word = getWord(s,&tmp);
		if (word.empty())//�󲻸�����,ɾ��������м�¼
		{
			ap->recordDelete(tableName);//����API����
			return 1;
		}
		else if (strcmp(word.c_str(),"where") == 0)//���where����
		{
			string attributeName = "";
			string value = "";
			int operate = Condition::OPERATOR_EQUAL;
			std::vector<Condition> conditionVector;
			word = getWord(s,&tmp);	
			while(1){
				try {
					if(word.empty())
						throw SyntaxException();
					attributeName = word ;
					word = getWord(s,&tmp);
					if (strcmp(word.c_str(),"<=") == 0)
						operate = Condition::OPERATOR_LESS_EQUAL;
					else if (strcmp(word.c_str(),">=") == 0)
						operate = Condition::OPERATOR_MORE_EQUAL;
					else if (strcmp(word.c_str(),"<") == 0)
						operate = Condition::OPERATOR_LESS;
					else if (strcmp(word.c_str(),">") == 0)
						operate = Condition::OPERATOR_MORE;
					else if (strcmp(word.c_str(),"=") == 0)
						operate = Condition::OPERATOR_EQUAL;
					else if (strcmp(word.c_str(),"<>") == 0)
						operate = Condition::OPERATOR_NOT_EQUAL;
					else
						throw SyntaxException();
					word = getWord(s,&tmp);
					if(word.empty())
						throw SyntaxException();
					value = word;//���Ƚ�ֵ
					word = getWord(s, &tmp);
					Condition c(attributeName,value,operate);
					conditionVector.push_back(c);//���������������������
					if(word.empty()) //�������Ƚ�
						break;
					if (strcmp(word.c_str(),"and") != 0)//ֻ֧��and����
						throw SyntaxException();
					word = getWord(s,&tmp);
				  }
				//��׽�쳣
				catch(SyntaxException&)
				{
					cout<<"Syntax Error!"<<endl;
					return 0;
				}
		      }
			ap->recordDelete(tableName, &conditionVector);//����API����ɾ�����������ļ�¼
			return 1;
		   }
	}//delete��䲿��

	else if(strcmp(word.c_str(), "insert") == 0)//�����insert���
	{
		string tableName = "";
		std::vector<string> valueVector;//����ֵ����
		word = getWord(s,&tmp);
		try 
		{
			if (strcmp(word.c_str(),"into") != 0)
				throw SyntaxException();
			word = getWord(s,&tmp);
			if (word.empty())
				throw SyntaxException();
			tableName = word;
			word = getWord(s,&tmp);
			if (strcmp(word.c_str(),"values") != 0)
				throw SyntaxException();
			word = getWord(s,&tmp);
			if (strcmp(word.c_str(),"(") != 0)
				throw SyntaxException();
			word = getWord(s,&tmp);
			while (!word.empty() && strcmp(word.c_str(),")") != 0)//��ȡ����ֵ
			{
				valueVector.push_back(word);
				word = getWord(s,&tmp);
				if (strcmp(word.c_str(),",") == 0)//������,����
					word = getWord(s,&tmp);
			}
			if (strcmp(word.c_str(),")") != 0)//����������
				throw SyntaxException();
		}
		catch(SyntaxException&)
		{
			cout<<"Syntax Error!"<<endl;
			return 0;
		}
		ap->recordInsert(tableName,&valueVector);//����API��������һ����¼
		return 1;
	}//insert��䲿��

	else if (strcmp(word.c_str(), "quit") == 0)
	{ return 587;}

	else if (strcmp(word.c_str(), "execfile") == 0)
	{
		fileName = getWord(s,&tmp);
		cout<<"try to open "<<fileName<<"..."<<endl;
		return 2;
	}
	else
	{
		if(word != "")
			cout<<"Error, command "<<word<<" not found"<<endl;
		return 0;
	}
	return 0;

} 


//��ȡ�ַ�����ָ���±꿪ʼ��һ������
//��������ַ���s,int��ָ��ָ���±�
//����һ���ַ�������word
string Interpreter::getWord(string s,int* tmp)//*tmp���ű���sһֱ�ڸı�
{
	string word;//�洢���
	int idx1,idx2;//word��ʼ�±�,��ֹ�ĺ�һ���±�

	while((s[*tmp] == ' ' || s[*tmp] == 10  || s[*tmp] == '\t') && s[*tmp] != 0)//ǰ�������������word��ʼλ��,s[*tmp] == 10,���м�
	{
		(*tmp)++;//tmp����,����s��һ�±��Ӧֵ
	}
    idx1 = *tmp;

	if (s[*tmp] == '(' || s[*tmp] == ',' || s[*tmp] == ')')//���ⵥ�����š����Ŵ���
	{
		(*tmp)++;
		idx2 = *tmp;
		word = s.substr(idx1,idx2-idx1);//��ȡ�����ַ���
		return word;
	}
	else if (s[*tmp] == 39)//ASCII��39����'(������)
	{
		(*tmp)++;
		while (s[*tmp] != 39 && s[*tmp] !=0)
			(*tmp)++;
		if (s[*tmp] == 39)
		{
			idx1++;//�Թ�����¼���ʿ�ʼ�±�
			idx2 = *tmp;
			(*tmp)++;
			word = s.substr(idx1,idx2-idx1);
			return word;
		}
		else
		{
			word = "";
			return word;
		}
	}
	else
	{
		while (s[*tmp] != ' ' &&s[*tmp] != '(' && s[*tmp] != 10 && s[*tmp] != 0 && s[*tmp] != ')' && s[*tmp] != ',')//һ������������ȡ����ĩβ��Ӧ�±�
			(*tmp)++;
		idx2 = *tmp;
		if (idx1 != idx2)
			word = s.substr(idx1,idx2-idx1);
		else
			word = "";

		return word;
	}
}

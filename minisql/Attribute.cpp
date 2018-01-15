#include "Attribute.h" 

Attribute::Attribute(string n,int t, bool i)//属性类构造函数实现 
{
	name = n;
	type = t;
	ifUnique = i;
	index = ""; 
}

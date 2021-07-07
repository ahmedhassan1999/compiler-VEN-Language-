#include"Hashtable.h"
HashTable::HashTable()
{
	
	Htable = new Hnode * [SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		Htable[i] = NULL;
	}



}
int HashTable::Hashfun(string sname)
{
	
	int sum = 0;
	for (int i = 0; i < sname.size(); i++)
		sum += sname.at(i);
	return sum % SIZE;
 }
void HashTable::Null()
{
	for (int i = 0; i < SIZE; i++)
		Htable[i] = NULL;
}
void HashTable::Insert(Symbol symbol)
{
	if (!search(symbol.sname))
	{
		int index = Hashfun(symbol.sname);
		Hnode* node = new Hnode(symbol);
		if (Htable[index] == NULL)
			Htable[index] = node;
		else
		{
			node = Htable[index];
			while (node->next != NULL)
				node = node->next;
			node->next = new Hnode(symbol);
		}
	}
	else
		cout << "the symbol name is aready exisst" << endl;


}
bool HashTable::search(string name)
{
	
	int index = Hashfun(name);
	Hnode  * search = Htable[index];
	while (search!=NULL)
	{
		if (search->symbol.sname == name)
			return true;
		search = search->next;

	}
	return false;
		

}
void HashTable::Insert_List(SymbolType stype, vector<string>names)
{
	
	for (int i = 0; i < names.size(); i++)
	{
		if (!search(names[i])) {
			Symbol symbol(names[i], stype, variable);
			Insert(symbol);
		}
		else 

			cout <<names[i] <<" "<< "symbol name is aready exisst" << endl;

	}

}
SymbolType HashTable::getSymbolTtype(string name)
{
	
	if (search(name))
	{
		int index = Hashfun(name);
		Hnode* search = Htable[index];
		while (search != NULL)
		{
			if (search->symbol.sname == name)
			{
				//Symbol 	s(name, search->symbol.stype, search->symbol.skind);
				Symbol s;
				s.stype = search->symbol.stype;
				return s.stype ;
			}
			search = search->next;

		}
		
	}
	return SymbolType::undefined;
}
SymbolKind HashTable::getSymbolKind(string name)
{
	if (search(name))
	{
		int index = Hashfun(name);
		Hnode* search = Htable[index];
		while (search != NULL)
		{
			if (search->symbol.sname == name)
				return search->symbol.skind;
			search = search->next;

		}

	}
	return SymbolKind::Empty;
}
Symbol HashTable::getsymbol(string name)
{

	if (search(name))
	{
		int index = Hashfun(name);
		Hnode* search = Htable[index];
		while (search != NULL)
		{
			if (search->symbol.sname == name)
				return search->symbol;
			search = search->next;

		}

	}

}
void HashTable::CompineTwoHtable(HashTable hashtable)
{ 

	Hnode* insert;
	for (int i = 0; i < SIZE; i++)
	{
		insert = hashtable.Htable[i];
		while (insert != NULL)
		{
			Insert(insert->symbol);
			insert = insert->next;
		}
	}

}
bool HashTable::Intersection(HashTable hashtable)
{
	Hnode* node;
	for (int i = 0; i < SIZE; i++)
	{
		node = hashtable.Htable[i];
		while (node!=NULL)
		{
			if (search(node->symbol.sname))
				return true;
			node=node->next;

		}
	}
	return false;

}
void HashTable::Display()
{
	Hnode* print;
	for (int i = 0; i < SIZE; i++)
	{
		print = Htable[i];
		
		while (print!=NULL)
		{
			cout << print->symbol.sname << endl;
			cout << print->symbol.getSymbolType() << endl;
			cout << print->symbol.getSymbolkind() << endl;
			
			print = print->next;

		}
		 
	}


}
/*int main()
{
	vector<string>name;
	name.push_back("x");
	name.push_back("y");
	name.push_back("x");
	name.push_back("n");
	HashTable ht;
	ht.Insert_List(integer,name);
	cout << endl;
	SymbolType t = ht.getSymbolTtype("n");
	cout<<t<<endl;
	
	Symbol symbol1("ahmed", integer, variable), symbol2("hadeer", boolean, variable)
		, symbol3("ali", character, variable),symbol4("mai",integer,variable)
		,symbol5("hadeer",boolean,constant),symbol6("aalaa",integer,Empty),
		symbol8;
	
	//Hnode node1(symbol1),node2(symbol2),node3(symbol3);
	/*HashTable hasht,hasht2,hasht3;
	hasht.Insert(symbol1);
	hasht.Insert(symbol2);
	hasht.Insert(symbol3);
	hasht2.Insert(symbol4);
	hasht2.Insert(symbol5);
	hasht2.Insert(symbol6);
	hasht.Display();*/
	//hasht.CompineTwoHtable(hasht2);
	//hasht3.Insert_List(integer, name);*/
	//cout << hasht.Intersection(hasht2)<<endl;
	//hasht.Display();
	//hasht3.Display();*/

	
	//system("pause");
	//return 0;

//}*/

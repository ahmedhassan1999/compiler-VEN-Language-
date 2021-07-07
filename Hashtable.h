#ifndef HashTable_HEADER
#define HashTable_HEADER
#include"Hnode.h"

class HashTable {
public:
	const int SIZE = 10;
	Hnode** Htable;
	HashTable();
	int Hashfun(string sname);

	void Insert(Symbol symbol);
	Symbol getsymbol(string name);
	SymbolType getSymbolTtype(string name);
	SymbolKind getSymbolKind(string name);
	void Insert_List(SymbolType stype, vector<string>sname);
	bool search(string sname);
	void Null();
	void CompineTwoHtable(HashTable hashtable);
	bool Intersection(HashTable hashtable);
	void Display();



};
#endif
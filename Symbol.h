#ifndef Included_NameModel_H
#define Included_NameModel_H
#include<iostream>
#include<string>
#include<vector>
using namespace std;
enum SymbolType {procuder_t ,  boolean, integer, program, character , undefined };
enum SymbolKind { procuder_k,variable , Empty, constant };

class Symbol {
public:
	string sname;
	SymbolType stype;
	SymbolKind skind;
	vector<Symbol> parametars;
	Symbol();
	//Symbol(string name, SymbolType type, vector<Symbol>parametars=vector<Symbol>v);
	string getSymbolType();
	string getSymbolkind();
	Symbol(string name, SymbolType type, SymbolKind kind);
};
#endif
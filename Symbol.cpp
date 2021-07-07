#include"Symbol.h"

Symbol::Symbol()
{

	sname = " ";
	stype = SymbolType::undefined;
	skind = SymbolKind::Empty;
	parametars = vector<Symbol>(0);
}
Symbol::Symbol(string name, SymbolType type, SymbolKind kind)
{
	sname = name;
	stype = type;
	skind = kind;
	//parametars = v;
}
string Symbol::getSymbolType()
{
	switch (stype)
	{
	case integer: return "integer"; break;
	case boolean: return "boolean"; break;
	case program: return "program"; break;
	case character: return "charchter"; break;
	default: return "undefinde"; break;
	}

}
string Symbol::getSymbolkind()
{
	switch (skind)
	{
	case variable: return "varible"; break;
	case constant: return "constant"; break;
	default: return "Empty"; break;
	}
}
/*int main()
{
	Symbol s;
	cout << s.getSymbolType() << endl;
	cout << s.getSymbolkind() << endl;
}*/

#include"HashTable.h"
#include"Scanner.h"
#include<vector>
#include"Symbol.h"

#include<stack>
class Scope {
public:
	Scanner scan;
	Token curret_token;
	stack<HashTable*>s;
	Scope(string filename) :scan(filename)
	{

	}
	string conv(SymbolType t)
	{
		switch (t)
		{

		case boolean:
		{
			return "boolean";
		}
		break;
		case integer:
		{
			return "integer";
		}
		break;

		default:
			break;
		}
	}
	void VEN()
	{
		curret_token = scan.getToken();
		program();

	}
	void match(TokenType tt)
	{
		if (tt == curret_token.ttype)
		{
			cout << curret_token.getTokenType() << " " << "is matched" << endl;
			curret_token = scan.getToken();
			if (curret_token.ttype == STOP)
				return;
		}
		else
			Syntax_error(curret_token.ttype);


	}
	void Syntax_error(TokenType tt)
	{
		cout << "syntax erorr" << " " << tt << endl;
		this->curret_token = scan.getToken();
	}
	void program()
	{
		block();
	}
	void block(vector<Symbol>symbol = vector<Symbol>(0))
	{
		match(BEGIN_SY);
		HashTable* ht_ptr = new HashTable();
		s.push(ht_ptr);
		for (int i = 0; i < symbol.size(); i++)
			ht_ptr->Insert(symbol[i]);
		dec_Seq(ht_ptr);
		com_Seq(ht_ptr);
		match(END_SY);
		ht_ptr->Null();

	}
	void dec_Seq(HashTable* ht_ptr)
	{
		decl(ht_ptr);
		while (curret_token.ttype == INTEGER_SY || curret_token.ttype == BOOLEAN_SY || curret_token.ttype == PROC_SY)
		{
			decl(ht_ptr);
		}
	}
	void decl(HashTable* ht_ptr)
	{

		if (curret_token.ttype == INTEGER_SY || curret_token.ttype == BOOLEAN_SY)
		{
			SymbolType t = type();
			vector<string> names = name_List();
			ht_ptr->Insert_List(t, names);

		}
		else if (curret_token.ttype == PROC_SY)
		{

			Symbol s;
			s.skind = SymbolKind::procuder_k;
			s.stype = SymbolType::procuder_t;
			vector<Symbol>param;
			match(PROC_SY);
			s.sname = curret_token.tname;
			match(ID_SY);
			if (curret_token.ttype == LPAREN_SY)
			{
				match(LPAREN_SY);

				param = parameter_list();


				s.parametars = param;
				match(RPAREN_SY);
			}
			ht_ptr->Insert(s);
			match(EQUAL_SY);
			command(ht_ptr, param);
		}
		else  Syntax_error(curret_token.ttype);

	}
	SymbolType type()
	{

		if (curret_token.ttype == INTEGER_SY)
		{
			match(INTEGER_SY);
			return SymbolType::integer;
		}

		else {
			match(BOOLEAN_SY);
			return SymbolType::boolean;
		}
		Syntax_error(curret_token.ttype);

	}
	bool search_name_instack(string name)
	{
		stack<HashTable*>temp = s;
		HashTable* put;
		while (!temp.empty())
		{
			put = temp.top();
			if (put->search(name))
				return true;
			temp.pop();
		}
		return false;
	}
	Symbol get_symbol_instack(string name)
	{
		Symbol sy;
		stack<HashTable*>temp = s;
		HashTable* put;
		while (!temp.empty())
		{
			put = temp.top();
			if (put->search(name))
				return put->getsymbol(name);
			temp.pop();
		}
		return sy;
	}
	bool searchpara(string name, vector<Symbol>symbol)
	{
		if (symbol.size() == 0)
			return false;
		for (int i = 0; i < symbol.size(); i++)
		{
			if (symbol[i].sname == name)
				return true;
		}
		return false;
	}
	Symbol getparameter(string name, vector<Symbol>symbol)
	{
		Symbol s;
		if (symbol.size() == 0)
			return s;
		for (int i = 0; i < symbol.size(); i++)
		{
			if (name == symbol[i].sname)
				return symbol[i];
		}
		return s;
	}
	bool check_para_names(string s, vector<Symbol>symbol)
	{
		if (symbol.size() == 0)
			return false;
		for (int i = 0; i < symbol.size(); i++)
		{
			if (s == symbol[i].sname)
				return true;
		}
		return false;

	}
	vector<Symbol> parameter_list()
	{
		vector<Symbol>symbol;
		SymbolType t = type();
		vector<string> names = name_List();
		for (int i = 0; i < names.size(); i++)
		{


			Symbol s(names[i], t, SymbolKind::variable);
			symbol.push_back(s);
		}
		while (curret_token.ttype == SEMICOLON_SY)
		{
			match(SEMICOLON_SY);
			SymbolType t = type();
			vector<string> names = name_List();
			for (int i = 0; i < names.size(); i++)
			{


				Symbol s(names[i], t, SymbolKind::variable);
				if (check_para_names(names[i], symbol))
				{

					cout << "semantic error the name" << " " << names[i] << " " << "is decalted once" << endl;
					continue;
				}

				symbol.push_back(s);
			}

		}
		return symbol;

	}
	vector<string> name_List()
	{
		vector<string>names;
		names.push_back(curret_token.tname);
		match(ID_SY);
		while (curret_token.ttype == COMMA_SY)
		{
			match(COMMA_SY);
			names.push_back(curret_token.tname);
			match(ID_SY);
		}
		return names;

	}
	void com_Seq(HashTable* ht_ptr, vector<Symbol>symbol = vector<Symbol>(0))
	{
		command(ht_ptr);
		while (curret_token.ttype == SEMICOLON_SY)
		{
			match(SEMICOLON_SY);
			command(ht_ptr);
		}
	}
	void command(HashTable* ht_ptr, vector<Symbol>symbol = vector<Symbol>(0))
	{

		if (curret_token.ttype == ID_SY)
		{
			Symbol s;
			if (searchpara(curret_token.tname, symbol))
			{
				cout << "the varible decleted in parameter list" << endl;
				s = getparameter(curret_token.tname, symbol);
			}
			else if (ht_ptr->search(curret_token.tname))
			{
				cout << "the varible decleted in symboltable" << endl;
				s = ht_ptr->getsymbol(curret_token.tname);
			}
			else if (search_name_instack(curret_token.tname))
			{
				cout << "the varible declarted in stack" << endl;
				s = get_symbol_instack(curret_token.tname);
			}
			else cout << "the varible" << " " << curret_token.tname << "not declted" << endl;

			SymbolType lefttype = s.stype;

			match(ID_SY);
			match(EQUAL_SY);

			SymbolType righttype = expr(ht_ptr, symbol);
			if (lefttype != righttype)
				cout << "type of the left not equal type of the right" << endl;
			else
				cout << "type of expr done!" << endl;

		}
		else if (curret_token.ttype == READ_SY)
		{
			Symbol s;

			match(READ_SY);

			if (searchpara(curret_token.tname, symbol))
			{
				cout << "the varible decleted in parameter list" << endl;
				s = getparameter(curret_token.tname, symbol);
			}
			else if (ht_ptr->search(curret_token.tname))
			{
				cout << "the varible decleted in symboltable" << endl;
				s = ht_ptr->getsymbol(curret_token.tname);
			}
			else if (search_name_instack(curret_token.tname))
			{
				cout << "the varible declarted in stack" << endl;
				s = get_symbol_instack(curret_token.tname);
			}
			else cout << "the varible" << " " << curret_token.tname << "not declted" << endl;

			if (s.stype != integer)
				cout << curret_token.tname << "type is not integer" << endl;
			else
				cout << "the type is integer" << endl;
			match(ID_SY);


		}
		else if (curret_token.ttype == WRITE_SY)
		{
			match(WRITE_SY);

			expr(ht_ptr, symbol);
		}
		else if (curret_token.ttype == IF_SY)
		{
			match(IF_SY);
			expr(ht_ptr, symbol);
			match(THEN_SY);
			com_Seq(ht_ptr, symbol);
			if (curret_token.ttype == ELSE_SY)
			{
				match(ELSE_SY);
				com_Seq(ht_ptr, symbol);
			}
			match(ENDIF_SY);
		}
		else if (curret_token.ttype == WHILE_SY)
		{
			match(WHILE_SY);
			expr(ht_ptr, symbol);
			match(DO_SY);
			com_Seq(ht_ptr, symbol);
			match(ENDWHILE_SY);
		}
		else if (curret_token.ttype == CALL_SY)
		{
			Symbol s;
			match(CALL_SY);
			if (ht_ptr->search(curret_token.tname))
			{
				cout << "the procder in symboltable" << endl;
				s = ht_ptr->getsymbol(curret_token.tname);
			}
			else if (search_name_instack(curret_token.tname))
			{
				cout << "the procuder in stack" << endl;
				s = get_symbol_instack(curret_token.tname);
			}
			else
				cout << curret_token.tname << "not declareted" << endl;
			SymbolType t = ht_ptr->getSymbolTtype(curret_token.tname);
			if (t != SymbolType::procuder_t)
				cout << "the type is not procuder" << endl;
			Symbol sy = ht_ptr->getsymbol(curret_token.tname);
			match(ID_SY);
			if (curret_token.ttype == LPAREN_SY)
			{
				vector<Symbol>InMain;
				match(LPAREN_SY);
				while (curret_token.ttype != RPAREN_SY)
				{
					if (curret_token.ttype == COMMA_SY)
						match(COMMA_SY);
					if (curret_token.ttype == ID_SY)
					{
						Symbol s;
						if (searchpara(curret_token.tname, symbol))
						{
							cout << "the varible decleted in parameter list" << endl;
							s = getparameter(curret_token.tname, symbol);
						}
						else if (ht_ptr->search(curret_token.tname))
						{
							cout << "the varible decleted in symboltable" << endl;
							s = ht_ptr->getsymbol(curret_token.tname);
						}
						else if (search_name_instack(curret_token.tname))
						{
							cout << "the varible declarted in stack" << endl;
							s = get_symbol_instack(curret_token.tname);
						}
						else cout << "the varible" << " " << curret_token.tname << "not declted" << endl;

						InMain.push_back(s);
						match(ID_SY);
					}
				}
				if (sy.parametars.size() != InMain.size())
					cout << "the number of parameter in main not match to the number of parameter in procuder" << endl;
				for (int i = 0; i < sy.parametars.size(); i++)
				{

					if (sy.parametars[i].stype != InMain[i].stype)
					{
						cout << "type of" << " " << InMain[i].sname << " " << "must be" << " " << conv(sy.parametars[i].stype) << endl;
					}
				}
				match(RPAREN_SY);
			}

		}
		else if (curret_token.ttype == BEGIN_SY)
			block(symbol);

	}
	SymbolType expr(HashTable* ht_ptr, vector<Symbol>symbol = vector<Symbol>(0))
	{
		SymbolType t, t1, t2;
		t1 = expr1(ht_ptr, symbol);
		t = t1;
		while (curret_token.ttype == OR_SY)
		{
			match(OR_SY);
			t2 = expr1(ht_ptr, symbol);
			if (t2 != SymbolType::boolean || t1 != SymbolType::boolean)
			{
				cout << "type of expr must be boolean" << endl;
				return SymbolType::undefined;
			}

		}
		return t;

	}
	SymbolType expr1(HashTable* ht_ptr, vector<Symbol>symbol = vector<Symbol>(0))
	{

		SymbolType t, t1, t2;
		t1 = expr2(ht_ptr, symbol);
		t = t1;
		while (curret_token.ttype == AND_SY)
		{
			match(AND_SY);
			t2 = expr1(ht_ptr, symbol);
			if (t2 != SymbolType::boolean || t1 != SymbolType::boolean)
			{
				cout << "type of expr must be boolean" << endl;
				return SymbolType::undefined;
			}

		}
		return t;
	}
	SymbolType expr2(HashTable* ht_ptr, vector<Symbol>symbol = vector<Symbol>(0))
	{
		SymbolType t;

		if (curret_token.ttype == NOT_SY)
		{
			match(NOT_SY);
			t = expr(ht_ptr, symbol);
			if (t != SymbolType::boolean)
				cout << "the type must be boolean" << endl;
			return t;
		}
		else if (curret_token.ttype == ID_SY || curret_token.ttype == NUMBER_SY || curret_token.ttype == LPAREN_SY || curret_token.ttype == MINUS_SY)
		{
			t = expr3(ht_ptr, symbol);
			return t;
		}
		else
			return SymbolType::undefined;
	}
	SymbolType expr3(HashTable* ht_ptr, vector<Symbol>symbol = vector<Symbol>(0))
	{
		SymbolType t, t1, t2;
		t1 = expr4(ht_ptr, symbol);
		t = t1;
		while (curret_token.ttype == LTHAN_SY || curret_token.ttype == LOE_SY || curret_token.ttype == NOTEQUAL_SY
			|| curret_token.ttype == GTHAN_SY || curret_token.ttype == GOE_SY || curret_token.ttype == EQUAL_SY)
		{
			relation();
			t2 = expr4(ht_ptr, symbol);
			if (t1 != SymbolType::integer || t2 != SymbolType::integer) {

				cout << " type must be of type integer!" << endl;
			}
		}


		return t;

	}
	SymbolType expr4(HashTable* ht_ptr, vector<Symbol>symbol = vector<Symbol>(0))
	{
		SymbolType t, t1, t2;
		t1 = term(ht_ptr, symbol);
		t = t1;
		while (curret_token.ttype == PLUS_SY || curret_token.ttype == MINUS_SY)
		{
			WeakOp();
			t2 = term(ht_ptr, symbol);
			if (t1 != SymbolType::integer || t2 != SymbolType::integer)
				cout << "the type must be integer" << endl;
			return SymbolType::undefined;
		}
		return t;
	}
	SymbolType term(HashTable* ht_ptr, vector<Symbol>symbol = vector<Symbol>(0))
	{
		SymbolType t, t1, t2;
		t1 = element(ht_ptr, symbol);
		t = t1;
		while (curret_token.ttype == PLUS_SY || curret_token.ttype == DIV_SY)
		{
			StrongOp();
			t2 = element(ht_ptr, symbol);
			if (t1 != SymbolType::integer || t2 != SymbolType::integer)
				cout << "the type must be integer" << endl;
			return SymbolType::undefined;

		}
		return t;
	}
	SymbolType element(HashTable* ht_ptr, vector<Symbol>symbol = vector<Symbol>(0))
	{
		SymbolType t;
		if (curret_token.ttype == NUMBER_SY) {
			t = SymbolType::integer;
			match(NUMBER_SY);
		}
		else if (curret_token.ttype == ID_SY)
		{
			Symbol s;
			if (searchpara(curret_token.tname, symbol))
			{
				cout << "the varible decleted in parameter list" << endl;
				s = getparameter(curret_token.tname, symbol);
			}
			else if (ht_ptr->search(curret_token.tname))
			{
				cout << "the varible decleted in symboltable" << endl;
				s = ht_ptr->getsymbol(curret_token.tname);
			}
			else if (search_name_instack(curret_token.tname))
			{
				cout << "the varible declarted in stack" << endl;
				s = get_symbol_instack(curret_token.tname);
			}
			else cout << "the varible" << " " << curret_token.tname << "not declted" << endl;
			t = s.stype;
		}
		else if (curret_token.ttype == LPAREN_SY) {
			match(LPAREN_SY);
			t = expr(ht_ptr, symbol);
			match(RPAREN_SY);
		}
		else {

			t = SymbolType::undefined;
			Syntax_error(curret_token.ttype);
		}
		return t;
	}



	void WeakOp() {
		switch (curret_token.ttype) {
		case PLUS_SY: match(PLUS_SY); break;
		case MINUS_SY: match(MINUS_SY); break;
		default: Syntax_error(curret_token.ttype); break;
		}
	}
	void StrongOp() {
		switch (curret_token.ttype) {
		case MULTI_SY: match(MULTI_SY); break;
		case DIV_SY: match(DIV_SY); break;
		default: Syntax_error(curret_token.ttype); break;
		}
	}
	void  relation() {
		switch (curret_token.ttype) {
		case LTHAN_SY: match(LTHAN_SY); break;
		case LOE_SY: match(LOE_SY); break;
		case NOTEQUAL_SY: match(NOTEQUAL_SY); break;
		case EQUAL_SY: match(EQUAL_SY); break;
		case GTHAN_SY: match(GTHAN_SY); break;
		case GOE_SY: match(GOE_SY); break;
		default: Syntax_error(curret_token.ttype); break;
		}
	}


};
int main()
{
	cout << "the semantic and scop" << endl;
	Scope scope("scope.txt");
	scope.VEN();

	system("pause");
	return 0;
}
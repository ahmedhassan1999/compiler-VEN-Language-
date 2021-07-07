#include"Apstract.h"


Abs_Node tree;
int label = 0;
int temp = 0;
Code* C;
void code_gen(Abs_Node& n);
string getOpCode(OpCode op);

string newlabel() {
	return "L" + to_string(++label);
}
string newtemp() {
	return "T" + to_string(++temp);
}
string place(Abs_Node node) {
	if (node.code == NULL)
		return node.name;
	else {

		Code* c = node.code;
		while (c->next != NULL)
			c = c->next;
		return c->dest;
	}
}
Code* add(Code* code, Code* code2) {
	Code* temp = code;
	if (temp == NULL)
		return code2;
	while (temp->next != NULL)
		temp = temp->next;
	temp->next = code2;
	//code = temp;
	return code;
}
void PrintCode() {
	cout << endl;
	cout << "_________________________________________" << endl;
	
	while (C->next != NULL) {
		cout << getOpCode(C->op) << "  " << C->s1 << "  " << C->s2 << "  " << C->dest << endl;
		C = C->next;
	}
}
Abs_parser::Abs_parser(string filename) :scan(filename) {
}
void Abs_parser::start()
{
	curret_token = scan.getToken();
	tree = program();
	code_gen(tree);
	C = tree.child[0].child[1].code;
	PrintCode();
}
Abs_Node Abs_parser::program()
{
	Abs_Node node = Abs_Node(1);
	node.nodetype = PROGRAMM;
	node.child[0] = block();
	return node;

}
void Abs_parser::match(TokenType ttype)
{

	
	if (curret_token.ttype == ttype)
	{
	
		
		curret_token = scan.getToken();
	}
	else
		Syntax_error(curret_token.ttype);

}
void  Abs_parser::Syntax_error(TokenType ttype)
{
	
	this->curret_token = scan.getToken();

}
Abs_Node Abs_parser::block()
{
	Abs_Node node = Abs_Node(2);
	node.nodetype = BLOCK;
	match(BEGIN_SY);
	node.child[0] = dec_seq();
	node.child[1] = command_seq();
	match(END_SY);
	return node;
}
Abs_Node Abs_parser::dec_seq()
{
	Abs_Node node = dec();
	bool enter = false;
	while (curret_token.ttype == INTEGER_SY || curret_token.ttype == BOOLEAN_SY || curret_token.ttype == PROC_SY)
	{
		enter = true;
		Abs_Node temp = Abs_Node(2);
		temp.name = " ";
		temp.child[0] = node;
		temp.child[1] = dec();
		node = temp;
	}
	if (enter)
		node.nodetype = DEC_SEQ;
	return node;

}
Abs_Node Abs_parser::dec()
{
	//
	Abs_Node node = Abs_Node(0);
	if (curret_token.ttype == INTEGER_SY || curret_token.ttype == BOOLEAN_SY)
	{
		node = Abs_Node(2);
		node.nodetype = DEC;
		node.dectype = VARIBLE;
		node.child[0] = type();
		node.child[1] = name_list();
	}
	else if (curret_token.ttype == PROC_SY)
	{
		match(PROC_SY);
		node = Abs_Node(3);
		node.dectype = PROC;
		node.nodetype = DEC;
		node.child[0] = Abs_Node(0);
		//node->dectype = VAR_ID;
		//yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy
		node.child[0].name = curret_token.tname;
		match(ID_SY);


		if (curret_token.ttype == LPAREN_SY)
		{
			match(LPAREN_SY);
			node.child[1] = param_list();
			match(RPAREN_SY);
		}
		match(EQUAL_SY);
		node.child[2] = command();
	}
	else {
		Syntax_error(curret_token.ttype);
		return NULL;
	}
	return node;

}
Abs_Node Abs_parser::type()
{
	Abs_Node node = Abs_Node(0);
	node.nodetype = TYPE;
	if (curret_token.ttype == INTEGER_SY)
	{
		match(INTEGER_SY);
		node.typeoft = INTEGER;
	}
	else if (curret_token.ttype == BOOLEAN_SY)
	{
		match(BOOLEAN_SY);
		node.typeoft = BOOLEAN;
	}
	else
		Syntax_error(curret_token.ttype);
	return node;

}
Abs_Node Abs_parser::param_list()
{
	Abs_Node node = Abs_Node(2);
	node.nodetype = PARAM_LIST;
	node.child[0] = type();
	node.child[1] = name_list();
	while (curret_token.ttype == SEMICOLON_SY)
	{
		match(SEMICOLON_SY);
		Abs_Node temp = Abs_Node(2);
		temp.nodetype = PARAM_LIST;
		temp.name = ";";
		temp.child[0] = node;
		temp.child[1] = Abs_Node(2);
		temp.child[1].child[0] = type();
		temp.child[1].child[1] = name_list();
		node = temp;


	}
	return node;
}
Abs_Node Abs_parser::name_list()
{
	Abs_Node node = Abs_Node(0);
	node.nodetype = IDN;
	node.name = curret_token.tname;
	match(ID_SY);
	while (curret_token.ttype == COMMA_SY)
	{
		match(COMMA_SY);
		Abs_Node temp = Abs_Node(2);
		temp.nodetype = NAME_LIST;
		temp.name = ",";
		temp.child[0] = node;
		temp.child[1] = Abs_Node(0);
		temp.child[1].nodetype = IDN;
		temp.child[1].name = curret_token.tname;

		match(ID_SY);
		node = temp;

	}
	return node;

}
Abs_Node Abs_parser::command_seq()
{
	Abs_Node node = command();
	bool enter = false;
	while (curret_token.ttype == SEMICOLON_SY)
	{
		enter = true;
		match(SEMICOLON_SY);
		Abs_Node temp = Abs_Node(2);
		temp.name = ";";
		temp.child[0] = node;
		temp.child[1] = command();
		node = temp;
	}
	if (enter)
		node.nodetype = COM_SEQ;
	return node;

}
Abs_Node Abs_parser::command()
{
	Abs_Node node = Abs_Node(0);
	if (curret_token.ttype == ID_SY)
	{
		node = Abs_Node(2);
		node.nodetype = COM;
		node.commtype = Commtype::ASSIGNN;
		node.child[0] = Abs_Node(0);
		node.child[0].name = curret_token.tname;
		match(ID_SY);
		match(AHMED);
		node.child[1] = expr();
	}
	else if (curret_token.ttype == READ_SY)
	{
		match(READ_SY);
		node = Abs_Node(1);
		node.nodetype = COM;
		node.commtype = Commtype::READD;
		node.child[0] = Abs_Node(0);
		node.child[0].name = curret_token.tname;
		match(ID_SY);
	}
	else if (curret_token.ttype == WRITE_SY)
	{
		match(WRITE_SY);
		node = Abs_Node(1);
		node.nodetype = COM;
		node.commtype = Commtype::WRITEE;
		node.child[0] = expr();

	}
	else if (curret_token.ttype == IF_SY)
	{
		match(IF_SY);
		node = Abs_Node(3);
		node.nodetype = COM;
		node.commtype = Commtype::IF;


		node.child[0] = expr();



		match(THEN_SY);
		node.child[1] = command();
		if (curret_token.ttype == ELSE_SY)
		{
			match(ELSE_SY);
			node.child[2] = command();
		}
		match(ENDIF_SY);
	}
	else if (curret_token.ttype == WHILE_SY)
	{
		match(WHILE_SY);
		node = Abs_Node(2);
		node.nodetype = COM;
		node.commtype = WHILE;
		node.child[0] = expr();
		match(DO_SY);
		node.child[1] = command_seq();
		match(ENDWHILE_SY);
	}
	else if (curret_token.ttype == CALL_SY)
	{
		match(CALL_SY);
		node = Abs_Node(2);
		node.nodetype = COM;
		node.commtype = CALLL;
		node.child[0] = Abs_Node(0);
		node.child[0].name = curret_token.tname;
		match(ID_SY);
		if (curret_token.ttype == LPAREN_SY)
		{
			match(LPAREN_SY);
			node.child[1] = name_list();
			match(RPAREN_SY);
		}
	}
	else Syntax_error(curret_token.ttype);
	return node;

}
Abs_Node Abs_parser::expr()
{

	Abs_Node node = expr1();
	while (curret_token.ttype == OR_SY)
	{
		match(OR_SY);
		Abs_Node temp = Abs_Node(2);
		temp.name = "or";
		temp.nodetype = EXP;
		temp.expertype = ExperType::ORR;
		temp.child[0] = node;
		temp.child[1] = expr1();
		node = temp;
	}
	return node;

}
Abs_Node Abs_parser::expr1()
{

	Abs_Node node = expr2();
	while (curret_token.ttype == AND_SY)
	{
		match(AND_SY);
		Abs_Node temp = Abs_Node(2);
		temp.name = "and";
		temp.nodetype = EXP;
		temp.expertype = ExperType::ANDD;
		temp.child[0] = node;
		temp.child[1] = expr2();
		node = temp;
	}
	return node;

}
Abs_Node Abs_parser::expr2()
{
	if (curret_token.ttype == ID_SY || curret_token.ttype == NUMBER_SY || curret_token.ttype == LPAREN_SY)
	{
		Abs_Node node = expr3();
		node.nodetype = EXP;
		return node;

	}
	else if (curret_token.ttype == NOT_SY)
	{
		match(NOT_SY);
		Abs_Node node = Abs_Node(1);
		node.name = "not";
		node.nodetype = EXP;
		node.expertype = ExperType::NOTT;
		node.child[0] = expr();
		return node;

	}

	else {
		Syntax_error(curret_token.ttype);
		return NULL;
	}


}
Abs_Node Abs_parser::expr3()
{
	//
	Abs_Node node = Abs_Node(0);
	node = expr4();
	while (curret_token.ttype == LTHAN_SY || curret_token.ttype == LOE_SY || curret_token.ttype == NOTEQUAL_SY
		|| curret_token.ttype == GTHAN_SY || curret_token.ttype == GOE_SY || curret_token.ttype == EQUAL_SY)
	{
		Abs_Node temp = relation();
		temp.child[0] = node;
		temp.child[1] = expr4();
		node = temp;
	}
	return node;

}

Abs_Node Abs_parser::expr4()
{
	Abs_Node node = term();
	while (curret_token.ttype == PLUS_SY || curret_token.ttype == MINUS_SY)
	{
		Abs_Node temp = weakop();
		temp.child[0] = node;
		temp.child[1] = term();
		node = temp;
	}
	return node;
}
Abs_Node Abs_parser::term()
{
	Abs_Node node = element();
	while (curret_token.ttype == MULTI_SY || curret_token.ttype == DIV_SY)
	{
		Abs_Node temp = strongop();
		temp.child[0] = node;
		temp.child[1] = element();
		node = temp;
	}
	return node;

}
Abs_Node Abs_parser::element()
{
	Abs_Node node = Abs_Node(0);
	if (curret_token.ttype == NUMBER_SY) {
		node = Abs_Node(0);
		node.nodetype = NodeType::ELEMENT;
		node.elementtype = NUMBERR;
		node.name = curret_token.tname;
		node.nodetype = NUMERAL;
		match(NUMBER_SY);
	}
	else if (curret_token.ttype == ID_SY)
	{
		node = Abs_Node(0);
		node.nodetype = NodeType::ELEMENT;
		node.elementtype = ID;
		node.name = curret_token.tname;
		match(ID_SY);
	}
	else if (curret_token.ttype == LPAREN_SY) {
		match(LPAREN_SY);
		node = expr();
		match(RPAREN_SY);
	}
	else if (curret_token.ttype == TokenType::MINUS_SY)
	{
		match(MINUS_SY);
		node.nodetype= ELEMENT;
		node.elementtype = MIN;
		node.name = "-";
		return node;
		
		
	}
	else {
		Syntax_error(curret_token.ttype);
		return NULL;
	}
	return node;

}
Abs_Node Abs_parser::weakop() {
	Abs_Node node = Abs_Node(2);
	node.nodetype = EXP;
	node.expertype = WEAK;

	if (curret_token.ttype == PLUS_SY) {
		node.name = "+";
		match(PLUS_SY);
	}
	else {
		Syntax_error(curret_token.ttype);
	}

	return node;
}
Abs_Node  Abs_parser::strongop() {
	Abs_Node node = Abs_Node(2);
	node.nodetype = EXP;
	node.expertype = STRONG;

	if (curret_token.ttype == MULTI_SY) {
		node.name = "*";
		match(MULTI_SY);
	}
	else if (curret_token.ttype == DIV_SY) {
		node.name = "/";
		match(DIV_SY);
	}
	else {
		Syntax_error(curret_token.ttype);
	}

	return node;
}
Abs_Node Abs_parser::relation() {
	Abs_Node node = Abs_Node(2);
	node.nodetype = EXP;
	node.expertype = RELATIONn;

	if (curret_token.ttype == GTHAN_SY) {
		node.name = "<";
		match(GTHAN_SY);
	}
	else if (curret_token.ttype == GOE_SY) {
		node.name = "<=";
		match(GOE_SY);
	}
	else if (curret_token.ttype == NOTEQUAL_SY) {
		node.name = "<>";
		match(NOTEQUAL_SY);
	}
	else if (curret_token.ttype == EQUAL_SY) {
		node.name = "=";
		match(EQUAL_SY);
	}
	else if (curret_token.ttype == LTHAN_SY) {
		node.name = ">";
		match(LTHAN_SY);
	}
	else if (curret_token.ttype == LOE_SY) {
		node.name = "<=";
		match(LOE_SY);
	}
	else {
		Syntax_error(curret_token.ttype);
	}

	return node;
}
string getOpCode(OpCode op)
{
	switch (op)
	{
	case GOTO:
	{
		return "GOTO";
	}break;

	case ADD:
	{
		return "ADD";
	}
	break;
	case SUB:
	{
		return"SUB";
	}
	break;
	case MULT:
	{
		return "MULT";
	}
	break;
	case DIV:
	{
		return "DIV";
	}
	break;
	case LTHAN:
	{
		return ">";
	}
	break;
	case LEPREQ:
	{
		return ">=";
	}
	break;
	case NEQ:
	{
		return "!=";
	}
	break;
	case EQ:
	{
		return "=";
	}
	break;
	case GTHAN:
	{
		return "<";
	}
	break;
	case GEOPEQ:
	{
		return "<=";
	}
	break;
	case AND:
	{
		return "AND";
	}
	break;
	case OR:
	{
		return "OR";
	}
	break;
	case OPUNDEFIEND:
	{
		return "undefinedop";
	}
	break;
	case XOR:
	{
		return "XOR";
	}
	break;
	case NEG:
	{
		return "NEG";
	}
	case LABEL:
	{
		return "label";
	}
	break;
	case NOT:
	{
		return "NOT";
	}
	break;
	case ASSIGN:
	{
		return "ASSIGN";
	}
	break;
	case BIF:
	{
		return  "BIE";
	}
	break;
	case BNIF:
	{
		return "BNIF";
	}
	break;
	case IFN:
	{
		return "IFN";
	}
	break;
	case IFLT:
	{
		return "IFLT";
	}
	break;
	case IFLE:
	{
		return "IFLE";
	}
	break;
	case IFNE:
	{
		return "IFNE";
	}
	break;
	case IFEQ:
	{
		return "IFNQ";
	}
	break;
	case IFGE:
	{
		return "IFGE";
	}
	break;
	case IFGT:
	{
		return "IFGT";
	}
	break;

	case READ:
	{
		return "READ";
	}
	break;
	case WRITE:
	{
		return "WRITE";
	}
	break;
	case CALLOP:
	{
		return "CALL";
	}
	break;
	case HALT:
	{
		return "HALT";
	}
	break;
	case PARAM:
	{
		return "PARAM";
	}
	break;
	default:
		return "un";
		break;
	}
}

void code_gen(Abs_Node& root)
{
	int size = root.child.size();
	for (int i = 0; i < size; i++)
		code_gen(root.child[i]);
	switch (root.nodetype)
	{
	case PROGRAMM: {
		Code* a = new Code(OpCode::OPUNDEFIEND);
		root.code = add(root.child[0].code, a);
	} break;
	case BLOCK: {
		root.code = root.child[1].code;
	}break;
	case COM_SEQ: {
		for (auto& command : root.child) {
			root.code = add(root.code, command.code);
		}
	}break;
	case COM:
	{
		if (root.commtype == ASSIGNN) {
			root.code = root.child[1].code;
			Code* a = new Code(OpCode::ASSIGN, place(root.child[1]), "", place(root.child[0]));
			root.code = add(root.code, a);
		}
		else if (root.commtype == READD) {
			root.code = root.child[0].code;
			Code* a = new Code(OpCode::READ, "", "", place(root.child[0]));
			root.code = add(root.code, a);
		}
		else if (root.commtype == WRITEE) {
			root.code = root.child[0].code;
			Code* a = new Code(OpCode::WRITE, "", "", place(root.child[0]));
			root.code = add(root.code, a);
		}
		else if (root.commtype == IF)
		{
			if (root.child.size() == 2) {
				string lab1 = newlabel();
				root.code = root.child[0].code;
				Code* a = new Code(OpCode::IFN, place(root.child[0]), getOpCode(OpCode::GOTO) + lab1, "");
				root.code = add(root.code, a);
				root.code = add(root.code, root.child[1].code);
				Code* aa = new Code(OpCode::LABEL, "", lab1 + ":", "");
				root.code = add(root.code, aa);
			} 
			else if (root.child.size() == 3) {
				string lab1 = newlabel();
				string lab2 = newlabel();
				root.code = root.child[0].code;
				Code* c = new Code(OpCode::IFN, place(root.child[0]), getOpCode(OpCode::GOTO) + lab1, "");
				root.code = add(root.code, c);
				root.code = add(root.code, root.child[1].code);
				Code* c1 = new Code(OpCode::GOTO, lab2, "", "");
				root.code = add(root.code, c1);
				Code* c2 = new Code(OpCode::LABEL, "", lab1 + ":", "");
				root.code = add(root.code, c2);
				root.code = add(root.code, root.child[2].code);
				Code* c3 = new Code(OpCode::LABEL, "", lab2 + ":", "");
				root.code = add(root.code, c3);
			}
		}
		else if (root.commtype == WHILE) {
			string lab1 = newlabel();
			string lab2 = newlabel();
			Code* c = new Code(OpCode::LABEL, "", lab1 + ":", "");
			root.code = c;
			root.code = add(root.code, root.child[0].code);
			Code* c1 = new Code(OpCode::IFN, place(root.child[0]), getOpCode(OpCode::GOTO) + lab2, "");
			root.code = add(root.code, c1);
			root.code = add(root.code, root.child[1].code);
			Code* c2 = new Code(OpCode::GOTO, "", lab1, "");
			root.code = add(root.code, c2);
			Code* c3 = new Code(OpCode::LABEL, "", lab2 + ":", "");
			root.code = add(root.code, c3);
		}
		else if (root.commtype == CALLL) {
			long c = 0;
			if (root.child[1].name != " ") {
				Abs_Node t = root.child[1];
				while (t.name == ",") {
					c++;
				}
				c++;

			}
			Code* c1 = new Code(CALLOP, root.child[0].name, c == 0 ? "" : to_string(c), "");
			root.code = c1;
		}
	}break;
	case EXP:
	{
		if (root.expertype == ORR) {
			string temp = newtemp();
			Code* a = new Code(OpCode::OR, place(root.child[0]), place(root.child[1]), temp);
			root.code = a;
		}
		else if (root.expertype == ANDD) {
			string temp = newtemp();
			Code* a = new Code(OpCode::ADD, place(root.child[0]), place(root.child[1]), temp);
			root.code = a;
		}
		else if (root.expertype == NOTT) {
			string temp = newtemp();
			Code* c = new Code(OpCode::NOT, place(root.child[0]), "", temp);
			root.code = c;
		}
		else if (root.expertype == RELATIONn) {
			if (root.name == "<")
			{
				string temp = newtemp();
				Code* c = new Code(OpCode::GTHAN, place(root.child[0]), place(root.child[1]), temp);
				root.code = c;
			}
			else if (root.name == "<=")
			{
				string temp = newtemp();
				Code* c = new Code(OpCode::GEOPEQ, place(root.child[0]), place(root.child[1]), temp);
				root.code = c;
			}
			else if (root.name == "!=")
			{
				string temp = newtemp();
				Code* c = new Code(NEQ, place(root.child[0]), place(root.child[1]), temp);
				root.code = c;
			}
			else if (root.name == "=")
			{
				string temp = newtemp();
				Code* c = new Code(OpCode::EQ, place(root.child[0]), place(root.child[1]), temp);
				root.code = c;
			}
			else if (root.name == ">")
			{
				string temp = newtemp();
				Code* c = new Code(OpCode::LTHAN, place(root.child[0]), place(root.child[1]), temp);
				root.code = c;
			}
			else if (root.name == ">=")
			{
				string temp = newtemp();
				Code* c = new Code(OpCode::LEPREQ, place(root.child[0]), place(root.child[1]), temp);
				root.code = c;
			}
		}
		else if (root.expertype == WEAK)
		{
			if (root.name == "+")
			{
				string temp = newtemp();
				Code* a = new Code(ADD, place(root.child[0]), place(root.child[1]), temp);
				root.code = a;
			}
			else if (root.name == "-")
			{
				string temp = newtemp();
				Code* a = new Code(SUB, place(root.child[0]), place(root.child[1]), temp);
				root.code = a;
			}

		}
		else if (root.expertype == STRONG)
		{
			if (root.name == "*")
			{
				string temp = newtemp();
				Code* a = new Code(MULT, place(root.child[0]), place(root.child[1]), temp);
				root.code = a;
			}
			else if (root.name == "/")
			{
				string temp = newtemp();
				Code* a = new Code(DIV, place(root.child[0]), place(root.child[1]), temp);
				root.code = a;
			}

		}





	}break;

	case ELEMENT:
	{
		if (root.elementtype == ID)
		{
			root.code = NULL;

		}
		else if (root.elementtype == NUMBERR)
		{
			root.code = NULL;

		}
		else if (root.elementtype == EXPER)
		{
			string t = newtemp();
			Code* a = new Code(OpCode::NEG, place(root.child[0]), "", t);
			root.code = a;
		}
		else if (root.elementtype == MIN)
		{
			
			string t = newtemp();
			Code* a = new Code(OpCode::NEG, place(root.child[0]), "", t);
			root.code = a;
		}

	}break;
	case NAME_LIST:
	{
		if (root.child[0].name != ",") {
			string temp = newtemp();
			Code* a = new Code(ASSIGN, root.child[0].name, "", temp);
			root.code = a;
			Code* b = new Code(PARAM, temp, "", "");
			root.code = add(root.code, b);
		}
		string temp = newtemp();
		Code* c = new Code(OpCode::ASSIGN, root.child[1].name, "", temp);
		root.code = add(root.code, c);
		Code* d = new Code(OpCode::PARAM, temp, "", "");
		root.code = add(root.code, d);
	}break;

	}

}

/*

int main()
{
	cout << "the code genrator for if" << endl;
	Abs_parser IF_OPJ("if.txt");
	IF_OPJ.start();
	cout << "________________________________________" << endl;
	cout << "the code genrator for while" << endl;
	Abs_parser WHILE_OPJ("while.txt");
	WHILE_OPJ.start();
	cout << "________________________________________" << endl;

	system("pause");
	return 0;
}
*/









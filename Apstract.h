#include<string>
#include"Scanner.h"
#include<vector>
#include"Symbol.h"
enum OpCode { ADD, SUB, MULT, DIV, LTHAN, LEPREQ, NEQ, EQ, GTHAN, GEOPEQ, AND, OR,OPUNDEFIEND,
	XOR, NEG, NOT, ASSIGN, BIF, BNIF ,IFN, IFLT, IFLE, IFNE, IFEQ, 
	IFGE, IFGT, GOTO, READ, WRITE,CALLOP, HALT,PARAM,LABEL };

class Code {
public:
	OpCode op;
	Code* next;
	string s1, s2, dest;


	Code(OpCode n, string m1 = "", string m2 = "", string m3 = "") // Constructor
	{
		op = n;
		s1 = m1;
		s2 = m2;
		dest = m3;
		next = NULL;
	}

	Code* add(Code* c1, Code* c2)
	{
		Code* temp = c1;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = c2;
		return c1;
	}
	
};

enum NodeType {
	PROGRAMM,BLOCK,DEC_SEQ,DEC,TYPE,PARAM_LIST,NAME_LIST,COM_SEQ,COM,EXP,EXP1,EXP2,
	EXP3,EXP4,TERM,ELEMENT,RELATION,WEAKOP,STRONGOP,NUMERAL, undefined_node,IDN
};
enum DecType {VARIBLE,PROC,VAR_ID,undefiend_dec};
enum TypeofType {INTEGER,BOOLEAN,undefined_type};
enum Commtype {ASSIGNN,READD,WRITEE,IF,WHILE,CALLL,undefind_comm};
enum ExperType{ORR,ANDD,NOTT, RELATIONn,WEAK,STRONG,undefiend_expr};
enum ElementType{ID,NUMBERR,EXPER,MIN,undefiend_element};

struct Abs_Node {
public:
	NodeType nodetype;
	DecType dectype;
	TypeofType typeoft;
	Commtype commtype;
	ExperType expertype;
	ElementType elementtype;
	string name;
	vector<Abs_Node>child;
	Code* code;
	
	Abs_Node()
	{

	}
	
	Abs_Node(int n)
	{
		nodetype = undefined_node;
		dectype = undefiend_dec;
		typeoft = undefined_type;
		commtype = undefind_comm;
		expertype = undefiend_expr;
		elementtype = undefiend_element;
			name = "";
			for (int i = 0; i < n; i++)
				child.push_back(NULL);
			code = NULL;
	}
	
};
class Abs_parser {
public:
	void start();
	
	Scanner scan;
	Token curret_token;
	Abs_parser(string filename);
	Abs_Node program();
	void match(TokenType TK);
	void Syntax_error(TokenType TK);
	Abs_Node block();
	Abs_Node dec_seq();
	Abs_Node command_seq();
	Abs_Node dec();
	Abs_Node type();
	Abs_Node name_list();
	Abs_Node param_list();
	Abs_Node command();
	Abs_Node expr();
	Abs_Node expr1();
	Abs_Node expr2();
	Abs_Node expr3();
	Abs_Node expr4();
	Abs_Node relation();
	Abs_Node term();
	Abs_Node weakop();
	Abs_Node strongop();
	Abs_Node element();



	
};
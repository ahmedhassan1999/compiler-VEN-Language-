#include"Scanner.h" 
#include<string>
Scanner::Scanner()
{

}
Token Scanner::checkReserved(string name)
{
	Token tk;
	if (name == "begin") { tk.tname = "begin"; tk.ttype = BEGIN_SY; }
	else if (name == "end if") { tk.tname = "end if"; tk.ttype = ENDIF_SY; }
	else if (name == "else if") { tk.tname = "else if"; tk.ttype = ELSEIF; }
	else if (name == "end while") { tk.tname = "end while"; tk.ttype = ENDWHILE_SY; }
	else if (name == "end") { tk.tname = "end"; tk.ttype = END_SY; }
	else if (name == "proc") { tk.tname = "proc"; tk.ttype = PROC_SY; }
	else if (name == "integer") { tk.tname = "integer"; tk.ttype = INTEGER_SY; }
	else if (name == "boolean") { tk.tname = "boolean"; tk.ttype = BOOLEAN_SY; }
	else if (name == "read") { tk.tname = "read"; tk.ttype = READ_SY; }
	else if (name == "write") { tk.tname = "write"; tk.ttype = WRITE_SY; }
	else if (name == "if") { tk.tname = "if"; tk.ttype = IF_SY; }
	else if (name == "then") { tk.tname = "then"; tk.ttype = THEN_SY; }
	else if (name == "else") { tk.tname = "else "; tk.ttype = ELSE_SY; }
	else if (name == "while") { tk.tname = "while"; tk.ttype = WHILE_SY; }
	else if (name == "do") { tk.tname = "do"; tk.ttype = DO_SY; }
	else if (name == "call") { tk.tname = "call"; tk.ttype = CALL_SY; }
	else if (name == "or") { tk.tname = "or"; tk.ttype = OR_SY; }
	else if (name == "and") { tk.tname = "and"; tk.ttype = AND_SY; }
	else if (name == "not") { tk.tname = "not"; tk.ttype = NOT_SY; }
	else if (name == "program") { tk.tname = "program"; tk.ttype = PROGRAM; }
	else if (name == "is") { tk.tname = "is"; tk.ttype = IS; }
	else { tk.tname = name; tk.ttype = ID_SY; return tk; }
	return tk;
}
Scanner::Scanner(string filename) {
	f.open(filename.c_str());
	if (!f) {
		cerr << "Unable to open file" << endl;
		system("pause");
		exit(1);
	}
}


Token Scanner::getToken() {
	char ch;
	string s;
	Token tk;
	s = "";
	string  comment;
	f.get(ch);
	while (isspace(ch)) { f.get(ch); } // to ignore whitespace
	while (!f.eof()) {
		if (ch == '#')
		{
			tk.ttype = STOP;
			//cout << tk.getTokenType();
			return tk;
		}

		else if (ch == ',') { tk.ttype = COMMA_SY; return tk; }
		else if (ch == ':') {
			f.get(ch);
			if (ch == '=')
			{

				tk.ttype = AHMED; return tk;
			}
			f.putback(ch);

		}
		else if (ch == ';') { tk.ttype = SEMICOLON_SY; return tk; }
		else if (ch == '(') { tk.ttype = LPAREN_SY; return tk; }
		else if (ch == ')') { tk.ttype = RPAREN_SY; return tk; }
		else if (ch == '-') { tk.ttype = MINUS_SY; return tk; }
		else if (ch == '{') { tk.ttype = LEFTPARANT; return tk; }
		else if (ch == '}') { tk.ttype = RIGHTPARANT; return tk; }


		else if (ch == '+') { tk.ttype = PLUS_SY; return tk; }
		else if (ch == '*') { tk.ttype = MULTI_SY; return tk; }

		else if (ch == '/') {
			f.get(ch);
			if (ch == '/')
			{
				getline(f, comment);
				tk.ttype = COMMENT_SY;
				return tk;
			}
			else if (ch == '*')
			{
				while (true)
				{
					if (ch == '*')
					{
						f.get(ch);
						if (ch == '/')
						{
							tk.ttype = COMMENT_SY;
							return tk;
							break;

						}
					}
					f.get(ch);
				}

			}
			else { tk.ttype = DIV_SY; return tk; }
		}
		else if (ch == '!')
		{
			f.get(ch);
			if (ch == '=')
			{
				tk.ttype = NOTEQUAL_SY;
				return tk;
			}
		}

		else if (ch == '=')
		{

			tk.ttype = EQUAL_SY;  return tk;
		}
		else if (ch == '>') {
			f.get(ch);
			if (ch == '=') { tk.ttype = LOE_SY; return tk; }
			else { f.putback(ch); tk.ttype = LTHAN_SY;  return tk; }
		}


		else if (ch == '<') {
			f.get(ch);
			if (ch == '=') { tk.ttype = GOE_SY; return tk; }
			//else if (ch == '>') { tk.ttype= NOTEQUAL_SY; return tk; }
			else { f.putback(ch);  tk.ttype = GTHAN_SY;  return tk; }
		}


		else if (isalpha(ch)) {
			s = ch;
			f.get(ch);
			while (isalnum(ch) && !f.eof()) {
				s += ch;
				f.get(ch);
			}
			f.putback(ch);
			if (s == "end" || s == "else")
			{


				string s1 = " ";
				//f.get(ch);
				
				
			
				f.get(ch);
				if (ch != 'i' && ch != 'w')
				{
					f.putback(ch);
					return checkReserved(s);
				}

				else if (ch == 'i' || ch == 'w') {

					while (isalpha(ch))
					{
						s1 += ch;
						f.get(ch);
					}
					s += s1;
					return checkReserved(s);
				}

			}
			else return checkReserved(s);

		}
		else if (isdigit(ch)) {
			s = ch;
			f.get(ch);
			while (isdigit(ch)) {
				s += ch;
				f.get(ch);
			}
			f.putback(ch); {tk.ttype = NUMBER_SY; tk.tname = s; return tk; }
		}

		else {
			tk.ttype = ERROR_SY; return tk;
		}  // LAST CASE



	}

}
void Scanner::displayToken()
{
	Token tk = getToken();
	while (true)

	{
		if (tk.ttype == STOP)
		{
			cout << tk.getTokenType() << endl;
			break;
		}

		cout << tk.getTokenType() << endl;
		//cout << tk.tname << endl;
		tk = getToken();
	}
}
/*int main()
{

	Scanner scan("scope.txt");
	scan.displayToken();

}*/
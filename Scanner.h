
#include<fstream>
#include"Token.h"
  

class Scanner {

	private:
		ifstream f;
		Token checkReserved(string tname);
public:
	Scanner();
	Scanner(string filename);
	Token getToken();
	void displayToken();
	



};
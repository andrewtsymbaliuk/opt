#include "LexAnaliz.h"

LeksAnaliz::LeksAnaliz()
{
	LexCounter = 1;

	Line = 1;
	Column = 1;
	ConstMax = 999;
	KeyWordsMin = 401;
	MinConst = 501;
	ConstCounter = MinConst;
	MinIdent = 1001;
	IdentCounter = MinIdent;
	LexErrorCode = 1000;
	Lex buf;
	buf.Name = "PROGRAM";
	buf.Code = 401;
	KeyWords.push_back(buf);
	buf.Name = "BEGIN";
	buf.Code = 410;
	KeyWords.push_back(buf);
	buf.Name = "END";
	buf.Code = 411;
	KeyWords.push_back(buf);
	buf.Name = "LOOP";
	buf.Code = 440;
	KeyWords.push_back(buf);
	buf.Name = "ENDLOOP";
	buf.Code = 441;
	KeyWords.push_back(buf);
	buf.Name = "CASE";
	buf.Code = 450;
	KeyWords.push_back(buf);
	buf.Name = "ENDCASE";
	buf.Code = 451;
	KeyWords.push_back(buf);
	buf.Name = "OF";
	buf.Code = 455;
	KeyWords.push_back(buf);
	buf.Name = "MOD";
	buf.Code = 470;
	KeyWords.push_back(buf);
	buf.Name = "IF";
	buf.Code = 498;
	KeyWords.push_back(buf);
	buf.Name = "THEN";
	buf.Code = 499;
	KeyWords.push_back(buf);
	buf.Name = "ENDIF";
	buf.Code = 497;
	KeyWords.push_back(buf);

	for (int i = 0; i < 128; i++) {
		if ((i == 9) || (i == 10) || (i == 32) || (i == 12) || (i == 13))
			ASCIIArr[i] = 0;
		else {
			if ((i < 91) && (i > 64))
				ASCIIArr[i] = 1;
			else {
				if ((i < 58) && (i > 47))
					ASCIIArr[i] = 2;
				else {
					if ((i == ')') || (i == '(') || (i == '*') || (i == '.') || (i == ':') || (i == ';') || (i == '/') || (i == '\\') || (i=='[') || (i=='='))
						ASCIIArr[i] = 3;
					else {
						ASCIIArr[i] = 4;
					}
				}
			}
		}
	}

}

void LeksAnaliz::AddLeksem(int Code, string Name, int Line, int Column)
{
	Lex buf;
	buf.Code = Code;
	buf.Line = Line;
	buf.Column = Column;
	buf.Name = Name;
	Lexems.push_back(buf);
}

bool LeksAnaliz::SizeOut()
{
	return !t.eof();
}

void LeksAnaliz::Analizator(string filename)
{
	t.open(filename);
	if (!t.is_open()) {
		std::cout << "failed to open " << filename << '\n';
	}
	Pos = 0;
	Line = 1;
	Column = 1; 
	sbuff = t.get();
	if (SizeOut())
		INP();
	MakeListing(filename);
}

void LeksAnaliz::INP()
{
	while (SizeOut())
	{
		switch (ASCIIArr[sbuff])
		{
		case 0:
			SEP();
			break;
		case 1:
			IDN();
			break;
		case 2:
			CONST();
			break;
		case 3:
			DM();
			break;
		case 4:
			ERR("");
			break;
		}
	}
}

void LeksAnaliz::SEP()
{
	if (SizeOut())
	{
		while((SizeOut())&&(ASCIIArr[sbuff] == 0))
		{
			if (sbuff == 10)
			{
				Pos++; Column++;
				sbuff = t.get();
				Line++;
				Column = 1;
			}
			else { 
				Pos++; Column++; 
				sbuff = t.get();
			}
		}
		return;
	}
}

void LeksAnaliz::CONST()
{
	int tmp;
	string bb = "";
	SaveLine = Line;
	SaveColumn = Column;
	string Buf = "";
	while ((SizeOut())&&(ASCIIArr[sbuff] == 2))
	{
		Buf += sbuff;
		Pos++;
		sbuff = t.get();
		Column++;
	}

	switch(ASCIIArr[sbuff])
	{
	case 0:
	case 3:
		{
		// ���� �������� ���
		if (sbuff == '.') {
			tmp = stoi(Buf);
			/*if (tmp < 1900) {
				ERR(Buf);
				break;
			}*/
			Buf += sbuff;
			Pos++;
			sbuff = t.get();
			Column++;
			while ((SizeOut()) && (ASCIIArr[sbuff] == 2))
			{
				Buf += sbuff;
				bb += sbuff;
				Pos++;
				sbuff = t.get();
				Column++;
			}
			if (sbuff == '.') {
				tmp = stoi(bb);
				/*if ((tmp > 12) || (tmp < 1)) {
					ERR(Buf);
					break;
				}*/
				Buf += sbuff;
				bb = "";
				Pos++;
				sbuff = t.get();
				Column++;
			}
			while ((SizeOut()) && (ASCIIArr[sbuff] == 2))
			{
				Buf += sbuff;
				bb += sbuff;
				Pos++;
				sbuff = t.get();
				Column++;
			}
			/*if ((tmp > 31) || (tmp < 1)) {
				ERR(Buf);
				break;
			}*/
			int m = SearchConst(Buf);
			if (m == -1)
			{
				AddLeksem(ConstCounter, Buf, SaveLine, SaveColumn);
				TableC.push_back(Buf);
				ConstCounter++;
			}
			else
			{
				AddLeksem(m + MinConst, Buf, SaveLine, SaveColumn);
			}
			Buf = "";
			return;
		}
		// ����� �������� ����
		int n = SearchConst(Buf);
		if (n == -1)
		{
			AddLeksem(ConstCounter, Buf, SaveLine, SaveColumn);
			TableC.push_back(Buf);
			ConstCounter++;
		}
		else
		{
			AddLeksem(n + MinConst, Buf, SaveLine, SaveColumn);
		}
		Buf = "";
		return;
		}
		break;
	case 1:
		ERR(Buf);
		break;
	case 4:
		ERR(Buf);
		break;
	}
}

void LeksAnaliz::IDN()
{
	int n;
	SaveLine = Line;
	SaveColumn = Column;
	string Buf = "";
	while ((SizeOut())&&((ASCIIArr[sbuff] == 2)||(ASCIIArr[sbuff] == 1)))
	{
		Buf += sbuff;
		Pos++;
		sbuff = t.get();
		Column++;
	}

	if (ASCIIArr[sbuff] == 4)
	{
		ERR(Buf);
	}
	else
	{
		n = SearchStandartIdent(Buf);
		if (n == -1) {
			n = SearchIdent(Buf);
			if (n == -1)
			{
				AddLeksem(IdentCounter, Buf, SaveLine, SaveColumn);
				Table.push_back(Buf);
				IdentCounter++;
			}
			else
			{
				AddLeksem(n + MinIdent, Buf, SaveLine, SaveColumn);
			}
		}
		else AddLeksem(KeyWords[n].Code, KeyWords[n].Name, SaveLine, SaveColumn);
		Buf = "";
		return;
	}
}

void LeksAnaliz::BCOM()
{
	if (sbuff =='*')
	{
		string Buf = "";
		Pos++;
		sbuff = t.get();
		Column++;
		COM();
	}
	else
	{
		ERR("(");
	}
}

void LeksAnaliz::COM()
{
	string Buf = "";
	while ((SizeOut())&&(sbuff != '*'))
	{
		if (sbuff == '\n')
		{
			Line++;
			Column = 1;
			Pos++;
			sbuff = t.get();
		}
		else {
			Pos++; 
			sbuff = t.get();
			Column++;
		}
	}
	if  ((SizeOut()) && (sbuff == '*')) ECOM();
}

void LeksAnaliz::ECOM()
{
	while (sbuff == '*' )
	{
		Pos++;
		sbuff = t.get();
		Column++;
	}
	if ((SizeOut())&&(sbuff == ')'))
	{
		Pos++;
		sbuff = t.get();
		Column++;
		return;
	}
	else COM();
}

void LeksAnaliz::DM()
{
	SaveLine = Line;
	SaveColumn = Column;
	string Buf = "";

	if (sbuff == '(')
	{
		Buf += sbuff;
		Pos++;
		sbuff = t.get();
		Column++;
		if (sbuff == '*') {
			BCOM();
		}
		else {
			AddLeksem('(', Buf, Line, Column-1);
			return;
		}
	}
	else {
		// ��� �������� �����
		int tmp;
		string bb = "";
		if (sbuff == '[') {
			Buf += sbuff;
			Pos++;
			sbuff = t.get();
			Column++;
			while ((SizeOut()) && (ASCIIArr[sbuff] == 2))
			{
				Buf += sbuff;
				bb += sbuff;
				Pos++;
				sbuff = t.get();
				Column++;
			}
			tmp = stoi(bb);
			if ((tmp > 24) || (tmp < 0)) {
				ERR(Buf);
				return;
			}
			bb = "";
			if (sbuff == ':') {
				Buf += sbuff;
				Pos++;
				sbuff = t.get();
				Column++;
			}
			while ((SizeOut()) && (ASCIIArr[sbuff] == 2))
			{
				Buf += sbuff;
				bb += sbuff;
				Pos++;
				sbuff = t.get();
				Column++;
			}
			tmp = stoi(bb);
			if ((tmp > 60) || (tmp < 0)) {
				ERR(Buf);
				return;
			}
			bb = "";
			if (sbuff == ']') {
				Buf += sbuff;
				Pos++;
				sbuff = t.get();
				Column++;
			}
			int m = SearchConst(Buf);
			if (m == -1)
			{
				AddLeksem(ConstCounter, Buf, SaveLine, SaveColumn);
				TableC.push_back(Buf);
				ConstCounter++;
			}
			else
			{
				AddLeksem(m + MinConst, Buf, SaveLine, SaveColumn);
			}
			Buf = "";
			return;
		}
		else {
			Buf += sbuff;
			AddLeksem(sbuff, Buf, Line, Column);
			Pos++;
			sbuff = t.get();
			Column++;
			return;
		}
	}
}

void LeksAnaliz::ERR(string pt)
{
	string Buf = pt;
	if ((sbuff == '*') || (sbuff == ')'))
	{
		Buf += sbuff;
		AddLeksem(LexErrorCode, Buf, SaveLine, SaveColumn); 
		Pos++;
		sbuff = t.get();
		Column++;
		Buf = "";
		INP();
	}
	else
	{
		while ((ASCIIArr[sbuff] != 0) && (ASCIIArr[sbuff] != 3) && (SizeOut()))
		{
			Buf += sbuff;
			Pos++;
			sbuff = t.get();
			Column++;
		}
	
		AddLeksem(LexErrorCode, Buf, SaveLine, SaveColumn);
		Buf = "";
		INP();
	}
}

int LeksAnaliz::SearchIdent(string Ident)
{
	for (int i = 0; i < Table.size(); i++)
	{
		if (Table[i] == Ident)
			return i;
	}
	return -1;
}

int LeksAnaliz::SearchStandartIdent(string Ident) 
{
	for (int i = 0; i < KeyWords.size(); i++)
	{
		if (KeyWords[i].Name == Ident)
			return i;
	}
	return -1;
}

int LeksAnaliz::SearchConst(string Const)
{
	for (int i = 0; i < TableC.size(); i++)
	{
		if (TableC[i] == Const)
			return i;
	}
	return -1;
}

void LeksAnaliz::MakeListing(string filename)
{
	filename = filename + ".lst";
	ofstream f(filename);
	f << setw(10) << "Code" << setw(20) << "Name" << setw(10) << "Line" << setw(10) << "Column" << endl;
	f << endl;
	for (unsigned int i = 0; i < Lexems.size(); i++)
	{
		f << setw(10) << Lexems[i].Code << setw(20) << Lexems[i].Name << setw(10) << Lexems[i].Line << setw(10) << Lexems[i].Column << endl;
	}
	f << endl;
	f << endl;
	int p = 0;
	for (unsigned int i = 0; i < Lexems.size(); i++)
	{
		if (Lexems[i].Code == LexErrorCode) {
			f << "Lexical error on Line " << Lexems[i].Line << " Column " << Lexems[i].Column << ": Impossible combination of characters" << endl;
			p++;
		}
	}
	f.close();
	if (p == 0) {
		cout << "lexical analysis completed successfully" << endl;
	}
	else {
		for (unsigned int i = 0; i < Lexems.size(); i++)
		{
			if (Lexems[i].Code == LexErrorCode) {
				cout << "Lexical error on Line " << Lexems[i].Line << " Column " << Lexems[i].Column << ": Impossible combination of characters" << endl;
			}
		}
	}
}

LeksAnaliz::~LeksAnaliz() {

}

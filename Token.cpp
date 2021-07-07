#include"Token.h"

Token::Token()
{
	tname = " ";
}
string Token::getTokenType()
{
	
	switch (ttype) {
	case STOP:    return " you are at the end file !"; break;
	case BEGIN_SY:   return "begin"; break;
	case END_SY:     return "end"; break;
	case PROGRAM:    return "program"; break;
	case AHMED:    return ":="; break;
	case IS:			return "is"; break;
	case PROC_SY:      return "proc"; break;
	case INTEGER_SY:    return "integer"; break;
	case BOOLEAN_SY:     return "boolean"; break;
	case SEMICOLON_SY:    return ";"; break;
	case COLON_SY:       return ":"; break;
	case ID_SY:          return tname; break;
	case COMMA_SY:    return  " ," ; break;
	case READ_SY:       return "read"; break;
	case WRITE_SY:     return "write"; break;
	case ELSEIF:		return "else if"; break;
	case IF_SY:    return "if"; break;
	case THEN_SY:    return "then"; break;
	case ENDIF_SY:   return "end if"; break;
	case ELSE_SY:     return "else "; break;
	case WHILE_SY:     return "while"; break;
	case DO_SY:      return "do"; break;
	case ENDWHILE_SY:        return "end while"; break;
	case CALL_SY:            return "call"; break;
	case LPAREN_SY:        return "("; break;
	case RPAREN_SY:        return ")"; break;
	case OR_SY:        return  "or"; break;
	case LEFTPARANT:   return "{"; break;
	case RIGHTPARANT:   return "}"; break;
	case COMMENT_SY:    return " "; break;
	case AND_SY:          return "and"; break;
	case NOT_SY:          return "not"; break;
	case MINUS_SY:      return "-"; break;
	case PLUS_SY:       return "+"; break;
	case DIV_SY:         return "/"; break;
	case MULTI_SY:       return "*"; break;
	case GTHAN_SY:       return "<"; break;
	case LTHAN_SY:         return ">"; break;
	case GOE_SY:         return "<="; break;
	case LOE_SY:           return ">="; break;
	case NOTEQUAL_SY:       return "!="; break;
	case EQUAL_SY:        return "="; break;
	case NUMBER_SY:     return  "NUMBER"; break;

	case EOF_SY:       return "end"; break;
	default: return  "Undefined Token!";
	}
}
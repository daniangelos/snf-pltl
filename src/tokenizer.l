/************************************************************
 *    Copyright (C) 2008-2009                               *
 *    Michel Ludwig (michel.ludwig@liverpool.ac.uk)         *
 *    University of Liverpool                               *
 *                                                          *
 *    This program is free software; you can redistribute   *
 *    it and/or modify it under the terms of the GNU        *
 *    General Public License as published by the Free       *
 *    Software Foundation; either version 3 of the License, *
 *    or (at your option) any later version.                *
 *                                                          *
 *    This program is distributed in the hope that it will  *
 *    be useful, but WITHOUT ANY WARRANTY; without even     *
 *    the implied warranty of MERCHANTABILITY or FITNESS    *
 *    FOR A PARTICULAR PURPOSE.  See the GNU General Public *
 *    License for more details.                             *
 *                                                          *
 *    You should have received a copy of the GNU General    *
 *    Public License along with this program; if not, see   *
 *    <http://www.gnu.org/licenses/>.                       *
 ************************************************************/
 
%{
#include <string.h>

#include "parser.tab.h"
%}

%option noyywrap

%%
&                        return AND;
and                      return AND_TEXT;
\x7c                     return OR;
or                       return OR_TEXT;
~                        return NOT;
not                      return NOT;
always                   return ALWAYS;
next                     return NEXT;
sometime                 return SOMETIME;
until                    return UNTIL;
unless                   return UNLESS;
\<=>                     return EQUIVALENCE;
\<->                     return EQUIVALENCE;
=>                       return IMPLICATION;
->                       return IMPLICATION;
\(                       return LEFT_BRACKET;
\)                       return RIGHT_BRACKET;
[tT][rR][uU][eE]         return TRUE;
[fF][aA][lL][sS][eE]     return FALSE;
[[:alpha:]][[:alnum:]_]* yylval.cstring = strdup(yytext); return NAME;
\n                       /* ignore end of line */;
\%.*                     /* comment */;
[ \t]+                   /* ignore whitespace */;
,                        return COMMA;
\[                       return LEFT_SQUARE_BRACKET;
\]                       return RIGHT_SQUARE_BRACKET;
.                        /* ignore dots */;
%%

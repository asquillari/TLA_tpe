%{
#include "BisonActions.h"
%}

%define api.value.union.name SemanticValue

%union {
	Token token;
	char* string;
	int integer;

	struct Node* node;

	struct Program* program;
	struct StatementList* stmt_list;
	struct Define* define;
	struct Use* use;
	struct Form* form;
	struct Footer* footer;
	struct Row* row;
	struct Column* column;
	struct Nav* nav;
	struct Text* text;
	struct Image* image;
    struct Button* button;
    struct Card* card;
	struct List* list;
	struct ListItem* list_item;
	struct OrderedList* ordered_list;
	struct UnorderedList* unordered_list;
	struct ParameterList* parameter_list;
	struct ListItem*     list_item_list;
}

%token <token> DEFINE USE FORM IMG FOOTER ROW COLUMN NAV ITEM END BUTTON CARD
%token <token> OPEN_PAREN CLOSE_PAREN OPEN_BRACE CLOSE_BRACE COLON COMMA EQUALS OPEN_BRACKET CLOSE_BRACKET PIPE QUOTE
%token <token> NEWLINE UNKNOWN HEADER_1 HEADER_2 HEADER_3
%token <string> ORDERED_ITEM BULLET
%token <string> QUOTED_VALUE UNQUOTED_VALUE IDENTIFIER TEXT
%token <string> VARIABLE
%token <string> ORDERED_LIST BULLET_LIST

%type <program> program
%type <node> statement atomic_statement
%type <stmt_list> statement_list statement_list_nonempty
%type <ordered_list>   ordered_list_statement
%type <unordered_list> unordered_list_statement
%type <define> define
%type <use> use
%type <form> form
%type <footer> footer
%type <row> row
%type <column> column
%type <nav> nav
%type <image> image
%type <text> text
%type <button> button
%type <card> card

%type <parameter_list> maybe_parameters parameters parameter_list

%%

program:
    statement_list
    { $$ = ProgramSemanticAction(currentCompilerState(), $1); }
;

statement_list:
    /* empty */ { $$ = NULL; }
    | statement_list_nonempty { $$ = $1; }
;

atomic_statement:
    NEWLINE                            { $$ = NULL; }
  | define                             { $$ = (struct Node*)$1; }
  | use                                { $$ = (struct Node*)$1; }
  | form                               { $$ = (struct Node*)$1; }
  | footer                             { $$ = (struct Node*)$1; }
  | row                                { $$ = (struct Node*)$1; }
  | column                             { $$ = (struct Node*)$1; }
  | nav                                { $$ = (struct Node*)$1; }
  | image                              { $$ = (struct Node*)$1; }
  | text                               { $$ = (struct Node*)$1; }
  | button                             { $$ = (struct Node*)$1; }
  | card                               { $$ = (struct Node*)$1; }
;

 statement:
     atomic_statement                   { $$ = $1; }
   | ordered_list_statement             { $$ = (struct Node*)$1; }
   | unordered_list_statement           { $$ = (struct Node*)$1; }
 ;

 statement_list_nonempty:
     statement                          { $$ = SingleStatementSemanticAction($1); }
   | statement_list_nonempty statement  { $$ = AppendStatementSemanticAction($1, $2); }
 ;

define:
    DEFINE IDENTIFIER maybe_parameters statement_list_nonempty END
    {
        $$ = DefineSemanticAction($2, $3, $4);
    }
;

use:
    USE IDENTIFIER OPEN_PAREN parameters CLOSE_PAREN
    {
        $$ = UseSemanticAction($2, $4);
    }
    | USE IDENTIFIER
    {
        $$ = UseSemanticAction($2, NULL);
    }
;

maybe_parameters:
    /* empty */ { $$ = EmptyParameterListSemanticAction(); }
    | OPEN_PAREN parameters CLOSE_PAREN { $$ = $2; }
;

parameters:
    parameter_list
;

parameter_list:
      IDENTIFIER COLON IDENTIFIER EQUALS QUOTED_VALUE
    { $$ = SingleParameterSemanticAction($1, $3, $5); }
    | IDENTIFIER COLON IDENTIFIER EQUALS QUOTED_VALUE COMMA parameter_list
    { $$ = AppendParameterSemanticAction($7, $1, $3, $5); }
;

text:
      HEADER_1 TEXT { $$ = TextSemanticAction($2, 1); }
    | HEADER_2 TEXT { $$ = TextSemanticAction($2, 2); }
    | HEADER_3 TEXT { $$ = TextSemanticAction($2, 3); }
    | TEXT { $$ = TextSemanticAction($1, 0); }
;

ordered_list_statement:
    ORDERED_LIST
    { $$ = OrderedListSemanticAction($1); }
;

unordered_list_statement:
    BULLET_LIST
    { $$ = UnorderedListSemanticAction($1); }
;


image:
    IMG OPEN_PAREN parameters CLOSE_PAREN
    { $$ = ImageSemanticAction($3); }
;

form:
    FORM IDENTIFIER OPEN_PAREN parameters CLOSE_PAREN OPEN_PAREN parameters CLOSE_PAREN statement_list_nonempty END
    { $$ = FormSemanticAction($2, $4, $7, $9); }
  | FORM IDENTIFIER OPEN_PAREN parameters CLOSE_PAREN statement_list_nonempty END
    { $$ = FormSemanticAction($2, $4, NULL, $6); }
  | FORM IDENTIFIER statement_list_nonempty END
    { $$ = FormSemanticAction($2, NULL, NULL, $3); }
;


footer:
    FOOTER OPEN_BRACE parameters CLOSE_BRACE statement_list_nonempty END
    { $$ = FooterSemanticAction($3, $5); }
;

row:
    ROW statement_list_nonempty END
    { $$ = RowSemanticAction($2); }
;

column:
    COLUMN OPEN_BRACE parameters CLOSE_BRACE statement_list_nonempty END
    { $$ = ColumnSemanticAction($3, $5); }
;

 nav:
     NAV OPEN_BRACE parameters CLOSE_BRACE unordered_list_statement END
     {
         UnorderedList* ul = (UnorderedList*)$5;
         $$ = NavSemanticAction($3, ul->items);
     }
   | NAV unordered_list_statement END
     {
         UnorderedList* ul = (UnorderedList*)$2;
         $$ = NavSemanticAction(NULL, ul->items);
     }
 ;


button:
    BUTTON OPEN_BRACE parameters CLOSE_BRACE statement_list_nonempty END
    { $$ = ButtonSemanticAction($3, $5); }
;

card:
    CARD OPEN_BRACE parameters CLOSE_BRACE statement_list_nonempty END
    { $$ = CardSemanticAction($3, $5); }
;

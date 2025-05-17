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
}



// -------------------------------
// TOKENS TERMINALES
// -------------------------------
%token <token> DEFINE USE ENDDEFINE END FORM IMG FOOTER ROW COLUMN NAV
%token <token> COLON COMMA EQUALS OPEN_PAREN CLOSE_PAREN 
%token <token> OPEN_BRACE CLOSE_BRACE OPEN_BRACKET CLOSE_BRACKET PIPE QUOTE LIST_CONTENT
%token <token> NEWLINE UNKNOWN
%token <token> BUTTON CARD

%token <string> TEXT STYLE_CONTENT IDENTIFIER UNQUOTED_VALUE QUOTED_VALUE KEY
%token <string> ORDERED_ITEM BULLET
%token <token> HEADER_1 HEADER_2 HEADER_3

// -------------------------------
// TIPOS DE DATOS PARA NO-TERMINALES
// -------------------------------
%type <program> program
%type <node> statement
%type <stmt_list> statement_list
%type <stmt_list> statement_list_nonempty

%type <define> define
%type <use> use
%type <form> form
%type <footer> footer
%type <row> row
%type <column> column
%type <nav> nav
%type <ordered_list> ordered_list_statement
%type <unordered_list> unordered_list_statement
%type <image> image
%type <text> text
%type <button> button
%type <card> card


%type <parameter_list> maybe_parameters
%type <parameter_list> parameters
%type <parameter_list> parameter_list
%type <list_item> ordered_items
%type <list_item> ordered_items_tail
%type <list_item> bullet_items
%type <list_item> bullet_items_tail



%%

// Reglas de producción
program:
    statement_list
    {
        $$ = ProgramSemanticAction(currentCompilerState(), $1);
    }
;

statement_list:
    /* vacío */
    {
        $$ = NULL;
    }
    | statement_list_nonempty
    {
        $$ = $1;
    }
;

statement_list_nonempty:
      statement
    {
        if ($1 == NULL)
          $$ = NULL;
        else
          $$ = SingleStatementSemanticAction($1);
    }
    | statement_list_nonempty statement
    {
        if ($2 == NULL)
          $$ = $1;
        else if ($1 == NULL)
          $$ = SingleStatementSemanticAction($2);
        else
          $$ = AppendStatementSemanticAction($1, $2);
    }
;


statement:
    NEWLINE { $$ = NULL; }
  | define { $$ = (struct Node*)$1; }
  | use    { $$ = (struct Node*)$1; }
  | form   { $$ = (struct Node*)$1; }
  | footer { $$ = (struct Node*)$1; }
  | row    { $$ = (struct Node*)$1; }
  | column { $$ = (struct Node*)$1; }
  | nav    { $$ = (struct Node*)$1; }
  | ordered_list_statement   { $$ = (struct Node*)$1; }
  | unordered_list_statement { $$ = (struct Node*)$1; }
  | image  { $$ = (struct Node*)$1; }
  | text   { $$ = (struct Node*)$1; }
  | button { $$ = (struct Node*)$1; }
  | card   { $$ = (struct Node*)$1; }
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
    /* vacío */
    {
        $$ = EmptyParameterListSemanticAction();
    }
    | OPEN_PAREN parameters CLOSE_PAREN
    {
        $$ = $2;
    }
;

parameters:
    parameter_list
;

parameter_list:
      IDENTIFIER COLON IDENTIFIER EQUALS QUOTED_VALUE
    {
        $$ = SingleParameterSemanticAction($1, $3, $5);
    }
    | IDENTIFIER COLON IDENTIFIER EQUALS QUOTED_VALUE COMMA parameter_list
    {
        $$ = AppendParameterSemanticAction($7, $1, $3, $5);
    }
;

text:
    HEADER_1 TEXT
    {
        $$ = TextSemanticAction($2, 1);
    }
    | HEADER_2 TEXT
    {
        $$ = TextSemanticAction($2, 2);
    }
    | HEADER_3 TEXT
    {
        $$ = TextSemanticAction($2, 3);
    }
    | TEXT
    {
        $$ = TextSemanticAction($1, 0); // 0 = texto plano
    }
;

ordered_list_statement:
    OPEN_BRACE ordered_items CLOSE_BRACE
    {
        $$ = OrderedListSemanticAction($2);
    }
;

unordered_list_statement:
    OPEN_BRACE bullet_items CLOSE_BRACE
    {
        $$ = UnorderedListSemanticAction($2);
    }
;

ordered_items:
    ORDERED_ITEM ordered_items_tail
    {
        $$ = PrependOrderedItemSemanticAction(createListItem($1), $2->content);
    }
;

ordered_items_tail:
    ORDERED_ITEM ordered_items_tail
    {
        $$ = PrependOrderedItemSemanticAction(createListItem($1), $2->content);
    }
  | /* vacío */
    {
        $$ = EmptyOrderedItemListSemanticAction();
    }
;

bullet_items:
    BULLET bullet_items_tail
    {
        $$ = PrependBulletItemSemanticAction($1, $2);
    }
;

bullet_items_tail:
    BULLET bullet_items_tail
    {
        $$ = PrependBulletItemSemanticAction($1, $2);
    }
  | /* vacío */
    {
        $$ = EmptyBulletItemListSemanticAction();
    }
;

image:
    IMG QUOTED_VALUE QUOTED_VALUE
    {
        $$ = ImageSemanticAction($2, $3);
    }
;

form:
    FORM IDENTIFIER OPEN_PAREN parameters CLOSE_PAREN OPEN_BRACE parameters CLOSE_BRACE statement_list_nonempty END
    {
        $$ = FormSemanticAction($2, $4, $7, $9);
    }
;

footer:
    FOOTER OPEN_BRACE parameters CLOSE_BRACE statement_list_nonempty END
    {
        $$ = FooterSemanticAction($3, $5);
    }
;

row:
    ROW statement_list_nonempty END
    {
        $$ = RowSemanticAction($2);
    }
;

column:
    COLUMN OPEN_BRACE parameters CLOSE_BRACE statement_list_nonempty END
    {
        $$ = ColumnSemanticAction($3, $5);
    }
;

nav:
    NAV OPEN_BRACE parameters CLOSE_BRACE bullet_items END
    {
        $$ = NavSemanticAction($3, $5);
    }
;

button:
    BUTTON OPEN_BRACE parameters CLOSE_BRACE statement_list_nonempty END
    {
        $$ = ButtonSemanticAction($3, $5);  
    }
;

card:
    CARD OPEN_BRACE parameters CLOSE_BRACE statement_list_nonempty END
    {
        $$ = CardSemanticAction($3, $5);  
    }
;


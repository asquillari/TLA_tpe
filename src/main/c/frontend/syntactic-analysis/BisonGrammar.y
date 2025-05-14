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
	struct List* list;
	struct OrderedList* ordered_list;
	struct UnorderedList* unordered_list;
	struct ParameterList* parameter_list;
}



// -------------------------------
// TOKENS TERMINALES
// -------------------------------

// Palabras clave
%token <token> DEFINE USE ENDDEFINE END FORM IMG FOOTER ROW COLUMN NAV

// Símbolos
%token <token> COLON COMMA EQUALS OPEN_PAREN CLOSE_PAREN
%token <token> OPEN_BRACE CLOSE_BRACE OPEN_BRACKET CLOSE_BRACKET PIPE QUOTE

// Tokens de control
%token <token> NEWLINE UNKNOWN

// Texto y contenido
%token <string> TEXT
%token <string> STYLE_CONTENT
%token <string> IDENTIFIER
%token <string> UNQUOTED_VALUE
%token <string> QUOTED_VALUE
%token <string> KEY

// Listas
%token <string> ORDERED_ITEM
%token <string> BULLET

// Encabezados
%token <token> HEADER_1 HEADER_2 HEADER_3

// -------------------------------
// TIPOS DE DATOS PARA NO-TERMINALES
// -------------------------------
%type <program> program
%type <node> statement
%type <node> statement_list
%type <node> statement_list_nonempty

%type <node> define
%type <node> use
%type <node> form
%type <node> footer
%type <node> row
%type <node> column
%type <node> nav
%type <node> ordered_list
%type <node> unordered_list
%type <node> image
%type <node> text

%type <node> maybe_parameters
%type <node> parameters
%type <node> parameter_list
%type <node> ordered_items
%type <node> bullet_items
%type <node> ordered_items_tail
%type <node> bullet_items_tail

%%

// Reglas de producción
program:
    statement_list
    {
        $$ = ProgramSemanticAction(currentCompilerState(), $1);
    }
;

// Lista vacía (solo permitida en el programa)
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

// Lista no vacía
statement_list_nonempty:
      statement
    {
        $$ = SingleStatementSemanticAction($1);
    }
    | statement_list_nonempty statement
    {
        $$ = AppendStatementSemanticAction($1, $2);
    }
;

statement:
      define
    | use
    | form
    | footer
    | row
    | column
    | nav
    | ordered_list
    | unordered_list
    | image
    | text
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

ordered_list:
    ordered_items
    {
        $$ = OrderedListSemanticAction($1);
    }
;

ordered_items:
    ORDERED_ITEM ordered_items_tail
    {
        $$ = PrependOrderedItemSemanticAction($1, $2); 
    }
;

ordered_items_tail:
    ORDERED_ITEM ordered_items_tail
    {
        $$ = PrependOrderedItemSemanticAction($1, $2);
    }
  | /* vacío */
    {
        $$ = EmptyOrderedItemListSemanticAction();
    }
;


unordered_list:
    bullet_items
    {
        $$ = UnorderedListSemanticAction($1);
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

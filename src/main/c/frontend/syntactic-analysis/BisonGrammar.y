%{

#include "BisonActions.h"

%}

%define api.value.union.name SemanticValue

%union {
	Token token;
	struct Node* node; // Para los no-terminales del AST
	char* string;      // Para textos capturados desde el scanner
}

// Tokens
%token <token> DEFINE USE ENDDEFINE END FORM IMG FOOTER ROW COLUMN NAV
%token <token> ORDERED_ITEM BULLET NEWLINE UNKNOWN
%token <token> HEADER_1 HEADER_2 HEADER_3 OPEN_BRACE CLOSE_BRACE OPEN_BRACKET CLOSE_BRACKET PIPE QUOTE
%token <string> TEXT STYLE_CONTENT

// Tipos para no-terminales
%type <node> program statement statement_list define use form footer row column nav ordered_list unordered_list text image parameters maybe_parameters parameter_list ordered_items bullet_items

%%

// Reglas de producción

program
	: statement_list { $$ = $1; }
	;

statement_list
	: statement_list statement { $$ = appendStatement($1, $2); }
	| statement { $$ = createStatementList($1); }
	;

statement
	: define { $$ = $1; }
	| use { $$ = $1; }
	| form { $$ = $1; }
	| footer { $$ = $1; }
	| row { $$ = $1; }
	| column { $$ = $1; }
	| nav { $$ = $1; }
	| ordered_list { $$ = $1; }
	| unordered_list { $$ = $1; }
	| text { $$ = $1; }
	| image { $$ = $1; }
	;

define
	: DEFINE TEXT maybe_parameters statement_list ENDDEFINE {
		$$ = createDefineNode($2, $3, $4);
	}
	;

use
	: USE TEXT maybe_parameters {
		$$ = createUseNode($2, $3);
	}
	;

maybe_parameters
	: parameters { $$ = $1; }
	| /* vacío */ { $$ = NULL; }
	;

parameters
	: '(' parameter_list ')' { $$ = $2; }
	;

parameter_list
	: TEXT { $$ = createParameterListNode($1); }
	| parameter_list ',' TEXT { $$ = appendParameterToList($1, $3); }
	;

form
	: FORM TEXT statement_list END {
		$$ = createFormNode($2, $3);
	}
	;

footer
	: FOOTER TEXT statement_list END {
		$$ = createFooterNode($2, $3);
	}
	;

row
	: ROW statement_list END {
		$$ = createRowNode($2);
	}
	;

column
	: COLUMN TEXT statement_list END {
		$$ = createColumnNode($2, $3);
	}
	;

nav
	: NAV TEXT statement_list END {
		$$ = createNavNode($2, $3);
	}
	;

ordered_list
	: TEXT ordered_items {
		$$ = createOrderedListNode($1, $2);
	}
	;

ordered_items
	: ORDERED_ITEM {
		$$ = createOrderedItemList($1);
	}
	| ordered_items ORDERED_ITEM {
		$$ = appendOrderedItem($1, $2);
	}
	;

unordered_list
	: TEXT bullet_items {
		$$ = createUnorderedListNode($1, $2);
	}
	;

bullet_items
	: BULLET {
		$$ = createBulletItemList($1);
	}
	| bullet_items BULLET {
		$$ = appendBulletItem($1, $2);
	}
	;

text
	: TEXT {
		$$ = createTextNode($1);
	}
	;

image
	: IMG TEXT TEXT {
		$$ = createImageNode($2, $3);
	}
	;

%%
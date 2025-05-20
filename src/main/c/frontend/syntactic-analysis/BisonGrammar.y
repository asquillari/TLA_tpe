%{
#include "BisonActions.h"
%}

%define api.value.union.name SemanticValue

%union {
	Token token;
	char* string;


	struct Program* program;
    struct Statement* statement;
    struct StatementList* statement_list;
    struct ParameterList* parameter_list;

    struct FormItem* form_item;
    struct NavItem* nav_item;
    struct TableRow* table_row;
    struct TableCell* table_cell;
    struct TableRowList* table_row_list;
    struct TableCellList* table_cell_list;
}

%token <token> DEFINE USE FORM IMG FOOTER ROW COLUMN NAV ITEM END BUTTON CARD LIST_BEGIN TABLE_BEGIN
%token <token> OPEN_PAREN CLOSE_PAREN OPEN_BRACE CLOSE_BRACE COLON COMMA OPEN_BRACKET CLOSE_BRACKET PIPE 
%token <token> NEWLINE HEADER_1 HEADER_2 HEADER_3

%token <token>  UNKNOWN EQUALS

%token <string> ORDERED_ITEM BULLET
%token <string> QUOTED_VALUE UNQUOTED_VALUE IDENTIFIER 
%token <string> VARIABLE


%type <program> program
%type <statement> statement 
%type <statement_list> statement_list content maybe_content column_list unordered_list_items ordered_list_items

%type <parameter_list> style_parameters action_parameters
%type <parameter_list> style_parameter_list identifier_list parameters use_parameters use_parameter_list
%type <parameter_list> maybe_style maybe_action maybe_parameters maybe_use

%type <form_item> form_item form_item_list
%type <nav_item> nav_item nav_item_list

%type <statement> define
%type <statement> use
%type <statement> form
%type <statement> footer
%type <statement> row
%type <statement> column
%type <statement> nav
%type <statement> image
%type <statement> text
%type <statement> button
%type <statement> card
%type <statement> table
%type <statement> ordered_list
%type <statement> unordered_list

%type <table_row_list> table_row_list
%type <table_row> table_row
%type <table_cell_list> nonempty_table_cell_list
%type <table_cell> table_cell

%%

program:
      /* vacío */ { $$ = StatementSemanticAction(currentCompilerState(), NULL); }
    | statement_list  { $$ = StatementSemanticAction(currentCompilerState(), $1); }
;


statement_list:
    statement { $$ = createSingleStatementList($1); }
  | statement_list statement { $$ = appendStatementToList($1, $2); }
  ;


statement:
    define { $$ = $1; }
    | use { $$ = $1; }
    | form { $$ = $1; }
    | footer { $$ = $1; }
    | row { $$ = $1; }
    | nav { $$ = $1; }
    | image { $$ = $1; }
    | text { $$ = $1; }
    | button { $$ = $1; }
    | card { $$ = $1; }
    | table { $$ = $1; }
    | ordered_list { $$ = $1; }
    | unordered_list { $$ = $1; }
;

define:
    DEFINE IDENTIFIER maybe_parameters maybe_style statement_list END {
        $$ = DefineSemanticAction($2, $3, $4, $5);
    }
;


maybe_parameters:
      /* vacío */ { $$ = NULL; }
    | parameters { $$ = $1; }
;

parameters:
    OPEN_PAREN identifier_list CLOSE_PAREN { $$ = $2; }
;

identifier_list:
    IDENTIFIER {
        ParameterList* list = createParameterList();
        appendParameter(list, $1, NULL); 
        $$ = list;
    }
  | IDENTIFIER COMMA identifier_list {
        appendParameter($3, $1, NULL);
        $$ = $3;
    }
;


style_parameters:
    OPEN_BRACE style_parameter_list CLOSE_BRACE { $$ = $2; }
  ;

style_parameter_list:
    /* vacío */ { $$ = createParameterList(); }
  | style_parameter_list IDENTIFIER COLON UNQUOTED_VALUE {
        appendParameter($1, $2, $4); $$ = $1;
    }
  ;

use:
    USE IDENTIFIER maybe_use {
        $$ = UseSemanticAction($2, $3);
    }
;

maybe_use:
      /* vacío */ { $$ = NULL; }
    | use_parameters { $$ = $1; }

use_parameters:
    OPEN_PAREN use_parameter_list CLOSE_PAREN { $$ = $2; }
;

use_parameter_list:
      QUOTED_VALUE {
          ParameterList* list = createParameterList();
          appendParameter(list, NULL, $1); 
          $$ = list;
      }
    | QUOTED_VALUE COMMA use_parameter_list {
          appendParameter($3, NULL, $1); 
          $$ = $3;
      }
;


form:
    FORM maybe_style maybe_action form_item_list END {
        $$ = FormSemanticAction($2, $3, $4);
    }
;

form_item_list:
    /* vacío */ { $$ = NULL; }
  | form_item_list form_item {
        $$ = appendFormItem($1, $2);
    }
;

form_item:
    ITEM OPEN_PAREN QUOTED_VALUE COMMA QUOTED_VALUE CLOSE_PAREN {
        $$ = FormItemSemanticAction($3, $5); // label, placeholder
    }
;



action_parameters:
    OPEN_BRACKET style_parameter_list CLOSE_BRACKET { $$ = $2; }
  ;



footer:
    FOOTER maybe_style statement_list END {
        $$ = FooterSemanticAction($2, $3);
    }


row:
    ROW maybe_style column_list END {
        $$ = RowSemanticAction($2, $3);
    }
;

column_list:
    column { $$ = createSingleStatementList($1); }
  | column_list column {
        $$ = appendStatementToList($1, $2);
    }
;

column:
    COLUMN style_parameters statement_list END {
        $$ = ColumnSemanticAction($2, $3);
    }
;


nav:
    NAV maybe_style maybe_action nav_item_list END {
        $$ = NavSemanticAction($2, $3, $4);
    }
;

nav_item_list:
    /* vacío */ { $$ = NULL; }
  | nav_item_list nav_item {
        $$ = appendNavItem($1, $2);
    }
;

nav_item:
    ITEM OPEN_PAREN QUOTED_VALUE COMMA QUOTED_VALUE CLOSE_PAREN {
        $$ = NavItemSemanticAction($3, $5); 
    }
;



image:
    IMG maybe_style OPEN_PAREN QUOTED_VALUE COMMA QUOTED_VALUE CLOSE_PAREN
        { $$ = ImageSemanticAction($2, $4, $6); }
;

maybe_style:
      /* vacío */                    { $$ = NULL; }
    | style_parameters               { $$ = $1; }
;


text:
      HEADER_1 QUOTED_VALUE   { $$ = TextFromQuotedValueSemanticAction($2); }
    | HEADER_2 QUOTED_VALUE   { $$ = TextFromQuotedValueSemanticAction($2); }
    | HEADER_3 QUOTED_VALUE   { $$ = TextFromQuotedValueSemanticAction($2); }
    | HEADER_1 VARIABLE       { $$ = TextFromVariableSemanticAction($2); }
    | HEADER_2 VARIABLE       { $$ = TextFromVariableSemanticAction($2); }
    | HEADER_3 VARIABLE       { $$ = TextFromVariableSemanticAction($2); }
    | QUOTED_VALUE            { $$ = TextFromQuotedValueSemanticAction($1); }
    | VARIABLE                { $$ = TextFromVariableSemanticAction($1); }


button:
    BUTTON maybe_style maybe_action maybe_content END {
        $$ = ButtonSemanticAction($2, $3, $4);
    }
;

maybe_action:
    /* vacío */ { $$ = NULL; }
  |  action_parameters  { $$ = $1; }
  ;


maybe_content:
    /* vacío */ { $$ = NULL; }
  | content     { $$ = $1; }
;

content:
    text { $$ = createSingleStatementList($1); }
  | content text { $$ = appendStatementToList($1, $2); }
;


card:
    CARD maybe_style maybe_content END { $$ = CardSemanticAction($2, $3); }


table:
    TABLE_BEGIN maybe_style table_row_list END
    {
        $$ = TableSemanticAction($2, $3); 
    }
;

table_row_list:
      table_row
        { $$ = SingleTableRowAction($1); }
    | table_row_list table_row
        { $$ = AppendTableRowAction($1, $2); }
;

table_row:
    PIPE nonempty_table_cell_list PIPE
    {
        $$ = TableRowSemanticAction($2);
    }
;

nonempty_table_cell_list:
      table_cell
        { $$ = SingleTableCellAction($1); }
    | nonempty_table_cell_list PIPE table_cell
        { $$ = AppendTableCellAction($1, $3); }
;

table_cell:
    content
    {
        $$ = TableCellSemanticAction($1); 
    }
;


ordered_list:
    LIST_BEGIN maybe_style ordered_list_items END {
        $$ = OrderedListSemanticAction($2, $3);
    }
;

ordered_list_items:
      ORDERED_ITEM text {
          Statement* item = OrderedItemSemanticAction($1, $2);
          $$ = createSingleStatementList(item);
      }
    | ordered_list_items ORDERED_ITEM text {
          Statement* item = OrderedItemSemanticAction($2, $3);
          $$ = appendStatementToList($1, item);
      }
;

unordered_list:
    LIST_BEGIN maybe_style unordered_list_items END {
        $$ = UnorderedListSemanticAction($2, $3);
    }
;

unordered_list_items:
      BULLET text {
          Statement* item = BulletItemSemanticAction($1, $2);
          $$ = createSingleStatementList(item);
      }
    | unordered_list_items BULLET text {
          Statement* item = BulletItemSemanticAction($2, $3);
          $$ = appendStatementToList($1, item);
      }
;


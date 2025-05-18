%{
#include "FlexActions.h"
#define ctx() createLexicalAnalyzerContext()
%}

%option stack

%x MULTILINE_COMMENT
%x ORDERED_LIST BULLET_LIST

%%

^[0-9]+\.[ \t].*\r?\n      {
   BEGIN(ORDERED_LIST);
   yymore();  /* empezamos a acumular en yytext */
}

<ORDERED_LIST>^[0-9]+\.[ \t].*\r?\n  {
   yymore();  /* seguimos acumulando cada línea adicional */
}

<ORDERED_LIST>[^0-9]         {
   /* cualquier cosa que no sea un nuevo ítem rompe la lista */
   BEGIN(INITIAL);
   /* yytext ya tiene todas las líneas; devolvemos un token atómico */
   ctx()->semanticValue->string = strdup(yytext);
   return ORDERED_LIST;
}


^\*[ \t][^\r\n]*\r?\n {
    /* entramos en estado BULLET_LIST y acumulamos */
    BEGIN(BULLET_LIST);
    yymore();
}
<BULLET_LIST>^\*[ \t][^\r\n]*\r?\n {
    /* seguimos acumulando cada línea que empiece con “* ” */
    yymore();
}
<BULLET_LIST>.* {
    /* al no coincidir con “* ”, salimos y emitimos todo el bloque */
    BEGIN(INITIAL);
    yylval.string = strdup(yytext);
    return BULLET_LIST;
}


"/*"                             { BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(ctx()); }
<MULTILINE_COMMENT>"*/"          { EndMultilineCommentLexemeAction(ctx()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+  { IgnoredLexemeAction(ctx()); }
<MULTILINE_COMMENT>[^*]+         { IgnoredLexemeAction(ctx()); }
<MULTILINE_COMMENT>.             { IgnoredLexemeAction(ctx()); }

"@define"                        { return DefineLexemeAction(ctx()); }
"@use"                           { return UseLexemeAction(ctx()); }
"@form"                          { return FormLexemeAction(ctx()); }
"@img"                           { return ImgLexemeAction(ctx()); }
"@row"                           { return RowLexemeAction(ctx()); }
"@column"                        { return ColumnLexemeAction(ctx()); }
"@end"                           { return EndLexemeAction(ctx()); }
"@footer"                        { return FooterLexemeAction(ctx()); }
"@button"                        { return ButtonLexemeAction(ctx()); }
"@card"                          { return CardLexemeAction(ctx()); }
"@nav"                           { return NavLexemeAction(ctx()); }
"@item"                          { return ItemLexemAction(ctx()); }

"\{\{[a-zA-Z_][a-zA-Z0-9_]*\}\}" { return VariableLexemeAction(ctx()); }

"###"                            { return HeaderLexemeAction(ctx(), HEADER_3); }
"##"                             { return HeaderLexemeAction(ctx(), HEADER_2); }
"#"                              { return HeaderLexemeAction(ctx(), HEADER_1); }

[0-9]+[[:space:]]*\.[[:space:]]* { return OrderedItemLexemeAction(ctx()); }
"\u2022"                         { return BulletLexemeAction(ctx()); }

"{"                              { return StyleLexemeAction(ctx(), OPEN_BRACE); }
"}"                              { return StyleLexemeAction(ctx(), CLOSE_BRACE); }

"\("                             { return OpenParenthesisLexemeAction(ctx()); }
"\)"                             { return CloseParenthesisLexemeAction(ctx()); }
":"                              { return ColonLexemeAction(ctx()); }
","                              { return CommaLexemeAction(ctx()); }
"="                              { return EqualLexemeAction(ctx()); }
"|"                              { return TableLexemeAction(ctx(), PIPE); }

\"[^\"]*\"                       { return QuotedValueLexemeAction(ctx()); }     
\'[^\']*\'                       { return QuotedParameterValueLexemeAction(ctx()); } 

[a-zA-Z_][a-zA-Z0-9_]*           { return IdentifierLexemeAction(ctx()); }
[^[:space:]:},#@()\[\]|"='`\n]+    { return UnquotedValueLexemeAction(ctx()); }

\n                               { return NewlineLexemeAction(ctx()); }
[[:space:]]+                     { IgnoredLexemeAction(ctx()); }


.                                { return UnknownLexemeAction(ctx()); }

%%

#include "FlexExport.h"

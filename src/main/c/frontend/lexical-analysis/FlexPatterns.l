%{
#include "FlexActions.h"
#define ctx() createLexicalAnalyzerContext()
%}

%option stack

%x MULTILINE_COMMENT QUOTED PARAM VARIABLE

%%

"/*"                             { BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(ctx()); }
<MULTILINE_COMMENT>"*/"          { EndMultilineCommentLexemeAction(ctx()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+  { IgnoredLexemeAction(ctx()); }
<MULTILINE_COMMENT>[^*]+         { IgnoredLexemeAction(ctx()); }
<MULTILINE_COMMENT>.             { IgnoredLexemeAction(ctx()); }

"\""                             { BEGIN(QUOTED); }
<QUOTED>(\\.|[^"\\\n])+          { return QuotedValueLexemeAction(ctx()); }
<QUOTED>"\""                     { BEGIN(INITIAL); }

"'"                              { BEGIN(PARAM); }
<PARAM>[^'\\\n]+                 { return QuotedParameterValueLexemeAction(ctx()); }
<PARAM>"'"                       { BEGIN(INITIAL); }

"{{"                             { BEGIN(VARIABLE); }
<VARIABLE>[A-Za-z_][A-Za-z0-9_]*         { return VariableLexemeAction(ctx()); }
<VARIABLE>"}}"                   { BEGIN(INITIAL); }

"@define"                        { return TagLexemeAction(ctx(), DEFINE); }
"@use"                           { return TagLexemeAction(ctx(), USE); }
"@form"                          { return TagLexemeAction(ctx(), FORM); }
"@img"                           { return TagLexemeAction(ctx(), IMG); }
"@row"                           { return TagLexemeAction(ctx(), ROW); }
"@column"                        { return TagLexemeAction(ctx(), COLUMN); }
"@end"                           { return TagLexemeAction(ctx(), END); }
"@footer"                        { return TagLexemeAction(ctx(), FOOTER); }
"@button"                        { return TagLexemeAction(ctx(), BUTTON); }
"@card"                          { return TagLexemeAction(ctx(), CARD); }
"@nav"                           { return TagLexemeAction(ctx(), NAV); }
"@item"                          { return TagLexemeAction(ctx(), ITEM); }
"@table"                         { return TagLexemeAction(ctx(), TABLE_BEGIN); }
"@list"                          { return TagLexemeAction(ctx(), LIST_BEGIN); }


"###"                            { return HeaderLexemeAction(ctx(), HEADER_3); }
"##"                             { return HeaderLexemeAction(ctx(), HEADER_2); }
"#"                              { return HeaderLexemeAction(ctx(), HEADER_1); }

[0-9]+[[:space:]]*\. { return OrderedItemLexemeAction(ctx()); }
"*"                              { return BulletLexemeAction(ctx()); }

"{"                              { return StyleLexemeAction(ctx(), OPEN_BRACE); }
"}"                              { return StyleLexemeAction(ctx(), CLOSE_BRACE); }

"["                              { return ActionLexemeAction(ctx(), OPEN_BRACKET); }
"]"                              { return ActionLexemeAction(ctx(), CLOSE_BRACKET); }

"("                              { return ParenthesisLexemeAction(ctx(), OPEN_PAREN); }
")"                              { return ParenthesisLexemeAction(ctx(), CLOSE_PAREN); }
":"                              { return ColonLexemeAction(ctx()); }
","                              { return CommaLexemeAction(ctx()); }
"="                              { return EqualLexemeAction(ctx()); }
"|"                              { return TableLexemeAction(ctx(), PIPE); }


[[:space:]]+                     { IgnoredLexemeAction(ctx()); }

[a-zA-Z_][a-zA-Z0-9_]*           { return IdentifierLexemeAction(ctx()); }
[^[:space:]:{},@()\[\]|"='`\n]+; { return UnquotedValueLexemeAction(ctx()); }




.                                { return UnknownLexemeAction(ctx()); }

%%

#include "FlexExport.h"

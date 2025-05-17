%{
#include "FlexActions.h"
#define ctx() createLexicalAnalyzerContext()
%}

%option stack

%x MULTILINE_COMMENT

%%

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
[^\s:},#@()\[\]|"='`\n]+         { return UnquotedValueLexemeAction(ctx()); }

[[:space:]]+                     { IgnoredLexemeAction(ctx()); }
\n                               { return NewlineLexemeAction(ctx()); }


.                                { return UnknownLexemeAction(ctx()); }

%%

#include "FlexExport.h"

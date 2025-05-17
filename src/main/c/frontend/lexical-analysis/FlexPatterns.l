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

"#"                              { return HeaderLexemeAction(ctx(), HEADER_1); }
"##"                             { return HeaderLexemeAction(ctx(), HEADER_2); }
"###"                            { return HeaderLexemeAction(ctx(), HEADER_3); }

"{"\n                            { return OpenBraceLexemeAction(ctx()); }
"}"                              { return CloseBraceLexemeAction(ctx()); }
"{"                              { return StyleLexemeAction(ctx(), OPEN_BRACE); }
"}"                              { return StyleLexemeAction(ctx(), CLOSE_BRACE); }

"\("                             { return OpenParenthesisLexemeAction(ctx()); }
"\)"                             { return CloseParenthesisLexemeAction(ctx()); }
":"                              { return ColonLexemeAction(ctx()); }
","                              { return CommaLexemeAction(ctx()); }
"="                              { return EqualLexemeAction(ctx()); }

"["                              { return InputLexemeAction(ctx(), OPEN_BRACKET); }
"]"                              { return InputLexemeAction(ctx(), CLOSE_BRACKET); }
"|"                              { return TableLexemeAction(ctx(), PIPE); }
"\""                             { return QuoteLexemeAction(ctx(), QUOTE); }

[0-9]+"."                        { return OrderedItemLexemeAction(ctx()); }
"\u2022"                         { return BulletLexemeAction(ctx()); }

\"[^\"]*\"                       { return QuotedValueLexemeAction(ctx()); }
[a-zA-Z_][a-zA-Z0-9_]*           { return IdentifierLexemeAction(ctx()); }
[^\s:},#@()\[\]|"=\n]+           { return UnquotedValueLexemeAction(ctx()); }

[^\n]+                           { return TextLexemeAction(ctx()); }
[[:space:]]+                     { IgnoredLexemeAction(ctx()); }
\n                               { return NewlineLexemeAction(ctx()); }
.                                { return UnknownLexemeAction(ctx()); }

%%

#include "FlexExport.h"

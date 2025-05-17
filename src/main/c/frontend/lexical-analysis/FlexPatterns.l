%{

#include "FlexActions.h"
#define ctx() createLexicalAnalyzerContext()

%}

%option stack

%x MULTILINE_COMMENT
%x STYLE_BLOCK
%x LIST_BLOCK
%x DIRECTIVE_ID
%x PARAMS
%x DIRECTIVE_BODY
%x BLOCK_DIRECTIVE

%%

"/*"                             { BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(ctx()); }
<MULTILINE_COMMENT>"*/"          { EndMultilineCommentLexemeAction(ctx()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+  { IgnoredLexemeAction(ctx()); }
<MULTILINE_COMMENT>[^*]+         { IgnoredLexemeAction(ctx()); }
<MULTILINE_COMMENT>.             { IgnoredLexemeAction(ctx()); }

"{"\n                           { BEGIN(LIST_BLOCK); return OpenBraceLexemeAction(ctx()); }
<LIST_BLOCK>"}"                 { BEGIN(INITIAL); return CloseBraceLexemeAction(ctx()); }
<LIST_BLOCK>[0-9]+"."           { return OrderedItemLexemeAction(ctx()); }
<LIST_BLOCK>"\u2022"            { return BulletLexemeAction(ctx()); }
<LIST_BLOCK>[^\n]+              { return ListContentLexemeAction(ctx()); }
<LIST_BLOCK>\n                  { return NewlineLexemeAction(ctx()); }

"@define"                       { BEGIN(DIRECTIVE_ID); return DefineLexemeAction(ctx()); }
"@use"                          { BEGIN(DIRECTIVE_ID); return UseLexemeAction(ctx()); }
"@form"                         { BEGIN(DIRECTIVE_ID); return FormLexemeAction(ctx()); }
"@img"                          { return ImgLexemeAction(ctx()); }
"@row"                          { BEGIN(DIRECTIVE_ID); return RowLexemeAction(ctx()); }
"@column"                          { BEGIN(DIRECTIVE_ID); return ColumnLexemeAction(ctx()); }
"@end"                          { BEGIN(INITIAL); return EndLexemeAction(ctx()); }

<DIRECTIVE_ID>[a-zA-Z_][a-zA-Z0-9_]*  { BEGIN(PARAMS); return IdentifierLexemeAction(ctx()); }

<PARAMS>"("                    { return OpenParenthesisLexemeAction(ctx()); }
<PARAMS>")"                    { BEGIN(DIRECTIVE_BODY); return CloseParenthesisLexemeAction(ctx()); }
<PARAMS>","                    { return CommaLexemeAction(ctx()); }
<PARAMS>":"                    { return ColonLexemeAction(ctx()); }
<PARAMS>"="                    { return EqualLexemeAction(ctx()); }
<PARAMS>[a-zA-Z_][a-zA-Z0-9_]* { return IdentifierLexemeAction(ctx()); }
<PARAMS>"\""[^\"]*"\""      { return QuotedValueLexemeAction(ctx()); }
<PARAMS>[[:space:]]+           { IgnoredLexemeAction(ctx()); }

<DIRECTIVE_BODY>"@footer"      { BEGIN(BLOCK_DIRECTIVE); return FooterLexemeAction(ctx()); }
<DIRECTIVE_BODY>"@button"      { BEGIN(BLOCK_DIRECTIVE); return ButtonLexemeAction(ctx()); }
<DIRECTIVE_BODY>"@card"        { BEGIN(BLOCK_DIRECTIVE); return CardLexemeAction(ctx()); }
<DIRECTIVE_BODY>"@nav"         { BEGIN(BLOCK_DIRECTIVE); return NavLexemeAction(ctx()); }
<DIRECTIVE_BODY>"{"            { return StyleLexemeAction(ctx(), OPEN_BRACE); }
<DIRECTIVE_BODY>"}"            { return StyleLexemeAction(ctx(), CLOSE_BRACE); }


<DIRECTIVE_BODY>[^\n]+         { return TextLexemeAction(ctx()); }
<DIRECTIVE_BODY>\n             { return NewlineLexemeAction(ctx()); }

<BLOCK_DIRECTIVE>"@end"        { BEGIN(DIRECTIVE_BODY); return EndLexemeAction(ctx()); }
<BLOCK_DIRECTIVE>"{"           { return StyleLexemeAction(ctx(), OPEN_BRACE); }
<BLOCK_DIRECTIVE>"}"           { return StyleLexemeAction(ctx(), CLOSE_BRACE); }
<BLOCK_DIRECTIVE>[^\n]+        { return TextLexemeAction(ctx()); }
<BLOCK_DIRECTIVE>\n            { return NewlineLexemeAction(ctx()); }

"#"                            { return HeaderLexemeAction(ctx(), HEADER_1); }
"##"                           { return HeaderLexemeAction(ctx(), HEADER_2); }
"###"                          { return HeaderLexemeAction(ctx(), HEADER_3); }

"{"                             { BEGIN(STYLE_BLOCK); return StyleLexemeAction(ctx(), OPEN_BRACE); }
<STYLE_BLOCK>"}"                { BEGIN(INITIAL); return StyleLexemeAction(ctx(), CLOSE_BRACE); }
<STYLE_BLOCK>[a-zA-Z_][a-zA-Z0-9_]*   { return KeyLexemeAction(ctx()); }
<STYLE_BLOCK>":"                { return ColonLexemeAction(ctx()); }
<STYLE_BLOCK>","                { return CommaLexemeAction(ctx()); }
<STYLE_BLOCK>"\""[^\"]*"\""  { return QuotedValueLexemeAction(ctx()); }
<STYLE_BLOCK>[^\s:},]+         { return UnquotedValueLexemeAction(ctx()); }

"["                            { return InputLexemeAction(ctx(), OPEN_BRACKET); }
"]"                            { return InputLexemeAction(ctx(), CLOSE_BRACKET); }
"|"                            { return TableLexemeAction(ctx(), PIPE); }
"\""                           { return QuoteLexemeAction(ctx(), QUOTE); }
\n                              { return NewlineLexemeAction(ctx()); }

"("                            { return OpenParenthesisLexemeAction(ctx()); }
")"                            { return CloseParenthesisLexemeAction(ctx()); }
":"                            { return ColonLexemeAction(ctx()); }
"="                            { return EqualLexemeAction(ctx()); }
[a-zA-Z_][a-zA-Z0-9_]*          { return IdentifierLexemeAction(ctx()); }

[^#@\n{\[\]|]+                  { return TextLexemeAction(ctx()); }
[[:space:]]+                    { IgnoredLexemeAction(ctx()); }
.                               { return UnknownLexemeAction(ctx()); }

%%

#include "FlexExport.h"
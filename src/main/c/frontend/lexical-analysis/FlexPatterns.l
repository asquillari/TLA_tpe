%{

#include "FlexActions.h"

%}

/**
 * Enable start condition manipulation functions.
 */
%option stack

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x MULTILINE_COMMENT
%x STYLE_BLOCK

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%

"/*"								{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+		{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+			{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"@define"                          { return DefineLexemeAction(createLexicalAnalyzerContext()); }
"@use"                             { return UseLexemeAction(createLexicalAnalyzerContext()); }
"@end"                             { return EndLexemeAction(createLexicalAnalyzerContext()); }

"@form"                            { return FormLexemeAction(createLexicalAnalyzerContext()); }
"@img"                             { return ImgLexemeAction(createLexicalAnalyzerContext()); }
"@footer"                          { return FooterLexemeAction(createLexicalAnalyzerContext()); }
"@row"                             { return RowLexemeAction(createLexicalAnalyzerContext()); }
"@column"                          { return ColumnLexemeAction(createLexicalAnalyzerContext()); }
"@nav"                             { return NavLexemeAction(createLexicalAnalyzerContext()); }

"# "								{ return HeaderLexemeAction(createLexicalAnalyzerContext(), HEADER_1); }
"## "								{ return HeaderLexemeAction(createLexicalAnalyzerContext(), HEADER_2); }
"### "								{ return HeaderLexemeAction(createLexicalAnalyzerContext(), HEADER_3); }

"[0-9]+\."                         { return OrderedItemLexemeAction(createLexicalAnalyzerContext()); } // ítem numerado
"•"                                { return BulletLexemeAction(createLexicalAnalyzerContext()); }     // ítem no numerado

"{"									{ BEGIN(STYLE_BLOCK); return StyleLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACE); }
<STYLE_BLOCK>"}"					{ BEGIN(INITIAL); return StyleLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACE); }
<STYLE_BLOCK>[^}]+					{ return StyleContentLexemeAction(createLexicalAnalyzerContext()); }
<STYLE_BLOCK>[a-zA-Z_][a-zA-Z0-9_]*   { return KeyLexemeAction(createLexicalAnalyzerContext()); }
<STYLE_BLOCK>":"                      { return ColonLexemeAction(createLexicalAnalyzerContext()); }
<STYLE_BLOCK>","                      { return CommaLexemeAction(createLexicalAnalyzerContext()); }
<STYLE_BLOCK>"\""[^"]*"\""            { return QuotedValueLexemeAction(createLexicalAnalyzerContext()); }
<STYLE_BLOCK>[^\s:},]+                { return UnquotedValueLexemeAction(createLexicalAnalyzerContext()); }


"["									{ return InputLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACKET); }
"]"									{ return InputLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACKET); }
"|"									{ return TableLexemeAction(createLexicalAnalyzerContext(), PIPE); }
"\""								{ return QuoteLexemeAction(createLexicalAnalyzerContext(), QUOTE); }
\n									{ return NewlineLexemeAction(createLexicalAnalyzerContext()); }
[^#@\n{[]|]+						{ return TextLexemeAction(createLexicalAnalyzerContext()); }

"("                              { return OpenParenthesisLexemeAction(createLexicalAnalyzerContext()); }
")"                              { return CloseParenthesisLexemeAction(createLexicalAnalyzerContext()); }
":"                              { return ColonLexemeAction(createLexicalAnalyzerContext()); }
"="                              { return EqualLexemeAction(createLexicalAnalyzerContext()); }
[a-zA-Z_][a-zA-Z0-9_]*           { return IdentifierLexemeAction(createLexicalAnalyzerContext()); }


[[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }


%%

#include "FlexExport.h"

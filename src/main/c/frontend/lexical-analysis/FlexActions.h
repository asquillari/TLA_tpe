#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../shared/Environment.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Type.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"
#include "LexicalAnalyzerContext.h"
#include <stdio.h>
#include <stdlib.h>


/** Initialize module's internal state. */
void initializeFlexActionsModule();

/** Shutdown module's internal state. */
void shutdownFlexActionsModule();

/**
 * Flex lexeme processing actions.
 */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);


Token DefineLexemeAction(LexicalAnalyzerContext * ctx);
Token UseLexemeAction(LexicalAnalyzerContext * ctx);
Token FormLexemeAction(LexicalAnalyzerContext * ctx);
Token ImgLexemeAction(LexicalAnalyzerContext * ctx);
Token RowLexemeAction(LexicalAnalyzerContext * ctx);
Token ColumnLexemeAction(LexicalAnalyzerContext * ctx);
Token EndLexemeAction(LexicalAnalyzerContext * ctx);
Token FooterLexemeAction(LexicalAnalyzerContext * ctx);
Token ButtonLexemeAction(LexicalAnalyzerContext * ctx);
Token CardLexemeAction(LexicalAnalyzerContext * ctx);
Token NavLexemeAction(LexicalAnalyzerContext * ctx);
Token ItemLexemAction(LexicalAnalyzerContext * ctx);

Token VariableLexemeAction(LexicalAnalyzerContext * ctx);

Token HeaderLexemeAction(LexicalAnalyzerContext * ctx, Token token);

Token OrderedItemLexemeAction(LexicalAnalyzerContext * ctx);
Token BulletLexemeAction(LexicalAnalyzerContext * ctx);

Token StyleLexemeAction(LexicalAnalyzerContext * ctx, Token token);

Token OpenParenthesisLexemeAction(LexicalAnalyzerContext * ctx);
Token CloseParenthesisLexemeAction(LexicalAnalyzerContext * ctx);
Token ColonLexemeAction(LexicalAnalyzerContext * ctx);
Token CommaLexemeAction(LexicalAnalyzerContext * ctx);
Token EqualLexemeAction(LexicalAnalyzerContext * ctx);
Token TableLexemeAction(LexicalAnalyzerContext * ctx, Token token);

Token QuotedValueLexemeAction(LexicalAnalyzerContext * ctx);
Token QuotedParameterValueLexemeAction(LexicalAnalyzerContext * ctx);

Token IdentifierLexemeAction(LexicalAnalyzerContext * ctx);
Token UnquotedValueLexemeAction(LexicalAnalyzerContext * ctx);

Token NewlineLexemeAction(LexicalAnalyzerContext * ctx);

#endif
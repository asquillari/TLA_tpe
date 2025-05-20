#include "FlexActions.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;
static boolean _logIgnoredLexemes = true;

void initializeFlexActionsModule() {
	_logIgnoredLexemes = getBooleanOrDefault("LOG_IGNORED_LEXEMES", _logIgnoredLexemes);
	_logger = createLogger("FlexActions");
}

void shutdownFlexActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/* PRIVATE FUNCTIONS */

static void _logLexicalAnalyzerContext(const char * functionName, LexicalAnalyzerContext * lexicalAnalyzerContext);

/**
 * Logs a lexical-analyzer context in DEBUGGING level.
 */
static void _logLexicalAnalyzerContext(const char * functionName, LexicalAnalyzerContext * lexicalAnalyzerContext) {
	char * escapedLexeme = escape(lexicalAnalyzerContext->lexeme);
	logDebugging(_logger, "%s: %s (context = %d, length = %d, line = %d)",
		functionName,
		escapedLexeme,
		lexicalAnalyzerContext->currentContext,
		lexicalAnalyzerContext->length,
		lexicalAnalyzerContext->line);
	free(escapedLexeme);
}

/* PUBLIC FUNCTIONS */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return UNKNOWN;
}

Token TagLexemeAction(LexicalAnalyzerContext *ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	destroyLexicalAnalyzerContext(ctx);
	return token;
}

Token VariableLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = ctx->lexeme;
	ctx->semanticValue->token = VARIABLE;
	destroyLexicalAnalyzerContext(ctx);
	return VARIABLE;
}


Token HeaderLexemeAction(LexicalAnalyzerContext * ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	destroyLexicalAnalyzerContext(ctx);
	return token;
}


Token OrderedItemLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = ctx->lexeme;
	ctx->semanticValue->token = ORDERED_ITEM;
	destroyLexicalAnalyzerContext(ctx);
	return ORDERED_ITEM;
}

Token BulletLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = ctx->lexeme;
	ctx->semanticValue->token = BULLET;
	destroyLexicalAnalyzerContext(ctx);
	return BULLET;
}

Token ListLexemeAction(LexicalAnalyzerContext * ctx, char *text, Token token) {
    _logLexicalAnalyzerContext(__FUNCTION__, ctx);
    ctx->semanticValue->string = text;
    ctx->semanticValue->token  = token;
    destroyLexicalAnalyzerContext(ctx);
    return token;
}

Token StyleLexemeAction(LexicalAnalyzerContext * ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	destroyLexicalAnalyzerContext(ctx);
	return token;
}

Token ActionLexemeAction(LexicalAnalyzerContext * ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	destroyLexicalAnalyzerContext(ctx);
	return token;
}


Token ParenthesisLexemeAction(LexicalAnalyzerContext * ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	destroyLexicalAnalyzerContext(ctx);
	return token;
}

Token ColonLexemeAction(LexicalAnalyzerContext * ctx){
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = COLON;
	destroyLexicalAnalyzerContext(ctx);
	return COLON;
}
Token CommaLexemeAction(LexicalAnalyzerContext * ctx){
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = COMMA;
	destroyLexicalAnalyzerContext(ctx);
	return COMMA;
}

Token EqualLexemeAction(LexicalAnalyzerContext * ctx){
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = EQUALS;
	destroyLexicalAnalyzerContext(ctx);
	return EQUALS;
}


Token TableLexemeAction(LexicalAnalyzerContext * ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	destroyLexicalAnalyzerContext(ctx);
	return token;
}

Token QuotedValueLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = ctx->lexeme;
	ctx->semanticValue->token = QUOTED_VALUE;
	destroyLexicalAnalyzerContext(ctx);
	return QUOTED_VALUE;
}

Token QuotedParameterValueLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = ctx->lexeme;
	ctx->semanticValue->token = QUOTED_VALUE;
	destroyLexicalAnalyzerContext(ctx);
	return QUOTED_VALUE;
}


Token IdentifierLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = ctx->lexeme;
	ctx->semanticValue->token = IDENTIFIER;
	destroyLexicalAnalyzerContext(ctx);
	return IDENTIFIER;
}

Token UnquotedValueLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = ctx->lexeme;
	ctx->semanticValue->token = UNQUOTED_VALUE;
	destroyLexicalAnalyzerContext(ctx);
	return UNQUOTED_VALUE;
}


Token NewlineLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = NEWLINE;
	destroyLexicalAnalyzerContext(ctx);
	return NEWLINE;
}
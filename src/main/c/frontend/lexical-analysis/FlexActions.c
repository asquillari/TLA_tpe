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
}

void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
}

void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
}

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	return UNKNOWN;
}

Token TagLexemeAction(LexicalAnalyzerContext *ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	return token;
}

Token VariableLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = ctx->lexeme;
	return VARIABLE;
}


Token HeaderLexemeAction(LexicalAnalyzerContext * ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	return token;
}


Token OrderedItemLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->lexeme[ctx->length - 1] = '\0';
	ctx->semanticValue->string = ctx->lexeme;
	return ORDERED_ITEM;
}

Token BulletLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = ctx->lexeme;
	return BULLET;
}

Token ListLexemeAction(LexicalAnalyzerContext * ctx, char *text, Token token) {
    _logLexicalAnalyzerContext(__FUNCTION__, ctx);
    ctx->semanticValue->token  = token;
    return token;
}

Token StyleLexemeAction(LexicalAnalyzerContext * ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	return token;
}

Token ActionLexemeAction(LexicalAnalyzerContext * ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	return token;
}


Token ParenthesisLexemeAction(LexicalAnalyzerContext * ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	return token;
}

Token ColonLexemeAction(LexicalAnalyzerContext * ctx){
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = COLON;
	return COLON;
}
Token CommaLexemeAction(LexicalAnalyzerContext * ctx){
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = COMMA;
	return COMMA;
}

Token EqualLexemeAction(LexicalAnalyzerContext * ctx){
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = EQUALS;
	return EQUALS;
}


Token TableLexemeAction(LexicalAnalyzerContext * ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	return token;
}

Token QuotedValueLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = ctx->lexeme;
	return QUOTED_VALUE;
}

Token QuotedParameterValueLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = ctx->lexeme;
	return QUOTED_VALUE;
}


Token IdentifierLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = ctx->lexeme;
	return IDENTIFIER;
}

Token UnquotedValueLexemeAction(LexicalAnalyzerContext *ctx) {
    _logLexicalAnalyzerContext(__FUNCTION__, ctx);
    ctx->lexeme[ctx->length - 1] = '\0';
    ctx->semanticValue->string = ctx->lexeme;
    return UNQUOTED_VALUE;
}


Token NewlineLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = NEWLINE;
	return NEWLINE;
}
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

Token DefineLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = DEFINE;
	destroyLexicalAnalyzerContext(ctx);
	return DEFINE;
}

Token UseLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = USE;
	destroyLexicalAnalyzerContext(ctx);
	return USE;
}

Token EndDefineLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = ENDDEFINE;
	destroyLexicalAnalyzerContext(ctx);
	return ENDDEFINE;
}

Token EndLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = END;
	destroyLexicalAnalyzerContext(ctx);
	return END;
}

Token FormLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = FORM;
	destroyLexicalAnalyzerContext(ctx);
	return FORM;
}

Token ImgLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = IMG;
	destroyLexicalAnalyzerContext(ctx);
	return IMG;
}

Token FooterLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = FOOTER;
	destroyLexicalAnalyzerContext(ctx);
	return FOOTER;
}

Token RowLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = ROW;
	destroyLexicalAnalyzerContext(ctx);
	return ROW;
}

Token ColumnLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = COLUMN;
	destroyLexicalAnalyzerContext(ctx);
	return COLUMN;
}

Token NavLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = NAV;
	destroyLexicalAnalyzerContext(ctx);
	return NAV;
}

Token OrderedItemLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = strdup(ctx->lexeme);
	ctx->semanticValue->token = ORDERED_ITEM;
	destroyLexicalAnalyzerContext(ctx);
	return ORDERED_ITEM;
}

Token BulletLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = strdup(ctx->lexeme);
	ctx->semanticValue->token = BULLET;
	destroyLexicalAnalyzerContext(ctx);
	return BULLET;
}

Token StyleContentLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = strdup(ctx->lexeme);
	destroyLexicalAnalyzerContext(ctx);
	return STYLE_CONTENT;
}

Token TableLexemeAction(LexicalAnalyzerContext * ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	destroyLexicalAnalyzerContext(ctx);
	return token;
}

Token QuoteLexemeAction(LexicalAnalyzerContext * ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	destroyLexicalAnalyzerContext(ctx);
	return token;
}

Token NewlineLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = NEWLINE;
	destroyLexicalAnalyzerContext(ctx);
	return NEWLINE;
}

Token TextLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = strdup(ctx->lexeme);
	destroyLexicalAnalyzerContext(ctx);
	return TEXT;
}

Token HeaderLexemeAction(LexicalAnalyzerContext * ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	destroyLexicalAnalyzerContext(ctx);
	return token;
}

Token StyleLexemeAction(LexicalAnalyzerContext * ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	destroyLexicalAnalyzerContext(ctx);
	return token;
}

Token InputLexemeAction(LexicalAnalyzerContext * ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	destroyLexicalAnalyzerContext(ctx);
	return token;
}

Token KeyLexemeAction(LexicalAnalyzerContext * ctx){
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = KEY;
	destroyLexicalAnalyzerContext(ctx);
	return KEY;
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
Token UnquotedStringLexemeAction(LexicalAnalyzerContext * ctx){
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = strdup(ctx->lexeme);
	ctx->semanticValue->token = UNQUOTED_VALUE;
	destroyLexicalAnalyzerContext(ctx);
	return UNQUOTED_VALUE;
}

Token OpenParenthesisLexemeAction(LexicalAnalyzerContext * ctx){
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = OPEN_PAREN;
	destroyLexicalAnalyzerContext(ctx);
	return OPEN_PAREN;
}
Token CloseParenthesisLexemeAction(LexicalAnalyzerContext * ctx){
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = CLOSE_PAREN;
	destroyLexicalAnalyzerContext(ctx);
	return CLOSE_PAREN;
}
Token EqualLexemeAction(LexicalAnalyzerContext * ctx){
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = EQUALS;
	destroyLexicalAnalyzerContext(ctx);
	return EQUALS;
}

Token IdentifierLexemeAction(LexicalAnalyzerContext * ctx){
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = strdup(ctx->lexeme);
	ctx->semanticValue->token = IDENTIFIER;
	destroyLexicalAnalyzerContext(ctx);
	return IDENTIFIER;
}

Token QuotedValueLexemeAction(LexicalAnalyzerContext * ctx){
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = strdup(ctx->lexeme);
	ctx->semanticValue->token = QUOTED_VALUE;
	destroyLexicalAnalyzerContext(ctx);
	return QUOTED_VALUE;
}
Token UnquotedValueLexemeAction(LexicalAnalyzerContext * ctx){
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = strdup(ctx->lexeme);
	ctx->semanticValue->token = UNQUOTED_VALUE;
	destroyLexicalAnalyzerContext(ctx);
	return UNQUOTED_VALUE;
}
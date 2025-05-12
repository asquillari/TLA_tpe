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

Token DefineLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = DEFINE;
	return DEFINE;
}

Token UseLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = USE;
	return USE;
}

Token EndDefineLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = ENDDEFINE;
	return ENDDEFINE;
}

Token EndLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = END;
	return END;
}

Token FormLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = FORM;
	return FORM;
}

Token ImgLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = IMG;
	return IMG;
}

Token FooterLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = FOOTER;
	return FOOTER;
}

Token RowLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = ROW;
	return ROW;
}

Token ColumnLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = COLUMN;
	return COLUMN;
}

Token NavLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = NAV;
	return NAV;
}

Token OrderedItemLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = strdup(ctx->lexeme);
	ctx->semanticValue->token = ORDERED_ITEM;
	return ORDERED_ITEM;
}

Token BulletLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = strdup(ctx->lexeme);
	ctx->semanticValue->token = BULLET;
	return BULLET;
}

Token StyleContentLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = strdup(ctx->lexeme);
	return STYLE_CONTENT;
}

Token TableLexemeAction(LexicalAnalyzerContext * ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	return token;
}

Token QuoteLexemeAction(LexicalAnalyzerContext * ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	return token;
}

Token NewlineLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = NEWLINE;
	return NEWLINE;
}

Token TextLexemeAction(LexicalAnalyzerContext * ctx) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->string = strdup(ctx->lexeme);
	return TEXT;
}

Token HeaderLexemeAction(LexicalAnalyzerContext * ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	return token;
}

Token StyleLexemeAction(LexicalAnalyzerContext * ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	return token;
}

Token InputLexemeAction(LexicalAnalyzerContext * ctx, Token token) {
	_logLexicalAnalyzerContext(__FUNCTION__, ctx);
	ctx->semanticValue->token = token;
	return token;
}


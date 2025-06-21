#include "Generator.h"

#include "../../shared/symbol-table/symbolTable.h"

static SymbolTable *_symbolTable = NULL;

/* MODULE INTERNAL STATE */
const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;
static DefineStatementList * _defineStatementList = NULL;
FILE * _outputFile = NULL;



static void _freeParameterList(ParameterList *list);


static void _freeDefineStatementList() {
    DefineStatementList *current = _defineStatementList;
    while (current) {
        DefineStatementList *next = current->next;
        if (current->define) {
            free(current->define->name);
            free(current->define);
        }
        free(current);
        current = next;
    }
    _defineStatementList = NULL;
}


static void _freeStatementList(StatementList *list) {
    while (list) {
        StatementList *next = list->next;
        if (list->statement) {
            // Si tenés una función para liberar statements según tipo, usala acá.
            // Por simplicidad, se asume solo se hace free del statement.
            free(list->statement);
        }
        free(list);
        list = next;
    }
}


void initializeGeneratorModule() {
	_logger = createLogger("Generator");

	const char * dir = "src/output";

	struct stat st = {0};
	if(stat(dir, &st) == -1) {
		if (mkdir(dir, 0755) != 0) {
			_outputFile = stdout; 
			return;
		}
	}
    const char * name = getStringOrDefault("OUTPUT_FILE", "output.html");

    size_t len = strlen(dir) + 1 + strlen(name) + 1;
    char * path = malloc(len);
    snprintf(path, len, "%s/%s", dir, name);

    _outputFile = fopen(path, "w");
    if (_outputFile == NULL) {
        _outputFile = stdout;
    }
    free(path);
}

void shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
	if (_outputFile != NULL && _outputFile != stdout) {
		fclose(_outputFile);
	}
	_freeDefineStatementList();
}

/** PRIVATE FUNCTIONS */

static void _generateEpilogue();
static void _generateProgram(Program * program);
static void _generatePrologue(void);
static char * _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char * const format, ...);
static void _generateStatement(unsigned indent, Statement *s);
static char * styleToString(ParameterList *style);
static char * attributesToString(ParameterList *attrs);

/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Latex document.
 */
//no se si necesitamos agregar algo al final, para html
static void _generateEpilogue(void) {
    _output(0,
      "</body>\n"
      "</html>\n"
    );
}

static char * styleToString(ParameterList *style) {
    if (!style || !style->head) {
        return strdup("");
    }

    size_t total = 1;
    for (Parameter *p = style->head; p; p = p->next) {
        total += strlen(p->key)  
               + 1              
               + strlen(p->value) 
               + 1;             
    }
    char *buf = malloc(total);
    if (!buf) return strdup(""); 

    buf[0] = '\0';
    for (Parameter *p = style->head; p; p = p->next) {
        strcat(buf, p->key);
        strcat(buf, ":");
        strcat(buf, p->value);
        strcat(buf, ";");
	}
    return buf;
}

static char * attributesToString(ParameterList *attrs) {
    if (!attrs || !attrs->head) return strdup("");

    size_t total = 1;
    for (Parameter *p = attrs->head; p; p = p->next) {
        total += strlen(p->key) + 2 + strlen(p->value) + 2;
    }

    char *buf = malloc(total);
    if (!buf) return strdup("");

    buf[0] = '\0';
    for (Parameter *p = attrs->head; p; p = p->next) {
        strcat(buf, p->key);
        strcat(buf, "=\"");
        strcat(buf, p->value);
        strcat(buf, "\" ");
    }
    if (strlen(buf) > 0) buf[strlen(buf) - 1] = '\0';
    return buf;
}


static void _generateStatement(unsigned indent, Statement *s) {
	if(!s){
		return;
	}
    switch (s->type) {
		//podemos cambiar que style no aparezca vacio pero no me parece mal dejar como para que 
		//lo complete el programador si quiere
        case STATEMENT_HEADER1: {
			const char *raw = s->text->content;
			char *resolved = NULL;
			if (symbolTableGetValue(_symbolTable, raw, &resolved)) {
				_output(indent, "<h1>%s</h1>", resolved);
			} else {
				_output(indent, "<h1>%s</h1>", raw);
			}
			break;
		}
		case STATEMENT_HEADER2: {
			const char *raw = s->text->content;
			char *resolved = NULL;
			if (symbolTableGetValue(_symbolTable, raw, &resolved)) {
				_output(indent, "<h2>%s</h2>", resolved);
			} else {
				_output(indent, "<h2>%s</h2>", raw);
			}
			break;
		}

		case STATEMENT_HEADER3: {
			const char *raw = s->text->content;
			char *resolved = NULL;
			if (symbolTableGetValue(_symbolTable, raw, &resolved)) {
				_output(indent, "<h3>%s</h3>", resolved);
			} else {
				_output(indent, "<h3>%s</h3>", raw);
			}
			break;
		}
		case STATEMENT_PARAGRAPH: {
			const char *raw = s->text->content;
			char *resolved = NULL;
			if (symbolTableGetValue(_symbolTable, raw, &resolved)) {
				_output(indent, "<p>%s</p>", resolved);
			} else {
				_output(indent, "<p>%s</p>", raw);
			}
			break;
		}
		case STATEMENT_IMAGE: {
			char *styleStr = styleToString(s->image->style);
			_output(indent,
					"<img src=\"%s\" alt=\"%s\" style=\"%s\"/>",
					s->image->src,
					s->image->alt,
					styleStr);
			free(styleStr);
			break;
		}
		case STATEMENT_NAV: {
			char *styleStr = styleToString(s->nav->style);
			char *attrsStr = attributesToString(s->nav->attributes);
			_output(indent, "<nav style=\"%s\" %s>", styleStr, attrsStr);
			for (NavItem *it = s->nav->items; it; it = it->next) {
				_output(indent+1, "<a href=\"%s\">%s</a>", it->link, it->label);
			}
			_output(indent, "</nav>");
			free(styleStr);
			free(attrsStr);
			break;
		}
		case STATEMENT_FORM: {
			char *styleStr = styleToString(s->form->style);
			char *attrsStr = attributesToString(s->form->attributes);
			_output(indent, "<form style=\"%s\" %s>", styleStr, attrsStr);
			for (FormItem *it = s->form->items; it; it = it->next) {
				_output(indent+1,
					"<label>%s<input placeholder=\"%s\"/></label>",
					it->label,
					it->placeholder
				);
			}
			_output(indent, "</form>");
			free(styleStr);
			free(attrsStr);
			break;
		}
		case STATEMENT_FOOTER: {
			char *styleStr = styleToString(s->footer->style);
			_output(indent, "<footer style=\"%s\">", styleStr);
			for (StatementList *it = s->footer->body; it; it = it->next) {
				_generateStatement(indent+1, it->statement);
			}
			_output(indent, "</footer>");
			free(styleStr);
			break;
		}
		case STATEMENT_CARD: {
			char *styleStr = styleToString(s->card->style);
			_output(indent, "<div class=\"card\" style=\"%s\">", styleStr);
			for (StatementList *it = s->card->body; it; it = it->next) {
				_generateStatement(indent+1, it->statement);
			}
			_output(indent, "</div>");
			free(styleStr);
			break;
		}
		case STATEMENT_BUTTON: {
			char *styleStr = styleToString(s->button->style);
			char *actionStr = attributesToString(s->button->action);
			_output(indent, "<button style=\"%s\" %s>", styleStr, actionStr);
			for (StatementList *it = s->button->body; it; it = it->next) {
				_generateStatement(indent+1, it->statement);
			}
			_output(indent, "</button>");
			free(styleStr);
			free(actionStr);
			break;
		}
		case STATEMENT_TABLE: {
			char *styleStr = styleToString(s->table->style);
			_output(indent, "<table style=\"%s\">", styleStr);
			for (TableRowList *r = s->table->rows; r; r = r->next) {
				_output(indent+1, "<tr>");
				for (TableCellList *c = r->row->cells; c; c = c->next) {
					_output(indent+2, "<td>");
					for (StatementList *cell = c->cell->content; cell; cell = cell->next) {
						_generateStatement(indent+3, cell->statement);
					}
					_output(indent+2, "</td>");
				}
				_output(indent+1, "</tr>");
			}
			_output(indent, "</table>");
			free(styleStr);
			break;
		}
		case STATEMENT_UNORDERED_LIST: {
			char *styleStr = styleToString(s->unordered_list->style);
			_output(indent, "<ul style=\"%s\">", styleStr);
			for (StatementList *it = s->unordered_list->items; it; it = it->next) {
				_generateStatement(indent+1, it->statement);
			}
			_output(indent, "</ul>");
			free(styleStr);
			break;
		}
		case STATEMENT_BULLET_ITEM: {
			_output(indent, "<li>");
			_generateStatement(indent+1, s->bullet_item->body);
			_output(indent, "</li>");
			break;
		}
		case STATEMENT_ORDERED_LIST: {
			char *styleStr = styleToString(s->ordered_list->style);
			_output(indent, "<ol style=\"%s\">", styleStr);
			for (StatementList *it = s->ordered_list->items; it; it = it->next) {
				_generateStatement(indent+1, it->statement);
			}
			_output(indent, "</ol>");
			free(styleStr);
			break;
		}
		case STATEMENT_ORDERED_ITEM: {
			_output(indent, "<li value=\"%s\">", s->ordered_item->number);
			_generateStatement(indent+1, s->ordered_item->body);
			_output(indent, "</li>");
			break;
		}
		case STATEMENT_ROW: {
			char *styleStr = styleToString(s->row->style);
			_output(indent, "<div class=\"row\" style=\"%s\">", styleStr);
			for (StatementList *it = s->row->columns; it; it = it->next) {
				_generateStatement(indent+1, it->statement);
			}
			_output(indent, "</div>");
			free(styleStr);
			break;
		}
		case STATEMENT_COLUMN: {
			char *styleStr = styleToString(s->column->style);
			_output(indent, "<div class=\"column\" style=\"%s\">", styleStr);
			for (StatementList *it = s->column->body; it; it = it->next) {
				_generateStatement(indent+1, it->statement);
			}
			_output(indent, "</div>");
			free(styleStr);
			break;
		}
		case STATEMENT_DEFINE: {
			DefineStatementList *newNode = malloc(sizeof(DefineStatementList));
    		newNode->define = calloc(1, sizeof(Define));
			newNode->define->name = strdup(s->define->name);
			newNode->define->parameters = s->define->parameters ? s->define->parameters : NULL;
			newNode->define->style = s->define->style ? s->define->style : NULL;
			newNode->define->body = s->define->body ? s->define->body : calloc(1, sizeof(StatementList));
    		newNode->next = NULL;

   			if (_defineStatementList == NULL) {
        		_defineStatementList = newNode;
    		} else {
        		DefineStatementList *it = _defineStatementList;
        		while (it->next != NULL) {
            		it = it->next;
        		}
        		it->next = newNode;
    		}
			
    	break;
			
		}
		case STATEMENT_USE: {
			DefineStatementList *it = _defineStatementList;
			while (it) {
				if (strcmp(it->define->name, s->use->name) == 0) {
					Parameter *pDefine = it->define->parameters->head;
					Parameter *pUse    = s->use->parameters->head;
					while (pDefine && pUse) {
						pDefine->value = pUse->value;
						pDefine = pDefine->next;
						pUse    = pUse->next;
					}

					for (StatementList *stmt = it->define->body;
						stmt;
						stmt = stmt->next)
					{
						_generateStatement(indent, stmt->statement);
					}
					break;
				}
				it = it->next;
			}
			break;
		}
        default:
            logError(_logger, "Tipo de statement no soportado: %d", s->type);
            break;
    }
}


/**
 * Generates the output of the program.
 */
//en el caso del ejemplo el programa solo arranca una expresion
static void _generateProgram(Program *program) {
    for (StatementList *it = program->statements; it; it = it->next) {
		if(it->statement){
        	_generateStatement(1, it->statement);
		}
    }
}

/**
 * Creates the prologue of the generated output, a Latex document that renders
 * a tree thanks to the Forest package.
 *
 * @see https://ctan.dcc.uchile.cl/graphics/pgf/contrib/forest/forest-doc.pdf
 */
// esto es lo que se genera al principio del archivo, el prologo
//tendriamos que poner lo de doctype ponele
static void _generatePrologue(void) {
    _output(0,
      "<!DOCTYPE html>\n"
      "<html lang=\"en\">\n"
      "<head>\n"
      "  <meta charset=\"UTF-8\">\n"
      "  <title>Salida TP</title>\n"
      "</head>\n"
      "<body>"
    );
}

/**
 * Generates an indentation string for the specified level.
 */
//lo hace mas lindo, para html sirve para que se vea mejor
static char * _indentation(const unsigned int level) {
	return indentation(_indentationCharacter, level, _indentationSize);
}

/**
 * Outputs a formatted string to standard output. The "fflush" instruction
 * allows to see the output even close to a failure, because it drops the
 * buffering.
 */
//esto hay que modificar si queres cambiar el tipo de salida, por ejemplo a un archivo
//el generador deberia quedar transparente al tipo de salida, por eso es importante
//que la salida la maneje output
static void _output(const unsigned int indentationLevel, const char * const format, ...) {
    va_list args;
    va_start(args, format);

    char *indent = _indentation(indentationLevel);
    fputs(indent, _outputFile);
    free(indent);

    vfprintf(_outputFile, format, args);
    fputc('\n', _outputFile);
    fflush(_outputFile);

    va_end(args);
}



/** PUBLIC FUNCTIONS */

void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");
	_symbolTable = compilerState->symbolTable;
	_generatePrologue();
	//si el arbol es nulo no hay nada que generar
	//podemos tener un generador de codigo por cada tipo de nodo y hacerlo recursivo
	//nos enfoocamos en cada nodo, arrancamos con el nodo raiz y vamos bajando
	//no si o si tiene que ser recursivo, pero es una buena forma de hacerlo
	_generateProgram(compilerState->abstractSyntaxtTree);
	_generateEpilogue();
	logDebugging(_logger, "Generation is done.");
}

#include "Generator.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
}

void shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PRIVATE FUNCTIONS */

static void _generateEpilogue();
static void _generateProgram(Program * program);
static void _generatePrologue(void);
static char * _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char * const format, ...);
static void _generateStatement(unsigned indent, Statement *s);
static char * styleToString(ParameterList *style);

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

static void _generateStatement(unsigned indent, Statement *s) {
    switch (s->type) {
		//podemos cambiar que style no aparezca vacio pero no me parece mal dejar como para que 
		//lo complete el programador si quiere

		//lo de action tambien hay que agregarlo segun corresponda
        case STATEMENT_HEADER1:
			_output(indent, "<h1>%s</h1>", s->text->content);
			break;
		case STATEMENT_HEADER2:
			_output(indent, "<h2>%s</h2>", s->text->content);
			break;
		case STATEMENT_HEADER3:
			_output(indent, "<h3>%s</h3>", s->text->content);
			break;
		case STATEMENT_PARAGRAPH:
			_output(indent, "<p>%s</p>", s->text->content);
			break;
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
			_output(indent, "<nav style=\"%s\">", styleStr);
			for (NavItem *it = s->nav->items; it; it = it->next) {
				_output(indent+1, "<a href=\"%s\">%s</a>", it->link, it->label);
			}
			_output(indent, "</nav>");
			free(styleStr);
			break;
		}
		case STATEMENT_FORM: {
			char *styleStr = styleToString(s->form->style);
			//falta la parte del action
			_output(indent, "<form style=\"%s\">", styleStr);
			for (FormItem *it = s->form->items; it; it = it->next) {
				_output(indent+1,
					"<label>%s<input placeholder=\"%s\"/></label>",
					it->label,
					it->placeholder
				);
			}
			_output(indent, "</form>");
			free(styleStr);
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
			_output(indent, "<button style=\"%s\" actions?=\"%s\">", 
				styleStr,
				s->button->action ? s->button->action->head->value : ""
			);
			for (StatementList *it = s->button->body; it; it = it->next) {
				_generateStatement(indent+1, it->statement);
			}
			_output(indent, "</button>");
			free(styleStr);
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
        // define y use queda medio para despues porque necesitamos variables
		case STATEMENT_DEFINE: {
			break;
		}
		case STATEMENT_USE: {
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
        _generateStatement(1, it->statement);
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
    fputs(indent, stdout);
    free(indent);

    vfprintf(stdout, format, args);

    fputc('\n', stdout);
    fflush(stdout);

    va_end(args);
}


/** PUBLIC FUNCTIONS */

void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");
	_generatePrologue();
	//podemos tener un generador de codigo por cada tipo de nodo y hacerlo recursivo
	//nos enfoocamos en cada nodo, arrancamos con el nodo raiz y vamos bajando
	//no si o si tiene que ser recursivo, pero es una buena forma de hacerlo
	_generateProgram(compilerState->abstractSyntaxtTree);
	_generateEpilogue();
	logDebugging(_logger, "Generation is done.");
}

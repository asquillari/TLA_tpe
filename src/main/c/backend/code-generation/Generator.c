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

static void _generateStatement(unsigned indent, Statement *s) {
    switch (s->type) {
		//lo de style esta mal pero porque hay que iterar por los parametros
		//y no se como hacerlo bien, por ahora lo dejamos asi
		//lo de action tambien hay que agregarlo segun corresponda
        case STATEMENT_TEXT:
			// esto en realidad no tiene que ser <p> porque puede ser texto que este entre headers	
			//quizas tenemos que poner un flag en el nodo de texto para saber si es un header o no	
			//tambien tenemos que tener cuidado con las variables {{}}	
			_output(indent, "<p>%s</p>", s->text->content);
			break;
		case STATEMENT_IMAGE: {
			const char *fmt = "<img src=\"%s\" alt=\"%s\" style=\"%s\"/>";
			_output(indent, fmt, s->image->src, s->image->alt, s->image->style ? s->image->style->head->value : "");
			break;
		}
		case STATEMENT_NAV: {
			_output(indent, "<nav style=\"%s\">", s->nav->style ? s->nav->style->head->value : "");
			for (NavItem *it = s->nav->items; it; it = it->next) {
				_output(indent+1, "<a href=\"%s\">%s</a>", it->link, it->label);
			}
			_output(indent, "</nav>");
			break;
		}
		case STATEMENT_FORM: {
			//falta la parte del action
			_output(indent, "<form style=\"%s\">", s->form->style ? s->form->style->head->value : "");
			for (FormItem *it = s->form->items; it; it = it->next) {
				_output(indent+1,
					"<label>%s<input placeholder=\"%s\"/></label>",
					it->label,
					it->placeholder
				);
			}
			_output(indent, "</form>");
			break;
		}
		case STATEMENT_FOOTER: {
			_output(indent, "<footer style=\"%s\">",
				s->footer->style ? s->footer->style->head->value : ""
			);
			for (StatementList *it = s->footer->body; it; it = it->next) {
				_generateStatement(indent+1, it->statement);
			}
			_output(indent, "</footer>");
			break;
		}
		case STATEMENT_CARD: {
			_output(indent, "<div class=\"card\" style=\"%s\">",
				s->card->style ? s->card->style->head->value : ""
			);
			for (StatementList *it = s->card->body; it; it = it->next) {
				_generateStatement(indent+1, it->statement);
			}
			_output(indent, "</div>");
			break;
		}
		case STATEMENT_BUTTON: {
			_output(indent, "<button style=\"%s\" actions?=\"%s\">", 
				s->button->style  ? s->button->style->head->value  : "",
				s->button->action ? s->button->action->head->value : ""
			);
			for (StatementList *it = s->button->body; it; it = it->next) {
				_generateStatement(indent+1, it->statement);
			}
			_output(indent, "</button>");
			break;
		}
		case STATEMENT_TABLE: {
			_output(indent, "<table style=\"%s\">",
				s->table->style ? s->table->style->head->value : ""
			);
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
			break;
		}
		case STATEMENT_UNORDERED_LIST: {
			_output(indent, "<ul style=\"%s\">",
				s->unordered_list->style ? s->unordered_list->style->head->value : ""
			);
			for (StatementList *it = s->unordered_list->items; it; it = it->next) {
				_generateStatement(indent+1, it->statement);
			}
			_output(indent, "</ul>");
			break;
		}
		case STATEMENT_BULLET_ITEM: {
			_output(indent, "<li>");
			_generateStatement(indent+1, s->bullet_item->body);
			_output(indent, "</li>");
			break;
		}
		case STATEMENT_ORDERED_LIST: {
			_output(indent, "<ol style=\"%s\">",
				s->ordered_list->style ? s->ordered_list->style->head->value : ""
			);
			for (StatementList *it = s->ordered_list->items; it; it = it->next) {
				_generateStatement(indent+1, it->statement);
			}
			_output(indent, "</ol>");
			break;
		}
		case STATEMENT_ORDERED_ITEM: {
			_output(indent, "<li value=\"%s\">", s->ordered_item->number);
			_generateStatement(indent+1, s->ordered_item->body);
			_output(indent, "</li>");
			break;
		}
		case STATEMENT_ROW: {
			_output(indent, "<div class=\"row\" style=\"%s\">",
				s->row->style ? s->row->style->head->value : ""
			);
			for (StatementList *it = s->row->columns; it; it = it->next) {
				_generateStatement(indent+1, it->statement);
			}
			_output(indent, "</div>");
			break;
		}
		case STATEMENT_COLUMN: {
			_output(indent, "<div class=\"column\" style=\"%s\">",
				s->column->style ? s->column->style->head->value : ""
			);
			for (StatementList *it = s->column->body; it; it = it->next) {
				_generateStatement(indent+1, it->statement);
			}
			_output(indent, "</div>");
			break;
		}
        // define y use queda medio para despues porque necesitamos variables
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

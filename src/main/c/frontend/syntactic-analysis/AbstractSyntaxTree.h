#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include <stdlib.h>

typedef struct Node Node;
typedef struct Program Program;
typedef struct StatementList StatementList;
typedef struct Parameter Parameter;
typedef struct ParameterList ParameterList;
typedef struct Define Define;
typedef struct Use Use;
typedef struct Form Form;
typedef struct Footer Footer;
typedef struct Row Row;
typedef struct Column Column;
typedef struct Nav Nav;
typedef struct Text Text;
typedef struct Image Image;
typedef struct Button Button;
typedef struct Card Card;
typedef struct ListItem ListItem;
typedef struct OrderedList OrderedList;
typedef struct UnorderedList UnorderedList;

// -----------------------------
// Enum de tipos de nodo
// -----------------------------
typedef enum NodeType {
    NODE_DEFINE,
    NODE_USE,
    NODE_FORM,
    NODE_FOOTER,
    NODE_ROW,
    NODE_COLUMN,
    NODE_NAV,
    NODE_TEXT,
    NODE_IMAGE,
    NODE_ORDERED_LIST,
    NODE_UNORDERED_LIST,
    NODE_BUTTON,
    NODE_CARD,
} NodeType;

// -----------------------------
// Nodo genérico
// -----------------------------
struct Node {
    NodeType type;
    union {
        Define* define;
        Use* use;
        Form* form;
        Footer* footer;
        Row* row;
        Column* column;
        Nav* nav;
        Button* button;
        Card* card;
        Text* text;
        Image* image;
        OrderedList* ordered_list;
        UnorderedList* unordered_list;
    };
};

// -----------------------------
// Programa y lista de sentencias
// -----------------------------
struct Program {
    StatementList* statements;
};

struct StatementList {
    Node* statement;
    StatementList* next;
};

// -----------------------------
// Parámetros
// -----------------------------
struct Parameter {
    char* name;
    char* type;
    char* default_value;
    Parameter* next;
};

struct ParameterList {
    Parameter* head;
};

// -----------------------------
// Nodo Define y Use
// -----------------------------
struct Define {
    char* name;
    ParameterList* parameters;
    StatementList* body;
};

struct Use {
    char* name;
    ParameterList* arguments; // reutiliza estructura de parámetros
};

// -----------------------------
// Formulario
// -----------------------------
struct Form {
    char* name;
    ParameterList* fields;
    ParameterList* attributes;
    StatementList* body;
};

// -----------------------------
// Footer, Row, Column, Nav
// -----------------------------
struct Footer {
    ParameterList* attributes;
    StatementList* body;
};

struct Row {
    StatementList* columns; // solo columnas permitidas
};

struct Column {
    ParameterList* attributes;
    StatementList* body;
};

struct Button {
    ParameterList* attributes;  // atributos como style, type, etc.
    StatementList* body;        // contenido del botón
};

struct Card {
    ParameterList* attributes;  // atributos como style, width, etc.
    StatementList* body;        // contenido del card
};


struct Nav {
    ParameterList* attributes;
    ListItem* items;
};

// -----------------------------
// Texto, Imagen, Listas
// -----------------------------
struct Text {
    char* content;
    int level; // 1, 2, 3 para encabezado o 0 para texto plano
};

struct Image {
    char * src;
    char * alt;
};


struct ListItem {
    char* content;
    ListItem* next;
};

struct OrderedList {
    ListItem* items;
};

struct UnorderedList {
    ListItem* items;
};

// -----------------------------
// Funciones de creación y destrucción
// -----------------------------
void initializeAbstractSyntaxTreeModule();
void shutdownAbstractSyntaxTreeModule();

void releaseProgram(Program* program);
void releaseNode(Node* node);
void releaseStatementList(StatementList* list);
void releaseParameterList(ParameterList* params);
void releaseListItems(ListItem* items);

#endif

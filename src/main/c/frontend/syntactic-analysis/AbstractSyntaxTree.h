#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

/**
 * This typedefs allows self-referencing types.
 */

typedef struct Node Node; // Adelanta la existencia del struct Node

typedef enum ExpressionType ExpressionType;
typedef enum FactorType FactorType;

typedef struct Constant Constant;
typedef struct Expression Expression;
typedef struct Factor Factor;
typedef struct Program Program;

// New typedefs for AST nodes
typedef struct DefineNode DefineNode;
typedef struct UseNode UseNode;
typedef struct FormNode FormNode;
typedef struct FooterNode FooterNode;
typedef struct RowNode RowNode;
typedef struct ColumnNode ColumnNode;
typedef struct NavNode NavNode;
typedef struct OrderedListNode OrderedListNode;
typedef struct UnorderedListNode UnorderedListNode;
typedef struct TextNode TextNode;
typedef struct ImageNode ImageNode;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

enum ExpressionType {
    ADDITION,
    DIVISION,
    FACTOR,
    MULTIPLICATION,
    SUBTRACTION
};

enum FactorType {
    CONSTANT,
    EXPRESSION
};

struct Constant {
    int value;
};

struct Factor {
    union {
        Constant * constant;
        Expression * expression;
    };
    FactorType type;
};

struct Expression {
    union {
        Factor * factor;
        struct {
            Expression * leftExpression;
            Expression * rightExpression;
        };
    };
    ExpressionType type;
};

struct Program {
    Expression * expression;
};

// New structures for AST nodes

typedef enum {
    DEFINE_NODE,
    USE_NODE,
    PARAMETER_LIST_NODE,
    FORM_NODE,
    FOOTER_NODE,
    ROW_NODE,
    COLUMN_NODE,
    NAV_NODE,
    ORDERED_LIST_NODE,
    ORDERED_ITEM_LIST_NODE,
    UNORDERED_LIST_NODE,
    BULLET_ITEM_LIST_NODE,
    TEXT_NODE,
    IMAGE_NODE
} NodeType;


struct DefineNode {
    char *name;
    Node *parameters;
    Node *statements;
};

struct UseNode {
    char *name;
    Node *parameters;
};

struct FormNode {
    char *name;
    Node *statements;
};

struct FooterNode {
    char *name;
    Node *statements;
};

struct RowNode {
    Node *statements;
};

struct ColumnNode {
    char *name;
    Node *statements;
};

struct NavNode {
    char *name;
    Node *statements;
};

struct OrderedListNode {
    char *name;
    Node *items;
};

struct UnorderedListNode {
    char *name;
    Node *items;
};

struct TextNode {
    char *text;
};

struct ImageNode {
    char *url;
    char *altText;
};

struct Node {
    NodeType type;
    Node *next;
    union {
        DefineNode define;
        UseNode use;
        struct {
            const char *parameter;
            Node *next;
        } parameterList;
        FormNode form;
        FooterNode footer;
        RowNode row;
        ColumnNode column;
        NavNode nav;
        OrderedListNode orderedList;
        struct {
            const char *item;
            Node *next;
        } orderedItemList;
        UnorderedListNode unorderedList;
        struct {
            const char *item;
            Node *next;
        } bulletItemList;
        TextNode text;
        ImageNode image;
    };
};

/**
 * Node recursive destructors.
 */
void releaseConstant(Constant * constant);
void releaseExpression(Expression * expression);
void releaseFactor(Factor * factor);
void releaseProgram(Program * program);

// New destructors for AST nodes
void releaseDefineNode(DefineNode *node);
void releaseUseNode(UseNode *node);
void releaseFormNode(FormNode *node);
void releaseFooterNode(FooterNode *node);
void releaseRowNode(RowNode *node);
void releaseColumnNode(ColumnNode *node);
void releaseNavNode(NavNode *node);
void releaseOrderedListNode(OrderedListNode *node);
void releaseUnorderedListNode(UnorderedListNode *node);
void releaseTextNode(TextNode *node);
void releaseImageNode(ImageNode *node);

#endif
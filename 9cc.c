
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// type of token
typedef enum {
    TK_RESERVED, //記号
    TK_NUM,      //整数トークン
    TK_EOF,      //入力の終わりを表すトークン
} TokenKind;

typedef struct Token Token;

//トークン型
struct Token {
    TokenKind kind;
    Token *next;
    int val;
    char *str;
};

//現在着目しているトークン
Token *token;

//エラーを報告するための関数
//printfと同じ引数を取る
void error(char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

//次のトークンが期待している記号のときには、トークンを１つ読み進めて
//真を返す。　それ以外の場合には偽を返す。
bool consume(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op)
      return false;
    token = token->next;
    return true;
}

//次のトークンが期待している記号のときには、トークンを１つ読み進める。
//それ以外の場合にはエラーを報告する。
int expect_number(){
    if(token->kind != TK_NUM)
      error("数ではありません");
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof() {
    return token->kind == TK_EOF;
}

//　新しいトークンを作成してcurに繋げる
Token *new_token(TokenKind kind, Token *cur, char *str) {
    Token *tok = calloc(1,sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    cur -> next = tok;
    return tok;
}

//　入力文字列pをトーク内図してそれを返す
Token *tokenize(char *p){
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
      //空白文字をスキップ
    if (isspace(*p)){
        p++;
        continue;
    }
    if (*p == '+' || *p == '-'){
        cur = new_token(TK_RESERVED, cur, p++);
        continue;
    }

    if (isdigit(*p)) {
        cur = new_token(TK_NUM,cur,p);
        cur->val = strtol(p,&p,10);    
        continue;
    }

     error("トークナイズできません")；
    }

    new_token(TK_EOF, cur, p);
    return head.next;
}

int main(int argc, char **argv){
    if (argc != 2){
        fprintf(stderr,"引数の個数が正しくありません\n");
        return 1;
    }
    //　トークナイズする
    token = tokenize(argv[1]);

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");
    printf("  mov rax, %ld\n", strtol(p, &p, 10));

    while (*p) {
        if (*p == '+'){
            p++;
            printf("  add rax, %ld\n", strtol(p, &p, 10));
            continue;
        }

        if (*p == '-'){
            p++;
            printf("  sub rax, %ld\n", strtol(p, &p, 10));
            continue;
         }
         
         fprintf(stderr,"予期しない文字です: '%c'\n", *p);
         return 1;
    }

    printf("  ret\n");
    return 0;
}
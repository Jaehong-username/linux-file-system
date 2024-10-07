#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct node {
	char  name[64];       // node's name string
	char  type;
	struct node *child, *sibling, *parent;
	// other fields if needed
} NODE;

//initializing a node if not explicit modificiation is not passed in.

void init_node(NODE *pMem);

void seperate_command_pathname(char user_input[], char *command, char *pathname);

bool split_dir_base(char pathname[], char *dirname, char *basename);

bool directory_exists(char dirname[],NODE* pRoot, NODE** CWD);

bool mkdir(char pathname[], NODE* pRoot ,NODE* CWD, char type);

bool rmdir(char pathname[], NODE* pRoot ,NODE* CWD, char type);

bool ls(char pathname[], NODE* pRoot, NODE* CWD);

bool cd(char pathname[], NODE* pRoot ,NODE** CWD);

void reverse_string_helper(char *str, int start_index, int end_index);

void reverse_string(char* str);

void pwd(NODE* pRoot, NODE* CWD, FILE* outfile);

void creat(char pathname[], NODE* pRoot, NODE* CWD, char type);

void rm(char pathname[], NODE* pRoot ,NODE* CWD, char type);

bool reload(FILE* infile, NODE* pRoot ,NODE* CWD, char* filename);

void recDown(NODE* pRoot, NODE* pCur, FILE* outfile);

bool save(FILE* outfile, NODE* pRoot, char* filename);

void quit(FILE* outfile, NODE* pRoot);

/****************************************************************************************************************************
* Programmer: Jaehong Lee
* Date: September 12th, 2024
* Description: This assignment mimics the directory/file system in our computer using linux commands such as mkdir, rmdir,
			   ls, cd, pwd, creat, rm, reload, save, quit. Data will be represented in a Left-child right-sibling binary tree.
****************************************************************************************************************************/


#include "command.h"          

NODE *root;
NODE *cwd;
char *cmd[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm", 
               "reload", "save", "quit", 0};  // fill with list of commands
//o at the end is terminating the string will null terminator
//because it is a char array.


int initialize() {//This will initialize the root node as soon as the program is executed

	root = (NODE *)malloc(sizeof(NODE)); // first creating a root node named /
	if(root == NULL){
		printf("Memory allocation for root node failed!\n");
		return -1;
	}
	strcpy(root->name, "/");
	root->parent = root; //for root node both parentPtr and siblingPtr will point to itself
	root->sibling = root;  //for root node both parentPtr and siblingPtr will point to itself
	root->child = 0;
	root->type = 'D';
	cwd = root;
	printf("Filesystem initialized!\n");
	printf("To end the program, please type quit\n");
	return 0;
}

int find_command(char* user_command){
	int i = 0;
	while(cmd[i]){
		if((strcmp(user_command, cmd[i]) == 0))
			return i;
		i++;
	}

	return -1;
}


int main(int argc, char* argv[]){

	FILE *infile = NULL;
	FILE *outfile = NULL;

	initialize();
	
	char user_input[300] = ""; //declare a string to store a command from user input
	char command[20] = "";
	char pathname[280] = "";
	
	bool again = true;
	int command_choice;

	while (again == true)
	{
		printf("Enter command: ");
		// complete implementations
		fgets(user_input, 300, stdin);
		//command[strcspn(command, "\n")] = '\0'; // to remove any a newline character at the end
		// since fgets will include \n from user input at the end. ---not needed!!
		seperate_command_pathname(user_input, command, pathname);
		// cant use switch statement since command is an array of char instead of int or char
		// so had to use a bunch of if statements
		
		command_choice = find_command(command);
		
		switch(command_choice){
			case 0: //mkdir
				mkdir(pathname, root, cwd, 'D');
				break;
			case 1: //rmdir
				rmdir(pathname, root, cwd, 'D');
				break;
			case 2: //ls
				ls(pathname, root, cwd);
				break;
			case 3: //cd
				cd(pathname, root, &cwd);
				break;
			case 4: //pwd
				pwd(root, cwd, outfile);
				break;
			case 5: //creat
				creat(pathname, root, cwd, 'F');
				break;
			case 6: //rm
				rm(pathname, root, cwd, 'F');
				break;
			case 7: //reload
				reload(infile, root, cwd, pathname);
				break;
			case 8: //save
				save(outfile, root, pathname);
				break;
			case 9: //quit
				quit(outfile, root);
				again = false;
				break;
			default:
				printf("Invalid Command! Please type the command again!\n");
		}
	}

	printf("Quit Command Entered. Terminating the program!\n");
	return 0;
}

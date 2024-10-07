#include "command.h"

void init_node(NODE *pMem)
{
	strcpy(pMem->name, "");
	pMem->type = '\0';
	pMem->child = NULL;
	pMem->sibling = NULL;
	pMem->parent = NULL;
}

void seperate_command_pathname(char user_input[], char *command, char *pathname)
{
    char raw_pathname[100] = "";
    char *token = strtok(user_input, " \n");
    //loading the command!
    strcpy(command, token);
    
    token = strtok(NULL, "\n");//pathname   0        pathname
    if(token != NULL){
        strcpy(raw_pathname, token); //could be only white space!! or an actual filename!
    }else{
        strcpy(raw_pathname, "");
    }

    //check whitespace
    int i = 0;
    while(raw_pathname[i] != '\0'){ //make sure! !=
        if(raw_pathname[i] != ' '){
            
            break;
        }
        i++; //increment the pointer!
    }
    
    //loading the pathname!
    strcpy(pathname, &raw_pathname[i]);
}

//hello\0 01234
bool split_dir_base(char pathname[], char *dirname, char *basename)
{
    int i = strlen(pathname) - 1;
    
    while(pathname[i] != '/' && i >= 0){
        i--;
    }

    // once out of the loop and couldn't find / like mkdir helllo
    if (i == -1){
        strcpy(dirname, ""); //there is nothing on the dirname
        strcpy(basename, &pathname[0]);
        return true;
    }

    pathname[i] = '\0'; //replace the last / with a null terminator
    strcpy(dirname, pathname);
    strcpy(basename, &pathname[i + 1]);
    
    return true;
}

bool directory_exists(char dirname[],NODE *pRoot, NODE **CWD)
{
    //NODE *initial_CWD = *CWD; //so that when the function fails, CWD goes back to the original ptr modifed by
    //directory_exist function!
    char *dir_token;
    bool is_absolute_path = false;
    //   /a/b/c vs a/b/c
    // This code will get through if the file name is an absolute path!!
    // NODE *sibling_check = pRoot; not needed!!
    if(dirname[0] == '/'){ //this is an absolute path!!
        //dir_token = strtok(dirname, "/"); No need for this otherwise strtok executed one more time!
        //Key point I realized: if the starting point has a delimiter, this delimiter will be skipped even if we use strtok!!
        //Therefore, this if condition strcmp(dir_token, "") == 0 is pointless will never get through.
        // Error! Once the string is Nullt terminated (NULL), we can;y use any of string built in functions
        //if (strcmp(dir_token, "") == 0){// it indicates that dirname is an absolute path!
            *CWD = pRoot; //starting at the root node!
            is_absolute_path = true;
        //}
    }
    
    //relative pathname: Code starts here!
    //Example:a/b/c (absolute path got tokenized once) vs a/b/c
    
    //*CWD = pRoot->child; //going down to the child node!
    //dir_token = strtok(NULL, ",");
    //if(is_absolute_path == true){ //if it's an absolute path
    //    dir_token = strtok(dirname, "/");
    //}else{//if its not an absolute path
    //    dir_token = strtok(dirname, "/");
    //}
    
    dir_token = strtok(dirname, "/");
    //if command is for example mkdir hello code will just stop here

    //Error! while (strcmp(dir_token, "\0") '\o' is not a string!, we dont put a nullterminator as a delimitier!
    while (dir_token != NULL){ //as long as it not null terminator, keep while loop going!
        *CWD = (*CWD)->child;//once we know that we have directory to go down indicated by strtok not resulting in NULL CWD needs to go one step down to loof for any matching among siblings
        while(*CWD != NULL){
            if(strcmp((*CWD)->name, dir_token) == 0 && (*CWD)->type == 'D'){//same directory exists!
                printf("Current Directory found in the file system!\n");
                break;
            }
            
            else if(strcmp((*CWD)->name, dir_token) == 0 && (*CWD)->type == 'F'){
                printf("Error: Entry exists but it is a file not a directory!\n");
                //*CWD = initial_CWD;
                return false;
            }
            
            *CWD = (*CWD)->sibling; //moving onto the next sibling to check!!
        }
        
        //Case 1: CWD does not have any child nodes
        //Case 2: CWD has som sibling nodes! Hoowever, none of the files mathces with either name of directory or file
        if(*CWD == NULL){
            printf("Error: Entry does not exists in the file system!\n");
            //*CWD = initial_CWD;
            return false;
        }
        //once out of the loop!found the directory! time to go down again if there is more!
        dir_token = strtok(NULL, "/");
    }
    //once out of the while loop!! now CWD points to where you need to make the new directory!
    return true;
}

bool mkdir(char pathname[], NODE* pRoot ,NODE* CWD, char type)
{
    char dirname[100];
    char basename[100];

    // if no pathname is given. or the pathname is only composed of white errors! Error!
    if(strcmp(pathname, "") == 0){ //make sure to put == 0
        printf("mkdir: missing operand\n");
        return false;
    }

    //spliting the pathname into dirname and basename!
    if(!split_dir_base(pathname, dirname, basename)){
        if(type == 'D'){
        printf("mkdir command failed!\n");
        }else if(type == 'F'){
        printf("creat command failed!\n"); //make sure its in double quotes!!
        }
        return false;
    }
    
    
    //dir name example: /a/b/c from absolute pathname or a/b/c from relative pathname
    //char *dir_token = strtok(dirname, "/");
    //if(strcmp(dir_token, "") == 0){ //it indicates that dirname is an absolute path!
    //    CWD = pRoot; //starting at the root node!
    //}
    
    
    //checks for if the following directory exists before creating a new directory!
    if(!directory_exists(dirname, pRoot, &CWD)){
        if(type == 'D'){
        printf("mkdir command failed!\n");
        }else if(type == 'F'){
        printf("creat command failed!\n");
        }
        return false;
    }
    
    // if there are some files under that directory (No child nodes)
    // Search for basename in (under) the dirname node To see if it already exists or not!!
    /*
    if(CWD->child != NULL){
        NODE *sibling_check = CWD->child;
        
        while(sibling_check != NULL){
            if(strcmp(sibling_check->name, basename) == 0 && sibling_check->type == 'D'){//same directory exists!
                printf("Error: Same directory exists! Please rename your directory!\n");
                return false;
            }

            sibling_check = sibling_check->sibling; //go to the next sibiling!
        }
    }*/
    
    //ADD a new DIR node under dirname
    //make node and initialize it!
    NODE *pMem = (NODE *)malloc(sizeof(NODE));
    init_node(pMem);
    strcpy(pMem->name, basename);
    pMem->type = type; // File type is directory (D)
    //pMem->parent = CWD->child; /.issue: CWD->child is NULL!
    
    if(CWD->child == NULL){
        CWD->child = pMem;//connecting to the parent node.
        pMem->parent = CWD; //connecting it back!
    }else{//means that there is at least one child node. Need to insert it through the sibling pointer
        CWD = CWD->child; //go down to traverse through the sibiling nodes
        if(strcmp(CWD->name, pMem->name) == 0 && CWD->type == 'D'){
            printf("Error: A directory with the same name already exists!\n");
            return false;
        }else if(strcmp(CWD->name, pMem->name) == 0 && CWD->type == 'F'){
            printf("Error: Cannot have both a file and a directory with the same name!\n");
            return false;
        } //do checking before going into the while loop other wise mkdir h mkdir would not make an error!
        while(CWD->sibling != NULL){ //needs to do checking if there is already existing directory
            CWD = CWD->sibling;
            if(strcmp(CWD->name, pMem->name) == 0 && CWD->type == 'D'){
                printf("Error: A directory with the same name already exists!\n");
                return false;
            }else if(strcmp(CWD->name, pMem->name) == 0 && CWD->type == 'F'){
                printf("Error: Cannot have both a file and a directory with the same name!\n");
                return false;
            } //do checking before going into the while loop other wise mkdir h mkdir would not make an error!
        } //once it out of the while loop, that's where insertion will happen
        CWD->sibling = pMem; //adding a node to one way linked list
        //Error!!! Make sure to connect it to the parent node!!!!
        NODE *temp = CWD->parent;
        pMem->parent = temp;
    }

    if(type == 'D'){
        printf("mkdir command success!\n");
    }else if(type == 'F'){
        printf("creat command success!\n");
    }
    
    return true;
}

// I didn't make CWD a double pointer so that after the function ends CWD goes back to where it was originally!
bool rmdir(char pathname[], NODE *pRoot, NODE *CWD, char type)
{
    NODE *temp = NULL;
    char dirname[100];
    char basename[100];
  

    if (pathname[0] == '/'){
        printf("It is an absolute pathname!\n");
        CWD = pRoot;
    }
    
    if(!split_dir_base(pathname, dirname, basename)){
        printf("Error: pathname is not in the correct format!\n");
        if(type == 'D'){
            printf("rmdir command failed!\n");
        } else if(type == 'F'){
            printf("rm command failed!\n");
        }
        
        return false;
    }
    
    
    //before removing the directory, you have to know that the directory exists in the file system!
    if(!directory_exists(dirname, pRoot, &CWD)){
        if(type == 'D'){
            printf("rmdir command failed!\n");
        } else if(type == 'F'){
            printf("rm command failed!\n");
        }
        
        return false;
    }
    
    
    //if missing operand, return false!
    if(strcmp(basename, "") == 0 && type == 'D'){
        printf("rmdir: missing operand\n");
        return false;
    }else if(strcmp(basename, "") == 0 && type == 'F'){
        printf("rm: missing operand\n");
        return false;
    }

    if(CWD->child == NULL){
        printf("DIR %s does not exist!\n", basename);
        return false;
    }

    //There is one directory we need to check for whether it exists. the basename!
    //Case 0 deleting the ONLY child node directly connected to the parent node
    if(strcmp(CWD->child->name, basename) == 0 && CWD->child->sibling == NULL){
        if(CWD->child->type != type && type == 'D'){
            printf("It's a file not a directory! DIR %s does not exist!\n", basename);
            return false;
        }
        else if (CWD->child->type != type && type == 'F')
        {
            printf("It's a directory not a file! File %s does not exist!\n", basename);
            return false;
        }
        else if(CWD->child->type == type && CWD->child->child != NULL && type == 'D'){//not empty
            printf("Error: Cannot delete the directory because the directory is not empty!\n");
            return false;
        }
        else if(CWD->child->type == type && CWD->child->child != NULL && type == 'F'){//not empty
            printf("Error: Cannot delete the file because the file is not empty!\n");
            return false;
        }
        temp = CWD->child;
        CWD->child = NULL;
    }
    //Case 1 deleting the node directly connected to the parent node.
    else if(strcmp(CWD->child->name, basename) == 0 && CWD->child->sibling != NULL){
        if(CWD->child->type != type && type == 'D'){
            printf("It's a file not a directory! DIR %s does not exist!\n", basename);
            return false;
        }
        else if (CWD->child->type != type && type == 'F')
        {
            printf("It's a directory not a file! File %s does not exist!\n", basename);
            return false;
        }
        else if(CWD->child->type == type && CWD->child->child != NULL && type == 'D'){//not empty
            printf("Error: Cannot delete the directory because the directory is not empty!\n");
            return false;
        }
        else if(CWD->child->type == type && CWD->child->child != NULL && type == 'F'){//not empty
            printf("Error: Cannot delete the file because the file is not empty!\n");
            return false;
        }
        
        if(CWD->child->type == type && CWD->child->child != NULL && type == 'D'){//not empty
            printf("Error: Cannot delete the directory because the directory is not empty!\n");
            return false;
        }
        else if(CWD->child->type == type && CWD->child->child != NULL && type == 'F'){//not empty
            printf("Error: Cannot delete the file because the file is not empty!\n");
            return false;
        }
        
        temp = CWD->child;
        CWD->child = temp->sibling; //relinking the pointer
        temp->sibling->parent = CWD; //relinking the pointer
        
    }
    else //if not! we need to go down and further search for it.
    {
        NODE *pPrev = NULL;
        CWD = CWD->child; // go down to the child node
        // traverse to the next until one node before the target
        while(CWD != NULL){
            if(strcmp(CWD->name, basename) == 0 && CWD->sibling != NULL && CWD->type == type){
                //Case 2: deleting the sibling node in the middle
                pPrev->sibling = CWD->sibling;
                temp = CWD;
                break;
            }else if (strcmp(CWD->name, basename) == 0 && CWD->sibling == NULL && CWD->type == type){
                if(CWD->child == NULL && type == 'D'){
                    printf("Error: Cannot delete the directory because the directory is not empty!\n");
                    return false;
                }
                else if (CWD->child == NULL && type == 'F')
                {
                    printf("Error: Cannot delete the file because the file is not empty!\n");
                    return false;
                }
                //Case 3: deleting the last sibling node at the end!!
                //Warning!! Once we cut the pointer it can no longer be retreived!!
                //we need to make sure that this if statement only goes through when deletion happend!!
                //which was the reason why I added CWD->type condition as well!
                pPrev->sibling = NULL; //cut the pointer!
                temp = CWD;
                break;
            }else if(strcmp(CWD->name, basename) == 0 && CWD->sibling != NULL && CWD->type != type && type == 'D'){
                printf("It's a file not a directory! DIR %s does not exist!\n", basename);
                return false;
            }else if(strcmp(CWD->name, basename) == 0 && CWD->sibling != NULL && CWD->type != type && type == 'F'){
                printf("It's a directory not a file! File %s does not exist!\n", basename);
                return false;
            }else if (strcmp(CWD->name, basename) == 0 && CWD->sibling == NULL && CWD->type != type && type == 'D'){
                printf("It's a file not a directory! DIR %s does not exist!\n", basename);
                return false;
            }else if (strcmp(CWD->name, basename) == 0 && CWD->sibling == NULL && CWD->type != type && type == 'F'){
                printf("It's a directory not a file! File %s does not exist!\n", basename);
                return false;
            }
            
            
            pPrev = CWD;
            CWD = CWD->sibling;
        }
        
        if(CWD == NULL){ //or file DIrectory to delete doesn't exist in the file system
            if(type == 'D'){
                printf("DIR %s does not exist! The directory name that you are trying to delete doesn't exist in the file system!\n", basename);
            } else if(type == 'F'){
                printf("File %s does not exist! The file name that you are trying to delete doesn't exist in the file system!\n", basename);
            }
            
            return false;
        }
    }

    //Now we have the pointer pointing to the node we want to delete! But before that
    //we want to make sure that the node type is correct! D for rmdir or F for rm
    //If the command is rm! which is removing the file instead of the directory!
    if(temp->type == 'F' && type == 'F'){
        init_node(temp); //reset to init values!
        free(temp);
        printf("rm command success!\n");
        return true;
    }else if(temp->type == 'D' && type == 'F'){ //if file you are trying to delete is a directory instead!
        printf("It's a directory not a file! rm command failed!\n");
        return false;
    }


    //rmdir command! but the node type is F
    else if(temp->type == 'F' && type == 'D'){
        printf("It's not a directory but a file!\n");
        printf("rmdir command failed!\n");
        return false;
    }else if(temp->type == 'D' && type == 'D'){//once out if and else if condition here now we know that node type is a directory!
        //now we need to check if the directorty has any contents or not!
        //Now CWD found the pointer to the directory to be deleted!
        //First Let's check if the directory is empty!
        if(temp->child != NULL){
            printf("Error: Cannot delete the directory because the directory is not empty!\n");
            printf("rmdir command failed!\n");
            return false;
        }else{ //if CWD is empty! temp->child == NULL
            init_node(temp); //reset to init values!
            free(temp);
            printf("rmdir command success!\n");
            return true;
        }
    }
}

//there is no turning back!
bool ls(char pathname[], NODE* pRoot, NODE* CWD)
{
    
    if(strcmp(pathname, "") == 0){
        printf("List the contents under %s: \n", pathname);
        CWD = CWD->child;
        while (CWD != NULL)
        {
            //CWD = CWD->child; //we need to put it down to avoid seg error.
            printf("%c %s  ", CWD->type, CWD->name);
            // CWD = CWD->sibling; will cause an infinite error!
            //becuase the root node will point inself to the sibling and parent
            CWD = CWD->sibling; //traverse to the next sibling instead of to the child
            
        }
        printf("\n");
        return true;
    }

    //ex: ls a/b/c
    else{
        
        char dirname[100];
        char basename[100];
        if(!split_dir_base(pathname, dirname, basename)){
            printf("No such file or directory: %s\n", pathname);
            return false;
        }
        
        if(!directory_exists(dirname, pRoot, &CWD)){
            printf("No such file or directory: %s\n", pathname);
            return false;
        }
        
        CWD = CWD->child; //need this code! otherwise the root node will just loop forever!
        while(CWD != NULL){
            if(strcmp(CWD->name, basename) == 0){ //if found the base directory
                break;
            }
            CWD = CWD->sibling;
        }
        
        if(CWD == NULL){
            printf("No such file or directory: %s\n", pathname);
            return false;
        }
        
        //once out of the while loop! //its descendant node are the ones to be printed
        CWD = CWD->child;
        printf("List the contents under %s: \n", pathname);
        while (CWD != NULL){
            printf("TYPE: %c      NAME: %s\n", CWD->type, CWD->name);
            CWD = CWD->sibling;
        }
        return true;
    }
}

//double pointer needed! becuase we need to save the update on CWD memory location
bool cd(char pathname[], NODE* pRoot ,NODE** CWD)
{
    NODE *initial_CWD = *CWD; //initialize the currenrt directory just in case it fails!!
    char dirname[100];
    char basename[100];
    
    
    if (strcmp(pathname, "") == 0)
    {
        //if no pathname specified!
        printf("No pathname specified! It goes back to the root file!\n");
        *CWD = pRoot;
        printf("cd command success!\n");
        return true;
    }
    
    else if(strcmp(pathname, "..")==0){
        //if pathname .. go to the parent node
        printf(".. pathname entered! It goes back to the parent file!\n");
        *CWD = (*CWD)->parent;
        printf("cd command success!\n");
        return true;
    }
    //there is no turning back!
    else{
        
        if(!split_dir_base(pathname, dirname, basename)){
        printf("No such file or directory: %s\n", pathname);
        printf("cd command failed!\n");
        return false;
        }
        
        //&CWD remember that we are passing CWD as a ptr to ptr! so
        //we should pass in jsut CWD instead of &CWD oterwise it woulmd be ***
        if(!directory_exists(pathname, pRoot, CWD)){
            printf("No such file or directory: %s\n", pathname);
            *CWD = initial_CWD; //set CWD back to the initial memory location
            return false;
        }
        //         a/b/c
        //once directory_exists function returns true CWD will point to the last name

        //printf("%s\n", (*CWD)->name);
        if(strcmp(dirname, "") != 0){ //only goes through when you use / in the cd command!
            (*CWD) = (*CWD)->child; //it needs to go one step down!!
        }
        
        while (*CWD != NULL)
        {
            if(strcmp((*CWD)->name, basename) == 0){ //Error make sure to include == 0!! for strcmp!!
                printf("Found the final directory location!\n");
                printf("cd command success!\n");
                //since CWD passed as a double pointer it will be saved!
                return true;
            }
            *CWD = (*CWD)->sibling;
        }

        if(*CWD == NULL){
            printf("No such file or directory: %s\n", pathname);
            printf("cd command failed!\n");
            *CWD = initial_CWD; //Error !! make sure to put *! since CWD is a double pointer!
            return false;
        }

        printf("cd command success!\n");
        return true;
    }   
}

void reverse_string_helper(char *str, int start_index, int end_index)
{
    if(start_index < end_index){
        char temp = str[start_index];
        str[start_index] = str[end_index];
        str[end_index] = temp;
        reverse_string_helper(str, start_index + 1, end_index - 1);
    }else{//adding a nullterminator
        str[strlen(str)] = '\0';
    }
}

void reverse_string(char* str)
{
    int start_index = 0;
    int end_index = strlen(str) - 1;
    reverse_string_helper(str, start_index, end_index);
    //str[end_index + 1] = '\0'; //another method of putting a null terminator!
    
    // if(*str != '\0'){
    // reverseString(str + 1, dest);
    // int len = strlen(dest);
    // dest[len] = *str;
    // dest[len + 1] = '\0';
    //  printf("%c", *str);
    //  strcat(dest, str); error! will print the gibberish value!
    //}
}

//print the absolute pathname of CWD
void pwd(NODE* pRoot, NODE* CWD, FILE* outfile)
{
    if (CWD == pRoot){
        printf("Absolute pathname of CWD: /\n");
        return;
    }
    
    char absolute_pathname[200] = "";
    NODE *ptrCur = CWD;
    char temp[100] = "";
    while (ptrCur != pRoot)
    {
        strcpy(temp, ptrCur->name); //fir
        reverse_string(temp);
        // issue Fixed: file name will be stored in non reversed order! So before using strcat!
        // I have to store it in temp variable which will be reversed.
        // We shouldn;t reverse the original string to not distort the original data
        strcat(absolute_pathname, temp);
        strcat(absolute_pathname, "/"); //instead add / here!!
        ptrCur = ptrCur->parent;
    }

    //once it out of the while loop, pCur will point to the root node actaully no need!
    //strcat(absolute_pathname, "/");
    
    //when using the pwd command!
    if(outfile == NULL){
        //now we just need to reverse the string
        printf("Absolute pathname of CWD: ");
        reverse_string(absolute_pathname); // will reverse back to the original.
        printf("%s\n", absolute_pathname); //make sure to print after reversing the string!
        return;
    }else{//outfile != NULL if outfile is open! during the saving process
        reverse_string(absolute_pathname); // will reverse back to the original.
        fprintf(outfile, "          %s\n", absolute_pathname);
    }
}

//create a file under the given Directory
void creat(char pathname[], NODE* pRoot ,NODE* CWD, char type)
{
    mkdir(pathname, pRoot, CWD, 'F');
}

//remove a file under the given Directory!
void rm(char pathname[], NODE* pRoot ,NODE* CWD, char type)
{
    rmdir(pathname, pRoot, CWD, 'F');
}

//Precondition: Input file must be open before implementing the function
bool reload(FILE* infile, NODE* pRoot ,NODE* CWD, char* filename)
{
    if(strcmp(filename, "fssim_Lee.txt") == 0){
        infile = fopen(filename, "r");
        if(infile == NULL){
            printf("Input file is not open successfully!\n");
            printf("reload command failed!\n");
            return false;
        }
    
        char line[300];
        char node_type;
        char absolute_pathname[280];
    
        if(feof(infile)){
            //if empty!
            printf("Nothing to reload the file system from the input file!\n");
            return false;
        }

        fgets(line, 300, infile);
        while (fgets(line, 300, infile) != NULL)
        {
            //fgets(line, 300, infile); // h e l l o \n \0
            line[strlen(line) - 1] = '\0'; //replacing \n with a null terminator!!
            node_type = line[0];
            strcpy(absolute_pathname, &line[11]);
        
            if(node_type == 'D'){ //creating a directory!
                mkdir(absolute_pathname, pRoot, CWD, 'D');
            }
        
            else if(node_type == 'F'){
                creat(absolute_pathname, pRoot, CWD, 'F');
            }
        }

        printf("reload command success!\n");
        fclose(infile);
        return true;
    }else{
        printf("%s name doesn't exist! reload command failed!\n", filename);
        return false;
    }
}

void recDown(NODE* pRoot, NODE* pCur, FILE* outfile)
{
    if(pCur->child != NULL){ //keep going down untile there is no child node!
        fprintf(outfile, "%c", pCur->type);
        pwd(pRoot, pCur, outfile);
        recDown(pRoot, pCur->child, outfile); //keeping the pRoot the same!
    }else{
        //if pCur reaches to the bottom!
        fprintf(outfile, "%c", pCur->type);
        pwd(pRoot, pCur, outfile);
    }
    //pwd(pRoot, pCur, outfile);
}

bool save(FILE* outfile, NODE* pRoot, char* filename)
{
    if(strcmp(filename, "fssim_Lee.txt") == 0){
        outfile = fopen(filename, "w");
        if(outfile == NULL){
            printf("outfile is not open successfully!\n");
            printf("save failed!\n");
            return false;
        }
        NODE *pCur = pRoot;
        fprintf(outfile, "%c          ", pCur->type);
        fprintf(outfile, "/\n");
        pCur = pRoot->child; //pCur going down to the root's child node!
        while (pCur != NULL){
            recDown(pRoot, pCur, outfile);
            pCur = pCur->sibling;
        }
        fseek(outfile, -1, SEEK_CUR);
        //fputc('\0', outfile);
        // char last_sentence[100];
        // fgets(last_sentence, 100, outfile);
        // last_sentence[strcspn(last_sentence, "\n")] = '\0';
        fclose(outfile); // close the outfile at the end!

        printf("save success!\n");
        return true;
    }else{
        printf("%s name doesn't exist! save command failed!\n", filename);
        return false;
    }
}

void quit(FILE* outfile, NODE* pRoot)
{
    //before terminating the program, let's save the tree to output file!
    char filename[] = "fssim_Lee.txt";
    save(outfile, pRoot, filename);
    printf("Quitting the program!\n");
    return;
}

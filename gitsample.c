#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define MAX_COMMIT_IDS 16
#define COMMIT_ID_LENGTH 12
#define COMMAND_SIZE 512

typedef uint8_t u8;

struct git_wraper {
	u8 command[COMMAND_SIZE];
	int (*command_process) (const char *command);
};

void ussage(void) {
        printf("Please use cli arguments in like gitsample <branch>\n");
        printf("Hello Dirty code\n");
        printf("It seems your system is crashed\n");
        printf("I will tell you how to use.Don't worry Take a deep breath\n");
        printf("<<<<<<<=======Ussage guide=======>>>>>>\n");
        printf("-a is for cherry-pick and Please give <commit-id>\n");
        printf("-b git log --oneline\n");
        printf("-c is for checkout and Please give branch name is required\n");
        printf("-d is for meld it is showing the diff btween the two files\n");
        printf("-e is for conflict resolution method\n");
        
	return;
}
/*void search_commit(const char *commit_id) {
    char command[512];
    char parsecommit_id[13];
    int counter = -1;
    int len = strlen(commit_id);
    
    strcpy(command, "git log --oneline | grep -E '(");
    for(int i = 0; i < len; i+counter) {
        counter++;
        if (commit_id[i] != ' ') {
            strncpy(parsecommit_id, &commit_id[i], counter);
            parsecommit_id[++counter] = '\0';
            strcat(command, parsecommit_id);
        }
        if (i + counter < len)
            strcat(command, "|");
            counter = -1; //reset counter
        }
    }
    //snprintf(command, sizeof(command), "git log --oneline | grep %s", commit_id);
    printf("Searching for commit: %s\n", commit_id);
    system(command);
}*/

/*void search_commit(struct git_wraper *ptr_commit, const char *commit_id) {
    char command[512];
    
    snprintf(ptr_commit->command, sizeof(ptr_commit->command), "git log --oneline | grep -E %s", commit_id);
    printf("Searching for commit: %s\n", commit_id);
    //system(command);
}*/

void search_commit(struct git_wraper *wraper, const char *commit_ids) {
    char *commit_array[MAX_COMMIT_IDS];
    int commit_count = 0;

    // Split commit_ids into an array of individual commit IDs
    char *commit_ids_copy = strdup(commit_ids);
    char *token = strtok(commit_ids_copy, "|");
    while (token != NULL && commit_count < MAX_COMMIT_IDS) {
        if (strlen(token) == COMMIT_ID_LENGTH) {
            commit_array[commit_count] = token;
            commit_count++;
        } else {
            fprintf(stderr, "Error: Each commit ID must be %d characters long\n", COMMIT_ID_LENGTH);
            free(commit_ids_copy);
            return;
        }
        token = strtok(NULL, "|");
    }

    // Prepare the command to include the commit IDs
    snprintf(wraper->command, sizeof(wraper->command), "git log --oneline | grep -E '%s'", commit_ids);

    free(commit_ids_copy);
}

int execute_command(const char *command) {
    return system(command);
}

int main(int argc, char *argv[])
{
	int opt, status;
	bool search = false;
	u8 conflict[40], version[40], commit_id[60];
	struct git_wraper wraper;

	if(argc <= 1) {
		ussage();
		return 1;
	}
                ussage();
                // Initialize command and process function
                wraper.command_process = execute_command;

	        // Loop through each option (flag) provided to the program
                while ((opt = getopt(argc, argv, "a:bc:d:e:f")) != -1) {
		        switch (opt) {
		        case 'a':
                         printf("Option -a is set for cherry-pick \n");
                         // Format the command string
                         snprintf((char *)wraper.command, sizeof(wraper.command), "git cherry-pick %s", argv[2]);
			 status = execute_command((char *)wraper.command);
                         break;
                case 'b':
                        printf("Option -b is set git log --oneline");
                        search = true;
                        search_commit(&wraper, argv[2]);
                        //snprintf((char *)wraper.command, sizeof(wraper.command), "git log --oneline > /tmp/log");
                        break;
                case 'c':
		        	    printf("Option -c is set for git checkout <branch_name>\n");
        		        snprintf((char *)wraper.command, sizeof(wraper.command), "git checkout %s", argv[2]);
	        		    break;
	        	case 'd':
	        	        printf("Option -d is set for meld <file-1.c> <file-2.c>\n");
	        	        snprintf((char *)wraper.command, sizeof(wraper.command), "meld %s %s", argv[2], argv[3]);
		case 'e':
			printf("option -e Welcome to conflict resolution tool just pay some attention\n");
			printf("Please provid me the Conflict provided file paths&names:version make sure Enter proper version\n");
			scanf("%s", &conflict);
			printf("PLease enter a version make sure Enter proper version\n");
			scanf("%s", &version);
			strcpy((char *)wraper.command, "git cherry-pick --abort");
			status = execute_command((char *)wraper.command);
			snprintf((char *)wraper.command, sizeof(wraper.command), "git checkout %s", version);
			status = execute_command((char *)wraper.command);
			snprintf((char *)wraper.command, sizeof(wraper.command), "cp ~/%s ~/home*", conflict);
			status = execute_command((char *)wraper.command);
			snprintf((char *)wraper.command, sizeof(wraper.command), "git checkout %s", argv[2]);
			printf("Please Enter conflicted commit-id");
			scanf("%s", &commit_id);
			snprintf((char *)wraper.command, sizeof(wraper.command), "git cherry-pick %s", commit_id);
			status = execute_command((char *)wraper.command);
			snprintf((char *)wraper.command, sizeof(wraper.command), "meld %s ~/home/*.c", conflict);
			status = execute_command((char *)wraper.command);
			break;
			
	        	case 'f':
	        	        printf("Bye Bye Dirty code\n");
	        	        exit(EXIT_FAILURE);
		        default:
			        fprintf(stderr, "Usage: %s [-a] [-b value] [-c]\n", argv[0]);
			        return 1;
		}
	}

	// Process the command
//	int status = wraper.command_process((const char *)wraper.command);

	if(status == -1)
		perror("system");
	else {
		if (WIFEXITED(status)) {
			int exit_status = WEXITSTATUS(status);
			printf("Command exited with status %d\n", exit_status);
		} else {
			printf("Command did not exit successfully\n");
		}
	}
  //  if(search)
    //    search_commit(argv[2]);
        
    return 0;

}


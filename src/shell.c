#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <dirent.h>
char *pwd;
char *buf;
char *thing;
char builtin_com[15][10] = {"cd", "help", "exit", "exec", "ls", "pwd"};
// the commands
int shell_cd(char **args) {
  if ((args[1] == NULL) || (args[1][0] == '\0')) {
      printf("Expected atleast 1 argument\n");
    }
  int i = chdir(args[1]);
  if ((i != 0) && (args[1][0] != '\0')) {
    printf("No such file or directory: %s\n", args[1]);
    return -1;
  } 
}
int shell_help() {
  printf("MaSS V0.03\n List of commands:\n CD: Changes directory\n exec: Executes a file or command with no arguments\n LS: Displays all files in a directory\n PWD: Displays the current working directory\n help: displays this menu\n exit: exits the terminal\nIf you encouter any problems please report them on github.\n");
  return 0;
}

int shell_exit() {
  free(buf);
  free(thing);
  exit(0);
}
int shell_exec(char *com) {
  system(com); //throwback to RCMSH
}
int shell_pwd(void) {
  pwd = getcwd(NULL, 0);
  printf("%s\n", pwd);
}
int shell_ls(char **args) {
  DIR *folder;
  struct dirent *foldername;
  if ((args[1] == NULL) || (args[1][0] == '\0')) { // how is a null terminator and NULL different (well I know, but it's still ridiculous)
    folder = opendir("."); // opens directory
    } else {
      folder = opendir(args[1]);
    }
      if (folder == NULL) {
	printf("Folder %s does not exist\n", args[1]);
	return -1;
      }
      while (foldername = readdir(folder)) { //reads variable which contains dir contents
	printf("%s\n", foldername->d_name); 
      }
      closedir(folder);
      return 0;
}
 
int shell_execute(char *com, char **args) {
  int status;
  int error;
  pid_t spoon = fork();
  if (spoon == 0) {
  fflush(stdout);
  error = execvp(com, args); //It's not a valid string? huh
      if (error == -1) {
	printf("No such command: %s\n", com);
	exit(errno);
    }
        fflush(stdout);
	} else if (spoon < 0) {
    printf("Error forking\n");
    return -1;
  } else {
    if(WIFEXITED(status)) {
      //printf("child exited with = %d\n",WEXITSTATUS(status));
    }
        do {
      waitpid(spoon, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 0;
}
// Handling of the commands
int shell(char *command, char **arguments) {
    int i;
  for (i = 0; i < 6; i++) {
    if (strcmp(command, builtin_com[i]) == 0) { //This should work, why the [hell] does it not then // it was the switch I am actually braindead
      break;
    }
  }
  switch (i) {
  case 0:
    shell_cd(arguments);
    break;
  case 1:
    shell_help();
    break;
  case 2:
    shell_exit();
    break;
  case 3:
    shell_exec(command);
    break;
  case 4:
    shell_ls(arguments);
    break;
  case 5:
    shell_pwd();
    break;
  default:
    if (command == NULL) {
      return 1;
    } else {
      shell_execute(command, arguments);
    }
  }
  return 0;
}

int main(void) {
  // variable declarations
  char input[100];
  char hostname[25];
  char *user = getenv("USER");
  char *home = getenv("HOME");
  buf = (char*)malloc(51);
  thing = (char*)malloc(10*sizeof(char*)+1); // had to manually allocate memory instead of using strtok for initialization
  FILE *test;
  FILE *musc;
  bool muscisyes; 
    int x = 0;
          int a = 0;
      int z = 1;
      int t = 0;
      int v = 0;
      int l = 1;
      	int o = 0;
	int p = 0;
      char **comarg2 = calloc(2+100,sizeof(char**));
      // opening .msuc (which is basically a lobotimzed .bashrc) file
      char muscloc[30];
      sprintf(muscloc, "%s/.msuc", home);
      musc = fopen(muscloc, "r");
      if (musc == NULL) {
	muscisyes = false; // musc is no
      }
      muscisyes = true; // musc is yes, cue the applause mike // Looking back on this I realise I am not funny and never will be
    // grabbing the host name
    test = fopen("/etc/hostname", "r");
    fgets(hostname, 25, test);
    int i = strcspn(hostname, "\n");
    hostname[i] = 0;
    while(1) {
      //input
      pwd = getcwd(NULL, 0);
      if (muscisyes == false) {
	printf("|%s@%s %s|# ",user,hostname,pwd);
      fflush(stdout);
	fgets(input, 99, stdin);
      } else {
	char home[100][100];
	while (1) {
	  fgets(home[o], 99, musc);
	  if ((home[o][0] == '\0') && (o != 0)) {
	    break;
	  }
	  	  o++;
	}
	o = o - 1;
	if (p <= o) {
	  strcpy(input, home[p]);
	} else {
	  muscisyes = false;
	}
	p++;
	if ((input == NULL || muscisyes == false)) {
	    muscisyes = false;
	    /* struct termios modes;
  modes.c_lflag &= ISIG;
  tcsetattr(STDIN_FILENO, TCSANOW, &modes);*/ // set terminal attributes
	    printf("|%s@%s %s|# ",user,hostname,pwd);
	    fflush(stdout);
	    fgets(input, 99, stdin);
	  }
      }
    // tokenization/processing
      thing = strtok(input, " \n\r");
    x = 0;
      while (thing != NULL) {
	comarg2[x] = strdup(thing); // this has taken me several days to figure out
	x++;
	thing = strtok(NULL, " \n");
      }
      comarg2[x] = NULL;
    char com[100];
    if (comarg2[0] == NULL) {
      continue;
    } else {
    strcpy(com, comarg2[0]);
    }
    shell(com, comarg2);
    }
}


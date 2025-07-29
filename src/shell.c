#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <dirent.h>
char builtin_com[15][10] = {"cd", "help", "exit", "exec", "ls"};
// input (work in progress)
char readln(char **input, size_t size) {
  int rows;
  int collumns;
  int bytes;
  while (1) {
    read(1, (char *)input[rows][collumns], 1);
    bytes++;
    if (input[rows][collumns] == ' ') {
      input[rows][collumns++] = '\0';
      rows++;
      collumns = 0;
    } else if (input[rows][collumns] == '\n') {
      input[rows][collumns++] = '\0';
      return bytes;
    } else if (collumns == (int)size-1) {
      input[rows][collumns++] = '\0';
      rows++;
    } else if ((collumns == (int)size-1) && (rows == (int)size)) {
	return 2; //exit code 2 means that it was succesful in reading bytes, but exited due to input about to be overflowed
      }
    collumns++;
  }
}
// the commands
int shell_cd(char args[100][100], char* pwd) {
    if (args[1] == NULL) {
      printf("Expected atleast 1 argument\n");
    }
  int i = chdir(args[1]);
  if (i != 0) {
    printf("No such file or directory: %s\n", args[1]);
    return -1;
  } 
}
int shell_help() {
  printf("RSH V0.01\n List of commands:\n cd: changes directory\n exec: executes a file or command with no arguments\n ls: displays all files in a directory \n help: displays this menu\n exit: exits the terminal\nIf you encouter any problems please report them on github.\n");
  return 0;
}

int shell_exit() {
  exit(0);
}
int shell_exec(char *com) {
  system(com); //throwback to RCMSH
}
int shell_ls(char args[100][100]) {
  DIR *folder;
  struct dirent *foldername;
    if (args[1] == NULL) {
      folder = opendir(".");
    } else {
      folder = opendir(args[1]);
    }
      if (folder == NULL) {
	printf("Folder %s does not exist\n");
	return -1;
      }
      while (foldername = readdir(folder)) {
	printf("%s\n", foldername->d_name);
      }
      closedir(folder);
      return 0;
}
 
int shell_execute(char *com, char *args[100]) {
  int status;
  int error;
  pid_t spoon = fork();
  if (spoon == 0) {
  fflush(stdout);
  error = execvp(com, args);
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
         printf("child exited with = %d\n",WEXITSTATUS(status));
    }
        do {
      waitpid(spoon, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 0;
}
// Handling of the commands
int shell(char *command, char **arguments, char args[100][100], char *pwd) {
    int i;
  for (i = 0; i < 6; i++) {
    if (strcmp(command, builtin_com[i]) == 0) { //This should work, why the fuck does it not then // it was the switch I am actually braindead
      break;
    }
  }
  switch (i) {
  case 0:
    shell_cd(args, pwd);
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
    shell_ls(args);
    break;
  default:
    if (command == NULL) {
      return 1;
    } else {
      shell_execute(command, arguments);
    }
  }
  /*  int t = 0;
        char null[100][100];
              while (1) {
	      arguments[t] = strndup(/null[t], 100);
	      args[t][0] = 0;
	if (t == 20) {
	  break;
	}
	t++;
	} */ // wouldn't work

}

int main() {
  char input[100];
  char hostname[25];
  char *user = getenv("USER");
  char comarg[100][100]; // I'm praying this works, If it doesn't I'll hug my blahaj and cry myself to sleep tonight // I will be crying myself to sleep tonight
  char *buf = (char*)malloc(51);
  char *pwd = getcwd(NULL, 0);
  FILE *test;
    int x = 0;
    // grabbing the host name
    test = fopen("/etc/hostname", "r");
    fgets(hostname, 25, test);
    int i = strcspn(hostname, "\n");
    hostname[i] = 0;
    while(1) {
      //input
      pwd= getcwd(NULL, 0);
      printf("[%s@%s %s]$ ",user,hostname,pwd);
      fflush(stdout);
      //readln(comarg, 100);
      read(1, input, 100);
    // tokenization/processing
    char *thing = strtok(input, " \n");
    int r = x;
    x = 0;
    while (thing != NULL) {
      strncpy(comarg[x], thing, 100); // I love valgrind and gdb //kill me // why //fixed
          x++;
    thing = strtok(NULL, " \n");
    }
    comarg[x][0] = '\0';
    //spamification/more processing (I could go for some spam ham actually)
    x = x - 1;
    char com[100];
    strncpy(com, comarg[0], 100);
      char arg[100][100];
      int a;
      int z = 1;
 //my code works when I write it, and then I don't touch it, otherwise it'll break somehow
      int t = 0;
      char **comarg2 = calloc(100,sizeof(char*));
      while (1) {
	comarg2[t] = strndup(comarg[t], 100); // this has taken me several days to figure out
	if (t == x) {
	  break;
	}
	t++;
      }
      shell(com, comarg2, comarg, pwd);
    }
}


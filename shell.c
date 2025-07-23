#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
char builtin_com[15][10] = {"cd", "help", "exit"};
// the commands
  int shell_cd(char *args[100], char* pwd) {
    if (args[1] == NULL) {
      printf("Expected atleast 1 argument\n");
    }
  int i = chdir(args[1]);
  if (i != 0) {
    printf("No such file or directory: %s\n", args[1]);
    return -1;
  } 
  pwd = getenv("PWD");
}
int shell_help() {
  printf("RCRSH V 0.01\n List of commands:\n cd: changes directory\n help: displays this menu\n exit: exits the terminal\n");
  return 0;
}

int shell_exit(char *arguments[100], char *pwd) {
          printf("test");
	  //free(arguments);
	  //free(pwd);
  exit(0);
}

int shell_execute(char *com, char *args[100]) {
  int status;
  pid_t spoon = fork();
  if (spoon == 0) {
  fflush(stdout);
      if (execvp(com, args) == -1) {
	printf("No such command: %s\n", com);
    }
        fflush(stdout);
      return -1;
	} else if (spoon < 0) {
    printf("Error forking\n");
    return -1;
  } else {
        do {
      waitpid(spoon, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 0;
}
// Handling of the commands
  int shell(char *command, char *arguments[100], char *pwd) {
    int i;
  for (i = 0; i < 4; i++) {
    if (strcmp(command, builtin_com[i]) == 1) {
      break;
    }
  }
  switch (i) {
  case 1:
    shell_cd(arguments, pwd);
    break;
  case 2:
    shell_help();
    break;
  case 3:
    shell_exit(arguments, pwd);
    break;
  default:
    if (command == NULL) {
      return 1;
    } else {
      shell_execute(command, arguments);
    }
  }
}

int main() {
  char input[100];
  char hostname[25];
  char *user = getenv("USER");
  char comarg[100][100]; // I'm praying this works, If it doesn't I'll hug my blahaj and cry myself to sleep tonight // I will be crying myself to sleep tonight
  char *pwd = (char*)malloc(100 * sizeof(char*));
  FILE *test;
    int x;
    // grabbing the host name
    test = fopen("/etc/hostname", "r");
    fgets(hostname, 25, test);
    int i = strcspn(hostname, "\n");
    hostname[i] = 0;
    while(1) {
      //input
      pwd = getenv("PWD");
      printf("[%s@%s %s]$ ",user,hostname,pwd);
      fflush(stdout);
    read(1, input, 100);
    // tokenization/processing
    char *thing = strtok(input, " \n");
    while (thing != NULL) {
      strncpy(comarg[x], thing, 100); // I love valgrind and gdb //kill me // why
          x++;
    thing = strtok(NULL, "/,.-");
    }
    x = 0;
    //spamification (I could go for some spam ham actually)
    x = x - 1;
    char com[100];
    strncpy(com, comarg[0], 100);
      char arg[100][100];
      int a;
      int z = 1;
      /*    for (z = 1; z < x; z++) {
	strcpy(arg[a], comarg[z]);
	a++;
      }
      printf(" %s\n", arg[0]); */ //my code works where I write it, and then I don't touch it. Otherwise it'll break somehow
      shell(com, (char**)comarg, pwd);
    }
}


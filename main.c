#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <linux/limits.h>

int main() {
	char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd));
	printf("%s", cwd);

	char input[200];
	while(1) {
		read(STDIN_FILENO, input, 200);
		if (strlen(input) > 0) {
			printf("%s", input);
		}
	}
}

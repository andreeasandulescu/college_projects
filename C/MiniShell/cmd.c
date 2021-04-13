/**
 * Operating Systems 2013-2017 - Assignment 2
 *
 * TODO Name, Group
 * Sandulescu Andreea, 331CB
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmd.h"
#include "utils.h"

#define READ		0
#define WRITE		1
#define APPEND     	(O_RDWR | O_CREAT | O_APPEND)
#define NORMAL 		(O_RDWR | O_CREAT | O_TRUNC)


//used to redirect stdin, stdout and stderr
static void redirect(simple_command_t *s)
{
	int check, fd_out, fd_err;
	char *name = NULL;

	if (s->in != NULL) {
		//the name of the input file
		name = get_word(s->in);
		int fd_in = open(name, O_RDONLY);

		DIE(fd_in < 0, "Error opening input file");
		//close STDIN
		check = close(STDIN_FILENO);
		DIE(check < 0, "Error closing stdin");

		//redirect the input
		check = dup(fd_in);
		DIE(check < 0, "Error redirecting stdin");
	}
	if (s->out != NULL) {
		//the name of the output file
		name = get_word(s->out);
		if (s->io_flags == IO_OUT_APPEND)
			//open file in append mode
			fd_out = open(name, APPEND, 0644);
		else
			fd_out = open(name, NORMAL, 0644);
		DIE(fd_out < 0, "Error opening output file");

		check = close(STDOUT_FILENO);
		DIE(check < 0, "Error closing stdout");

		check = dup(fd_out);
		DIE(check < 0, "Error redirecting stdout");
	}
	if (s->err != NULL) {
		if (s->out != NULL && strcmp(s->out->string, s->err->string) == 0)
			//&> operator was used,so the file which stderr must be
			//redirected to was already opened for redirecting stdout
			fd_err = fd_out;
		else {
			name = get_word(s->err);
			if (s->io_flags == IO_ERR_APPEND)
				fd_err = open(name, APPEND, 0644);
			else
				fd_err = open(name, NORMAL, 0644);
			DIE(fd_err < 0, "Error opening err file");
		}

		check = close(STDERR_FILENO);
		DIE(check < 0, "Error closing stderr");

		check = dup(fd_err);
		DIE(check < 0, "Error redirecting stderr");
	}
	free(name);

}

//set environment variables
static int set_env_vars(simple_command_t *s)
{
	char *var;
	int check;

	//the name of the variable to be set
	var = get_word(s->verb);
	if (strchr(var, '=') == NULL) {
		free(var);
		//the operation to be executed is not setting variables
		return -2;
	}
	check = putenv(var);

	if (check == 0)
		return 0;
	else
		return 1;
}


/**
 * Internal exit/quit command.
 */
static int shell_exit(void)
{
	return SHELL_EXIT;
}

/**
 * Parse a simple command (internal, environment variable assignment,
 * external command).
 */
static int parse_simple(simple_command_t *s, int level, 
	command_t *father)
{
	pid_t pid;
	int status, param_cnt, i, check, cd_comm;
	char **argv;

	DIE(s->verb == NULL, "verb is NULL!\n");

	check = strcmp("quit", s->verb->string);
	if (check == 0)
		return shell_exit();
	check = strcmp("exit", s->verb->string);
	if (check == 0)
		return shell_exit();

	cd_comm = strcmp("cd", s->verb->string);
	if (cd_comm != 0)
		//there will be no call execvp() for cd, so there is no point
		//in getting the command argument list for a cd command
		argv = get_argv(s, &param_cnt);

	check = set_env_vars(s);
	if (check != -2)
		//operation to be execd was not setting environment vars
		return check;

	pid = fork();
	switch (pid) {
	case -1:
		printf("Error forking child!\n");
		return 1;
	case 0:
		/* child process */
		//redirect stdin/stdout/stderr, if necessary
		redirect(s);
		if (cd_comm != 0)
			//only if the command to execute is not cd
			if (execvp(argv[0], (char *const *) argv) == -1)
				printf("Execution failed for '%s'\n", argv[0]);
		//only if exec failed
		exit(1);
	default:
		/* parent process */
		break;
	}

	waitpid(pid, &status, 0);

	//the command to be executed is cd
	if (cd_comm == 0) {
		if (s->params != NULL)
			check = chdir(s->params->string);
		else
			check = 0;

		if (check == 0)
			return 0;
		else
			return 1;
	} else {
		for (i = 0; i < param_cnt + 1; i++) {
			free(argv[i]);
			argv[i] = NULL;
		}
		free(argv);
	}

	//if command was false, return 1
	check = strcmp("false", s->verb->string);
	if (check == 0)
		return 1;
	check = strcmp("true", s->verb->string);
	if (check == 0)
		return 0;
	if (WIFEXITED(status))
		//if child process terminated normally
		return WEXITSTATUS(status);
	return status;
}


/**
 * Process two commands in parallel, by creating two children.
 */
static int do_in_parallel(command_t *cmd1, command_t *cmd2, int level,
		command_t *father)
{
	pid_t pid;
	int status, res1, res2;

	pid = fork();
	switch (pid) {
	case -1:
		printf("Error forking child!\n");
		exit(1);
	case 0:
		//the child will execute a command, the parent the other
		res1 = parse_command(cmd2, level+1, father);
		exit(res1);
	default:
		break;
	}

	res2 = parse_command(cmd1, level+1, father);
	if (waitpid(pid, &status, 0) == -1) {
		printf("Error waiting.\n");
		return 1;
	}

	if (WIFEXITED(res1))
		res1 = WEXITSTATUS(res1);
	if (res1 == res2)
		return res1;
	if (res1 != 0)
		return res1;
	return res2;
}

/**
 * Run commands by creating an anonymous pipe (cmd1 | cmd2)
 */
static int do_on_pipe(command_t *cmd1, command_t *cmd2, int level,
		command_t *father)
{
	pid_t pid;
	int status, exit_status, res, check, old_stdin;
	int pipefd[2];

	check = pipe(pipefd);
	DIE(check < 0, "Error opening pipe\n");

	pid = fork();
	switch (pid) {
	case -1:
		printf("Error forking child!\n");
		exit(1);
	case 0:
		check = close(pipefd[0]) == -1;
		DIE(check < 0, "Error closing reading end of pipe\n");

		//close stdout, in order to redirect stdout to writing end of pipe
		check = close(STDOUT_FILENO);
		DIE(check < 0, "Error closing stdout");

		check = dup2(pipefd[1], STDOUT_FILENO);
		DIE(check < 0, "Error in dup2 for pipefd\n");

		res = parse_command(cmd1, level+1, father);

		check = close(pipefd[1]);
		DIE(check < 0, "Error closing writing end of pipe\n");
		exit(res);
	default:
		break;
	}

	check = close(pipefd[1]) == -1;
	DIE(check < 0, "Error closing writing end of pipe\n");

	//keep a copy of stdin
	old_stdin = dup(STDIN_FILENO);
	DIE(check < 0, "Error in dup2 for STDIN\n");

	check = close(STDIN_FILENO);
	DIE(check < 0, "Error closing stdin");

	//redirect stdin to reading end of pipe
	check = dup2(pipefd[0], STDIN_FILENO);
	DIE(check < 0, "Error in dup2 for pipefd\n");

	exit_status = parse_command(cmd2, level+1, father);
	check = waitpid(pid, &status, 0);
	DIE(check < 0, "Error waiting.\n");

	check = close(pipefd[0]);
	DIE(check < 0, "Error closing reading end of pipe\n");

    //close stdin(which points to pipefd[0]) and restore it
	check = close(STDIN_FILENO);
	DIE(check < 0, "Error closing writing end of pipe\n");

	check = dup(old_stdin);
	DIE(check < 0, "Error in dup2 for STDOUT\n");

	if (WIFEXITED(exit_status))
		return WEXITSTATUS(exit_status);
	return exit_status;
}

/**
 * Parse and execute a command.
 */
int parse_command(command_t *c, int level, command_t *father)
{
	int res1, res2;

	if (c->op == OP_NONE)
		return parse_simple(c->scmd, level, father);

	switch (c->op) {
	case OP_SEQUENTIAL:
		res1 = parse_command(c->cmd1, level + 1, c);
		res2 = parse_command(c->cmd2, level + 1, c);
		if (res1 == SHELL_EXIT)
			return res1;
		else
			return res2;

	case OP_PARALLEL:
		return do_in_parallel(c->cmd1, c->cmd2, level+1, c);

	case OP_CONDITIONAL_NZERO:
		//the second command is executed only if the error code of 
		//the first one is not zero (shell_exit is seen as 0)
		res1 = parse_command(c->cmd1, level + 1, c);
		if (res1 == 0 || res1 == SHELL_EXIT)
			return res1;
		return parse_command(c->cmd2, level + 1, c);

	case OP_CONDITIONAL_ZERO:
		//the second command is executed only if the error code
		//of the first one is zero
		res1 = parse_command(c->cmd1, level + 1, c);
		if (res1 == 1 || res1 == SHELL_EXIT)
			return res1;
		return parse_command(c->cmd2, level + 1, c);

	case OP_PIPE:
		return do_on_pipe(c->cmd1, c->cmd2, level + 1, c);

	default:
		return SHELL_EXIT;
	}

	return 0;
}

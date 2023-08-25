#include "monty.h"

/**
 * open - opens a file
 * @n_file: the file namepath
 * Return: void
 */

void open(char *n_file)
{
	FILE *fi = fopen(n_file, "r");

	if (n_file == NULL || fi == NULL)
		errors(2, n_file);

	read_f(fi);
	fclose(fi);
}


/**
 * read - reads a file
 * @fi: pointer to file descriptor
 * Return: void
 */

void read_f(FILE *fi)
{
	int line_nbr, format = 0;
	char *buffer = NULL;
	size_t len = 0;

	for (line_nbr = 1; getline(&buffer, &len, fi) != -1; line_nbr++)
	{
		format = l_parse(buffer, line_nbr, format);
	}
	free(buffer);
}


/**
 * l_parse - Separates each line into tokens to determine
 * which function to call
 * @buffer: line from the file
 * @line_nbr: line number
 * @format:  storage format. If 0 Nodes will be entered as a stack.
 * if 1 nodes will be entered as a queue.
 * Return: Returns 0 if the opcode is stack. 1 if queue.
 */

int l_parse(char *buffer, int line_nbr, int format)
{
	char *opcde, *value;
	const char *delim = "\n ";

	if (buffer == NULL)
		errors(4);

	opcde = strtok(buffer, delim);
	if (opcde == NULL)
		return (format);
	value = strtok(NULL, delim);

	if (strcmp(opcde, "stack") == 0)
		return (0);
	if (strcmp(opcde, "queue") == 0)
		return (1);

	find_function(opcde, value, line_nbr, format);
	return (format);
}

/**
 * find_function - find the appropriate function for the opcode
 * @opcode: opcde
 * @value: argument of opcode
 * @format:  storage format. If 0 Nodes will be entered as a stack.
 * @ln: line number
 * if 1 nodes will be entered as a queue.
 * Return: void
 */
void find_function(char *opcde, char *value, int ln, int format)
{
	int i;
	int flag;

	instruction_t function_list[] = {
		{"push", add_t_stack},
		{"pall", st_print},
		{"pint", top_print},
		{"pop", top_pop},
		{"nop", nop},
		{"swap", swap_nodes},
		{"add", add_node},
		{"sub", sub_node},
		{"div", div_nodes},
		{"mul", mul_nodes},
		{"mod", mod_nodes},
		{"pchar", print_chr},
		{"pstr", print_str},
		{"rotl", rotate},
		{"rotr", rot_last},
		{NULL, NULL}
	};

	if (opcde[0] == '#')
		return;

	for (flag = 1, i = 0; function_list[i].opcode != NULL; i++)
	{
		if (strcmp(opcde, function_list[i].opcode) == 0)
		{
			call_function(function_list[i].f, opcde, value, ln, format);
			flag = 0;
		}
	}
	if (flag == 1)
		errors(3, ln, opcde);
}


/**
 * call_function - Calls the required function.
 * @function: Pointer to the function that is about to be called.
 * @op: string representing the opcode.
 * @val: string representing a numeric value.
 * @ln: line numeber for the instruction.
 * @format: Format specifier. If 0 Nodes will be entered as a stack.
 * if 1 nodes will be entered as a queue.
 */
void call_function(op_func function, char *op, char *val, int ln, int format)
{
	stack_t *node;
	int flag;
	int i;

	flag = 1;
	if (strcmp(op, "push") == 0)
	{
		if (val != NULL && val[0] == '-')
		{
			val = val + 1;
			flag = -1;
		}
		if (val == NULL)
			errors(5, ln);
		for (i = 0; val[i] != '\0'; i++)
		{
			if (isdigit(val[i]) == 0)
				errors(5, ln);
		}
		node = n_create(atoi(val) * flag);
		if (format == 0)
			function(&node, ln);
		if (format == 1)
			add_to_queue(&node, ln);
	}
	else
		function(&head, ln);
}

#include "main.h"
#include <unistd.h> // Include the necessary header for the write function

/**
 * get_width - Calculates the width for printing
 * @format: Formatted string in which to print the arguments.
 * @i: Pointer to the current index in the format string.
 * @list: List of arguments.
 *
 * Return: Width.
 */
int get_width(const char *format, int *i, va_list list)
{
	int curr_i;
	int width = 0;

	for (curr_i = *i + 1; format[curr_i] != '\0'; curr_i++)
	{
		if (isdigit(format[curr_i]))
		{
			width *= 10;
			width += format[curr_i] - '0';
		}
		else if (format[curr_i] == '*')
		{
			curr_i++;
			width = va_arg(list, int);
			break;
		}
		else
			break;
	}

	*i = curr_i - 1;

	return width;
}

/**
 * handle_print - Prints an argument based on its type
 * @fmt: Formatted string in which to print the arguments.
 * @ind: Pointer to the current index in the format string.
 * @list: List of arguments to be printed.
 * @buffer: Buffer array to handle print.
 * @flags: Calculates active flags.
 * @width: Width.
 * @precision: Precision specification.
 * @size: Size specifier.
 *
 * Return: Number of characters printed or -1 if invalid format specifier.
 */
int handle_print(const char *fmt, int *ind, va_list list, char buffer[],
	int flags, int width, int precision, int size)
{
	int i, unknown_len = 0, printed_chars = -1;

	typedef struct {
		char fmt;
		int (*fn)(va_list, char[], int, int, int, int);
	} fmt_t;

	fmt_t fmt_types[] = {
		{'c', print_char}, {'s', print_string}, {'%', print_percent},
		{'i', print_int}, {'d', print_int}, {'b', print_binary},
		{'u', print_unsigned}, {'o', print_octal}, {'x', print_hexadecimal},
		{'X', print_hexa_upper}, {'p', print_pointer}, {'S', print_non_printable},
		{'r', print_reverse}, {'R', print_rot13string}, {'\0', NULL}
	};

	for (i = 0; fmt_types[i].fmt != '\0'; i++)
	{
		if (fmt[*ind] == fmt_types[i].fmt)
			return fmt_types[i].fn(list, buffer, flags, width, precision, size);
	}

	if (fmt[*ind] == '\0')
		return -1;

	unknown_len += write(1, "%", 1);

	if (fmt[*ind - 1] == ' ')
		unknown_len += write(1, " ", 1);
	else if (width)
	{
		(*ind)--;
		while (fmt[*ind] != ' ' && fmt[*ind] != '%')
			(*ind)--;
		if (fmt[*ind] == ' ')
			(*ind)--;
		return 1;
	}

	unknown_len += write(1, &fmt[*ind], 1);

	return unknown_len;
}


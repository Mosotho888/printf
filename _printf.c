#include "holberton.h"

void cleanup(va_list args, buffer_t *output);
int run_printf(const char *format, va_list args, buffer_t *output);
int _printf(const char *format, ...);

/**
 *   cleanup - Peforms cleanup operations for _printf.
 *   @args: A va_list of arguments provided to _printf.
 *   @output: A buffer_t struct.
 */
void cleanup(va_list args, buffer_t *output)
{
	va_end(args);
	write(1, output->start, output->len);
	free_buffer(output);
}

/**
 *   run_printf - Reads through the format string for _printf.
 *   @format: Character string to print - may contain directives.
 *   @output: A buffer_t struct containing a buffer.
 *   @args: A va_list of arguments.
 *   Return: The number of characters stored to output.
 */
int run_printf(const char *format, va_list args, buffer_t *output)
{
	int count, wid, prec;
	int ret = 0;
	char tmp;
	unsigned char flags, len;
	unsigned int (*f)(va_list, buffer_t *,
			unsigned char, int, int, unsigned char);

	for (count = 0; *(format + count); count++)
	{
		len = 0;
		if (*(format + count) == '%')
		{
			tmp = 0;
			flags = handle_flags(format + count + 1, &tmp);
			wid = handle_width(args, format + count + tmp + 1, &tmp);
			prec = handle_precision(args,
					format + count + tmp + 1, &tmp);
			len = handle_length(format + count + tmp + 1, &tmp);
			f = handle_specifiers(format + count + tmp + 1);
			if (f != NULL)
			{
				count += tmp + 1;
				ret += f(args, output, flags, wid, prec, len);
				continue;
			}
			else if (*(format + count + tmp + 1) == '\0')
			{
				ret = -1;
				break;
			}
		}
		ret += _memcpy(output, (format + count), 1);
		count += (len != 0) ? 1 : 0;
	}
	cleanup(args, output);
	return (ret);
}

/**
 *   _printf - Outputs a formatted string.
 *   @format: Character string to print - may contain directives.
 *   Description - printing Characters that may contain directories
 *
 *   Return: The number of characters printed.
 */
int _printf(const char *format, ...)
{
	buffer_t *output;
	va_list args;
	int ret;

	if (format == NULL)
	{
		return (-1);
	}
	output = init_buffer();
	if (output == NULL)
	{
		return (-1);
	}
	va_start(args, format);
	ret = run_printf(format, args, output);
	return (ret);
}

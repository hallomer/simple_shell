#include <stdio.h>
#include <unistd.h>

/**
 * main - A program that prints the PID of the parent process
 *
 * Return: Always 0.
*/
int main(void)
{
	unsigned int parent;

	parent = getppid();
	printf("%u\n", parent);

	return (0);
}

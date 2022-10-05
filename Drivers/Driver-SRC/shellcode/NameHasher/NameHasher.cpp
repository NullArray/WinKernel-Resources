#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

DWORD getHashA(char *str)
{
	DWORD hash;
	hash = 0;

	while (*str != '\0')
	{
		hash = hash + (*(unsigned char*)str | 0x60);
		hash = hash << 1;
		str = str + 1;
	}
	return (hash);
}

int main(int argc, char *argv[])
{
	char name[250];
	DWORD hash;

	if (argc != 2)
	{
		printf("USAGE: %s <name_to_hash>\n", argv[0]);
		return 1;
	}

	hash = getHashA(argv[1]);

	if (strlen(argv[1]) > 100)
	{
		printf("[-] ERROR name too long\n");
		return 1;
	}
	
	snprintf(name, 249, "DWORD %s_hash = 0x%08X;", argv[1], hash);

	printf("%s\n", name);

	return 0;
}

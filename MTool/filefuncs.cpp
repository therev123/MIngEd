#include "filefuncs.h"

#include <stdio.h>

void writeFile(const char* path, const char* name, const char* data, long size)
{
    FILE* fp = fopen(path, "wb");
    if(fp == NULL)
	return;

    fprintf(fp, "#ifndef __%s_H__\n", name);
    fprintf(fp, "#define __%s_H__\n\n", name);
    fprintf(fp, "long %sSize() { return %d; }\n\n", name, size);
    fprintf(fp, "const char %s[%d] = \n", name, size);
    fprintf(fp, "{\n");
    for(long i = 0; i < size; i++)
    {
	fprintf(fp, "\t0x%.2hhx,", data[i]);
	if(i != 0 && i%10 == 9) fprintf(fp, "\n");
    }
    fprintf(fp, "\n};\n");
    fprintf(fp, "\n\n");
    fprintf(fp, "#endif/*__%s_H__*/\n", name);

    fclose(fp);
}

int file_embed(lua_State* L)
{
    const char* path = lua_tostring(L, -3);
    const char* dest = lua_tostring(L, -2);
    const char* name = lua_tostring(L, -1);

    FILE* fp = fopen(path, "rb+");
    if(fp == NULL)
	return 0;

    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    char* buf = new char[size];
    long read = fread(buf, sizeof(char), size, fp);

    fclose(fp);
    
    // write the header
    writeFile(dest, name, buf, size);

    delete [] buf;
    return 0;
}

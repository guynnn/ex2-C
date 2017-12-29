/*
 * My algorithm uses:
 * 1. a struct File: has a file name, array of indexes of files that the
 * file depends on in the main array (dependency) and a field for store a color of a file
 * (description follows).
 * 2. static array of structs File. In each index in the array there is a struct File.
 * The files with their array of neighbors. if we consider the files as vertices and their
 * neighbors as edges.
 * Algorithm description: in order to find a cyclic dependency in the graph that the
 * dependency array represents, we are going to traverse the graph. for doing so we have a data
 * member called color, which can be white, gray or black. In the white group, there are all the
 * vertices that we still didn't visit. in the gray group vertices that we visited but still
 * didn't finish to traverse their neighbors.
 * In the black group we have vertices that we finished to visit.
 * So we take a vertex from the white group and change his color to gray. Then we traverse on his
 * neighbors and check for each neighbor his color. if it's white, we visit it and change his
 * color to gray. it means that we have a circle between vertices, thus we return 1 in that case
 * (i.e. there is a circle). if we finished to visit any neighbor of the current vertex and
 * didn't find any gray vertex, we change the color of the current vertex to black and go to
 * the next white vertex. when we are done with all the vertex, there was no cycle.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BUFF_SIZE 1024
#define FILE_NUM 100000
#define LINE_LENGTH 1000
#define NEIGHBORS_NUM 100
#define CYCLE 1
#define NO_CYCLE 0
#define WHITE 'w'
#define GRAY 'g'
#define BLACK 'b'

/**
 * represents a file (vertex) in the graph.
 */
typedef struct
{
    char name[255];
    int index;
    int neighbors[NEIGHBORS_NUM];
    int neighborsNum;
    char color;
} File;

int readFile(char * fileName, File array[]);
int isInArray(char * name, File array[]);
void updateLine(char * line, File array[]);
void addToArray(char * name, File array[]);
void updateNeighbors(File * file, File * neighbor);
int dfs(File array[]);
int dfsHelper(File * file, File array[]);

/**
 * check a given files are cyclic dependency. prints "Cyclic Dependency" if they does and " No
 * Cyclic Dependency" otherwise.
 * @param argc number of parameters - should be 1.
 * @param argv the path of the file to read.
 * @return 0 iff succedd.
 */
int main(int argc, char * argv[])
{
    if (argc != 2)
    {
        printf("Usage: CheckDependency <fileName>\n");
        return -1;
    }
    static File dependencies[FILE_NUM];
    int i;
    for (i = 0; i < FILE_NUM; ++i)
    {
        dependencies[i].index = 0;
    }
    char * fileName = argv[1];
    if (readFile(fileName, dependencies) == 0)
    {
        printf("couldn't read the file");
        return -1;
    }
    int result = dfs(dependencies);
    if (result == 1)
    {
        printf("Cyclic dependency\n");
    }
    else
    {
        printf("No Cyclic dependency\n");
    }
    return 0;
}

/**
 * @brief reads the given file line by line, and update array accordingly.
 * @param fileName the path of the file to read from.
 * @param array the array to update.
 * @return 0 if the file reading was successful, 1 otherwise.
 */
int readFile(char *fileName, File array[])
{
    FILE *fp;
    char buf[BUFF_SIZE];
    if ((fp = fopen(fileName, "r")) == NULL)
    {
        return 0;
    }
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        buf[strlen(buf) - 1] = '\0';
        updateLine(buf, array);
    }
    fclose(fp);
    return 1;
}

/**
 * @brief check if a given string is an existing name of a file in a given array.
 * @param name the string to check.
 * @param array the array to search in.
 * @return the index of the file whose name is name, and -1 if it wasn't found.
 */
int isInArray(char * name, File array[])
{
    assert(name != NULL);
    int i;
    for (i = 0; i < FILE_NUM; ++i)
    {
        if (strcmp(name, array[i].name) == 0)
        {
            return i;
        }
    }
    return -1;
}

/**
 * @brief given a string and array of files, updates the files in that line to be in array, and
 *        update thier neighbors.
 * @param line a string to update the array.
 * @param array array of files.
 */
void updateLine(char * line, File array[])
{
    assert(line != NULL);
    const char delim[4] = ":,\n";
    char str[LINE_LENGTH];
    strcpy(str, line);
    char * token;
    token = strtok(str, delim);
    int fileNum = 1;
    File * currentFile = NULL;
    while (token != NULL)
    {
        if (fileNum == 2)
        {
            memmove(token, token + 1, strlen(token));
        }
        if (isInArray(token, array) == -1)
        {
            addToArray(token, array);
        }
        if (fileNum == 1)
        {
            currentFile = &(array[isInArray(token, array)]);
        }
        else
        {
            updateNeighbors(currentFile, &array[isInArray(token, array)]);
        }
        fileNum++;
        token = strtok(NULL, delim);
    }
}

/**
 * @brief add a File with a name name to the first available place in array.
 * @param name the file name to add.
 * @param array the array to put the new file in.
 */
void addToArray(char * name, File array[])
{
    assert(name != NULL);
    int i = 0;
    while (array[i].index != 0)
    {
        // searching for the first available index
        i++;
    }
    File file;
    int j;
    for (j = 0; j < NEIGHBORS_NUM; ++j)
    {
        // initialize all neighbors to be -1
        file.neighbors[j] = -1;
    }
    file.index = i + 1;
    file.neighborsNum = 0;
    file.color = WHITE;
    strcpy(file.name, name);
    array[i] = file;
}

/**
 * @brief make neighbor to be a neighbor of file.
 * @param file the file to which we add a new neighbor.
 * @param neighbor the neighbor to add.
 */
void updateNeighbors(File * file, File * neighbor)
{
    // checking if neighbor is already a neighbor of file. if so do nothing.
    int i;
    for (i = 0; (i < NEIGHBORS_NUM) && (i != -1); ++i)
    {
        if (file->neighbors[i] == neighbor->index - 1)
        {
            return;
        }
    }
    // Not found, make them neighbors
    file->neighbors[file->neighborsNum] = neighbor->index - 1;
    file->neighborsNum++;
}

/**
 * @brief checks if there is a cyclic dependency.
 * @param array the array to check the cyclic dependency on.
 * @return 1 if there is a cyclic dependency, 0 otherwise.
 */
int dfs(File array[])
{
    int i;
    for (i = 0; i < FILE_NUM; ++i)
    {
        if (array[i].color != WHITE)
        {
            continue;
        }
        if(dfsHelper(&array[i], array) == CYCLE)
        {
            return CYCLE; // Has a cycle
        }
    }
    return NO_CYCLE; // Doesn't has a cycle
}

/**
 * @brief helper function to dfs.
 * @param file a file the has not been checked yet (white color).
 * @param array the array of file.
 * @return 1 if there is a cyclic dependency, 0 if cyclic dependency was not detect yet.
 */
int dfsHelper(File * file, File array[])
{
    file->color = GRAY;
    int result;
    int i;
    for (i = 0; i < file->neighborsNum; ++i)
    {
        int current = file->neighbors[i];
        File * currentFile = &array[current];
        if (currentFile->color == WHITE)
        {
            result = dfsHelper(currentFile, array);
            if (result == CYCLE)
            {
                return CYCLE;
            }
        }
        else if (currentFile->color == GRAY)
        {
            return CYCLE; // Has cycle
        }
        else
        {
            continue;
        }
    }
    file->color = BLACK;
    return NO_CYCLE;
}

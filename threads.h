#include <unistd.h>
#include <fuse.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mysql/mysql.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/time.h>


#define DIR_BUF_SIZE 256

#define SLEEP_LENGTH 1024

#define MD5_LEN 32+1

#define EXT_LEN 8

#define DATA_FOLDER "/wang8/Dandyl MK8/"


struct thread_data
{
	struct list_node *tags;
	struct list_node *files;
};

struct list_node
{
	char *ent;
	char *md5;
	char *ext;

	bool show;

	struct stat meta;

	struct list_node *next;
};

void *dandyl_query_thread(void *args);

bool init_list_node(struct list_node **node);

struct list_node *list_has(struct list_node *head, char *string);


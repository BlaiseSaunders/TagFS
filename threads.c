#include "threads.h"

bool init_list_node(struct list_node **node)
{
	if (((*node) = malloc(sizeof (struct list_node))) == NULL)
	{
		perror("Error");
		return false;
	}

	if (((*node)->ent = malloc(DIR_BUF_SIZE)) == NULL)
	{
		perror("Error");
		return false;
	}
	if (((*node)->md5 = malloc(MD5_LEN)) == NULL)
	{
		perror("Error");
		return false;
	}
	if (((*node)->ext = malloc(EXT_LEN)) == NULL)
	{
		perror("Error");
		return false;
	}

	(*node)->next = NULL;
	(*node)->show = false;


	return true;
}


struct list_node *list_has(struct list_node *head, char *string)
{
	struct list_node *cur = head;

	while (cur->next != NULL)
	{
		if (!strcmp(cur->ent, string))
			return cur;

		cur = cur->next;
	}

	return NULL;
}

void *dandyl_query_thread(void *params)
{


	struct thread_data *args = (struct thread_data *)params;

	char *buf;
	char *server = "localhost", *user = "dandyl", *password = "REDACTED",
	     *database = "dandyl";
	struct list_node *cur, *all_cur;
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;

	conn = mysql_init(NULL);

	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(EXIT_FAILURE);
	}


	fprintf(stdout, "SQL Thread: Succesfully started and connected\n");

	if (!init_list_node(&args->tags))
		return NULL;
	if (!init_list_node(&args->files))
		return NULL;

	while (true)
	{

		if (mysql_query(conn,
		   "select queries_matched, md5, ext from image_meta"
		   ))
		{
			fprintf(stderr, "%s\n", mysql_error(conn));
			break;
		}
		res = mysql_use_result(conn);

		all_cur = args->files;
		cur = args->tags;
		while ((row = mysql_fetch_row(res)) != NULL)
		{
			printf("ROW 0: %s\n\n", row[0]);
			printf("ROW 1: %s\n\n", row[1]);
			printf("ROW 2: %s\n\n", row[2]);
			if (row[0] == NULL)
				continue;
			printf("Inserting row\n\n");


			printf("Copying in %s.\n", row[0]);

			if (!init_list_node(&all_cur->next))
				return NULL;

			strncpy(all_cur->ent, row[0], DIR_BUF_SIZE);


			buf = strtok(row[0], " ");

			do
			{
				printf("Working on %s.\n", buf);
				if (!list_has(args->tags, buf))
				{
					printf("We don't have it, inserting now.\n");
					if (!init_list_node(&cur->next))
						return NULL;

					strcpy(cur->ent, buf);

					cur = cur->next;
				}
			} while ((buf = strtok(NULL, " ")) != NULL);


			all_cur = all_cur->next;

		}

		mysql_free_result(res);
		sleep(SLEEP_LENGTH);
	}


	mysql_close(conn);

	return NULL;
}

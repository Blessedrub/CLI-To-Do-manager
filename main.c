#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASK        (256U)
#define MAX_DATE        (20U)
#define MAX_CATEGORIES  (50U)

typedef struct
{
    uint8_t completed;
    uint8_t priority;
    char created_at[MAX_DATE];
    char category[MAX_CATEGORIES];
    char due_date[MAX_DATE];
    char text[MAX_TASK];
    uint32_t id;
} Task_t;

typedef struct
{
    const char *name;
    void (*handler)(int argc, char *argv[]);
} Command;

typedef enum
{
    SUCCESS = 0,
    ERROR
} CLI_TODO_STATUS;

Task_t *task_ptr = NULL;
uint16_t g_task_cnt = 0;

void add_task(const char *text)
{
    task_ptr = realloc(task_ptr, sizeof(Task_t) * (g_task_cnt + 1));

    task_ptr[g_task_cnt].id = g_task_cnt + 1;
    task_ptr[g_task_cnt].completed = 0;

    strncpy(task_ptr[g_task_cnt].text, text, MAX_TASK - 1);
    task_ptr[g_task_cnt].text[MAX_TASK - 1] = '\0';

    g_task_cnt++;
}

void list_tasks(void)
{
    if (g_task_cnt == 0)
    {
        printf("No tasks\n");
        return;
    }

    for (uint16_t i = 0; i < g_task_cnt; i++)
    {
        printf("%u. [%c] %s\n",
               task_ptr[i].id,
               task_ptr[i].completed ? 'x' : ' ',
               task_ptr[i].text);
    }
}

CLI_TODO_STATUS delete_task(uint32_t id)
{
    for (uint16_t i = 0; i < g_task_cnt; i++)
    {
        if (task_ptr[i].id == id)
        {
            for (uint16_t j = i; j < g_task_cnt - 1; j++)
            {
                task_ptr[j] = task_ptr[j + 1];
            }

            g_task_cnt--;

            task_ptr = realloc(task_ptr, sizeof(Task_t) * g_task_cnt);

            return SUCCESS;
        }
    }

    return ERROR;
}

void help(void)
{
    printf("Todo CLI\n");
    printf("Commands:\n");
    printf("  add \"text\"   Add task\n");
    printf("  list          List tasks\n");
    printf("  delete id     Delete task\n");
}

void cmd_add(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Task text required\n");
        return;
    }

    add_task(argv[2]);
    printf("Task added\n");
}

void cmd_list(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    list_tasks();
}

void cmd_delete(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Task id required\n");
        return;
    }

    uint32_t id = atoi(argv[2]);

    if (delete_task(id) == SUCCESS)
        printf("Task deleted\n");
    else
        printf("Task not found\n");
}

void cmd_help(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    help();
}

Command commands[] =
{
    {"add", cmd_add},
    {"list", cmd_list},
    {"delete", cmd_delete},
    {"help", cmd_help},
};

const int command_count = sizeof(commands) / sizeof(Command);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        help();
        return 0;
    }

    for (int i = 0; i < command_count; i++)
    {
        if (strcmp(argv[1], commands[i].name) == 0)
        {
            commands[i].handler(argc, argv);
            free(task_ptr);
            return 0;
        }
    }

    printf("Unknown command\n");
    help();

    free(task_ptr);

    return 0;
}

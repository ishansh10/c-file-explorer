#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node
{
    char name[50];
    int type;              // 0 = Folder, 1 = File
    struct Node *child;
    struct Node *next;
};

struct Stack
{
    struct Node *items[100];
    int top;
};

struct Recent
{
    char name[50];
    struct Recent *next;
};

struct Queue
{
    struct Recent *front;
    struct Recent *rear;
};

struct Node *root, *current;

// Create a new file or folder
struct Node* createNode(char name[], int type)
{
    struct Node *newNode;

    newNode = (struct Node*)malloc(sizeof(struct Node));

    strcpy(newNode->name, name);
    newNode->type = type;
    newNode->child = NULL;
    newNode->next = NULL;

    return newNode;
}

// Add file or folder to current directory
void addNode(char name[], int type)
{
    struct Node *newNode, *temp;

    newNode = createNode(name, type);

    if(current->child == NULL)
    {
        current->child = newNode;
    }
    else
    {
        temp = current->child;

        while(temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = newNode;
    }

    printf("Created successfully!\n");
}

// Display current directory contents
void display()
{
    struct Node *temp;

    temp = current->child;

    if(temp == NULL)
    {
        printf("Folder is empty.\n");
        return;
    }

    printf("\nContents:\n");

    while(temp != NULL)
    {
        if(temp->type == 0)
            printf("[Folder] %s\n", temp->name);
        else
            printf("[File]   %s\n", temp->name);

        temp = temp->next;
    }
}

// Add recently accessed file to queue
void addRecent(struct Queue *q, char name[])
{
    struct Recent *newNode;

    newNode = (struct Recent*)malloc(sizeof(struct Recent));

    strcpy(newNode->name, name);
    newNode->next = NULL;

    if(q->rear == NULL)
    {
        q->front = newNode;
        q->rear = newNode;
    }
    else
    {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

// Display recent files
void showRecent(struct Queue q)
{
    struct Recent *temp = q.front;

    if(temp == NULL)
    {
        printf("No recent files.\n");
        return;
    }

    printf("\nRecently accessed files:\n");

    while(temp != NULL)
    {
        printf("%s\n", temp->name);
        temp = temp->next;
    }
}

// Search for a file or folder
void search(struct Node *folder, char name[])
{
    struct Node *temp;

    temp = folder->child;

    while(temp != NULL)
    {
        if(strcmp(temp->name, name) == 0)
        {
            if(temp->type == 0)
                printf("Folder found: %s\n", temp->name);
            else
                printf("File found: %s\n", temp->name);
        }

        if(temp->type == 0)
            search(temp, name);

        temp = temp->next;
    }
}

// Delete a file or folder from current directory
void deleteNode(char name[])
{
    struct Node *temp, *prev;

    temp = current->child;
    prev = NULL;

    while(temp != NULL)
    {
        if(strcmp(temp->name, name) == 0)
        {
            if(prev == NULL)
                current->child = temp->next;
            else
                prev->next = temp->next;

            free(temp);
            printf("Deleted successfully!\n");
            return;
        }

        prev = temp;
        temp = temp->next;
    }

    printf("File or folder not found.\n");
}

// Main function
int main()
{
    int choice;
    char name[50];

    struct Stack s;
    struct Queue q;

    s.top = -1;
    q.front = NULL;
    q.rear = NULL;

    root = createNode("Root", 0);
    current = root;

    // Default folders
    addNode("Documents", 0);
    addNode("Pictures", 0);
    addNode("Projects", 0);

    while(1)
    {
        printf("\n========== FILE EXPLORER ==========\n");
        printf("Current Folder: %s\n", current->name);

        printf("\n1. View Contents");
        printf("\n2. Create File");
        printf("\n3. Create Folder");
        printf("\n4. Open Folder");
        printf("\n5. Go Back");
        printf("\n6. Delete File/Folder");
        printf("\n7. Search");
        printf("\n8. Open File");
        printf("\n9. Recent Files");
        printf("\n10. Exit");

        printf("\n\nEnter choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                display();
                break;

            case 2:
                printf("Enter file name: ");
                scanf("%49s", name);
                addNode(name, 1);
                break;

            case 3:
                printf("Enter folder name: ");
                scanf("%49s", name);
                addNode(name, 0);
                break;

            case 4:
            {
                struct Node *temp;

                printf("Enter folder name: ");
                scanf("%49s", name);

                temp = current->child;

                while(temp != NULL)
                {
                    if(strcmp(temp->name, name) == 0 &&
                       temp->type == 0)
                    {
                        if(s.top < 99)
                        {
                            s.items[++s.top] = current;
                            current = temp;
                            printf("Folder opened.\n");
                        }
                        else
                        {
                            printf("Navigation limit reached.\n");
                        }
                        break;
                    }

                    temp = temp->next;
                }

                if(temp == NULL)
                    printf("Folder not found.\n");

                break;
            }

            case 5:
                if(s.top == -1)
                {
                    printf("Already in Root folder.\n");
                }
                else
                {
                    current = s.items[s.top--];
                    printf("Moved back.\n");
                }
                break;

            case 6:
                printf("Enter name to delete: ");
                scanf("%49s", name);
                deleteNode(name);
                break;

            case 7:
                printf("Enter name to search: ");
                scanf("%49s", name);
                search(root, name);
                break;

            case 8:
            {
                struct Node *temp;
                int found = 0;

                printf("Enter file name: ");
                scanf("%49s", name);

                temp = current->child;

                while(temp != NULL)
                {
                    if(strcmp(temp->name, name) == 0 &&
                       temp->type == 1)
                    {
                        printf("Opening file: %s\n", temp->name);
                        addRecent(&q, temp->name);
                        found = 1;
                        break;
                    }

                    temp = temp->next;
                }

                if(found == 0)
                    printf("File not found.\n");

                break;
            }

            case 9:
                showRecent(q);
                break;

            case 10:
                printf("Exiting File Explorer...\n");
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}

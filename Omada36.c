/*

Alexandros Maragkakis
202220 1900 120
dit19120@uop.gr

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// structure for list
struct Node
{
    char data;
    struct Node *next;
    struct Node *prev;
} typedef Node;

// pointer for list
typedef Node *NodePointer;

// functions for list
void insert(NodePointer *pointer, char value);
void printList(NodePointer current);
void printListReverse(NodePointer current);
void menu(void);
void printResult(NodePointer result);
int number_entry(char **string, int *size, int turn);

int findLargerNumber(NodePointer head1, NodePointer head2);
void calculateSigns(NodePointer num1, NodePointer num2, char *sign1, char *sign2, int *result_sign, int operation, int larger);

// functions for operations
NodePointer add(NodePointer head1, NodePointer head2, int result_sign);
NodePointer subtract(NodePointer head1, NodePointer head2, int result_sign);

int main(void)
{
    /*
    The numbers are being read, and inserted with their digits reversed into doubly linked lists.
    See more information on how the functions work in their implementations

    */

    NodePointer head1 = NULL;
    NodePointer head2 = NULL;

    char *num1;
    char *num2;
    int size = 1;
    unsigned int choice;
    menu();
    scanf("%u", &choice);
    int result_sign = 0;
    char sign1 = 0, sign2 = 0;
    int operation = choice;
    int larger = 0;
    int i;
    switch (choice)
    {
    case 1:

        printf("\nYou have selected addition.");

        if (number_entry(&num1, &size, 0) != 0)
        {
            return 1;
        }

        // insert number reversed, case for '\n' to make it work correctly
        for (i = strlen(num1) - 1; i >= 0; i--)
        {
            if (num1[i] == '\n')
            {
                continue;
            }
            insert(&head1, num1[i]);
        }

        free(num1);

        size = 1;
        if (number_entry(&num2, &size, 1) != 0)
        {
            return 1;
        }
        for (i = strlen(num2) - 1; i >= 0; i--)
        {
            insert(&head2, num2[i]);
        }

        free(num2);

        larger = findLargerNumber(head1, head2);

        // calculate the sign of the result
        calculateSigns(head1, head2, &sign1, &sign2, &result_sign, operation, larger);

        // if signs are different then call subtract()
        if (sign1 != sign2)
        {
            puts("\nSigns are different\n");
            if (larger == 0)
            {
                puts("\nFirst number is larger\n");
                NodePointer diff = subtract(head1, head2, result_sign);
                printResult(diff);
            }
            else if (larger == 1)
            {
                puts("\nSecond number is larger\n");
                NodePointer diff = subtract(head2, head1, result_sign);
                printResult(diff);
            }
            // else case is for 10 + (-10) = 0, -10 + 10 = 0, etc...
            else
            {
                puts("Result: 0");
            }

            break;
        }

        // call add()
        NodePointer sum = add(head1, head2, result_sign);
        printResult(sum);

        break;
    case 2:
        printf("\nYou have selected subtraction.\n");
        if (number_entry(&num1, &size, 0) != 0)
        {
            return 1;
        }

        for (i = strlen(num1) - 1; i >= 0; i--)
        {
            if (num1[i] == '\n')
            {
                continue;
            }
            insert(&head1, num1[i]);
        }

        free(num1);

        size = 1;
        if (number_entry(&num2, &size, 1) != 0)
        {
            return 1;
        }
        for (i = strlen(num2) - 1; i >= 0; i--)
        {
            if (num2[i] == '\n')
            {
                continue;
            }
            insert(&head2, num2[i]);
        }

        free(num2);

        larger = findLargerNumber(head1, head2);
        calculateSigns(head1, head2, &sign1, &sign2, &result_sign, operation, larger);

        if (sign1 == sign2)
        {
            if (larger == 0)
            {
                NodePointer diff = subtract(head1, head2, result_sign);

                printResult(diff);
            }
            else if (larger == 1)
            {
                NodePointer diff = subtract(head2, head1, result_sign);
                printResult(diff);
            }
            else
            {
                puts("Result: 0");
            }
        }
        else
        {
            NodePointer sum = add(head1, head2, result_sign);
            printResult(sum);
        }

        break;
    case 3:
        printf("Terminating program.\n");
        break;

    default:
        exit(0);
    }

    return 0;
}

// used in debugging
void printList(NodePointer current)
{
    if (current == NULL)
    {
        printf("List is empty");
    }
    else
    {
        printf("List: ");

        while (current != NULL)
        {
            printf("%c --> ", current->data);
            current = current->next;
        } // end while
        puts("NULL\n");
    }
}

// I think I copied this from the slides
void insert(NodePointer *head, char value)
{
    NodePointer newPointer;
    NodePointer currentPointer;

    newPointer = malloc(sizeof(Node));

    if (newPointer != NULL)
    {
        newPointer->data = value;

        if (*head == NULL)
        {
            *head = newPointer;
            newPointer->prev = NULL;

            newPointer->next = NULL;
        }
        else
        {
            currentPointer = *head;
            while (currentPointer->next != NULL)
            {
                currentPointer = currentPointer->next;
            }
            currentPointer->next = newPointer;
            newPointer->prev = currentPointer;
            newPointer->next = NULL;
        }
    }
    else
    {
        printf("Out of memory\n");
    }
}

void menu(void)
/* Simple menu view function */
{
    printf("-----------------Welcome----------------\n");
    printf("[1]: Add\n[2]: Subtract\n[3]: Exit\nEnter your choice: ");
}

void printListReverse(NodePointer current)
{
    if (current == NULL)
    {
        printf("List is empty");
    }
    else
    {
        printf("List in Reverse Order: ");

        // Move to the last node
        while (current->next != NULL)
        {
            current = current->next;
        }

        // Traverse the list in reverse using prev pointers
        while (current != NULL)
        {
            printf("%c --> ", current->data);
            current = current->prev;
        }
        puts("NULL\n");
    }
}

NodePointer add(NodePointer head1, NodePointer head2, int result_sign)
{
    /*
    The two lists are passed, along with the sign of the result.

    The function iterates through the linked lists, adding corresponding
    digits and any carry from the previous addition. The result is stored
    in a new linked list sum. If there is a remaining carry, it is added to the sum.
    Finally, if result_sign is -1, a negative sign is inserted at the beginning of the sum.
    The function returns the sum.
    */

    NodePointer current1 = head1;
    NodePointer current2 = head2;
    NodePointer sum = NULL;

    int carry = 0;
    int tmp_sum = 0;
    int digit = 0;

    while (current1 != NULL || current2 != NULL)
    {
        int x = (current1 != NULL) ? atoi(&current1->data) : 0;
        int y = (current2 != NULL) ? atoi(&current2->data) : 0;

        tmp_sum = carry + x + y;
        carry = tmp_sum / 10;
        digit = tmp_sum % 10;

        insert(&sum, digit + '0');

        current1 = (current1 != NULL) ? current1->next : NULL;
        current2 = (current2 != NULL) ? current2->next : NULL;
    }
    if (carry > 0)
    {
        insert(&sum, carry + '0');
    }

    if (result_sign == -1)
    {
        insert(&sum, '-');
    }

    return sum;
}

int findLargerNumber(NodePointer head1, NodePointer head2)
{
    /*
    This function takes in two linked lists head1 and head2 as parameters.
    It compares the numbers represented by the linked lists and returns
    0 if the number represented by head1 is larger,
    1 if the number represented by head2 is larger, and
    -1 if the numbers are equal in length.
    The function iterates through the linked lists to determine the lengths and
    then compares the digits of the numbers starting from the most significant digit.
    */
    NodePointer current1 = head1;
    NodePointer current2 = head2;
    int larger = -1;
    int length1 = 0;
    int length2 = 0;

    // Move to the last node
    while (current1->next != NULL && current1->next->data != '-')
    {
        current1 = current1->next;
        length1++;
    }

    while (current2->next != NULL && current2->next->data != '-')
    {
        current2 = current2->next;
        length2++;
    }

    // Compare the lengths
    if (length1 > length2)
    {
        larger = 0;
    }
    else if (length1 < length2)
    {
        larger = 1;
    }
    else
    {
        // Compare the digits
        while (current1 != NULL && current2 != NULL)
        {
            if (current1->data > current2->data)
            {
                larger = 0;
                break;
            }
            else if (current1->data < current2->data)
            {
                larger = 1;
                break;
            }
            current1 = current1->prev;
            current2 = current2->prev;
        }
    }

    return larger;
}

NodePointer subtract(NodePointer head1, NodePointer head2, int result_sign)
{
    /*
    This function takes in two linked lists (head1 and head2) and an integer result_sign.
    The function subtracts the values stored in the nodes of the two linked lists and
    returns a new linked list diff that represents the difference between the two lists.

    The function iterates through the linked lists, subtracting the corresponding node values and
    taking into account any borrow from the previous subtraction.
    The result is stored in a new linked list diff.
    If result_sign is -1, indicating a negative result, a negative sign is inserted at the beginning of diff.
    */
    NodePointer current1 = head1;
    NodePointer current2 = head2;
    NodePointer diff = NULL;
    int borrow = 0;
    int tmp_diff = 0;
    int digit = 0;
    int i = 0;
    while (current1 != NULL || current2 != NULL)
    {
        int x = (current1 != NULL) ? atoi(&current1->data) : 0;
        int y = (current2 != NULL) ? atoi(&current2->data) : 0;
        if (x - borrow < y)
        {
            tmp_diff = x - borrow + 10 - y;
            borrow = 1;
        }
        else
        {
            tmp_diff = x - borrow - y;
            borrow = 0;
        }
        digit = tmp_diff % 10;
        insert(&diff, digit + '0');
        current1 = (current1 != NULL) ? current1->next : NULL;
        current2 = (current2 != NULL) ? current2->next : NULL;
        i++;
    }

    if (result_sign == -1)
    {
        insert(&diff, '-');
    }

    return diff;
}

void printResult(NodePointer result)
{
    /*
    This function is used to print the result.
    Because the number is reversed in the list,
    the function traverses to the end before printing the characters
    by traversing backwards.
    */
    if (result == NULL)
    {
        printf("List is empty.\n");
    }
    else
    {
        printf("Result: ");

        // Move to the last node
        while (result->next != NULL)
        {
            result = result->next;
        }

        // If the first digit is a minus sign, print it
        if (result->data == '-')
        {
            printf("%c", result->data);
            result = result->prev;
        }

        while (result != NULL && result->data == '0' && result->prev != NULL)
        {
            result = result->prev;
        }

        // Traverse the list in reverse using prev pointers
        while (result != NULL)
        {
            printf("%c", result->data);
            result = result->prev;
        }
        puts("\n");
    }
}

void calculateSigns(NodePointer num1, NodePointer num2, char *sign1, char *sign2, int *result_sign, int operation, int larger)
{
    /*
    This code snippet defines a function calculateSigns that takes in
    two linked list nodes (num1 and num2),
    two character pointers (sign1 and sign2),
    an integer pointer (result_sign), and
    two integers (operation and larger).

    The function calculates the signs of the numbers represented by the linked lists and
    determines the sign of the result based on the given operation.

    The function iterates through the linked lists to reach the last node.
    It then checks if the last node of num1 or num2 contains a "-" character and updates the corresponding sign pointer accordingly.
*/

    while (num1->next != NULL)
    {
        num1 = num1->next;
    }

    while (num2->next != NULL)
    {
        num2 = num2->next;
    }

    if (num1->data == '-')
    {
        *sign1 = '-';
    }
    if (num2->data == '-')
    {
        *sign2 = '-';
    }

    if (operation == 1) // addition
    {

        // calculate sign of result
        if (*sign1 == '-' && *sign2 == '-')
        { // both negative results in negative
            *result_sign = -1;
        }
        else if (*sign1 == 0 && *sign2 == 0)
        { // both positive results in positive
            *result_sign = 1;
        }
        else // this is actually the case subtraction
        {    // the result sign is the sign of the larger number

            if (larger == 0)
            {
                if (*sign1 == '-')
                {
                    *result_sign = -1;
                }
                else
                {
                    *result_sign = 1;
                }
            }
            else
            {

                if (*sign2 == '-')
                {
                    *result_sign = -1;
                }
                else
                {
                    *result_sign = 1;
                }
            }
        }
    }
    else if (operation == 2)
    {
        if (*sign1 == '-' && *sign2 == '-')
        {
            if (larger == 0)
            {
                *result_sign = -1;
            }
            else
            {
                *result_sign = 1;
            }
        }
        else if (*sign1 != '-' && *sign2 != '-')
        {
            if (larger == 0)
            {
                *result_sign = 1;
            }
            else
            {
                *result_sign = -1;
            }
        }
        else if (*sign1 != '-' && *sign2 == '-')
        {
            *result_sign = 1;
        }
        else if (*sign1 == '-' && *sign2 != '-')
        {
            *result_sign = -1;
        }
    }
}

int number_entry(char **string, int *size, int turn)
{

    /*
    This function takes in three parameters:

    a double pointer to a character array (string),
    a pointer to an integer (size), and
    an integer (turn).

    Inside the function, it prompts the user to enter a number and reads a character from the input.
    It then dynamically allocates memory for the string array using malloc, with a size of *size + 1.
    If the memory allocation fails, it prints an error message and returns 1.
    If the size parameter is greater than 0, it enters a loop that iteratively adds characters to the string array.
    It assigns the current character to (*string)[*size - 1],
    increments the size pointer,
    and reallocates the string array to accommodate more characters using realloc.

    If the memory reallocation fails, it prints an error message and returns 1.

    After the loop, it null-terminates the string by assigning '\0' to (*string)[*size - 1].

    Finally, it returns 0 to indicate successful execution of the function.
*/

    printf("\nEnter number %d: ", turn + 1);
    char c = getchar();
    *string = (char *)malloc(*size + 1); // Allocate dynamically memory for our string.
    if (*string == NULL)
    {
        printf("Memory allocation failed - not enough memory.\n");
        return 1;
    }

    if (*size > 0)
    {
        do
        {
            (*string)[*size - 1] = c; // Putting character into the string "array"
            (*size)++;                // Increasing the size of "array"

            *string = (char *)realloc(*string, *size + 1); // Reallocate array to put more characters
            if (*string == NULL)
            {
                printf("Memory allocation failed - not enough memory.\n");
                return 1;
            }

            (*string)[*size - 1] = '\0'; // Last character should be '\0'
        } while ((c = getchar()) != EOF && c != '\n');
        (*string)[*size - 1] = '\0'; // Null-terminate the string after the loop
    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    char *method = getenv("REQUEST_METHOD");
    char *query = getenv("QUERY_STRING");
    char *content_length_str = getenv("CONTENT_LENGTH");
    char *content_type = getenv("CONTENT_TYPE");

    int content_length = 0;

    if (content_length_str)
        content_length = atoi(content_length_str);

    /* --- CGI RESPONSE HEADERS --- */
    printf("Content-Type: text/plain\r\n");
    printf("Status: 200 OK\r\n");
    printf("\r\n");  // end of headers

    printf("METHOD: %s\n", method ? method : "");
    printf("QUERY: %s\n", query ? query : "");
    printf("CONTENT_LENGTH: %d\n", content_length);
    printf("CONTENT_TYPE: %s\n", content_type ? content_type : "");

    printf("---- BODY ----\n");

    // /* --- READ BODY FROM STDIN --- */
    // if (content_length > 0)
    // {
    //     char *body = (char *)malloc(content_length + 1);
    //     if (!body)
    //     {
    //         printf("Memory allocation failed\n");
    //         return 1;
    //     }

    //     int total = 0;
    //     while (total < content_length)
    //     {
    //         int n = read(STDIN_FILENO, body + total, content_length - total);
    //         if (n <= 0)
    //             break;
    //         total += n;
    //     }

    //     body[total] = '\0';
    //     printf("%s\n", body);

    //     free(body);
    // }
    // else
    // {
    //     printf("(no body)\n");
    // }

    printf("----------------\n");
    printf("<html><body><h1>Hello from C CGI!</h1></body></html>\n");

    return 0;
}
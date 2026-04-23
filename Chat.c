#include <fcgi_stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 1024

void print_chat() {
    FILE *file = fopen("chat.txt", "r");
    if (!file) return;

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        printf("<p>%s</p>", line);
    }

    fclose(file);
}

void save_message(const char *msg) {
    FILE *file = fopen("chat.txt", "a");
    if (!file) return;

    fprintf(file, "%s\n", msg);
    fclose(file);
}

int main() {
    while (FCGI_Accept() >= 0) {
        printf("Content-type: text/html\r\n\r\n");

        char *method = getenv("REQUEST_METHOD");

        if (method && strcmp(method, "POST") == 0) {
            int len = atoi(getenv("CONTENT_LENGTH"));
            if (len > 0 && len < MAX_INPUT) {
                char input[MAX_INPUT];
                fread(input, 1, len, stdin);
                input[len] = '\0';

                char *msg = strstr(input, "message=");
                if (msg) {
                    msg += 8;
                    save_message(msg);
                }
            }
        }

        printf("<html><body>");
        printf("<h1>Mini Chat</h1>");

        print_chat();

        printf("<form method='POST'>");
        printf("<input name='message' />");
        printf("<button type='submit'>Send</button>");
        printf("</form>");

        printf("</body></html>");
    }

    return 0;
}

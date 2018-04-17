#include <stdio.h>
#include <malloc.h>

short int is_alpha(char c) {
    return (c>='a' && c<='z') || (c>='A' && c<='Z');
}

/*short int word_finished = 0;*/
short int was_hyphen = 0;
short int here_word = 0;
char* buffer = NULL;
unsigned int count = 0;
unsigned int offset = 0;

char our_getchar() { /*nasz buforujący getchar, robiący prawie wszystko*/
    char c = (char)getchar();
    if(is_alpha(c)) { alpha:
        realloc(buffer, sizeof(char)*(++count));
        buffer[count-1] = ' ';
    }
    switch (c) {
        case ' ':
        space:
            if(count==0) {
                return ' '; /*jeżeli zaczynamy od spacji to od razu ona przechodzi*/
            } else if (count==offset) {
                count = 0;
                offset = 0;
                free(buffer);
                buffer = NULL;
                return ' '; /*również zaczęliśmy od spacji ale mieliśmy stary bufor do zwolnienia*/
            } else { /* ale jeżeli coś mamy to ją buforujemy bo nie wiadomo czy nie będzie hyphena
                      * bo może być hyphen poprzedzony spacją, patrząc na treść zadania */
                /*word_finished = 1;*/
                realloc(buffer, sizeof(char)*(++count));
                buffer[count-1] = ' ';
                /*return buffer[offset++];*/ /*a tego nie robimy po prostu właśnie dla hyphena*/
                while(1) { /*mamy nasze słowo odspacjowane, więc teraz musimy zseekować co dalej*/
                    c = (char)getchar();
                    if(is_alpha(c)) goto alpha; /* spacja zbuforowana, zacznie buforować kolejne
                                                 * słowo, które będzie potem trzeba przesunąć
                                                 * na początek chyba że uda się to załatwić offsetem */
                    switch (c) {
                        case ' ':
                            goto space; /*zbuforuje kolejną spację i wróci do tego while'a*/
                        case '-':
                            goto hyphen; /*spacja zbuforowana, pojedzie do hyphena jak po słowie*/
                        case '\n':
                            goto newline; /*spacja zbuforowana, pojedzie do newline'a jak po słowie*/
                    }
                }
            }
        case '-':
        hyphen:
            was_hyphen = 1;
            c = (char)getchar();
            switch (c) {
                case ' ':
                    goto space;
                case '-':
                    goto hyphen;
                case '\n':
                    while(1) {
                        c = (char)getchar();
                        switch (c) {
                            case '-': continue;
                            case ' ':
                                if(!here_word) continue;
                                else {
                                    realloc(buffer, sizeof(char)*(++count));
                                    buffer[count-1] = '\n'
                                }
                        }
                        if(is_alpha(c)) {
                            here_word = 1;
                            realloc(buffer, sizeof(char)*(++count));
                            buffer[count-1] = c;
                        }
                    }
            }
        case '\n':
        newline: {}
    }
}

int main() {
    printf("Hello, World!\n");
    return 0;
}
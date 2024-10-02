#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define FILE_NAME "primes.txt"

int isPrime(int p){
    int ret = 1;

    if ((p == 0) || (p == 1))
        ret = 0;
    for (int i = 2; (ret && ( i <= p / 2)) ; i++)
        if (! (p % i))
            ret = 0;
    return ret;
}

//initiallize node
typedef struct node {
    int data;
    struct node *next;
} node;

//Globals
node *head = NULL;
int counter; //global counter for number of nodes
int runPThreads = 1; //run threads lets the threads talk to each other
int num; //total to find primes up to

//insert a new node at head
void insertAtHead(int data) {
    node *newNode = (node *)malloc(sizeof(node));
    newNode -> data = data;
    newNode -> next = head;
    head = newNode;
}

//prints Primes to Primes.txt
void displayPrime(FILE * primes) {
    node *current = head;
    while (current != NULL) {
        fprintf(primes, "%d\n", current->data);
        current = current -> next;
    }
}

//display the top N of primes
void displayTopPrimes(int amount) {
    //stops from asking for data that doesnt exist
    if (amount > 5){
        amount = 5;
    }
    int count = 0;
    node *current = head;
    printf("\nTop %d primes {\n", amount);
    while (current != NULL && count < amount){
        printf("%d\n", current -> data);
        current = current -> next;
        count++;
    }
}

//display Head
void displayCurrentNode(){
    node *current = head;
    printf("Current Node:\n %d\n", current -> data);
}

//function for H input
void displayCommands() {
    printf("h/H: Help\nc/C: Count\nl/L: Largest\nt/T: Top 5\nq/Q: Quit\n");
}

//function for C input
void displayNodeCount(){
    printf("Numbers Processed:\n %d\n", counter);
}

//Sorts through the options users have
void *userInput(void *arg){

    printf("Command (h for help): ");
    FILE *primes;
    primes = fopen("primes.txt", "w");
    while(runPThreads == 1){
        
        //get input
        char input = getchar();

        //handle user input
        switch (input) {
            case 'T': 
            case 't':
                displayTopPrimes(5);
                printf("Command (h for help): ");
                break;
            case 'L':
            case 'l':
                displayCurrentNode();
                printf("Command (h for help): ");
                break;
            case 'H':
            case 'h':
                displayCommands();
                printf("Command (h for help): ");
                break;
            case 'C':
            case 'c':
                displayNodeCount();
                printf("Command (h for help): ");
                break;
            case 'Q':
            case 'q':
                printf("Manually Ending Program\n");
                //shut off createPrimeList()
                runPThreads = 0;
                break;
            default:
                //for when just enter is pressed
                break;
        }
    }
    displayPrime(primes);
    fclose(primes);
    //exit thread
    pthread_exit(NULL);
}

void *createPrimeList(void *args){
    //run threads lets the threads talk to each other
    runPThreads = 1;
    for(int i = 0; i <= num; i++) {

        if (runPThreads == 0){
            break;
        }
        if(isPrime(i) == 1) {
            insertAtHead(i);
            counter++;
        }  
        if (i == num){
            printf("\nAll Primes found. Ending Program\n");
        }

        sleep(1);
    }
    runPThreads = 0; 
    //exit thread
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){

    //top number you want to find primes to
    num = atoi(argv[1]);
    printf("%d is your max\n", num);

    //handling pthreads
    pthread_t idPrimer, idUser;
    pthread_create(&idPrimer, NULL, createPrimeList, NULL);
    pthread_create(&idUser, NULL, userInput, NULL);
    pthread_join(idPrimer, NULL);
    pthread_join(idUser, NULL);

    printf("Primes printed on Primes.txt");
    return 0;
}
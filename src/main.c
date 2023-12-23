//Ruben Wihler
//12-23-2023
//Ce programme permet de résourdre le problème du Grid Traveler avec la programmation dynamique
//J'ai vu la vidéo de freeCodeCamp sur le sujet et j'ai décidé de le refaire en C pour m'entrainer
//https://youtu.be/oBt53YbR9Kk?si=cDwN7mN6FvvGKhqA&t=2322
//
//Source utilisée pour la réalisation de ce programme:
// - Tableau de taille dynamique (https://stackoverflow.com/questions/3536153/c-dynamically-growing-array)
// - Fonction de mesure du temps d'exécution (https://stackoverflow.com/questions/5248915/execution-time-of-c-program#:~:text=To%20get%20the%20CPU%20time,as%20a%20floating%20point%20type)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Definition des différentes méthodes de résolution du problème
#define RECURSIVE 0
#define RECURSIVE_MEMO 1

#pragma region Recursive method

int gridTravelerRecursive(unsigned long m, unsigned long n){
    if (m == 0 || n == 0) return 0;
    if (m == 1 && n == 1) return 1;

    return gridTravelerRecursive(m - 1, n) + gridTravelerRecursive(m, n - 1);
}

#pragma endregion

#pragma region Recursive method with memoization

struct memo{
    unsigned long m;
    unsigned long n;
    unsigned long result;
};

struct memoCollection{
    size_t size;
    size_t used;
    struct memo* memos;
};

unsigned long gridTravelerRecursiveMemo(unsigned long m, unsigned long n, struct memoCollection *memo){
    //On vérifie si le résultat est déjà enregistré dans la mémoire
    for (int i = 0; i < memo->size; i++){
        if (memo->memos[i].m == m && memo->memos[i].n == n) return memo->memos[i].result;
    }

    //Si le résultat n'est pas enregistré, on le calcule
    //On doit d'abord vérifier la condition d'arrêt de la récursion
    if (m == 0 || n == 0) return 0;
    if (m == 1 && n == 1) return 1;

    unsigned long result = gridTravelerRecursiveMemo(m - 1, n, memo) + gridTravelerRecursiveMemo(m, n - 1, memo);

    //On vérifie si la mémoire est assez grande pour enregistrer le résultat
    if (memo->used == memo->size){
        //Si elle n'est pas assez grande, on l'agrandit
        memo->size *= 2;
        memo->memos = realloc(memo->memos, memo->size * sizeof(struct memo));
    }
    
    //On enregistre le résultat dans la mémoire
    memo->memos[memo->used].m = m;
    memo->memos[memo->used].n = n;
    memo->memos[memo->used].result = result;
    memo->used++;

    return result;
}

#pragma endregion

//Cette fonction permet de tester si le résultat calculé est le même que celui attendu
//Elle prend en paramètre les valeurs de m et n, le résultat attendu et la méthode utilisée
void test(unsigned long m, unsigned long n, unsigned long expected, int method){
    unsigned long result;
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    //On appelle la fonction correspondant à la méthode choisie
    switch (method){
        case RECURSIVE:
            result = gridTravelerRecursive(m, n);
            break;
        
        case RECURSIVE_MEMO:
            struct memoCollection memoCol;
            memoCol.size = 16;
            memoCol.used = 0;
            memoCol.memos = malloc(memoCol.size * sizeof(struct memo));
            result = gridTravelerRecursiveMemo(m, n, &memoCol);
            free(memoCol.memos);
            break;

        default:
            result = 0;
            break;
    }

    //On arrête le timer et on calcule le temps d'exécution
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    //comparaison des résultats et affichage
    if(result == expected){
        printf("[OK] Test (%ld, %ld) with %s method, passed with result of %ld time: %fs\n", m, n, method == 0 ? "recursive" : "recursive memo", result, cpu_time_used);
    }else{
        printf("[KO] Test (%ld, %ld) with %s method, failed | result: %ld expected: %ld time: %fs\n", m, n, method == 0 ? "recursive" : "recursive memo", result, expected, cpu_time_used);
    }
}

int main(){
    //Méthode récursive
    test(1, 1, 1, RECURSIVE);
    test(2, 3, 3, RECURSIVE);
    test(3, 2, 3, RECURSIVE);
    test(3, 3, 6, RECURSIVE);

    //Méthode récursive avec mémoization
    test(1, 1, 1, RECURSIVE_MEMO);
    test(2, 3, 3, RECURSIVE_MEMO);
    test(3, 2, 3, RECURSIVE_MEMO);
    test(3, 3, 6, RECURSIVE_MEMO);
    test(18, 18, 2333606220, RECURSIVE_MEMO);

    return 0;
}
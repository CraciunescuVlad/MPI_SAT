#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LITERALI 10
#define MAX_CLAUZE 100

typedef struct {
    int literali[MAX_LITERALI];
    int nr_literali;
} Clauza;

typedef struct {
    Clauza clauze[MAX_CLAUZE];
    int nr_clauze;
} Formula;

// ================== UTILITARE ==================

bool exista_clauza_unitara(Formula *f, int *literal) {
    for (int i = 0; i < f->nr_clauze; i++) {
        if (f->clauze[i].nr_literali == 1) {
            *literal = f->clauze[i].literali[0];
            return true;
        }
    }
    return false;
}

bool are_clauza_vid(Formula *f) {
    for (int i = 0; i < f->nr_clauze; i++) {
        if (f->clauze[i].nr_literali == 0)
            return true;
    }
    return false;
}

Formula copiaza_formula(Formula *f) {
    Formula copie = *f;
    return copie;
}

void elimina_clauze_cu_literal(Formula *f, int l) {
    int k = 0;
    for (int i = 0; i < f->nr_clauze; i++) {
        bool gasit = false;
        for (int j = 0; j < f->clauze[i].nr_literali; j++) {
            if (f->clauze[i].literali[j] == l) {
                gasit = true;
                break;
            }
        }
        if (!gasit) {
            f->clauze[k++] = f->clauze[i];
        }
    }
    f->nr_clauze = k;
}

void elimina_literal(Formula *f, int l) {
    for (int i = 0; i < f->nr_clauze; i++) {
        int k = 0;
        for (int j = 0; j < f->clauze[i].nr_literali; j++) {
            if (f->clauze[i].literali[j] != l) {
                f->clauze[i].literali[k++] = f->clauze[i].literali[j];
            }
        }
        f->clauze[i].nr_literali = k;
    }
}

int gaseste_literal_pur(Formula *f) {
    int frecvPoz[101] = {0};
    int frecvNeg[101] = {0};

    for (int i = 0; i < f->nr_clauze; i++) {
        for (int j = 0; j < f->clauze[i].nr_literali; j++) {
            int l = f->clauze[i].literali[j];
            if (l > 0) frecvPoz[l] = 1;
            else frecvNeg[-l] = 1;
        }
    }

    for (int i = 1; i <= 100; i++) {
        if (frecvPoz[i] && !frecvNeg[i]) return i;
        if (!frecvPoz[i] && frecvNeg[i]) return -i;
    }
    return 0;
}

// ================== ALGORITM DPLL ==================

bool dpll(Formula *f) {
    // Condiții de oprire
    if (f->nr_clauze == 0) return true;
    if (are_clauza_vid(f)) return false;

    // PASUL 1: Propagare literal unitar
    int literal;
    if (exista_clauza_unitara(f, &literal)) {
        Formula copie = copiaza_formula(f);
        elimina_clauze_cu_literal(&copie, literal);
        elimina_literal(&copie, -literal);
        return dpll(&copie);
    }

    // PASUL 2: Eliminare literal pur
    int pur = gaseste_literal_pur(f);
    if (pur != 0) {
        Formula copie = copiaza_formula(f);
        elimina_clauze_cu_literal(&copie, pur);
        return dpll(&copie);
    }

    // PASUL 3: SPLIT (alegere literal și recursie)
    literal = f->clauze[0].literali[0]; // alegem un literal oarecare

    // Încercăm cu literal adevărat
    Formula copie_adevarat = copiaza_formula(f);
    elimina_clauze_cu_literal(&copie_adevarat, literal);
    elimina_literal(&copie_adevarat, -literal);
    if (dpll(&copie_adevarat)) return true;

    // Încercăm cu literal fals
    Formula copie_fals = copiaza_formula(f);
    elimina_clauze_cu_literal(&copie_fals, -literal);
    elimina_literal(&copie_fals, literal);
    return dpll(&copie_fals);
}

int main() {
    clock_t start_time = clock();
    Formula f;
    f.nr_clauze = 90;

f.clauze[0] = (Clauza){{1, -2, 3}, 3};
f.clauze[1] = (Clauza){{-1, 4}, 2};
f.clauze[2] = (Clauza){{2, -3, -5, 6}, 4};
f.clauze[3] = (Clauza){{1, -4, 5, -6, 7}, 5};
f.clauze[4] = (Clauza){{-2, 3, -8}, 3};
f.clauze[5] = (Clauza){{-1}, 1};
f.clauze[6] = (Clauza){{9, 10}, 2};
f.clauze[7] = (Clauza){{-5, -6, -7, -8, -9, -10}, 6};
f.clauze[8] = (Clauza){{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 10};
f.clauze[9] = (Clauza){{-3, 4, -5}, 3};
f.clauze[10] = (Clauza){{1, -2, 3, -4, 5}, 5};
f.clauze[11] = (Clauza){{6, 7, -8}, 3};
f.clauze[12] = (Clauza){{-9, 10, 1}, 3};
f.clauze[13] = (Clauza){{-10}, 1};
f.clauze[14] = (Clauza){{3, -4, -5, 6}, 4};
f.clauze[15] = (Clauza){{-1, 2, -3, 4, -5}, 5};
f.clauze[16] = (Clauza){{-6, -7}, 2};
f.clauze[17] = (Clauza){{8, -9, 10}, 3};
f.clauze[18] = (Clauza){{1, -2}, 2};
f.clauze[19] = (Clauza){{3, -4, 5, -6, 7}, 5};
f.clauze[20] = (Clauza){{-8, 9, -10}, 3};
f.clauze[21] = (Clauza){{2}, 1};
f.clauze[22] = (Clauza){{-3, 4, 5}, 3};
f.clauze[23] = (Clauza){{-6, -7, -8, -9, -10}, 5};
f.clauze[24] = (Clauza){{1, 2, 3, 4}, 4};
f.clauze[25] = (Clauza){{-5, -6, 7}, 3};
f.clauze[26] = (Clauza){{8, 9, -10}, 3};
f.clauze[27] = (Clauza){{1, 3, 5, 7, 9}, 5};
f.clauze[28] = (Clauza){{2, 4, 6, 8, 10}, 5};
f.clauze[29] = (Clauza){{-1, -3, -5, -7, -9}, 5};
f.clauze[30] = (Clauza){{-2, -4, -6, -8, -10}, 5};
f.clauze[31] = (Clauza){{1}, 1};
f.clauze[32] = (Clauza){{-2, 3}, 2};
f.clauze[33] = (Clauza){{4, -5, 6}, 3};
f.clauze[34] = (Clauza){{-7, 8, -9, 10}, 4};
f.clauze[35] = (Clauza){{-1, 2, -3, 4, -5, 6}, 6};
f.clauze[36] = (Clauza){{-6, 7, -8, 9, -10}, 5};
f.clauze[37] = (Clauza){{1, -2, 3, -4, 5, -6, 7}, 7};
f.clauze[38] = (Clauza){{8, -9, 10, -1, 2, -3}, 6};
f.clauze[39] = (Clauza){{-4, 5, -6, 7, -8}, 5};
f.clauze[40] = (Clauza){{9, -10, 1}, 3};
f.clauze[41] = (Clauza){{-2, 3, -4, 5}, 4};
f.clauze[42] = (Clauza){{6, -7, 8, -9}, 4};
f.clauze[43] = (Clauza){{10, -1, 2, -3}, 4};
f.clauze[44] = (Clauza){{-4, 5, -6}, 3};
f.clauze[45] = (Clauza){{7, -8, 9}, 3};
f.clauze[46] = (Clauza){{-10, 1, -2}, 3};
f.clauze[47] = (Clauza){{3, -4, 5, -6}, 4};
f.clauze[48] = (Clauza){{-7, 8, -9, 10}, 4};
f.clauze[49] = (Clauza){{-1, 2, -3, 4}, 4};
f.clauze[50] = (Clauza){{-5, 6, -7, 8}, 4};
f.clauze[51] = (Clauza){{-9, 10}, 2};
f.clauze[52] = (Clauza){{1, 3, 5, 7, 9}, 5};
f.clauze[53] = (Clauza){{-2, -4, -6, -8, -10}, 5};
f.clauze[54] = (Clauza){{2, 4, 6, 8, 10}, 5};
f.clauze[55] = (Clauza){{-1, -3, -5, -7, -9}, 5};
f.clauze[56] = (Clauza){{-2, -3, -4}, 3};
f.clauze[57] = (Clauza){{5, 6, 7}, 3};
f.clauze[58] = (Clauza){{-8, -9}, 2};
f.clauze[59] = (Clauza){{10}, 1};
f.clauze[60] = (Clauza){{1, -2, 3, -4}, 4};
f.clauze[61] = (Clauza){{5, -6, 7, -8}, 4};
f.clauze[62] = (Clauza){{9, -10, 1, -2}, 4};
f.clauze[63] = (Clauza){{3, -4, 5, -6}, 4};
f.clauze[64] = (Clauza){{7, -8, 9, -10}, 4};
f.clauze[65] = (Clauza){{-1, 2, -3, 4}, 4};
f.clauze[66] = (Clauza){{-5, 6, -7, 8}, 4};
f.clauze[67] = (Clauza){{9, -10, 1, -2}, 4};
f.clauze[68] = (Clauza){{3, -4, 5}, 3};
f.clauze[69] = (Clauza){{6, -7, 8}, 3};
f.clauze[70] = (Clauza){{-9, 10, -1}, 3};
f.clauze[71] = (Clauza){{2, -3, 4}, 3};
f.clauze[72] = (Clauza){{-5, 6, -7}, 3};
f.clauze[73] = (Clauza){{8, -9, 10}, 3};
f.clauze[74] = (Clauza){{-1, 2, -3}, 3};
f.clauze[75] = (Clauza){{4, -5, 6}, 3};
f.clauze[76] = (Clauza){{-7, 8, -9}, 3};
f.clauze[77] = (Clauza){{10, -1, 2}, 3};
f.clauze[78] = (Clauza){{-3, 4, -5}, 3};
f.clauze[79] = (Clauza){{6, -7, 8}, 3};
f.clauze[80] = (Clauza){{-9, 10, -1}, 3};
f.clauze[81] = (Clauza){{2, -3, 4}, 3};
f.clauze[82] = (Clauza){{-5, 6, -7}, 3};
f.clauze[83] = (Clauza){{8, -9, 10}, 3};
f.clauze[84] = (Clauza){{-1, 2, -3}, 3};
f.clauze[85] = (Clauza){{4, -5, 6}, 3};
f.clauze[86] = (Clauza){{-7, 8, -9}, 3};
f.clauze[87] = (Clauza){{10, -1, 2}, 3};
f.clauze[88] = (Clauza){{-3, 4, -5}, 3};
f.clauze[89] = (Clauza){{6, -7, 8}, 3};






    if (dpll(&f))
        printf("Formula este satisfiabila.\n");
    else
        printf("Formula este nesatisfiabila.\n");

    clock_t end_time = clock();  // Încheie măsurarea timpului

    double total_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;  // Timpul total de execuție
    printf("Timpul total de rulare este: %f secunde.\n", total_time);

    return 0;
}
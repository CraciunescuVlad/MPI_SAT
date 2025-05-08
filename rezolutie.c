#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LITERALI 10
#define MAX_CLAUZE 90000000000000

typedef struct {
    int literali[MAX_LITERALI];
    int nr_literali;
} Clauza;

typedef struct {
    Clauza clauze[MAX_CLAUZE];
    int nr_clauze;
} Formula;

// =============== UTILITARE ===============

bool clauze_egale(Clauza *c1, Clauza *c2) {
    if (c1->nr_literali != c2->nr_literali) return false;
    for (int i = 0; i < c1->nr_literali; i++) {
        bool found = false;
        for (int j = 0; j < c2->nr_literali; j++) {
            if (c1->literali[i] == c2->literali[j]) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    return true;
}

bool contine_clauza(Formula *f, Clauza *c) {
    for (int i = 0; i < f->nr_clauze; i++) {
        if (clauze_egale(&f->clauze[i], c))
            return true;
    }
    return false;
}

void adauga_clauza(Formula *f, Clauza *c) {
    if (!contine_clauza(f, c)) {
        f->clauze[f->nr_clauze++] = *c;
    }
}

Clauza rezolva(Clauza *c1, Clauza *c2, int l) {
    Clauza rez = { .nr_literali = 0 };

    for (int i = 0; i < c1->nr_literali; i++) {
        if (c1->literali[i] != l && c1->literali[i] != -l)
            rez.literali[rez.nr_literali++] = c1->literali[i];
    }

    for (int i = 0; i < c2->nr_literali; i++) {
        int lit = c2->literali[i];
        if (lit != l && lit != -l) {
            bool found = false;
            for (int j = 0; j < rez.nr_literali; j++) {
                if (rez.literali[j] == lit) {
                    found = true;
                    break;
                }
            }
            if (!found)
                rez.literali[rez.nr_literali++] = lit;
        }
    }

    return rez;
}

bool contine_literal(Clauza *c, int l) {
    for (int i = 0; i < c->nr_literali; i++) {
        if (c->literali[i] == l)
            return true;
    }
    return false;
}

// =============== ALGORITM DE REZOLUȚIE ===============

bool rezolutie_completa(Formula *f) {
    Formula baza = *f;
    bool modificat = true;

    while (modificat) {
        modificat = false;

        for (int i = 0; i < baza.nr_clauze; i++) {
            for (int j = i + 1; j < baza.nr_clauze; j++) {
                // Caută literalul de rezoluție comun
                for (int u = 0; u < baza.clauze[i].nr_literali; u++) {
                    int l = baza.clauze[i].literali[u];
                    if (contine_literal(&baza.clauze[j], -l)) {
                        // Aplică rezoluția
                        Clauza rez = rezolva(&baza.clauze[i], &baza.clauze[j], l);
                        if (rez.nr_literali == 0) {
                            // s-a generat clauza vidă → nesatisfiabil
                            return false;
                        }
                        if (!contine_clauza(&baza, &rez)) {
                            adauga_clauza(&baza, &rez);
                            modificat = true;
                        }
                    }
                }
            }
        }
    }

    // Nu s-a generat clauza vidă → satisfiabil
    return true;
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






    if (rezolutie_completa(&f))
        printf("Formula este satisfiabila.\n");
    else
        printf("Formula este nesatisfiabila (s-a generat clauza vida).\n");

    clock_t end_time = clock();
    double total_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Timpul total de rulare este: %f secunde.\n", total_time);

    return 0;
}
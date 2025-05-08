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
        if (clauze_egale(&f->clauze[i], c)) return true;
    }
    return false;
}

void adauga_clauza(Formula *f, Clauza *c) {
    if (!contine_clauza(f, c)) {
        f->clauze[f->nr_clauze++] = *c;
    }
}

bool are_clauza_vid(Formula *f) {
    for (int i = 0; i < f->nr_clauze; i++) {
        if (f->clauze[i].nr_literali == 0)
            return true;
    }
    return false;
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

int gaseste_literal_unitar(Formula *f) {
    for (int i = 0; i < f->nr_clauze; i++)
        if (f->clauze[i].nr_literali == 1)
            return f->clauze[i].literali[0];
    return 0;
}

int gaseste_literal_pur(Formula *f) {
    int poz[101] = {0}, neg[101] = {0};
    for (int i = 0; i < f->nr_clauze; i++) {
        for (int j = 0; j < f->clauze[i].nr_literali; j++) {
            int l = f->clauze[i].literali[j];
            if (l > 0) poz[l] = 1;
            else neg[-l] = 1;
        }
    }
    for (int i = 1; i <= 100; i++) {
        if (poz[i] && !neg[i]) return i;
        if (!poz[i] && neg[i]) return -i;
    }
    return 0;
}

void elimina_clauze_cu_literal(Formula *f, int l) {
    int k = 0;
    for (int i = 0; i < f->nr_clauze; i++) {
        bool gasit = false;
        for (int j = 0; j < f->clauze[i].nr_literali; j++)
            if (f->clauze[i].literali[j] == l)
                gasit = true;
        if (!gasit)
            f->clauze[k++] = f->clauze[i];
    }
    f->nr_clauze = k;
}

void elimina_literal(Formula *f, int l) {
    for (int i = 0; i < f->nr_clauze; i++) {
        int k = 0;
        for (int j = 0; j < f->clauze[i].nr_literali; j++)
            if (f->clauze[i].literali[j] != l)
                f->clauze[i].literali[k++] = f->clauze[i].literali[j];
        f->clauze[i].nr_literali = k;
    }
}

bool dp(Formula *f) {
    while (1) {
        if (f->nr_clauze == 0) return true;
        if (are_clauza_vid(f)) return false;

        // Regula 1: literal unitar
        int lit = gaseste_literal_unitar(f);
        if (lit != 0) {
            elimina_clauze_cu_literal(f, lit);
            elimina_literal(f, -lit);
            continue;
        }

        // Regula 2: literal pur
        int pur = gaseste_literal_pur(f);
        if (pur != 0) {
            elimina_clauze_cu_literal(f, pur);
            continue;
        }

        // Regula 3: rezoluție
        int l = f->clauze[0].literali[0];
        Formula noua = {.nr_clauze = 0};

        for (int i = 0; i < f->nr_clauze; i++) {
            bool cu_l = false, cu_opus = false;
            for (int j = 0; j < f->clauze[i].nr_literali; j++) {
                if (f->clauze[i].literali[j] == l) cu_l = true;
                if (f->clauze[i].literali[j] == -l) cu_opus = true;
            }

            if (!cu_l && !cu_opus)
                adauga_clauza(&noua, &f->clauze[i]);
        }

        for (int i = 0; i < f->nr_clauze; i++) {
            for (int j = 0; j < f->nr_clauze; j++) {
                bool a_l = false, b_opus = false;
                for (int u = 0; u < f->clauze[i].nr_literali; u++)
                    if (f->clauze[i].literali[u] == l)
                        a_l = true;
                for (int v = 0; v < f->clauze[j].nr_literali; v++)
                    if (f->clauze[j].literali[v] == -l)
                        b_opus = true;

                if (a_l && b_opus) {
                    Clauza rez = rezolva(&f->clauze[i], &f->clauze[j], l);
                    if (rez.nr_literali == 0) return false;
                    adauga_clauza(&noua, &rez);
                }
            }
        }

        *f = noua;
    }
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





    if (dp(&f))
        printf("Formula este satisfiabila.\n");
    else
        printf("Formula este nesatisfiabila.\n");

    clock_t end_time = clock();
    double total_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Timpul total de rulare este: %f secunde.\n", total_time);

    return 0;
}
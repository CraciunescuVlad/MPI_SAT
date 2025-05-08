# MPI_SAT
# SAT Solver în C – Rezoluție, Davis–Putnam, DPLL

Acest proiect conține implementări în C ale celor mai cunoscuți algoritmi pentru rezolvarea **problemei satisfiabilității booleene (SAT)**:  
- **Rezoluție** (Resolution)
- **Davis–Putnam (DP)**
- **Davis–Putnam–Logemann–Loveland (DPLL)**

---

## 📘 Descriere

Problema SAT constă în determinarea dacă o formulă booleană în formă normală conjunctivă (CNF) este **satisfiabilă** – adică dacă există o atribuire de valori de adevăr variabilelor astfel încât formula să fie adevărată.

### 🔍 Algoritmi implementați:

1. **Rezoluție**
   - Tehnică bazată pe aplicarea regulii de rezoluție între clauze.
   - Dacă se poate deduce clauza vidă, formula este **nesatisfiabilă**.

2. **Davis–Putnam (DP)**
   - Aplică eliminarea literalilor puri și propagarea literalilor unici.
   - Algoritm complet, dar fără recursivitate (față de DPLL).

3. **DPLL (Davis–Putnam–Logemann–Loveland)**
   - Extinde DP cu **împărțirea (split)** și recursivitate.
   - Este baza pentru majoritatea solverelor SAT moderne.

---

## 🛠️ Cum rulezi programele

Toți cei trei algoritmi sunt implementați în fișiere C separate. Compilează fiecare fișier:

```bash
gcc rezolutie.c -o rezolutie
./rezolutie

gcc dp.c -o dp
./dp

gcc dpll.c -o dpll
./dpll

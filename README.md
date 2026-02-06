# ALSI51 - Compilation et exécution d'un mini-langage

## Introduction

### Objectif du projet

L'objectif de ce projet est d'exécuter des programmes écrits dans un mini-langage, qui possède :

- **26 variables globales** (`a`, …, `z`) pouvant contenir des valeurs entières
- **Des conditionnelles** (`if`)
- **Des boucles** (`while`)

Et qui permet :

- D'afficher le résultat d'une expression ou le contenu d'une variable
- De renvoyer une valeur à la fin de l'exécution

**Exemple de programme en mini-langage :**

```
a = 0
b = 1
c = 0
if (a <= b)
    c = 2
    if (b < c)
        c = 3
    else
        c = 4
while (c)
    print c
    if (c == 1)
        print 0
    c = c - 1
return 0
```

Pour cela, nous devons écrire un compilateur du mini-langage, vers un langage intermédiaire qui sera exécuté par une machine virtuelle que l'on va également écrire.

### Le processus de compilation

La compilation d'un programme consiste à convertir le code source, écrit dans un langage de haut-niveau (d'abstraction, donc plutôt compréhensible par les humains) en une suite d'instructions de bas-niveau, directement exécutables par la machine. On parle alors de programme exécutable.

Le langage C est un langage compilé, par opposition par exemple au Python, qui est interprété, c'est-à-dire que le code source est lu, décodé et exécuté uniquement à l'exécution.

Le processus de compilation se décline généralement en trois étapes :

#### 1. Analyse lexicale (lexing, réalisée par le lexer)

Le code source, sous forme de chaîne de caractères, est traduite en liste de symboles, que l'on appelle généralement **tokens** (jetons). Ces tokens sont des unités lexicales, qui peuvent être des noms de variables, des nombres, ou encore des mots-clefs (`if`, `while`, `return`…).

**Exemple :**

Code source :
```
a = 1
if a
    print 2
else
    print 3
return a
```

Liste de tokens générée :
```
[ Var(a), Keyword(Assign), Expr(1), Keyword(If), Expr(a), Keyword(Print), Expr(2), Keyword(Else), Keyword(Print), Expr(3), Keyword(Endblock), Keyword(Return), Expr(a) ]
```

#### 2. Analyse syntaxique (parsing, réalisée par le parser)

La liste de tokens est lue, et un **arbre de syntaxe abstraite (AST)** est construit à partir d'elle. Un AST permet de représenter le programme en mémoire sous une forme adaptée à son exécution. Un AST est en réalité un graphe orienté, dans lequel les sommets représentent les différentes instructions à exécuter, et les arcs représentent les transitions entre instructions.

#### 3. Génération de code machine

L'arbre de syntaxe abstraite est utilisé en tant que langage intermédiaire entre le code source et le code machine, pour réaliser diverses opérations d'optimisation. Il sert ensuite de base pour la génération du code en langage machine qui composera le programme exécutable final.

### Alternative : compilation en bytecode

Dans certains langages, comme le Java, le code produit n'est pas directement du code machine, mais un code en langage intermédiaire appelé **bytecode**, indépendant de l'architecture de la machine, et qui est interprété et exécuté par un programme appelé **machine virtuelle**. Cela a pour avantage notamment d'améliorer la portabilité du code compilé.

C'est cette approche que nous adoptons pour ce projet. Nous écrivons un programme qui prend en entrée un AST et exécute le code qu'il décrit.

**Exemple d'exécution :**

Pour le code :
```
a = 1
if a
    print 2
else
    print 3
return a
```

Le résultat produit est :
```
2
-> 1
```

Les nombres sont affichés par l'instruction `print`, et la flèche finale représente la valeur renvoyée en sortie de l'exécution par l'instruction `return`.

## Aperçu de la base de code

### Lancement

1. Téléchargez l'archive zip ou clonez le dépôt complet sur votre machine
2. Vérifiez que vous arrivez à compiler le code dans CLion, en tant que projet CMake
3. Vérifiez qu'en exécutant le programme, vous obtenez bien le fichier `output/code_ex.mmd`

**Recommandation :** Créer un dépôt Github pour votre groupe, afin de collaborer plus efficacement.

### Contenu de la base de code

Le code est organisé de la manière suivante :

- **`include/`** : Contient les fichiers d'en-tête (`.h`)
- **`src/`** : Contient les fichiers source (`.c`)
  - **`src/expressions/`** : Implémentation complète de l'évaluation des expressions
    - `expr.c` : Parsing et évaluation des expressions (RPN, Shunting Yard)
    - `operator.c` : Application des opérateurs arithmétiques, logiques et de comparaison
    - `expr_token.c` : Gestion des tokens d'expressions
  - **`src/structures/`** : Implémentations des structures de données
    - Listes à deux extrémités, pile, file
    - `t_prog_token_list` : Liste dynamique pour les tokens du programme
  - **`src/file_io/`** : Code de lecture dans un fichier
  - **`src/program/`** : Implémentation complète du compilateur/interpréteur
    - `lexer.c` : Analyse lexicale complète 
    - `parser.c` : Analyse syntaxique complète 
    - `run.c` : Exécution de l'AST complète 
    - `program.c` : Fonctions auxiliaires et destructeur `destroy_ast` 
    - `lexical.c` : Fonctions auxiliaires pour les tokens

## Partie 3 : Analyse lexicale et fonctionnalités avancées

Cette partie implémente l'analyseur lexical complet ainsi que des fonctionnalités avancées du compilateur.

### Implémentations réalisées

#### 1. Analyseur lexical complet (`src/program/lexer.c`)

- **`lex()`** : Analyse lexicale complète du code source
  - Tokenise le code source en liste de tokens (`t_prog_token_list`)
  - Reconnaît tous les mots-clés : `if`, `else`, `while`, `return`, `print`, `=`
  - Détecte les variables (caractères `a-z`)
  - Parse les expressions et les convertit en RPN via `shunting_yard()`
  - Gère les chaînes de caractères entre guillemets doubles
  - Gère l'indentation pour détecter les blocs (4 espaces)
  - Génère automatiquement les tokens `KW_ENDBLOCK` lors des changements d'indentation

#### 2. Opérateurs booléens dans les expressions

Le langage supporte un ensemble complet d'opérateurs logiques :

- **ET** (`&`) : Conjonction logique
- **OU** (`|`) : Disjonction logique
- **NON** (`N`) : Négation logique (opérateur unaire)
- **XOR** (`^`) : Ou exclusif

Ces opérateurs sont intégrés dans l'algorithme Shunting Yard avec gestion correcte de la priorité et évalués correctement dans `eval_rpn()` via `apply_op()`.

#### 3. Chaînes de caractères dans les instructions print

Le langage permet d'afficher des chaînes de caractères littérales :

- Parsing des chaînes de caractères entre guillemets doubles (`"..."`)
- Support dans le lexer pour créer des tokens de type `PT_STRING`
- Gestion dans le parser pour différencier les expressions RPN et les chaînes
- Affichage correct des chaînes dans `run_aux()` via `eval_string_expr()`

**Exemple :**
```
print "Hello world"
print 42
```

#### 4. Précalcul des sous-expressions constantes
Le compilateur identifie et évalue à la compilation les sous-expressions qui ne dépendent d'aucune variable, c'est-à-dire qui sont composées uniquement de constantes. Ces sous-expressions sont alors remplacées par leur résultat calculé, ce qui permet d'optimiser l'exécution du programme.

Algorithme (haute-niveau)
1. L'expression est d'abord convertie en RPN (notation polonaise inverse) avec `shunting_yard()`.
2. On parcourt la représentation RPN pour identifier des séquences d'opérandes et d'opérateurs qui ne dépendent d'aucune variable (tous des constantes).
3. Pour chaque sous-séquence constante trouvée, on calcule son résultat (en évaluant localement la sous-expression) et on remplace la séquence par un unique opérande constant.
4. On répète l'opération jusqu'à stabilisation (aucune sous-expression constante supplémentaire possible).

Implémentation (dans le projet)
- Fichier principal : `src/program/lexer.c` (après le parsing de l'expression).
- Fonctions clés appelées dans la pipeline :
  - `shunting_yard()` : conversion infixe → RPN.
  - `simplify_constant_subexpressions_rpn(&expr_rpn)` : parcourt et remplace les sous-expressions constantes dans la structure RPN.
  - `is_constant_expr_rpn(&expr_rpn)` : teste si l'expression RPN est entièrement constante (utile pour décider si on peut remplacer l'expression par un littéral simple).
  - `precompute_constant_expr_rpn(&expr_rpn)` : évalue une expression RPN entièrement constante et la remplace par un seul opérande constant (optimisation finale).

Exemples
- Avant : expression infixe `2 + 3 * (4 - 1)`
  - RPN initial : `2 3 4 1 - * +`
  - Après simplification : `2 3 3 * +` → puis `2 9 +` → puis `11`
  - Résultat stocké : un littéral `11` (RPN réduit à un seul opérande).

- Avant : `a + (2 * 3)`
  - RPN initial : `a 2 3 * +`
  - Seule la sous-expression `2 3 *` est constante et sera remplacée par `6` → RPN devient `a 6 +`.

#### 5. Support des boucles "for"
Le langage supporte désormais les boucles `for` avec la syntaxe suivante :
```
for ([init]; [cond]; [expr])
    [block]
```

## Annexes : Syntaxe du mini-langage

### Instructions (statements)

#### Affectation (assignment)

La variable `var` reçoit le résultat de l'expression `expr` :

```
[var] = [expr]
```

**Exemple :**
```
a = 3
```

#### Affichage (print)

Affiche la valeur de l'expression `expr` :

```
print [expr]
```

**Exemple :**
```
print 2 * x + 3 * y
```

#### Conditionnelle "if"

Si l'expression `cond` s'évalue à une valeur différente de 0, on exécute le programme `prog_if_true`, sinon le programme `prog_if_false` :

```
if [cond]
    [prog_if_true]
else
    [prog_if_false]
```

**Exemple :**
```
if a == 0
    return 0
else
    a = a + 1
```

#### Boucle "while"

Tant que l'expression `cond` s'évalue à une valeur différente de 0, on exécute le programme `block` :

```
while [cond]
    [block]
```

**Exemple :**
```
while x != 0
    x = x - 1
    print x
```

#### Boucle "for"

itère sur une variable d'itération avec une initialisation **OU** une variable existante `init`, tant que `cond` est vrai. `expr` est assigné à la variable à chaque itération.

```
for ([init]; [cond]; [expr])
    [block]
```

**Exemple :**
```
for (i = 0; i < 10; i + 1)
    print i

k = 1
for (k; k < 50; k * 2)
    print k 
```

### Indentation

À chaque entrée dans un bloc de `if`, `else` ou `while`, on indente le code du bloc avec **4 espaces**. Cette convention est importante si vous souhaitez écrire l'analyseur lexical.

### Expressions

Le langage supporte des expressions complexes avec :

- **Opérateurs arithmétiques** : `+`, `-`, `*`, `/`, `^` (puissance)
- **Opérateurs de comparaison** : `==`, `!=`, `<`, `>`, `<=`, `>=`
- **Opérateurs logiques** : `&` (ET), `|` (OU), `N` (NON), `X` (XOR)
- **Parenthèses** : pour grouper les expressions
- **Nombres entiers** : valeurs numériques (support des nombres négatifs)
- **Variables** : références aux variables définies (caractères `a` à `z`)

Les expressions sont converties en notation polonaise inverse (RPN) à l'aide de l'algorithme de Shunting Yard pour l'évaluation.

## Compilation et exécution

### Prérequis

- CMake (version 3.5 ou supérieure)
- Compilateur C compatible C23 (gcc/clang)
- Make

### Build

Utilisez le script de compilation fourni :

```bash
./build.sh
```

Ou manuellement :

```bash
mkdir -p build
cd build
cmake ..
make
```

L'exécutable `compiler_proj` sera généré dans le répertoire `build/`.

### Exécution

**Utilisation basique :**

```bash
./run.sh
```

Cela compile et exécute le programme avec le fichier `code/code.txt` par défaut.

**Exécution manuelle :**

```bash
cd build
./compiler_proj
```

Le programme lit le fichier `code/code.txt` (relatif au répertoire d'exécution), le parse, l'exécute, et génère un fichier AST en format Mermaid (`.mmd`) dans le répertoire de sortie.

### Export de l'AST

Le programme génère automatiquement une représentation de l'AST au format Mermaid (`.mmd`) qui peut être visualisée avec des outils comme Mermaid Live Editor ou des extensions VS Code/Cursor.

## Structure du projet

```
.
├── CMakeLists.txt          # Configuration CMake
├── build.sh                # Script de compilation
├── run.sh                  # Script d'exécution
├── debug.sh                # Script de débogage
├── DEBUG_GUIDE.md          # Guide de débogage avec GDB
├── code/                   # Fichiers de code source exemples
│   ├── code.txt
│   └── code_example.txt
├── include/                # Fichiers d'en-tête
│   ├── expressions/        # Gestion des expressions
│   ├── file_io/            # Entrée/sortie de fichiers
│   ├── program/             # Lexer, parser, runtime
│   └── structures/         # Structures de données
└── src/                    # Code source
    ├── main.c              # Point d'entrée
    ├── expressions/        # Implémentation des expressions
    ├── file_io/            # Implémentation I/O
    ├── program/            # Implémentation lexer/parser/runtime
    └── structures/         # Implémentation des structures
```

## Débogage

Consultez `DEBUG_GUIDE.md` pour un guide complet sur le débogage avec GDB.

### Démarrage rapide

```bash
./debug.sh
```

Ou manuellement :

```bash
cd build
gdb ./compiler_proj
```

### Points d'arrêt utiles

- `break main` - Point d'entrée
- `break lex` - Analyseur lexical
- `break parse` - Analyseur syntaxique
- `break run` - Début de l'exécution
- `break run_aux` - Fonction d'exécution récursive

## Exemples

### Exemple simple

**Code :**
```
a = 1
if a
    print 2
else
    print 3
return a
```

**Sortie attendue :**
```
2
-> 1
```

### Exemple complet

**Code :**
```
a = 0
b = 1
c = 0
if (a <= b)
    c = 2
    if (b < c)
        c = 3
    else
        c = 4
while (c)
    print c
    if (c == 1)
        print 0
    c = c - 1
return 0
```

**Sortie attendue :**
```
3
2
1
0
-> 0
```

## Auteur

Moi.

## Licence

Ce projet est un travail académique.

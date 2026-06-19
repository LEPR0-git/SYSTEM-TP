# ============================================
# VARIABLES
# ============================================
CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -std=c99 -g -Iinclude
# -Iinclude : dit au compilateur de chercher les .h dans include/

SRCDIR  = src
OBJDIR  = obj
BINDIR  = bin
LIBDIR  = lib
TESTDIR = tests
DEMODIR = demo

# Modules : les noms des fichiers sans extension
MODULES = llist strutil

# Fichiers source
SRCS = $(addprefix $(SRCDIR)/, $(addsuffix .c, $(MODULES)))
# SRCS = src/llist.c src/strutil.c

# Fichiers objets
OBJS = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(MODULES)))
# OBJS = obj/llist.o obj/strutil.o

# Fichiers de dépendances (générés par -MMD)
DEPS = $(OBJS:.o=.d)
# DEPS = obj/llist.d obj/strutil.d

# Tests (exécutables)
TESTS = $(BINDIR)/test_llist $(BINDIR)/test_strutil

# Démo
DEMO = $(BINDIR)/demo

# ============================================
# .PHONY : cibles qui ne créent pas de fichier
# ============================================
.PHONY: all test lib-static lib-dynamic clean fclean re help nm-check objdump-check

# ============================================
# RÈGLE PAR DÉFAUT : compile tout
# ============================================
all: $(DEMO) $(TESTS)

# ============================================
# CRÉATION DES DOSSIERS (si inexistants)
# ============================================
$(OBJDIR) $(BINDIR) $(LIBDIR):
	mkdir -p $@
# -p : crée les dossiers parents si nécessaire, pas d'erreur si existe déjà

# ============================================
# RÈGLE GÉNÉRIQUE : compile un .c en .o
# $< = le fichier .c source
# $@ = le fichier .o produit
# -MMD : génère un fichier .d de dépendances
# -MP  : évite les erreurs si un .h est supprimé
# -c   : compile sans lier
# | $(OBJDIR) : dépend de l'existence du dossier obj/
# ============================================
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# ============================================
# INCLUSION DES DÉPENDANCES
# Permet de recompiler automatiquement si un .h change
# Le - devant include évite l'erreur si le fichier .d n'existe pas encore
# ============================================
-include $(DEPS)

# ============================================
# PROGRAMME DE DÉMO
# Lie demo.c avec les 2 .o des modules
# $^ = demo/demo.c obj/llist.o obj/strutil.o
# ============================================
$(DEMO): $(DEMODIR)/demo.c $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^
# Exemple : gcc -Wall... -Iinclude -o bin/demo demo/demo.c obj/llist.o obj/strutil.o

# ============================================
# TESTS
# Chaque test est lié avec le .o de son module
# ============================================
$(BINDIR)/test_llist: $(TESTDIR)/test_llist.c $(OBJDIR)/llist.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BINDIR)/test_strutil: $(TESTDIR)/test_strutil.c $(OBJDIR)/strutil.o | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

# ============================================
# BIBLIOTHÈQUE STATIQUE (.a)
# ar   : outil d'archivage
# r    : insère/remplace les fichiers
# c    : crée l'archive si elle n'existe pas
# s    : écrit un index (accélère l'accès)
# ============================================
lib-static: $(OBJS) | $(LIBDIR)
	ar rcs $(LIBDIR)/libsysutil.a $(OBJS)
# Crée lib/libsysutil.a à partir de obj/llist.o et obj/strutil.o

# ============================================
# BIBLIOTHÈQUE DYNAMIQUE (.so)
# -shared : crée une bibliothèque partagée
# -fPIC   : Position Independent Code (obligatoire pour .so)
# ============================================
lib-dynamic: $(OBJS) | $(LIBDIR)
	$(CC) -shared -fPIC -o $(LIBDIR)/libsysutil.so $(OBJS)
# Crée lib/libsysutil.so

# ============================================
# RÈGLE TEST : compile et exécute les tests
# @ : n'affiche pas la commande elle-même
# -e : active l'interprétation des backslashes
# -n : ne fait pas de saut de ligne
# ./$$test && echo PASS || echo FAIL
#   -> si le test réussit (code 0) : affiche PASS
#   -> si le test échoue (code != 0) : affiche FAIL
# ============================================
test: $(TESTS)
	@echo "=== Exécution des tests ==="
	@for test in $(TESTS); do \
		echo -n "$$test : "; \
		./$$test > /dev/null 2>&1 && echo "PASS" || echo "FAIL"; \
	done

# ============================================
# NETTOYAGE
# ============================================
clean:
	rm -rf $(OBJDIR)/*
# Supprime tout le contenu de obj/

fclean: clean
	rm -rf $(BINDIR)/* $(LIBDIR)/*
# Supprime aussi bin/ et lib/

re: fclean all
# Nettoie tout puis reconstruit

# ============================================
# AIDE
# @echo : affiche le texte (le @ cache la commande)
# ============================================
help:
	@echo "Cibles disponibles :"
	@echo "  make            : compile la démo et les tests"
	@echo "  make test       : exécute les tests (PASS/FAIL)"
	@echo "  make lib-static : crée lib/libsysutil.a"
	@echo "  make lib-dynamic: crée lib/libsysutil.so"
	@echo "  make clean      : supprime obj/"
	@echo "  make fclean     : supprime obj/, bin/, lib/"
	@echo "  make re         : fclean + all"
	@echo "  make nm-check   : symboles de la bibliothèque"
	@echo "  make objdump-check : désassemble strutil.o"

# ============================================
# INSPECTION DES BINAIRES
# ============================================

# nm-check : affiche les fonctions exportées
# nm -g  : montre seulement les symboles globaux
# grep T : filtre les fonctions (T = code/texte)
nm-check: lib-static
	@echo "=== Symboles dans libsysutil.a ==="
	nm -g $(LIBDIR)/libsysutil.a | grep " T "

# objdump-check : désassemble strutil.o
# objdump -d : montre le code assembleur
objdump-check: $(OBJDIR)/strutil.o
	@echo "=== Désassemblage de strutil.o ==="
	objdump -d $(OBJDIR)/strutil.o
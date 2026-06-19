# ══════════════════════════════════════════════════════════════
#  Makefile — libsysutil
#  INF362 Programmation Système — UY1 Faculté des Sciences
# ══════════════════════════════════════════════════════════════

# ── Variables imposées ─────────────────────────────────────────
CC      := gcc
CFLAGS  := -Wall -Wextra -Werror -std=c99 -g
CFLAGS_SO := -Wall -Wextra -std=c99 -g   # -Werror retiré pour -fPIC sur certains GCC

INCDIR  := include
SRCDIR  := src
OBJDIR  := obj
LIBDIR  := lib
BINDIR  := bin
TESTDIR := tests
DEMODIR := demo

# ── Sources et objets ──────────────────────────────────────────
SRCS    := $(SRCDIR)/strutil.c $(SRCDIR)/llist.c
OBJS    := $(OBJDIR)/strutil.o $(OBJDIR)/llist.o

# Dépendances automatiques générées par -MMD -MP
DEPS    := $(OBJS:.o=.d)

# Bibliothèques produites
LIB_STATIC  := $(LIBDIR)/libsysutil.a
LIB_DYNAMIC := $(LIBDIR)/libsysutil.so

# Exécutables de test
TEST_STRUTIL := $(BINDIR)/test_strutil
TEST_LLIST   := $(BINDIR)/test_llist

# Exécutables de démo
DEMO         := $(BINDIR)/demo
DEMO_DL      := $(BINDIR)/demo_dl

# ── Cible par défaut : tout compiler ──────────────────────────
.PHONY: all
all: $(TEST_STRUTIL) $(TEST_LLIST) $(DEMO) $(DEMO_DL)
	@echo ""
	@echo "╔══════════════════════════════════════╗"
	@echo "║  Build complet OK                    ║"
	@echo "╚══════════════════════════════════════╝"

# ── Règle générique : obj/%.o depuis src/%.c ──────────────────
# Utilise les variables automatiques $< (source) et $@ (cible)
# -MMD -MP génère les fichiers .d de dépendances automatiques
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -MMD -MP -I$(INCDIR) -c $< -o $@

# Inclusion des dépendances automatiques (silencieux si absents)
-include $(DEPS)

# ── Exécutables de test ────────────────────────────────────────
$(TEST_STRUTIL): $(TESTDIR)/test_strutil.c $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) $^ -o $@

$(TEST_LLIST): $(TESTDIR)/test_llist.c $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) $^ -o $@

# ── Exécutable de démo ─────────────────────────────────────────
$(DEMO): $(DEMODIR)/demo.c $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) $^ -o $@

# ── Bibliothèque statique ──────────────────────────────────────
.PHONY: lib-static
lib-static: $(LIB_STATIC)

$(LIB_STATIC): $(OBJS) | $(LIBDIR)
	ar rcs $@ $^
	@echo "[lib-static] $(LIB_STATIC) créée."

# ── Bibliothèque dynamique ─────────────────────────────────────
.PHONY: lib-dynamic
lib-dynamic: $(LIB_DYNAMIC)

# Les .o pour .so doivent être compilés avec -fPIC
$(OBJDIR)/strutil_pic.o: $(SRCDIR)/strutil.c | $(OBJDIR)
	$(CC) $(CFLAGS_SO) -fPIC -MMD -MP -I$(INCDIR) -c $< -o $@

$(OBJDIR)/llist_pic.o: $(SRCDIR)/llist.c | $(OBJDIR)
	$(CC) $(CFLAGS_SO) -fPIC -MMD -MP -I$(INCDIR) -c $< -o $@

$(LIB_DYNAMIC): $(OBJDIR)/strutil_pic.o $(OBJDIR)/llist_pic.o | $(LIBDIR)
	$(CC) $(CFLAGS_SO) -shared -fPIC $^ -o $@
	@echo "[lib-dynamic] $(LIB_DYNAMIC) créée."

# ── demo_dl : chargement dynamique via dlopen ─────────────────
$(DEMO_DL): $(DEMODIR)/demo_dl.c | $(BINDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) $< -o $@ -ldl
	@echo "[demo_dl] $(DEMO_DL) compilé."

.PHONY: demo-dl
demo-dl: $(LIB_DYNAMIC) $(DEMO_DL)
	@echo "--- Exécution demo_dl ---"
	LD_LIBRARY_PATH=$(LIBDIR) $(DEMO_DL)

# ── Règle test : compile + exécute + PASS/FAIL ────────────────
.PHONY: test
test: $(TEST_STRUTIL) $(TEST_LLIST)
	@echo ""
	@echo "=== test_strutil ==="
	@$(TEST_STRUTIL) ; \
	  if [ $$? -eq 0 ]; then echo ">> PASS : test_strutil"; \
	  else echo ">> FAIL : test_strutil"; fi
	@echo ""
	@echo "=== test_llist ==="
	@$(TEST_LLIST) ; \
	  if [ $$? -eq 0 ]; then echo ">> PASS : test_llist"; \
	  else echo ">> FAIL : test_llist"; fi

# ── Analyse nm ────────────────────────────────────────────────
.PHONY: nm-check
nm-check: $(LIB_STATIC)
	@echo "=== Symboles exportés (type T) de libsysutil.a ==="
	nm -g $(LIB_STATIC)
	@echo ""
	@echo "=== Dépendances externes (type U) ==="
	nm -u $(LIB_STATIC)

# ── Analyse objdump ───────────────────────────────────────────
.PHONY: objdump-check
objdump-check: $(OBJDIR)/llist.o
	@echo "=== Désassemblage de llist.o ==="
	objdump -d $(OBJDIR)/llist.o

# ── Création des répertoires si absents ───────────────────────
$(OBJDIR) $(LIBDIR) $(BINDIR):
	mkdir -p $@

# ── Nettoyage ─────────────────────────────────────────────────
.PHONY: clean
clean:
	rm -rf $(OBJDIR)
	@echo "[clean] Fichiers objets supprimés."

.PHONY: fclean
fclean: clean
	rm -rf $(LIBDIR) $(BINDIR)
	@echo "[fclean] Bibliothèques et exécutables supprimés."

.PHONY: re
re: fclean all

# ── Aide ──────────────────────────────────────────────────────
.PHONY: help
help:
	@echo ""
	@echo "Cibles disponibles :"
	@echo "  all           — compile tests + demo + demo_dl  (défaut)"
	@echo "  lib-static    — crée lib/libsysutil.a"
	@echo "  lib-dynamic   — crée lib/libsysutil.so"
	@echo "  test          — compile et exécute test_strutil + test_llist (PASS/FAIL)"
	@echo "  demo-dl       — compile et exécute demo_dl (dlopen)"
	@echo "  nm-check      — affiche les symboles de libsysutil.a"
	@echo "  objdump-check — désassemble llist.o"
	@echo "  clean         — supprime obj/"
	@echo "  fclean        — supprime obj/ lib/ bin/"
	@echo "  re            — fclean + all"
	@echo "  help          — affiche ce message"
	@echo ""
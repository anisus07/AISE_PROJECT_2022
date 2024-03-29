/* Fonctionnement pour obtenir la pile :

 * Creation d'un espace d'adressage pour utiliser le déroulement distant de la librairie libunwind
 * On construit un _UPT_create pour que libunwind puisque être utilisé sur le processus cible
 * initialisation du déroulementpar un appel à unw_init_remote()
 * unw_get_reg () lit un registre d'entiers, ici le registre de la pile
 * La routine unw_get_proc_name () renvoie le nom de la procédure qui a créé le cadre de pile identifié par le curseur
 * Le curseur de déroulement peut alors être déplacé `` vers le haut '' (vers les cadres de pile précédents) en appelant unw_step().
 * On detruit lorsque l'application a terminé d'utiliser libunwind sur le processus cible
 * 
 */

#include "../Include/backtrace.h"

/**
 * @brief FOnction qui effectue le bactrace pour un processus distant
 *
 * @param pid pid du processus distant
 */
void my_backtrace(pid_t pid)
{
    unw_addr_space_t as = unw_create_addr_space(&_UPT_accessors, 0);
    void *context = _UPT_create(pid);
    unw_cursor_t cursor;
    if (unw_init_remote(&cursor, as, context) != 0)
        printf("ERROR: cannot initialize cursor for remote unwinding\n");

    do
    {
        unw_word_t offset, pc;
        char sym[4096];
        if (unw_get_reg(&cursor, UNW_REG_IP, &pc))
            printf("ERROR: cannot read program counter\n");

        printf("0x%lx: ", pc);

        if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) == 0)
            printf("(%s+0x%lx)\n", sym, offset);
        else
            printf("-- no symbol name found\n");
    } while (unw_step(&cursor) > 0);

    _UPT_destroy(context);
}
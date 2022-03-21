/* Gestion de la mémoire
 * Une fonction pour calculer le nombre d'allocation ainsi que la taille en bytes
 * Une fonction pour calculer le nombre de bytes liberée 
 * Une fonction qui permet d'afficher tout les résultas précédents.
 */
#include "../Include/memory.h"

/**
 * Variable globale necessaire au comptage
 **/

size_t nb_alloc = 0;
size_t size_alloc = 0;

size_t nb_free = 0;
size_t size_free = 0;
size_t size_restant = 0;
void *(*malloc_func)(size_t) = NULL;
void (*free_func)(void *) = NULL;

alloc_table_t table;

/**
 * @brief Récupération du nombre de malloc dans notre programme
 *
 * @param nb_alloc nombre d'allocation
 * @param size_alloc la taille d'allocation
 * @return le nombre de malloc ainsi que sa taille
 */
void *malloc(size_t size)
{

  nb_alloc++;
  size_alloc += size;

  if (!malloc_func)
  {
    malloc_func = (void *(*)(size_t))dlsym(RTLD_NEXT, "malloc");

    if (!malloc_func)
    {
      abort();
    }
  }

  void *ptr = malloc_func(size);
  if (!ptr)
    return NULL;

  if (table.count < MAX_ALLOC)
  {
    for (int i = 0; i < MAX_ALLOC; i++)
    {
      if (table.ptr[i] == NULL)
      {
        //
        table.ptr[i] = ptr;
        table.size[i] = size;
        table.count++;

        //
        break;
      }
    }
  }

  return ptr;
}
/**
 * @brief Récupération du nombre de free dans notre programme
 *
 * @param nb_free nombre de free
 * @param size_free la taille de free
 * @return le nombre de free ainsi que sa taille
 */
void free(void *ptr)
{

  nb_free++;

  if (!free_func)
  {
    free_func = (void (*)(void *))dlsym(RTLD_NEXT, "free");

    if (!free_func)
    {
      abort();
    }
  }

  if (table.count)
  {
    for (int i = 0; i < MAX_ALLOC; i++)
    {
      if (table.ptr[i] == ptr)
      {
        //
        size_free += table.size[i];

        //
        table.ptr[i] = NULL;
        table.size[i] = 0;
        table.count--;

        //
        break;
      }
    }
  }

  free_func(ptr);
}

/**
 * @brief Affichage des résultats obtenus
 */
__attribute__((destructor)) void affic()
{

  printf("You used : %lu times malloc and %lu times free()\n", nb_alloc, nb_free);

  //
  printf("------------------------------------------------------------------------------------------------------------\n");
  printf("\033[1m|%30s |%30s |%30s \033[0m\n", "Allocate", "Free", "Non Free Memory");
  fflush(stdout);
  printf("------------------------------------------------------------------------------------------------------------\n");
  printf("|%30lu |%30lu |%30lu\n",
         size_alloc,
         size_free, size_alloc - size_free);

  fflush(stdout);
  printf("------------------------------------------------------------------------------------------------------------\n");
  printf("\033[1m|%30s |%30s |%30s\033[0m\n", "Bytes", "Bytes", "Bytes");
  fflush(stdout);
  printf("------------------------------------------------------------------------------------------------------------\n");
  if (size_alloc - size_free == 0)
  {
    printf("\033[42m SUCESS: Success of FREE MEMORY\033[0m");
  }
  else
  {
    printf("\033[42m WARNING: NON FREE MEMORY\033[0m");
  }
}
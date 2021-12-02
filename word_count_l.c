/*
 * Implementation of the word_count interface using Pintos lists.
 *
 * You may modify this file, and are expected to modify it.
 */

/*
 * Copyright © 2021 University of California, Berkeley
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PINTOS_LIST
#error "PINTOS_LIST must be #define'd when compiling word_count_l.c"
#endif

#include "word_count.h"


/*  
PINTOS_LIST defined: 
#include "list.h"
typedef struct word_count {
  char* word;
  int count;
  struct list_elem elem;
} word_count_t;
typedef struct list word_count_list_t;
word_count_list_t* -> *list
*/

void init_words(word_count_list_t* wclist) { 
  list_init(wclist);
}

size_t len_words(word_count_list_t* wclist) {
  /* traverse wclist with the help of list.h */
  int len = 0;
  struct list_elem *e;

  for (e = list_begin(wclist); e != list_end(wclist); 
      e = list_next (e)) {
    len++;
  }
  return len;
}

word_count_t* find_word(word_count_list_t* wclist, char* word) {
  // find the first wc using list_begin
  word_count_t *wc;
  struct list_elem *e;
  for (e = list_begin(wclist); e!=list_end(wclist); e = list_next(e)) {
    // use list_entry to conversion from a
    // struct list_elem back to a structure object that contains it
    wc = list_entry (e, word_count_t, elem);
      if(strcmp(wc->word, word)==0) 
          return wc;
  }
  return NULL;
}

word_count_t* add_word(word_count_list_t* wclist, char* word) {
  // find if word exists
  word_count_t *wc = find_word(wclist, word);
  if (wc) {
    wc->count++;
  } else {
    // according to list_push_back, we need the list* 
    // and list_elem*, which is a member of word_count_t 
    // fist, create a word_count_t
    wc = malloc(sizeof(word_count_t));

    /* 
    char* word: pass by reference
    maybe what word refers to is stored on the heap or caller stack
    therefore, just pointer assignment, rather than strcat(wc->word, word)
    */
    // strcat(wc->word, word);
    wc->word = word;
    wc->count = 1;
    // just give the address of list_elem, ignore the underlying details
    list_push_front(wclist, &(wc->elem));
  }
  return wc;
}

void fprint_words(word_count_list_t* wclist, FILE* outfile) { /* TODO */
  struct list_elem *e;
  word_count_t* wc;
  for (e = list_begin(wclist); e != list_end(wclist); 
      e = list_next (e)) {
    wc = list_entry (e, word_count_t, elem);
    fprintf(outfile, "%8d\t%s\n", wc->count, wc->word);
  }
}

static bool less_list(const struct list_elem* ewc1, const struct list_elem* ewc2, void* aux) {
  /* 
  less_list is fairly similar to comp() in qsort()
  according to wordcount_sorr, which invokes list_sort(wclist, less_list, less);
  aux acts as a comparator
  */ 
  
  bool (*less)(const word_count_t *, const word_count_t *) = aux;
  word_count_t *wc1 = list_entry(ewc1, word_count_t, elem);
  word_count_t *wc2 = list_entry(ewc2, word_count_t, elem);
  return less(wc1, wc2);
}

void wordcount_sort(word_count_list_t* wclist,
                    bool less(const word_count_t*, const word_count_t*)) {
  // comparator passed to it as the argument less
  list_sort(wclist, less_list, less);
}

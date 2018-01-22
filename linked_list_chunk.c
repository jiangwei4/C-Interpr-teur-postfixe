/*
Donovan FERRE
David INTHO
Louis LABEL
*/

# include <stdlib.h>   // malloc + free
# include <stdbool.h>
# include <assert.h>

# include "linked_list_chunk.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION




/*!
 * \file
 * \brief Structure used to store list of \link chunk\endlink 's.
 *
 * Please note that \c linked_list_chunk does not make any copy of \c chunk's.
 *
 * The list is bidirectional and can be accessed from both end.
 *
 * The structure is 2-level:
 * \li \c link's are used to provide the bidirectional links (with pointers) and points to \c chunk
 * \li the \c main structure has pointers to first and last \c link's.
 *
 * The empty list has the pointers of its \c main structure at \c NULL.
 *
 * \note Cela ressemble au TDM2 (sans être circulaire).
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


typedef struct maillon_struct {
	struct maillon_struct *prev;
	chunk val;
	struct maillon_struct *next;
} *maillon;

/*! 
 * \c linked_list_chunk is a pointer to a hidden structure (\c main structure). 
*/
struct linked_list_chunk_struct {
	unsigned int length;
	maillon first;
	maillon last;
};




/*!
 * Generate an empty \c linked_list_chunk
 *
 * \return an empty \c linked_list_chunk
 */
linked_list_chunk linked_list_chunk_create ( void )  {
	linked_list_chunk llc = malloc(sizeof(struct linked_list_chunk_struct));
	llc->first = llc->last = NULL;
	llc->length = 0;

	return llc;
}

/*!
 * Destroy the whole structure and the stored values.
 *
 * \param llc \c linked_list_chunk to destroy
 * \pre \c llc is valid (assert-ed)
 */
void linked_list_chunk_destroy ( linked_list_chunk llc )  {
	maillon m1 = llc->first, m2;

	while (m1 != NULL) {
		chunk_destroy(m1->val);
		m2 = m1;
		m1 = m1->next;

		free(m2);
	}

	free(llc);

	llc = NULL;
}


/*!
 * To know whether a \c linked_list_chunk is empty.
 *
 * \param llc \c linked_list_chunk to test
 * \pre \c llc is valid (assert-ed)
 * \return true iff \c llc is empty
 */
bool linked_list_chunk_is_empty ( linked_list_chunk llc)  {
	return llc->length == 0 ? true : false;
}


/*!
 * To print a \c linked_list_chunk.
 * Each chink is printed on a separate line with \c chunk_print.
 *
 * \param llc \c linked_list_chunk to print
 * \param f stream to print to
 * \pre \c llc is valid (assert-ed)
 * \pre \c f is not \c NULL (assert-ed)
 */
void linked_list_chunk_print ( linked_list_chunk llc ,
				      FILE * f )  {

	assert(llc != NULL);
	assert(f != NULL);

	maillon m = llc->first;

	while (m != NULL) {
		chunk_print(m->val, f);
		fprintf(f, "\n");
		m = m->next;
	}
}

/*!
 * Add a chunk at the beginning of the \c linked_list_chunk.
 *
 * \param llc \c linked_list_chunk to add to
 * \param ch \c chunk to add
 * \pre \c llc is valid (assert-ed)
 * \pre \c ch is not \c NULL (assert-ed)
 */
void linked_list_chunk_add_front ( linked_list_chunk llc ,
					  chunk ch )  {

	assert(llc != NULL);
	assert(ch != NULL);

	bool b = false;
	maillon m = malloc(sizeof(struct maillon_struct)), m2;
	m->next = m->prev = NULL;

	m2 = llc->first;

	while (m2 != NULL && !b) {
		if (m2->val == ch) {
			m->val = chunk_copy(ch);
			b = true;
		}
		else {
			m2 = m2->next;
		}
	}

	if (!b) {
		m->val = ch;
	}

	if (linked_list_chunk_is_empty(llc)) {
		llc->first = m;
		llc->last = m;
	}
	else {
		m->next = llc->first;
		llc->first->prev = m;
		llc->first = m;
	}

	llc->length++;
}

/*!
 * Add a \c chunk at the end of the \c linked_list_chunk.
 *
 * \param llc \c linked_list_chunk to add to
 * \param ch \c chunk to add
 * \pre \c llc is valid (assert-ed)
 * \pre \c ch is not \c NULL (assert-ed)
 */
void linked_list_chunk_add_back ( linked_list_chunk llc ,
					 chunk ch )  {

	assert(llc != NULL);
	assert(ch != NULL);

	bool b = false;
	maillon m = malloc(sizeof(struct maillon_struct)), m2;
	m->next = m->prev = NULL;

	m2 = llc->first;

	while (m2 != NULL && !b) {
		if (m2->val == ch) {
			m->val = chunk_copy(ch);
			b = true;
		}
		else {
			m2 = m2->next;
		}
	}

	if (!b) {
		m->val = ch;
	}

	if (linked_list_chunk_is_empty(llc)) {
		llc->first = m;
		llc->last = m;
	}
	else {
		m->prev = llc->last;
		llc->last->next = m;
		llc->last = m;
	}

	llc->length++;
}

/*!
 * Return the \c chunk at the beginning of the \c linked_list_chunk.
 * The \c chunk is removed from the \c linked_list_chunk.
 *
 * \param llc \c linked_list_chunk to pop from
 * \return The removed \c chunk at the beginning or \c NULL if linked_list_chunk empty
 */
chunk linked_list_chunk_pop_front ( linked_list_chunk llc )  {
	
	maillon m;
	chunk ch;

	if (linked_list_chunk_is_empty(llc)) {
		return NULL;
	}

	m = llc->first;
	ch = llc->first->val;

	if (llc->length == 1) {
		llc->first = llc->last = NULL;
	}
	else {
		llc->first->next->prev = NULL;
		llc->first = llc->first->next;
	}

	llc->length--;

	free(m);

	return ch;
}


/*!
 * Add a \b copy of the \c k first \c chunk at the beginning of the \c linked_list_chunk to it-self.
 * If there is less than \c k \c chunk then no copy is made.
 *
 * For \c k, the following \c linked_list_chunk
 * \verbatim [front]                                     ch0 ch_1 ch_2 ...  ch_k-2 ch_k-1   ch_k ch_k+1 ch_k+2 \endverbatim 
 * is transformed into
 * \verbatim [front]  ch0 ch_1 ch_2 ...  ch_k-2 ch_k-1   ch0 ch_1 ch_2 ...  ch_k-2 ch_k-1   ch_k ch_k+1 ch_k+2 \endverbatim 
 *
 * \param llc \c linked_list_chunk to add to
 * \param k size of the prefix to copy in front
 * \pre \c llc is valid (assert-ed)
 * \return false if there where less than k element. In such a case, no copy is made.
 */
bool linked_list_chunk_add_self_copy_front ( linked_list_chunk llc ,
						    unsigned int k )  {
	
	assert(llc != NULL);

	maillon m1, m2;
	unsigned int i = 1;

	if (k > llc->length) {
		return false;
	}

	if (k != 0) {

		m1 = m2 = llc->first;

		while (i < k) {
			m1 = m1->next;
			i++;
		}

		while (m1 != m2) {
			linked_list_chunk_add_front(llc, chunk_copy(m1->val));
			m1 = m1->prev;
		}

		linked_list_chunk_add_front(llc, chunk_copy(m1->val));
	}

	return true;
}


/*!
 * Generate a copy of the \c linked_list_chunk.
 *
 * \param llc \c linked_list_chunk to copy
 * \pre \c llc is valid (assert-ed)
 */linked_list_chunk linked_list_chunk_copy ( linked_list_chunk llc )  {
	
	assert(llc != NULL);

	linked_list_chunk llc2 = linked_list_chunk_create();
	maillon m;

	if (!linked_list_chunk_is_empty(llc)) {
		m = llc->first;

		while (m != NULL) {
			linked_list_chunk_add_back(llc2, chunk_copy(m->val));
			m = m->next;
		}
	}

	return llc2;
}



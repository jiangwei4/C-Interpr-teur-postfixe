/*
Donovan FERRE
David INTHO
Louis LABEL
*/

# include <stdlib.h> // malloc + free
# include <assert.h>

# include "dictionary.h"

# undef NDEBUG   // FORCE ASSERT ACTIVATION





/*!
 * \file
 * \brief Provide a dictionary (associative table).
 * \li keys must be non-empty \c sstring's
 * \li values must be \c chunk's
 *
 * Keys and values are copied to get independent long term storage.
 * In the same spirit, queried values are \b copies. 
 * The <b>caller is in charge of destroying copies</b>; this is indicated by the \c _copy in the query function name.
 *
 * Values are stored in an ordered binary tree.
 * They are two data structures: one for the dictionary and one for nodes.
 *
 * \note Cela ressemble au TDM 3, même s'il y a des différences.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


typedef struct node_struct {
	sstring key;
	chunk val;
} *node;

/*! \c dictionary is a pointer to the hidden structure for dictionary. */
struct dictionary_struct {
	node node;
	dictionary left;
	dictionary right;
};

/*!
 * Generate an empty \c dictionary.
 *
 * \return an empty \c dictionary
 */

dictionary dictionary_create ( void )  { 

	dictionary d = malloc(sizeof(struct dictionary_struct));
	d->node = NULL;
	d->left = d->right = NULL;
	return d;
}


/*!
 * Add an entry \c (key,val) into a \c dictionary.
 *
 * Copies of key and val are made.
 * The original values can be safely destroyed after the call.
 *
 * \param dic \c dictionary to modify
 * \param key insertion key
 * \param val inserted value
 * \pre no pointer is NULL (assert-ed)
 * \pre key is not an empty string  (assert-ed)
 */
void dictionary_set(dictionary dic, sstring key, chunk val) {
	
	assert(key != NULL);
	assert(val != NULL);

	node n;
	int i;
	
	if (dic->node == NULL) {
		n = malloc(sizeof(struct node_struct));
		n->key = sstring_copy(key);
		n->val = chunk_copy(val);
		dic->node = n;
	}
	else {
		i = sstring_compare(dic->node->key, key); // Si 0 egaut,  Si positife, SS2 < ss1, si nega ss1 < ss2 //oh putain les fautes d'orthographe

		if (i > 0) {
			if (dic->right != NULL) {
				dictionary_set(dic->right, key, val); //recursif
			}
			else {
				n = malloc(sizeof(struct node_struct));
				n->key = sstring_copy(key);
				n->val = chunk_copy(val);
				dic->right = dictionary_create();
				dic->right->node = n;
			}
		}
		else if (i < 0) {
			if (dic->left != NULL) {
				dictionary_set(dic->left, key, val); //recursif
			}
			else {
				n = malloc(sizeof(struct node_struct));
				n->key = sstring_copy(key);
				n->val = chunk_copy(val);
				dic->left = dictionary_create();
				dic->left->node = n;
			}
		}
		else {
			chunk_destroy(dic->node->val);
			dic->node->val = chunk_copy(val);
		}
	}
}


/*!
 * Retrieve a \b copied value from a \c dictionary according to a \c key.
 *
 * \param dic \c dictionary to query from
 * \param key key to search a value for
 * \pre no pointer is NULL (assert-ed)
 * \pre key is not empty
 * \return a \b copy of the associated \c chunk or NULL if undefined 
 */

chunk dictionary_get_copy(dictionary dic, sstring key) { 
	
	assert(key != NULL);
	assert( dic != NULL);

	chunk a = NULL, b = NULL;

	if (sstring_compare(dic->node->key, key) == 0) {
		return chunk_copy(dic->node->val);
	}

	if (dic->right != NULL) {
		a = dictionary_get_copy(dic->right, key);
	}

	if (dic->left != NULL) {
		b = dictionary_get_copy(dic->left, key);
	}

	if (a != NULL) {
		return a;
	}
	else if (b != NULL) {
		return b;
	}

	return NULL;
}


/*!
 * Destroy a \c dictionary and released associated resources.
 * All keys and values are destroyed.
 *
 * \param dic \c dictionary to destroy
 * \pre no pointer is NULL (assert-ed)
 */
void dictionary_destroy ( dictionary dic )  {

	assert(dic != NULL);

	if (dic->right != NULL) {
		dictionary_destroy(dic->right);
	}

	if (dic->left != NULL) {
		dictionary_destroy(dic->left);
	}

	sstring_destroy(dic->node->key);
	chunk_destroy(dic->node->val);

	free(dic->node);
	free(dic);

	dic = NULL;
}


/*!
 * Print a \c dictionary to a stream.
 * Entries are printed in \c key alphabetical order.
 * The format is as in the following example with and \c value_integer and a \c value_blok
 \verbatim
 "Bob" => 3
 "Increment_Bob" => {
 Bob
 1
 +
 \Bob
 def
 } \endverbatim
 * \param dic \c dictionary to print
 * \param f stream to print to
 * \pre no pointer is NULL (assert-ed)
 */
void dictionary_print ( dictionary dic , FILE * f )  {
	assert(dic != NULL);

	fprintf(f, "\"");
	sstring_print(dic->node->key, f);
	fprintf(f, "\" => ");
	chunk_print(dic->node->val, f);
	fprintf(f, "\n");

	if (dic->right != NULL) {
		dictionary_print(dic->right, f);
	}
	
	if (dic->left != NULL) {
		dictionary_print(dic->left, f);
	}
}






/*
Donovan FERRE
David INTHO
Louis LABEL
*/

# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <assert.h>

# include "sstring.h"

# undef NDEBUG    // FORCE ASSERT ACTIVATION



/*!
 * \file
 * \brief This module provides a « safer » string.
 * 
 * These are not C-string but a \c struct that holds the length of the string and a pointer to the actual char sequence.
 * Please note that there is no \c '\0' to mark the end of the \c sstring.
 *
 * Empty string is encoded by 0 \c length and \c NULL \c pointer.
 *
 * assert is enforced.
 *
 * \note Cela ressemble au TDM 1, même s'il y a des différences.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


/*! \c sstring is a pointer to a hidden structure. */
struct sstring_struct {
	unsigned int length;
	char *str;
};



/*!
 * Generate an empty \c sstring.
 *
 * \return an empty \c sstring
 */
sstring sstring_create_empty ( void )  {
	sstring s = malloc(sizeof(struct sstring_struct));
	s->str = NULL;
	s->length = 0;

	return s;
}


/*!
 * Generate a \c sstring with the same \c char sequence as a C-string.
 *
 * \param st C-string
 * \pre st is not \c NULL (assert-ed)
 * \return a sstring corresponding to st
 */
sstring sstring_create_string ( char const * const st )  {

	assert(st != NULL);

	sstring s = malloc(sizeof(struct sstring_struct));
	s->length = strlen(st);
	s->str = malloc(s->length * sizeof(char*));
	strcpy(s->str,st);

	return s;
}


/*!
 * Destroy a \c sstring and release related resources.
 *
 * \param ss C-string to destroy
 * \pre ss is a valid \c sstring (assert-ed)
 */
void sstring_destroy ( sstring ss )  {

	assert(ss != NULL);

	if (!sstring_is_empty(ss)) {
		free(ss->str);
	}

	free(ss);

	ss = NULL;
}


/*!
 * Print a \c sstring to a stream.
 * There is no line return (unless it is contained in \c ss).
 *
 * \param ss \c sstring to print
 * \param f stream to print to
 * \pre ss is a valid \c sstring (assert-ed)
 * \pre f is not \c NULL (assert-ed)
 */
void sstring_print ( sstring ss ,
			    FILE * f )  {

	assert(f != NULL);
	assert(ss != NULL);

	fprintf(f, "%s", ss->str);
}

/*!
 * Concatenate a \c sstring at the end of another.
 *
 * \param ss1 \c sstring to be modified
 * \param ss2 \c sstring to concatenate to \c ss1
 * \pre \c ss1 and \c ss2 are valid \c sstring (assert-ed)
 */
void sstring_concatenate ( sstring ss1,
				  sstring ss2 )  {

	assert(ss1 != NULL);
	assert(ss2 != NULL);

	if (!(sstring_is_empty(ss2) && (ss2->str == NULL))) {
		if (sstring_is_empty(ss1)) {
			ss1->str = malloc(ss2->length * sizeof(char*));
			strcpy(ss1->str, ss2->str);
		}
		else {
			strcat(ss1->str, ss2->str);
		}

		ss1->length = strlen(ss1->str);
	}
}

/*!
 * Provide a copy of a string.
 *
 * \param ss \c sstring to copy
 * \pre ss is a valid \c sstring (assert-ed)
 * \return an independant copy of \c ss
 */
sstring sstring_copy ( sstring ss )  {
	
	assert(ss != NULL);

	return sstring_create_string(ss->str);
}


/*!
 * Indicate how two \c sstring are ordered alphabetically.
 *
 * \param ss1 \c sstring 
 * \param ss2 \c sstring 
 * \pre ss1 and ss2 are valid \c sstring (assert-ed)
 * \return 
 * \li 0 if ss1 == ss2
 * \li -1 if ss1 < ss2
 * \li 1 otherwise
 */
int sstring_compare ( sstring ss1 ,
			     sstring ss2 )  {

	assert(ss1 != NULL);
	assert(ss2 != NULL);

	if (sstring_is_empty(ss1) && sstring_is_empty(ss2)) {
		return 0;
	} 
	else if (sstring_is_empty(ss1)) {
		return -1;
	}
	else if (sstring_is_empty(ss2)) {
		return 1;
	}
	else {
		int i = strcmp(ss1->str, ss2->str);

		if (i < 0) {
			return -1;
		}
		else if (i > 0) {
			return 1;
		}

		return 0;
	}
}


/*!
 * Indicate whether a string is empty.
 *
 * This function has no side effect and can be safely used in asserts.
 *
 * \param ss \c sstring to test
 * \pre ss is a valid \c sstring (assert-ed)
 * \return true ssi \c ss is empty
 */
bool sstring_is_empty ( sstring ss )  {

	assert(ss != NULL);

	return ss->length == 0 ? true : false;
}



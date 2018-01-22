/*
Donovan FERRE
David INTHO
Louis LABEL
*/

# include <stdlib.h>
# include <stdio.h>
# include <assert.h>


# include "value_protected_label.h"

# include "macro_value_c.h"

# undef NDEBUG   // FORCE ASSERT ACTIVATION




/*!
 * \file 
 * \brief \c value to hold a constant label protected/inactivated.
 *
 * A label is a letter followed by a limited number of characters among: letters, digits and '_'.
 * A label may not be a keyword (or reserved word of the language).
 *
 * \c value_protected_label are read and printed in the following form: \c '\\' followed by the label.
 * For example: \c \Bob and \c\in_34
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */

typedef struct {
	unsigned int copies_count;
	sstring ss;
} value_protected_label_state_struct,
*value_protected_label_state;



static basic_type value_protected_label_get_value(chunk const ch,
	va_list va) {
	return basic_type_pointer(((value_protected_label_state)ch)->ss);
}


static basic_type value_protected_label_print(chunk const ch,
	va_list va) {
	FILE * f = va_arg(va, FILE *);
	sstring_print(((value_protected_label_state)(ch->state))->ss, f);
	return basic_type_void;
}


static basic_type value_protected_label_destroy(chunk const ch,
	va_list va) {
	value_protected_label_state vpls = (value_protected_label_state)(ch->state);

	if (1 == vpls->copies_count--) {
		sstring_destroy(vpls->ss);
		free(ch->state);
		ch->state = NULL;
		ch->reactions = NULL;
		free(ch);
	}
	return basic_type_void;
}


static basic_type value_protected_label_copy(chunk const ch,
	va_list va) {
	// not commented since it has to be explained
	((value_protected_label_state)(ch->state))->copies_count++;
	return basic_type_pointer(ch);
}


static const message_action value_protected_label_reactions[] = {
	MESSAGE_ACTION__BASIC_VALUE(protected_label) ,
	{ NULL, NULL }
};


chunk value_protected_label_create(sstring const ss) {
	//  Allocation
	chunk ch = (chunk)malloc(sizeof(chunk_struct));
	assert(NULL != ch);
	ch->state = malloc(sizeof(value_protected_label_state));
	assert(NULL != ch->state);
	//  Initialisation
	((value_protected_label_state)(ch->state))->copies_count = 1;
	((value_protected_label_state)(ch->state))->ss = ss;
	ch->reactions = value_protected_label_reactions;
	return ch;
}

//VALUE_DECLARE( protected_label , sstring ) 



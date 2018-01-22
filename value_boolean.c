/*
Donovan FERRE
David INTHO
Louis LABEL
*/

# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "value_boolean.h"

# include "macro_value_c.h"

# undef NDEBUG   // FORCE ASSERT ACTIVATION





/*!
 * \file 
 * \brief \c value used to hold a boolean.
 *
 * For I/O, they appear as keyword \c true and \c false.
 *
 * assert is enforced.
 *
 * \author Jérôme DURAND-LOSE
 * \version 1
 * \date 2015
 * \copyright GNU Public License.
 */


/*!
 * Keyword for boolean constant \c true. 
 * Defined for I/O.
 */
# define VALUE_BOOLEAN_STRING_TRUE "true"


/*!
 * Keyword for boolean constant \c false. 
 * Defined for I/O.
 */
# define VALUE_BOOLEAN_STRING_FALSE "false"


typedef struct {
	unsigned int copies_count;
	bool b;
} value_boolean_state_struct,
*value_boolean_state;



static basic_type value_boolean_get_value(chunk const ch,
	va_list va) {
	return basic_type_boolean(((value_boolean_state)ch)->b);
}


static basic_type value_boolean_print(chunk const ch,
	va_list va) {
	FILE * f = va_arg(va, FILE *);
	fprintf(f
		, "%s"
		, (((value_boolean_state)(ch->state))->b ? VALUE_BOOLEAN_STRING_TRUE : VALUE_BOOLEAN_STRING_FALSE));
	return basic_type_void;
}


static basic_type value_boolean_destroy(chunk const ch,
	va_list va) {
	if (1 == ((value_boolean_state)(ch->state))->copies_count--) {
		free(ch->state);
		ch->state = NULL;
		ch->reactions = NULL;
		free(ch);
	}
	return basic_type_void;
}


static basic_type value_boolean_copy(chunk const ch,
	va_list va) {
	// not commented since it has to be explained
	((value_boolean_state)(ch->state))->copies_count++;
	return basic_type_pointer(ch);
}


static const message_action value_boolean_reactions[] = {
	MESSAGE_ACTION__BASIC_VALUE(boolean) ,
	{ NULL, NULL }
};


chunk value_boolean_create(bool const b) {
	//  Allocation
	chunk ch = (chunk)malloc(sizeof(chunk_struct));
	assert(NULL != ch);
	ch->state = malloc(sizeof(value_boolean_state));
	assert(NULL != ch->state);
	//  Initialisation
	((value_boolean_state)(ch->state))->copies_count = 1;
	((value_boolean_state)(ch->state))->b = b;
	ch->reactions = value_boolean_reactions;
	return ch;
}

//VALUE_DECLARE ( boolean , bool )



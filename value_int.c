/*
Donovan FERRE
David INTHO
Louis LABEL
*/

# include <stdlib.h>
# include <stdio.h>
# include <assert.h>

# include "value_int.h"

# include "macro_value_c.h"


# undef NDEBUG   // FORCE ASSERT ACTIVATION





/*!
 * \file 
 * \brief \c value to hold an \c int.
 * 
 * Integers are read and printed in the usual way:
 * \li '-' sign if negative (nothing for positive) followed by
 * \li sequence of digits.
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
	long long int i;
} value_int_state_struct,
*value_int_state;



static basic_type value_int_get_value(chunk const ch,
	va_list va) {
	return basic_type_long_long_int(((value_int_state)ch)->i);
}


static basic_type value_int_print(chunk const ch,
	va_list va) {
	FILE * f = va_arg(va, FILE *);
	fprintf(f
		, "%lld"
		, ((value_int_state)(ch->state))->i);
	return basic_type_void;
}


static basic_type value_int_destroy(chunk const ch,
	va_list va) {
	if (1 == ((value_int_state)(ch->state))->copies_count--) {
		free(ch->state);
		ch->state = NULL;
		ch->reactions = NULL;
		free(ch);
	}
	return basic_type_void;
}


static basic_type value_int_copy(chunk const ch,
	va_list va) {
	// not commented since it has to be explained
	((value_int_state)(ch->state))->copies_count++;
	return basic_type_pointer(ch);
}


static const message_action value_int_reactions[] = {
	MESSAGE_ACTION__BASIC_VALUE(int) ,
	{ NULL, NULL }
};


chunk value_int_create(long long int const i) {
	//  Allocation
	chunk ch = (chunk)malloc(sizeof(chunk_struct));
	assert(NULL != ch);
	ch->state = malloc(sizeof(value_int_state));
	assert(NULL != ch->state);
	//  Initialisation
	((value_int_state)(ch->state))->copies_count = 1;
	((value_int_state)(ch->state))->i = i;
	ch->reactions = value_int_reactions;
	return ch;
}

//VALUE_DECLARE( int , long long int ) 




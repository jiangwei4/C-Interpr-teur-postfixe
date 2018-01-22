/*
Donovan FERRE
David INTHO
Louis LABEL
*/

# include <stdlib.h>
# include <stdio.h>
# include <assert.h>


# include "value_double.h"

# include "macro_value_c.h"



# undef NDEBUG   // FORCE ASSERT ACTIVATION!_




/*!
 * \file 
 * \brief \c value used to hold a \c double.
 *
 * For I/O these are just numbers with decimal point like 78.0 0.75 -568.58.
 * No exponential form is supported.
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
	long double d;
} value_double_state_struct,
*value_double_state;



static basic_type value_double_get_value(chunk const ch,
	va_list va) {
	return basic_type_long_double(((value_double_state)ch)->d);
}


static basic_type value_double_print(chunk const ch,
	va_list va) {
	FILE * f = va_arg(va, FILE *);
	fprintf(f
		, "%Lf"
		, ((value_double_state)(ch->state))->d);
	return basic_type_void;
}


static basic_type value_double_destroy(chunk const ch,
	va_list va) {
	if (1 == ((value_double_state)(ch->state))->copies_count--) {
		free(ch->state);
		ch->state = NULL;
		ch->reactions = NULL;
		free(ch);
	}
	return basic_type_void;
}


static basic_type value_double_copy(chunk const ch,
	va_list va) {
	// not commented since it has to be explained
	((value_double_state)(ch->state))->copies_count++;
	return basic_type_pointer(ch);
}


static const message_action value_double_reactions[] = {
	MESSAGE_ACTION__BASIC_VALUE(double) ,
	{ NULL, NULL }
};


chunk value_double_create(long double const d) {
	//  Allocation
	chunk ch = (chunk)malloc(sizeof(chunk_struct));
	assert(NULL != ch);
	ch->state = malloc(sizeof(value_double_state));
	assert(NULL != ch->state);
	//  Initialisation
	((value_double_state)(ch->state))->copies_count = 1;
	((value_double_state)(ch->state))->d = d;
	ch->reactions = value_double_reactions;
	return ch;
}

//VALUE_DECLARE( double , long double ) 



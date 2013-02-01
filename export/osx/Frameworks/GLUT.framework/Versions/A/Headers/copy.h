
/*
 *
 * Written By Linas Vepstas November 1991 
 */


#define COPY_THREE_WORDS(A,B) {						\
	struct three_words { int a, b, c, };				\
	*(struct three_words *) (A) = *(struct three_words *) (B);	\
}

#define COPY_FOUR_WORDS(A,B) {						\
	struct four_words { int a, b, c, d, };				\
	*(struct four_words *) (A) = *(struct four_words *) (B);	\
}

/* ============================================================= */

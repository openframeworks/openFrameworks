/*
 * miniunz.h
 *
 *  Created on: 01/05/2011
 *      Author: arturo
 */

#ifndef MINIUNZ_H_
#define MINIUNZ_H_

#include "unzip.h"

int do_extract(
    unzFile uf,
    int opt_extract_without_path,
    int opt_overwrite,
    const char* password);

#endif /* MINIUNZ_H_ */

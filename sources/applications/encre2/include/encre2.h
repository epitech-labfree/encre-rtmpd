#ifndef _ENCRE2_H
# define _ENCRE2_H

#include "common.h"

extern "C" DLLEXP BaseClientApplication *GetApplication_encre2(Variant configuration);
extern "C" DLLEXP void ReleaseApplication_encre2(BaseClientApplication *pApplication);

#endif /* !_ENCRE2_H */

#include "application/baseclientapplication.h"
#include "encre2.h"
#include "encre2application.h"
using namespace app_encre2;

extern "C" BaseClientApplication *GetApplication_encre2(Variant configuration) {
	Encre2Application *pApplication = new Encre2Application(
		configuration);
	return pApplication;
}

extern "C" void ReleaseApplication_encre2(BaseClientApplication *pApplication) {
	if (pApplication != NULL) {
		delete pApplication;
	}
}

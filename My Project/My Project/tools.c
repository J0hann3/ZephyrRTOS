#include "tools.h"

void ftoa(float fnum, char res[5])
{
  float tmpVal  = (fnum < 0) ? -fnum : fnum;

  int   tmpInt1 = tmpVal;            // Get the integer
  float tmpFrac = tmpVal - tmpInt1;  // Get fraction
  int   tmpInt2 = (tmpFrac * 100);   // Turn into integer

  snprintf(res, 5, "%02d.%01d", tmpInt1, tmpInt2);
}
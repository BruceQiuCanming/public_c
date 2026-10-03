#ifndef _DEAL_TEST_MODE_H_
#define _COMM_H_
#include "typedefs.h"

class CDealTestMode 
{
private:

	CDealTestMode();
	virtual ~CDealTestMode();

public:
	CTime m_CurSubModeBeginTime;
	void AuotTestStep_NormalPID(void);


};

#endif
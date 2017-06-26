#ifndef __APNHELPERRK_H
#define __APNHELPERRK_H

#include "Particle.h"

typedef struct {
	const char *prefix;
	const char *apn;
} APNHelperAPN;

class APNHelper {
public:
	APNHelper(const APNHelperAPN * const apns, size_t apnsCount);

	void setCredentials();

	bool getICCID(char *iccidBuf);

	static const size_t ICCID_BUF_SIZE = 24;

protected:
	static int getICCIDCallback(int type, const char* buf, int len, char* iccid);


	const APNHelperAPN * const apns;
	size_t apnsCount;

};



#endif /* __APNHELPERRK_H */

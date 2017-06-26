#include "APNHelperRK.h"

APNHelper::APNHelper(const APNHelperAPN * const apns, size_t apnsCount) : apns(apns), apnsCount(apnsCount) {

}

void APNHelper::setCredentials() {

	// Turn on cellular, just in case it was off (it will
	Cellular.on();

	// Try to get the ICCID. Repeat this a few times because in system threaded mode, turning on cellular
	// is asynchronous and the first few attempts to get the ICCID will fail
	char iccidBuf[ICCID_BUF_SIZE];

	for(size_t tries = 0; tries < 5; tries++) {
		if (getICCID(iccidBuf)) {
			// Got the iccid successfully
			break;
		}

		delay(1000);
	}

	if (iccidBuf[0]) {
		Log.info("got iccid=%s", iccidBuf);

		const char *apn = NULL;

		// Look up the ICCID in the table
		for(size_t ii = 0; ii < apnsCount; ii++) {
			if (strncmp(apns[ii].prefix, iccidBuf, strlen(apns[ii].prefix)) == 0) {
				// Found
				apn = apns[ii].apn;
				break;
			}
		}

		if (apn) {
			Log.info("setting apn=%s", apn);
			cellular_credentials_set(apn, "", "", NULL);
		}
	}

	// If not found, don't set the credentials at all, so the default of
	// { "89340765", "spark.telefonica.com" }
	// will be used.
}


bool APNHelper::getICCID(char *iccidBuf) {

	iccidBuf[0] = 0;

	int res = Cellular.command(getICCIDCallback, iccidBuf, 10000, "AT+CCID\r\n");
	if (res == RESP_OK && iccidBuf[0]) {
		return true;
	}
	else {
		return false;
	}
}

// [static]
int APNHelper::getICCIDCallback(int type, const char* buf, int len, char* iccid) {
	if ((type == TYPE_PLUS) && iccid) {
		const char *start = strstr(buf, "+CCID: ");
		if (start) {
			start += 7; // length of "+CCID: "
			if (start < &buf[len]) {
				const char *end = strchr(start, '\r');
				if ((end != NULL) && (end < &buf[len]) && ((size_t)(end - start) < ICCID_BUF_SIZE)) {
					// Looks valid
					memcpy(iccid, start, end - start);
					iccid[end - start] = 0;
				}
			}
		}
	}
	return WAIT;
}





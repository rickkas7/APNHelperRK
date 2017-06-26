#include "Particle.h"
#include "APNHelperRK.h"

// SEMI_AUTOMATIC or MANUAL mode is required. This won't work using AUTOMATIC mode
SYSTEM_MODE(SEMI_AUTOMATIC);

// Note: Requires system firmware 0.6.0 or later!

// Optional: If you include this information will be printed to debugging serial
SerialLogHandler logHandler;

// Add APNs to this table
const APNHelperAPN apns[1] = {
	// Always begins with 89, then country 01, provider 260 = T-Mobile
	{ "8901260", "fast.t-mobile.com" }

	// You don't need to specify the Particle SIM here. If there isn't otherwise a match, the
	// code will just continue and use the default settings.
};
APNHelper apnHelper(apns, sizeof(apns) / sizeof(apns[0]));

void setup() {
	Serial.begin(9600);

	// Optional: If you don't do this the debugging print statements will probably output before you're
	// able to attach the serial debugging terminal
	// delay(8000);

	// This must be done from setup, you can't do it from STARTUP.
	apnHelper.setCredentials();

	// You can't connect before you setCredentials. If the connection is lost in SEMI_AUTOMATIC mode it
	// will be retried automatically.
	Particle.connect();
}

void loop() {

}

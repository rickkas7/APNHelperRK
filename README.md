# Dynamically setting the Electron APN

When you are using a [third-party SIM card](https://docs.particle.io/faq/particle-devices/electron-3rdparty-sims/electron/) it's necessary to set the APN ("Access Point Name") for your cellular provider.

This is normally done in your code like this:

```
STARTUP(cellular_credentials_set("fast.tmobile.com", "", "", NULL));
```

However, it's also possible to set it dynamically, allowing for different SIMs with only a single firmware binary.

## ICCID format

Fortunately, the ICCID is a formatted number that begins with the Issuer identification number:

> Issuer identification number (IIN)

> Maximum of seven digits:

> Major industry identifier (MII), 2 fixed digits, 89 for telecommunication purposes.
> Country code, 1–3 digits, as defined by ITU-T recommendation E.164.
> Issuer identifier, 1–4 digits.

Particle SIM cards begin with 89340765. T-Mobile SIM cards in the US begin with 8901260, and so on. You can use this to determine which APN to use based on the SIM card that is inserted.

## APNHelperRK

The APNHelperRK library is in the community libraries and also [on Github](https://github.com/rickkas7/APNHelperRK).

The 1-simple-apn example should be straightforward to adapt to any application:

```
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

```

Of particular note:

- You must use `SYSTEM_MODE(SEMI_AUTOMATIC)` or `SYSTEM_MODE(MANUAL)`.
- System firmware 0.6.0 or later is required.
- You can include as many APNs in the table as you want (limited by flash size).
- Make sure you call `apnHelper.setCredentials` from `setup()`. It can't be called from `STARTUP()`. 
- Makes sure you call `apnHelper.setCredentials` before calling `Particle.connect`.




#ifndef Components_componentConfig
#define Components_componentConfig

//! MAX_MIN_TILL_FAIL
//!
//! The number of minutes until startup has failed and the system is restarted
#define MAX_MIN_TILL_FAIL 15

//! STARTUP_MAX_ITER
//!
//! The number of iterations of the startup_handler before MAX_MIN_TILL_FAIL minutes have passed
#define STARTUP_MAX_ITER MAX_MIN_TILL_FAIL*60

//! VOLTAGE_MIN
//!
//! The minimum voltage allowed for the system to run in standard power mode
#define VOLTAGE_MIN 3

//! CURRENT_MIN
//!
//! The minimum current allowed for the system to run in standard power mode
#define CURRENT_MIN .5

//! DEPLOY_WAIT_TIME
//!
//! The number of minutes to wait from system power on until deployment
//!!! WARNING !!! This MUST be 30 or more for actual mission deployment, can be set to lower number for testing purposes
#define DEPLOY_WAIT_TIME 1

//! DEPLOY_WAIT_ITER
//!
//! The number of iterations the startup handler must run through to have waited the full wait time
#define DEPLOY_WAIT_ITER 60*DEPLOY_WAIT_TIME

//! MAX_BACKGROUND_MESSAGES
//!
//! The number of iterations any background events will run
#define MAX_BACKGROUND_MESSAGES 1

//! BEACON_MIN
//!
//! The number of minutes between each beacon
#define BEACON_MIN .1

//! BEACON_INTERVAL
//!
//! The number of seconds between each beacon, used to set rategroup 4
#define BEACON_INTERVAL 60*BEACON_MIN

#define HEARTBEAT_GPIO 25


#endif

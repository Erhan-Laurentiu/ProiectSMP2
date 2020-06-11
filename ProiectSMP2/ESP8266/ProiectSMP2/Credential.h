const char* ssid = "insertName";
const char* password = "insertPassword";
#define BUILTIN_LED 2
#define ORG "74kq3p"
#define DEVICE_TYPE "ESP01"
#define DEVICE_ID "Demo"
#define TOKEN "V5(spyr3Ocv3B&sGbm"
//-------- Customise the above values --------
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

const char eventTopic[] = "iot-2/evt/status/fmt/json";
const char cmdTopic[] = "iot-2/cmd/led/fmt/json";

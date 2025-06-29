#include <pgmspace.h>

#define SECRET
#define THINGNAME "esp32_3"                         //change this
 
const char WIFI_SSID[] = "pop-os";               //change this
const char WIFI_PASSWORD[] = "111222333";           //change this
const char AWS_IOT_ENDPOINT[] = "a2xpqfvfr7pva7-ats.iot.ap-southeast-2.amazonaws.com";       //change this
 
// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
 
// Device Certificate                                               //change this
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUd0032ypzPJ0EqRop2lthMwgLaokwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI1MDYxMTA1MTIw
NloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANa8/CWZjBys6oVJaip8
lbBkOYcV2ZbiMebH/adYWAl9I2b5rjBvh7kjtkPZm9cBFsACtAZ82Pd2I5Pxdlve
ep61jNOSYubQZIrq+cJ/+ghFlDmtr1/Kz+q/bEnInDpLaDcR6i/EZuAeGdhJFtrO
rOiT4JY9x7Ip7KLAPr9OcV+8MEFXuxKCp+DcEOO6WiliFL3Zjq4ImcBQWjoctlkR
lQSgnH6dW80XD0nsNXMkqiVOF371THammKskbn61mdVXS1R7CVDCMX6AurbXwdya
rlic4GxUkK5XiD8tpk9Fp9U8FqND0utuDkHAkjTpxm0I4GXQu2AyC4Km5eD6MFVs
PxUCAwEAAaNgMF4wHwYDVR0jBBgwFoAUfj/Esc2UVk+B8lxVrOEEJHohlKUwHQYD
VR0OBBYEFD2zKDThH7jCZRp8tJD4mbxtbCVXMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQB8FEzzhN1Qul+GrzEUZIjbTpF3
j8+auDDjEyQBzeHw8b03x6tk+u5Wm3pny+1d9TeICMuMTckFekFRKMmqcPYmWZ1U
Zt3/OIvf7vZPvbxnNl0wPj4lgXObIfA28YLs+pA/WAdphEMJMNxFPIAEtu/HmzU5
/GfCv+84MSe3Hig3tr3HAPjBcwfyPHmYI9azVKwoWJv1M28H9eRHXdvtcTPKhtpE
vmWoZS7Fcz/RSZ5Yc9XXuFMgF3RbQxK1epE4uCW6zyorv30rWh2PINWSrS7KjwQF
IWFo2cF+//MQOclAGpT1XtSBqA/RZo6ewlqKp46e2GlI1nC6zUXnJxV+lHci
-----END CERTIFICATE----- 
)KEY";
 
// Device Private Key                                               //change this
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEA1rz8JZmMHKzqhUlqKnyVsGQ5hxXZluIx5sf9p1hYCX0jZvmu
MG+HuSO2Q9mb1wEWwAK0BnzY93Yjk/F2W956nrWM05Ji5tBkiur5wn/6CEWUOa2v
X8rP6r9sScicOktoNxHqL8Rm4B4Z2EkW2s6s6JPglj3HsinsosA+v05xX7wwQVe7
EoKn4NwQ47paKWIUvdmOrgiZwFBaOhy2WRGVBKCcfp1bzRcPSew1cySqJU4XfvVM
dqaYqyRufrWZ1VdLVHsJUMIxfoC6ttfB3JquWJzgbFSQrleIPy2mT0Wn1TwWo0PS
624OQcCSNOnGbQjgZdC7YDILgqbl4PowVWw/FQIDAQABAoIBADo0l5QJfrT3Gcpv
D9vcUSayw/UdsxD21tfbNaH6jEJmLQZofxKZ6IwCZhEWFuu8W9JH6Uwo4xaQayxa
st5L+6ixRS7D6xLtQZN2BaT202sSkQ7hSTwJ2TmkEVIekloqvmDO9k9kSQ5OmmrJ
xOwiQTP5gVK4BD0CqCWlPlzUgl6UQFqsoDZEumkQKw+LE7W/FU3OhjfgeaJexW1Y
Ilqk55lckGwPsethlVLbMMYzOeIFBMZG+B8U4Y5kMw1U4iayswagAosS8VFlTZVL
tFiVHMslpCFAtmtFtaWZOf42sWdDDApDgsoWeelbZnBi1kHeh/FTdp8vyMWHrhcc
CUgN6AECgYEA/7RjG86AJguTPlvfP6BZmOuVb5OG5b+8QoBEKCQ674DTmPEs9YmU
TCX7tnts8s0m0O8fpnk7TbDmN9Ei5JO7gcWnxnYakGjhQEwR/Fn960xyfk1xghKz
upKE+J+svN+/Fsr6+hI0ivb3hOrZiNIceDiTKDNIEawsxjnMmyDzgIECgYEA1vx7
32Hp8NS6fvM3k5PFvq0lOT1Xxg/KdFv6VvFY5CRWwkoqgW17uC57IwEdHKptnpht
ZGsaPDsgKO/ABjIIP/Dkiaes7UktW0BimVmp4pNOpvbekMUVo25NuJa+GqkaQ525
WrLuK9j50n6stTUrVC0Tfrsld0DwtEdIyRV4dJUCgYEA2BlA8Q0RrBV4OOk3wPPp
BJ2ESylohd4GYkU4VJxZ2nMLYGPiVRgSZlBrbArfRksulMCiBPTLs5re1Sk/zeE0
RDYg1k6VoJmiU852HMiJGUQgNrGITh80GWceTWfUekK1obOla5XbJd4VzRUVBtZb
DRby9rRwIc9af6P1UN0GLQECgYARDgCJrQvQ9h9BWOwMwglK4hN04f/nj3MhHfbT
FNxXZOaZDScoE1ZgOxe7pCf6ES8XCMKM185B/vLDLJnQJAHxiJuoq+uU+1wkxAvO
XIWUmtj/d9AaLfhUebiirxqvpMCXopiEsHMieVkAMNpkma4xRE4qnJARUM6Hjqwg
f5qJrQKBgQDhnzNJlmUTHbJTcdc29qVxu70bLkUZPbVd7bNco221j2bpDvzRI/Wy
eNV/9VVcPT/esLdM4fT37+9kKLm/1TusiOE4ZsKhQRCAbp48VxWdeBjFdt/CfMD5
JXAe44VsxYU28WGZHj3VBdn9nJ/sTd+T72g38auRN9QPYTuhIZgtnA==
-----END RSA PRIVATE KEY-----
)KEY";

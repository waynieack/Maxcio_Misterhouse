# Maxcio_Misterhouse
Maxcio Wifi swich Misterhouse Sketch

This sketch can be used on the ESP8266 based Maxcio smart wifi switch. 

Build Notes:

http://www.hagensieker.com/blog/page/?post_id=44&title=esp8266-hack-of-inexpensive-wifi-outlet

Tools menu:

Flash Size = 1M (128k SPIFFS)

IwIP Variant = 1.4 Prebuilt




Sample switch Homie config:
Note: base_topic and device_id must match MH user code

```
{
  "name": "OfficeCloset",
  "device_id": "officecloset",
  "wifi": {
    "ssid": "YourSSID",
    "password": "Your Wifi Password"
  },
  "mqtt": {
    "host": "IP of machine where mosquitto MQTT broker is installed",
    "port": 1883,
    "base_topic": "homie/"
  },
  "ota": {
    "enabled": true
  },
  "settings": {
    "percentage": 55
  }
}
```

Install MQTT broker (can be on any machine):
```
apt-get install mosquitto mosquitto-clients
```

Install Misterhouse MQTT client requirements:
```
cpan install Net::MQTT::Constants
```


Test from CLI (once the switch is connected):
```
mosquitto_pub -d -h 127.0.0.1 -q 0 -t homie/officecloset/switch/on/set -m "on"
```


Misterhouse code:
```
#noloop=start
require mqtt;

$mqttSubscribe = new mqtt('mqtt', '192.168.195.37', 1883, 'homie/#', "", "", 121);
$Office_Closet_Light = new mqtt_Item($mqttSubscribe, "homie/officecloset/switch/on/set");
$Office_Closet_Light ->set_states("on","off");

#noloop=stop
```



# Determalizator
Timer for save ending flight of free flight glider

## How it works
Pilot is going to press button -> LED indicator the time is running -> flight could start -> after time period, servo moves (kicks), unlock elevator and it move up. Soft landing, end of flight.

## Implementation
The most important parameter is weight of device, glider kit contains 30g balast for balancing. So the idea is limit device weight **<20g**.

| Komponenta                    | Hmotnost |
|------------------------------|----------|
| Microservo SG90              | 9 g      |
| ATtiny85                     | 1 g      |
| LiPol baterie 502020 150mAh  | 4 g      |
| JST konektor + vodiče        | 1 g      |
| (Microswitch   | 0.5 g    |
| LED   | 0.5 g    |


# ESP32 Master SPI Driver
| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-P4 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |

Once connected and flashed, the spi master and spi slave will communicate with each other. This includes a loopback test case that sends a configurable sized buffer and verify correct RX/TX of slave response.




### Connection
For different chip and host used, the connections may be different. Here show a example diagram of hardware connection, you can freely change the GPIO settings by editing defines in the top of `main/app_main.c` in the master/slave source code. and change the hardware relatively.

The default GPIOs used are the following:

<table>
<tr align="middle">
<th bgcolor=#ffffcc>Signal</th>  <td>MOSI</td> <td>MISO</td> <td>SCLK</td> <td>CS</td>
</tr>
<tr align="middle">
<th bgcolor=#ffffcc>Pin</th> <td>GPIO12</td> <td>GPIO13</td> <td>GPIO15</td> <td>GPIO14</td>
</tr>
</table>

## To Configure

```idf.py menuconfig```

*Select SPI Master Configuration*

Change settings as needed. 
<img width="663" alt="image" src="https://github.com/riverdale-soc/sender/assets/68623356/29cced12-5cbb-4ab5-ac8e-845c4469486a">

*Save changes*

```idf.py build flash monitor```

## Default Configurations
* CPOL = 0
* CPHA = 0
* Speed = 5Mhz
* Test = Loopback


## Setup on Host PC
Enter rocket home
```
cd esp/esp-idf
. ./export.sh
```
![image](https://github.com/riverdale-soc/sender/assets/68623356/abcd5f1d-62a4-45fc-888b-ffed544a2bd6)


```
cd projects/sender
idf.py build ### TO BUILD THE PROJECT
idf.py menuconfig ### TO CONFIGURE PROJECT
idf.py build flash monitor ### TO COMPILE FLASH AND MONITOR SERIAL CONSOLE OF TARGET
idf.py -h ### FOR HELP ON IDF.PY UTILITY
```




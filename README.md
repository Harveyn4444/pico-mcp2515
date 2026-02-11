Raspberry Pi Pico MCP2515 CAN interface library
---------------------------------------------------------
This Raspberry Pi Pico library is a ported & optimized version of the magnificent library [arduino-mcp2515](https://github.com/autowp/arduino-mcp2515) by [autowp](https://github.com/autowp).

This repo builds upon the work by [Piotr Adamczyk](https://github.com/adamczykpiotr) to include example code for the [Waveshare CAN bus Module (B)](https://www.waveshare.com/pico-can-b.htm). 

<p align="center">
<img src="https://github.com/Harveyn4444/pico-mcp2515/blob/main/assets/img/Pico-can-b_Pinout.jpg" width = 40% height = 40%>
</p>


## Building the Code
The code is written for the Raspberry Pi [Pico C SDK](https://www.raspberrypi.com/documentation/pico-sdk/). To build and upload the code follow the steps below.
Assuming the correct `.cpp` in `CMakeLists.txt` is selected.

```
cp ~/pico-sdk/external/pico_sdk_import.cmake . //Make sure this path is correct
mkdir build
cd build
export PICO_SDK_PATH="CORRECT PATH"
cmake ..
make

```
Once compiled to a `.uf2` file:
```
cp PROJECT_NAME.uf2 /"MOUNTPATH"/RPI-RP2/
```




----

This software is written by loovee ([luweicong@seeed.cc](luweicong@seeed.cc "luweicong@seeed.cc")) for seeed studio, updated by Dmitry ([https://github.com/autowp](https://github.com/autowp "https://github.com/autowp")) and then ported by Piotr Adamczyk [https://github.com/adamczykpiotr](https://github.com/adamczykpiotr) and is licensed under [The MIT License](http://opensource.org/licenses/mit-license.php). Check [LICENSE.md](LICENSE.md) for more information.

Contributing to this software is warmly welcomed. You can do this basically by [forking](https://help.github.com/articles/fork-a-repo), committing modifications and then [pulling requests](https://help.github.com/articles/using-pull-requests) (follow the links above for operating guide). Adding change log and your contact into file header is encouraged. Thanks for your contribution.

Seeed Studio is an open hardware facilitation company based in Shenzhen, China. Benefiting from local manufacture power and convenient global logistic system,
we integrate resources to serve new era of innovation. Seeed also works with global distributors and partners to push open hardware movement

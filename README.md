# CanSat Probe firmware base 


##### How to use this project? 
1. Clone repository.
2. Enter userspace: `cd User`.
3. Init submodules: `git submodule init`.
4. Download userspace submodule: `git submodule update`.
5. Checkout on develop branch: `git checkout develop`.
6. Get newest version of userspace repository: `git pull`.


#### I want to make changes in cube project, how to do that? 
Create issue in this project. Project supervisor will resolve your problem. 

##### Userspace structure
Project is preconfigured  to use userspace structure like this: 

``` 
User
├── Application
│   ├── Inc
│   │   └── usermain.h
│   └── Src
│       └── usermain.cpp
├── Drivers
│   ├── INA219
│   │   ├── ina219.c
│   │   └── ina219.h
│   ├── SX1278
│   │   ├── afsk1200.cpp
│   │   ├── afsk1200.h
│   │   ├── sx1278.cpp
│   │   └── sx1278.h
│   └── W25Qxxx
│       ├── w25qxxx.c
│       └── w25qxxx.h
└── Tools
    ├── APRS
    │   ├── aprs.cpp
    │   └── aprs.h
    └── NMEA
        ├── nmea.cpp
        └── nmea.h
```

Unfortunately, to include tools or drivers files, 'include' directive have to contain part of path, for example to include driver for W25Q128 SPI flash in usermain, You should write `#include <W25Qxxx/w25qxxx.h>`. 

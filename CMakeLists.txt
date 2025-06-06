cmake_minimum_required(VERSION 3.27)
project(vsratostat)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(lib)
include_directories(src)
include_directories(src/config)
include_directories(src/ha)
include_directories(src/ha/entity)
include_directories(src/ha/enum)
include_directories(src/mqtt)
include_directories(src/thermostat)
include_directories(src/thermostat/mqtt/consumer)
include_directories(src/thermostat/mqtt/entity)
include_directories(src/thermostat/mqtt/producer)
include_directories(src/utils)
include_directories(src/web)
include_directories(src/wifi)

add_executable(vsratostat
        include/README
        lib/README
        src/config/Config.h
        src/config/ConfigMgr.cpp
        src/config/ConfigMgr.h
        src/ha/entity/Base.cpp
        src/ha/entity/Base.h
        src/ha/entity/BinarySensor.cpp
        src/ha/entity/BinarySensor.h
        src/ha/entity/Button.cpp
        src/ha/entity/Button.h
        src/ha/entity/Climate.cpp
        src/ha/entity/Climate.h
        src/ha/entity/Device.cpp
        src/ha/entity/Device.h
        src/ha/enum/DeviceClass.h
        src/ha/enum/EntityCategory.h
        src/ha/DiscoveryMgr.cpp
        src/ha/DiscoveryMgr.h
        src/mqtt/Consumer.h
        src/mqtt/MQTT.cpp
        src/mqtt/MQTT.h
        src/mqtt/Producer.h
        src/thermostat/mqtt/consumer/CommandConsumer.cpp
        src/thermostat/mqtt/consumer/CommandConsumer.h
        src/thermostat/mqtt/entity/CommandEntity.cpp
        src/thermostat/mqtt/entity/CommandEntity.h
        src/thermostat/mqtt/entity/StateEntity.cpp
        src/thermostat/mqtt/entity/StateEntity.h
        src/thermostat/mqtt/producer/StateProducer.cpp
        src/thermostat/mqtt/producer/StateProducer.h
        src/thermostat/Boiler.cpp
        src/thermostat/Boiler.h
        src/thermostat/BoilerState.cpp
        src/thermostat/BoilerState.h
        src/utils/String.h
        src/utils/Utils.cpp
        src/utils/Utils.h
        src/web/Handler.cpp
        src/web/Handler.h
        src/wifi/WiFiMgr.cpp
        src/wifi/WiFiMgr.h
        src/defines.h
        src/main.cpp)

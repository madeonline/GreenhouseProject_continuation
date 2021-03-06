#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// НАСТРОЙКИ ДЕБАГ-РЕЖИМОВ КОМПИЛЯЦИИ
// COMPILE-TIME DEBUG MODES (uncomment any line you wish, if needed)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------------------------------
// режим отладки Wi-Fi-модуля (КОНФИГУРАТОР НЕ ЗАПУСКАТЬ, ТОЛЬКО МОНИТОР ПОРТА!)
// Wi-Fi module debug mode (Don't use configuration software, only Port Monitor!)
//#define WIFI_DEBUG
//--------------------------------------------------------------------------------------------------------------------------------
// режим отладки GSM-модуля (КОНФИГУРАТОР НЕ ЗАПУСКАТЬ, ТОЛЬКО МОНИТОР ПОРТА!)
// GSM module debug mode (Don't use configuration software, only Port Monitor!)
//#define GSM_DEBUG_MODE 
//--------------------------------------------------------------------------------------------------------------------------------
// режим отладки модуля логгирования информации (КОНФИГУРАТОР НЕ ЗАПУСКАТЬ, ТОЛЬКО МОНИТОР ПОРТА!)
// LOG module debug mode (Don't use configuration software, only Port Monitor!)
//#define LOGGING_DEBUG_MODE
//--------------------------------------------------------------------------------------------------------------------------------
// отладочный режим модуля полива (КОНФИГУРАТОР НЕ ЗАПУСКАТЬ, ТОЛЬКО МОНИТОР ПОРТА!)
// Watering module debug mode (Don't use configuration software, only Port Monitor!)
//#define WATER_DEBUG 
//--------------------------------------------------------------------------------------------------------------------------------
// отладочный режим nRF, (КОНФИГУРАТОР НЕ ЗАПУСКАТЬ, ТОЛЬКО МОНИТОР ПОРТА!)
// nRF debug mode (Don't use configuration software, only Port Monitor!)
//#define NRF_DEBUG
//--------------------------------------------------------------------------------------------------------------------------------
// отладочный режим LoRa, (КОНФИГУРАТОР НЕ ЗАПУСКАТЬ, ТОЛЬКО МОНИТОР ПОРТА!)
// LoRa debug mode (Don't use configuration software, only Port Monitor!)
//#define LORA_DEBUG
//--------------------------------------------------------------------------------------------------------------------------------
// отладочный режим RS-485, (КОНФИГУРАТОР НЕ ЗАПУСКАТЬ, ТОЛЬКО МОНИТОР ПОРТА!)
// RS-485 debug mode (Don't use configuration software, only Port Monitor!)
//#define RS485_DEBUG 
//--------------------------------------------------------------------------------------------------------------------------------
// отладочный режим универсальных модулей (КОНФИГУРАТОР НЕ ЗАПУСКАТЬ, ТОЛЬКО МОНИТОР ПОРТА!)
// Universal modules debug mode (Don't use configuration software, only Port Monitor!)
//#define UNI_DEBUG 
//--------------------------------------------------------------------------------------------------------------------------------
// Режим отладки модуля pH (КОНФИГУРАТОР НЕ ЗАПУСКАТЬ, ТОЛЬКО МОНИТОР ПОРТА!)
// PH module debug mode (Don't use configuration software, only Port Monitor!)
//#define PH_DEBUG
//--------------------------------------------------------------------------------------------------------------------------------
// Режим отладки модуля IOT (КОНФИГУРАТОР НЕ ЗАПУСКАТЬ, ТОЛЬКО МОНИТОР ПОРТА!)
// IOT module debug mode (Don't use configuration software, only Port Monitor!)
//#define IOT_DEBUG
//--------------------------------------------------------------------------------------------------------------------------------
// отладочный режим провайдеров HTTP-запросов, как правило - надо включать совместно с WIFI_DEBUG (КОНФИГУРАТОР НЕ ЗАПУСКАТЬ, ТОЛЬКО МОНИТОР ПОРТА!)
// HTTP provider debug mode (Don't use configuration software, only Port Monitor!). WIFI_DEBUG SHOULD BE defined with this mode!
//#define HTTP_DEBUG 
//--------------------------------------------------------------------------------------------------------------------------------
// отладочный режим MQTT (КОНФИГУРАТОР НЕ ЗАПУСКАТЬ, ТОЛЬКО МОНИТОР ПОРТА!)
// MQTT debug mode (Don't use configuration software, only Port Monitor!)
//#define MQTT_DEBUG
//--------------------------------------------------------------------------------------------------------------------------------
// отладочный режим старта контроллера - ищем проблемы старта на STM32
//#define STM32_START_DEBUG
//--------------------------------------------------------------------------------------------------------------------------------
// отладочный режим модуля сценариев
//#define SCENE_DEBUG
//--------------------------------------------------------------------------------------------------------------------------------
//#define WINDOW_MANAGE_DEBUG // закомментировать для выключения отладочной информации по управлению окнами
//--------------------------------------------------------------------------------------------------------------------------------
//#define HEAT_MODULE_DEBUG // закомментировать для выключения отладочной информации по отоплению
//--------------------------------------------------------------------------------------------------------------------------------
//#define SHADOW_MODULE_DEBUG // закомментировать для выключения отладочной информации по затенению
//--------------------------------------------------------------------------------------------------------------------------------
//#define DEBUG_MEASURE_UPDATE_TIME // закомментировать для выключения проверки быстродействия (выводит в Serial)
//--------------------------------------------------------------------------------------------------------------------------------
//#define CYCLE_VENT_DEBUG // отладочный режим модуля циклической вентиляции
//--------------------------------------------------------------------------------------------------------------------------------
//#define VENT_DEBUG // отладочный режим модуля вентиляции по температуре
//--------------------------------------------------------------------------------------------------------------------------------
//#define DS18B20_DEBUG // отладочный режим опроса DS18B20
//--------------------------------------------------------------------------------------------------------------------------------
//#define THERMOSTAT_DEBUG // отладочный режим модуля термостатов
//--------------------------------------------------------------------------------------------------------------------------------
// #define POWER_DEBUG // отладочный режим управления питанием

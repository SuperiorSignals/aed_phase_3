#pragma once
#ifndef TESTFUNCTIONTWO_H_
#define TESTFUNCTIONTWO_H_

#include "gpsdata.h"

/*
double clock_test(int duration); // Return clock calibration constant
void gpio_set(); // Set device IO
unsigned char gpio_test(); // Return GPIO values
void indicators(bool isGpsValid, bool isCellConnected); // Set indicator lights
void report_shared(GpsData gpsData); // Log GPS data for display
void test_menu(int selection); // Allow selection of test functions

void test_function_1(); // Test function for startup
void test_function_2(); // Test IO expander GPIO

//	Device test functions

void test_function_3(); // Test device GPS
void test_function_4();
void test_function_5();
void test_function_6(); // Set device IO
void test_function_7(); // Test device mesh
void test_function_8(); // Test GPS
void test_function_9(); // Test DataPacket class
void test_function_10(); // Test maintenance routine
void test_function_11(); // Test micropython
void test_function_12(); // Set clock constant
void test_function_13(); // Build up standard routine
void test_function_14(); // Test ability to  send text message
void test_function_15();
void test_function_16(); // Test digital outputs
void test_function_17(); // Test mesh functions
void test_function_18(); // Test new mesh functions
void test_function_19(); // Test generation of API AT packets
void test_function_20(); // Test address and port settings
void test_function_21(); // Run arbitrary AT command by API

void test_function_22(); // Test API
void test_function_23(); // Test XBee class
void test_function_24(); // Determine true clock ticks per second
void test_function_25(); // Test GPS output
void test_function_26(); // Test DataPacket class
void test_function_27(); // Test TCP
void test_function_28(); // Test GPIO
void test_function_29(); // Test GPIO
void test_function_30b(); // Test GPS/GPIO output
void test_function_30a(); // Test GPS/GPIO input
void test_function_31(); // Test IO on ttymxc1
void test_function_32(); // Test IO on ttymxc2
void test_function_33(); // Test digital inputs
void test_function_34(); // Test DigiMesh
void test_function_35(); // Test DigiMesh input
void test_function_36(); // Test packet storage
void test_function_37(); // Test maintenance routine
void test_function_38(); // Test duplicate removal
void test_function_39(); // Test ADC
void test_function_40(); // Test standard routine on SBC Pro
*/
void test_function_41(); // Test Micropython
void test_function_42(); // Mesh externalized to test_function_43
void test_function_43(std::vector<char> input); // Mesh packet transmit function
void test_function_44(); // Standard routine variant
void test_function_45(); // Standard routine variant
void test_function_46(); // Functional mesh, standard routine
void test_function_47(); // Test transmit status
void test_function_48(); // Last functional test_function_46()
void test_function_49();
void test_function_50();
void test_function_51();
void test_function_52();
void test_function_53();
void test_function_54();
void test_function_55();
void test_function_56();
void test_function_57();
void test_function_58();
void test_function_59();
void test_function_60();
void test_function_61();
void test_function_62();
void test_function_63();
void test_function_64();
void test_function_65();
void test_function_66();
void test_function_67();
void test_function_68();
void test_function_69();
void test_function_70();

#endif // !TESTFUNCTIONTWO_H_


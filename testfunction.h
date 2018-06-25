#pragma once
#ifndef TESTFUNCTION_H_
#define TESTFUNCTION_H_

#include "gpsdata.h"

double clock_test(int duration); // Return clock calibration constant
void gpio_set(); // Set device IO
unsigned char gpio_test(); // Return GPIO values
void indicators(bool isGpsValid, bool isCellConnected); // Set indicator lights
void report_shared(GpsData gpsData); // Log GPS data for display
void test_menu(int selection); // Allow selection of test functions
void test_function_1();	// Test Micropython
void test_function_2();	// Test API
void test_function_3();	// Test XBee class
void test_function_4(); // Determine true clock ticks per second
void test_function_5(); // Test GPS output
void test_function_6(); // Test DataPacket class
void test_function_7(); // Test TCP
void test_function_8(); // Test GPIO
void test_function_9(); // Test GPIO
void test_function_10b(); // Test GPS/GPIO output
void test_function_10a(); // Test GPS/GPIO input
void test_function_11(); // Test IO on ttymxc1
void test_function_12(); // Test IO on ttymxc2
void test_function_13(); // Test digital inputs
void test_function_14(); // Test DigiMesh
void test_function_15(); // Test DigiMesh input
void test_function_16(); // Test packet storage
void test_function_17(); // Test maintenance routine
void test_function_18(); // Test duplicate removal
void test_function_19(); // Test ADC
void test_function_20(); // Test standard routine on SBC Pro
void test_function_21(); // Test function for startup
void test_function_22(); // Test IO expander GPIO

/*
Device test functions
*/

void test_function_23(); // Test device GPS
void test_function_24(); // Test device GPIO
void test_function_25(); // Test device packet output
void test_function_26(); // Set device IO
void test_function_27(); // Test device mesh
void test_function_28(); // Test GPS
void test_function_29(); // Test DataPacket class
void test_function_30(); // Test maintenance routine
void test_function_31(); // Test micropython
void test_function_32(); // Set clock constant
void test_function_33(); // Build up standard routine
void test_function_34(); // Test ability to  send text message
void test_function_35(); // Test different ADC channels
void test_function_36(); // Test digital outputs
void test_function_37(); // Test mesh functions
void test_function_38(); // Test new mesh functions
void test_function_39();
void test_function_40();
#endif // !TESTFUNCTION_H_


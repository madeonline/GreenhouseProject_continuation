#include "ConfigPin.h"
#include <Arduino.h>
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ConfigPin::ConfigPin()
{
  
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ConfigPin::setup()
{
	pinMode(led_ready, OUTPUT);                         // ��������� ����������� "�����"
	digitalWrite(led_ready, LOW);                       // ��������� ����������� "�����"
	pinMode(led_failure, OUTPUT);                       // ��������� ����������� "������"
	digitalWrite(led_failure, LOW);                     // ��������� ����������� "������"
	pinMode(led_test, OUTPUT);                          // ��������� ����������� "����"
	digitalWrite(led_test, LOW);                        // ��������� ����������� "����"

	pinMode(button1, INPUT);                            // ������ �1
	digitalWrite(button1, INPUT_PULLUP);                // ������ �1  ���������� ���������
	pinMode(button2, INPUT);                            // ������ �2
	digitalWrite(button2, INPUT_PULLUP);                // ������ �2  ���������� ���������
	pinMode(button3, INPUT);                            // ������ �3
	digitalWrite(button3, INPUT_PULLUP);                // ������ �3  ���������� ���������
	pinMode(button4, INPUT);                            // ������ �4
	digitalWrite(button4, INPUT_PULLUP);                // ������ �4  ���������� ���������

	pinMode(relay_protection1, INPUT);                  // ���� "�������� ������ N1"
	digitalWrite(relay_protection1, INPUT_PULLUP);      // ���� "�������� ������ N1", ���������� ���������
	pinMode(relay_protection2, INPUT);                  // ���� "�������� ������ N2"
	digitalWrite(relay_protection2, INPUT_PULLUP);      // ���� "�������� ������ N2", ���������� ���������

	pinMode(inductive_sensor1, INPUT);                  // ���� ������������ ������� �1
	digitalWrite(inductive_sensor1, INPUT_PULLUP);      // ���� ������������ ������� �1, ���������� ���������
	pinMode(inductive_sensor2, INPUT);                  // ���� ������������ ������� �2
	digitalWrite(inductive_sensor2, INPUT_PULLUP);      // ���� ������������ ������� �2, ���������� ���������
	pinMode(inductive_sensor3, INPUT);                  // ���� ������������ ������� �3
	digitalWrite(inductive_sensor3, INPUT_PULLUP);      // ���� ������������ ������� �3, ���������� ���������

	pinMode(out_asu_tp1, OUTPUT);                       // ����� �� ��� �� �1
	digitalWrite(out_asu_tp1, LOW);                     // ����� �� ��� �� �1
	pinMode(out_asu_tp2, OUTPUT);                       // ����� �� ��� �� �2
	digitalWrite(out_asu_tp2, LOW);                     // ����� �� ��� �� �2
	pinMode(out_asu_tp3, OUTPUT);                       // ����� �� ��� �� �3
	digitalWrite(out_asu_tp3, LOW);                     // ����� �� ��� �� �3
	pinMode(out_asu_tp4, OUTPUT);                       // ����� �� ��� �� �4
	digitalWrite(out_asu_tp4, LOW);                     // ����� �� ��� �� �4


	pinMode(Upr_RS485, OUTPUT);                         // ����� ����������� ����� RS485  
	digitalWrite(Upr_RS485, LOW);                       // ����� ����������� ����� RS485  

														// CAN ����
	pinMode(ID0_Out, OUTPUT);                           // ����� ����������� CAN �����   
	digitalWrite(ID0_Out, LOW);                         // ����� ����������� CAN �����   
	pinMode(ID1_Out, OUTPUT);                           // ����� ����������� CAN �����   
	digitalWrite(ID1_Out, LOW);                         // ����� ����������� CAN �����   

	pinMode(IDE0_In, INPUT);                            // ���� ����������� CAN �����   
	digitalWrite(IDE0_In, INPUT_PULLUP);                // ���� ����������� CAN �����, ���������� ���������
	pinMode(IDE1_In, INPUT);                            // ���� ����������� CAN �����   
	digitalWrite(IDE1_In, INPUT_PULLUP);                // ���� ����������� CAN �����, ���������� ���������

	pinMode(PPS_Out, OUTPUT);                           // ����� ����������� CAN �����   
	digitalWrite(PPS_Out, LOW);                         // ����� ����������� CAN �����   
 
}
//void ConfigPin::update()
//{
//
//  
//}

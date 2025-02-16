/*
 * def.h
 *
 *  Created on: 25 янв. 2022 г.
 *      Author: shamilov
 */
/*
#ifdef __cplusplus
extern "C" {
#endif
*/

#ifndef INC_DEF_H_
#define INC_DEF_H_

#include <string>
#include "stm32f1xx_hal.h"

#define DEBUG 1
//#define otl 1

#define DWT_CYCCNT  *(volatile uint32_t *)0xE0001004
#define DWT_CONTROL *(volatile unsigned long *)0xE0001000
#define SCB_DEMCR   *(volatile unsigned long *)0xE000EDFC

#define Pause	HAL_Delay		//Так удобнее
#define TIMEOUT 10				//таймаут (Сек.)
#define STEP_TO_CUT 7450		//количество шагов ШД до отрезки

/*битовые операции*/
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)											//Читает бит под номером bit в числе value
#define bitSet(value, bit) ((value) |= (1UL << (bit)))											//Включает (ставит 1) бит под номером bit в числе value
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))										//Выключает (ставит 0) бит под номером bit в числе value
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))	//Ставит бит под номером bit в состояние bitvalue (0 или 1) в числе value
#define bit(b) (1UL << (b))								`										//Возвращает 2 в степени bit

/*объявление типа данных exit_error*/
typedef struct _exit_error{
	uint8_t fl_er;					//флаг завершения с ошибкой
	char mtk;						//имя метки где остановилась программа(здесь нужен массив символов)
}exit_error;

/*имена элементов массива*/
#define count_magn 				*(uint16_t*)buffer_i2c[0]
#define current_shtamp_close 	*(uint16_t*)buffer_i2c[2]
#define steps_to_cut 			*(uint16_t*)buffer_i2c[4]
#define timeout_wait_foto 		*(uint16_t*)buffer_i2c[6]
#define timeout_wait_magn		*(uint16_t*)buffer_i2c[8]
#define timeout_stamp 			*(uint16_t*)buffer_i2c[10]
#define timeout_sm_to_cut 		*(uint16_t*)buffer_i2c[12]
#define timeout_sm_to_sht 		*(uint16_t*)buffer_i2c[14]
#define pulse_pwm				*(uint16_t*)buffer_i2c[16]
#define voltage_pwr				*(uint16_t*)buffer_i2c[18]

extern I2C_HandleTypeDef hi2c1;
//флаги
extern uint8_t fl_er;			//флаг выхода из программы с ошибкой
extern uint8_t flag_stop;		//флаг остановки двигателя
extern uint8_t err_tm;			//флаг ошибки по таймауту
extern uint8_t fl_rx;			//флаг принятой команды

extern char mtk[8];				// метка программы
extern uint8_t input_UR;		//неактивное состояние датчиков "1" (подтяжка к питанию)
extern uint8_t input_pult;		//и кнопок
extern char usb_buf_tx[21];		//буфер для передачи
extern int param[5];			//буфер с параметрами команды

extern uint8_t adr_ur_sens;		//адрес расширителя портов для фотодатчиков
extern uint8_t adr_pult;		//адрес расширителя портов для пульта
extern uint8_t adr_EEPROM;		//адрес EEPROM

extern uint8_t fl_run_prg;		//флаг выполнения программы
extern uint16_t count_mg;		//счетчик магнита
extern uint8_t coun_prg;		//счетчик выпонения программы
extern int count_step;			//счетчик шагов ШД
extern long count_taho;			//счетчик тахогенератора
extern uint32_t count_100ms;	//счетчик 100мс
extern uint16_t cod_ADC_PW;		//контроль питания МК
extern uint16_t cod_ADC_CS;		//датчик тока
extern uint16_t count_1ms;		//увеличивается каждую мС
extern uint8_t accel_st;		//ускорение
extern uint16_t pediod_T1;		//величина переполнения регистра
extern uint16_t pulse_T1;		//скважность
extern int step;				//количество шагов
extern uint8_t fl_accel;		//флаг ускорения

/*
#ifdef __cplusplus
}
#endif
*/
#endif /* INC_DEF_H_ */

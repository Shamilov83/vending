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

#include "stm32f1xx_hal.h"
#include <string>
extern I2C_HandleTypeDef hi2c1;

//структура возвращаемого значения функций запуска двигателей
typedef enum
{
  MOT_OK       = 0x00U,
  MOT_ERROR    = 0x01U,
  MOT_BUSY     = 0x02U,
  MOT_TIMEOUT  = 0x03U
} StatusMotor;

//флаги
extern uint8_t fl_er;		//флаг выхода из программы с ошибкой
extern uint8_t flag_stop;	//флаг остановки двигателя
extern uint8_t err_tm;		//флаг ошибки по таймауту
extern uint8_t fl_rx;		//флаг принятой команды

extern char mtk[8];			// метка программы
extern uint8_t input_UR;	//неактивное состояние датчиков "1" (подтяжка к питанию)
extern uint8_t input_pult;	//и кнопок
//extern uint8_t bt = 100;
//extern string usb_buf_rx;
extern char usb_buf_tx[21];		//буфер для передачи
extern uint16_t param[5];		//буфер с параметрами команды

//#define otl 1
extern uint8_t adr_ur_sens;		//адрес расширителя портов для фотодатчиков
extern uint8_t adr_pult;		//адрес расширителя портов для пульта
extern uint8_t adr_EEPROM;	//адрес EEPROM

extern uint8_t fl_run_prg;		//флаг выполнения программы
extern uint16_t count_mg;		//счетчик магнита
extern uint8_t coun_prg;		//счетчик выпонения программы
extern int count_step;			//счетчик шагов ШД
extern long count_taho;		//счетчик тахогенератора
extern uint32_t count_100ms;	//счетчик 100мс
extern uint16_t cod_ADC_PW;		//контроль питания МК
extern uint16_t cod_ADC_CS;		//датчик тока
extern uint16_t count_1ms;		//увеличивается каждую мС
extern uint8_t accel_st;		//ускорение
extern uint16_t pediod_T1;		//величина переполнения регистра
extern uint16_t pulse_T1;		//скважность
extern int step;				//количество шагов
extern uint8_t fl_accel;		//флаг ускорения
//extern int t;
#define pediod_T2 1600
#define pulse_T2 pediod_T2/2
#define F_cnt 100000			//частота тактирования счетчика
#define steps_motor 200
#define puse_motor
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)											//Читает бит под номером bit в числе value
#define bitSet(value, bit) ((value) |= (1UL << (bit)))											//Включает (ставит 1) бит под номером bit в числе value
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))										//Выключает (ставит 0) бит под номером bit в числе value
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))	//Ставит бит под номером bit в состояние bitvalue (0 или 1) в числе value
#define bit(b) (1UL << (b))								`										//Возвращает 2 в степени bit
#define Pause	HAL_Delay
#define TIMEOUT 10				//таймаут (Сек.)

/*кнопки пульта*/
//ШД
#define stp_fr 1
#define stp_bk 2
//Штамп
#define sht_opn 3
#define sht_cls 4
//Магн
#define mg_fr 5
#define mg_bk 6
//Нож
#define cut_opn 7
#define cut_cls 8

#define RES_USB GPIOB,GPIO_PIN_7
//шаговый двигатель
#define EN_STEP_MOT GPIOB,GPIO_PIN_12
#define DIR_STEP_MOT GPIOB,GPIO_PIN_14
#define STEP_STEP_MOT GPIOB,GPIO_PIN_13
//двигатель штампа
#define DRAW_KD1_A GPIOA,GPIO_PIN_10
#define DRAW_KD1_B GPIOA,GPIO_PIN_9
#define MOT_SHTAMP  DRAW_KD1_A,DRAW_KD1_B

//двигатель выброса. сторона А, сторона Б
#define DRAW_KD2_A GPIOA,GPIO_PIN_8
#define DRAW_KD2_B GPIOA,GPIO_PIN_15
#define MOT_EJECT DRAW_KD2_A,DRAW_KD2_B

//двигатель ножа1
#define DRAW_KD3_A GPIOB,GPIO_PIN_10
#define DRAW_KD3_B GPIOB,GPIO_PIN_11
#define MOT_CUT DRAW_KD3_A,DRAW_KD3_B

//двигатель магнита
#define DRAW_KD4_A GPIOB,GPIO_PIN_1
#define DRAW_KD4_B GPIOB,GPIO_PIN_0
#define MOT_MAGN DRAW_KD4_A,DRAW_KD4_B
//соленойд выравнивателя
#define SOL1_ALIGN GPIOA,GPIO_PIN_4
//соленойд поджима
#define SOL2_PRESS GPIOA,GPIO_PIN_5
//соленойд склеивателя
#define SOL3_GLUE GPIOA,GPIO_PIN_6
//выдача. сторона А, сторона Б
#define SOL4_EJECT GPIOA,GPIO_PIN_7
//выход ИК светодиодов
#define UR_LED GPIOA,GPIO_PIN_3
//вход татчика тока штампа
#define DT_SHTAMP GPIOA,GPIO_PIN_1
//вход энкодера штампа
#define ENCODER GPIOA,GPIO_PIN_2
//шина I2C
#define I2C_CLK GPIOB,GPIO_PIN_8
#define I2C_DA GPIOB,GPIO_PIN_9
//вход.концевые выключатели ножа
#define Srv_mod GPIOB,GPIO_PIN_5
#define KV1_2 GPIOB,GPIO_PIN_6
//вход
#define KV2_1 GPIOC,GPIO_PIN_13
#define KV2_2 GPIOA,GPIO_PIN_15
//оптопары
#define opto_print_in 0
#define opto_print_2 1
#define opto_magn 2
#define kv_cut_up 3
#define kv_sht_open 4
#define kv_cut_down 5
#define opto_6 6
#define opto_7 7


void Main_func (uint16_t Steps,uint8_t stor,uint8_t timeout);
void PrintFoto(void);
void Service(void);
void MagnFrv(void);
void WaitForOptoStatus(uint8_t num,uint8_t status,uint8_t timeout,const char* mt);
void Solenoid(GPIO_TypeDef* PORT,uint16_t  PIN, uint8_t status,const char* mt);
void messege_err(char);

StatusMotor RunStepMotor(int steps,uint8_t speed,uint32_t accel, int8_t num_opt, uint8_t status ,uint16_t timeout,const char* mt);
StatusMotor RunMotor(GPIO_TypeDef* DRAW_A,uint16_t  PIN_A, GPIO_TypeDef* DRAW_B, uint16_t  PIN_B,uint16_t speed_kd,long steps_ust, int16_t current, int8_t num_opt, uint8_t status , uint16_t timeout,const char* mt);	//запуск колекторного двигателя StartMotorShtamp(кол-во шагов, уставка датчика тока, таймаут)
void StopMotor(GPIO_TypeDef* DRAW_A,uint16_t  PIN_A, GPIO_TypeDef* DRAW_B, uint16_t  PIN_B);
HAL_StatusTypeDef Read_I2C(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t len);
HAL_StatusTypeDef Write_I2C(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t len);
void PortRead(I2C_HandleTypeDef *hi2c, uint8_t DevAddress, uint8_t *port);
void WriteMtk(const char* mt);
//void executeCommand(string data);
void TestDev(void);
void InitDev(void);
void Msgint(int val);
//extern void Msg(string message);


/*
#ifdef __cplusplus
}
#endif
*/
#endif /* INC_DEF_H_ */

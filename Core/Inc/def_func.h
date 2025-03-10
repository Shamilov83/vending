/*
 * def_func.h
 *
 *  Created on: 20 сент. 2022 г.
 *  Author: Shamilov
 */

#ifndef INC_DEF_FUNC_H_
#define INC_DEF_FUNC_H_

//#include <string.h>

/*возвращаемое значение функций запуска двигателей*/
typedef enum
{
  MOT_OK       = 0x00U,
  MOT_ERROR    = 0x01U,
  MOT_BUSY     = 0x02U,
  MOT_TIMEOUT  = 0x03U
} StatusMotor;


void Main_func (uint16_t Steps,uint8_t stor,uint8_t timeout);	//функция печати фото с наклейкой на магнит и обрезанием по заданным параметрам
void PrintFoto(void);											//функция печати фото без наклейки на магнит
void MagnFrv(void);												//функция подачи магнита
void Foto_to_magn(uint16_t,uint8_t, uint8_t);					//функция печати фото и наклкйка на магнит без обрезки
void WaitForOptoStatus(uint8_t num,uint8_t status,uint16_t timeout, uint8_t *flag);	//функция ожидания фото
void Solenoid(GPIO_TypeDef* PORT,uint16_t  PIN, uint8_t status, uint8_t *flag);				//управление соленойдом
void messege_err(char);
void Event_err(uint8_t *error);			//отключение соленойдов при ошибке
void TestSol(void);						//тест соленойдов
void TestInput(void);
void ReadEEPROM(void);					//чтение данных из EEPROM
void WriteEEPROM(void);					//запись данных из EEPROM
void EraseEEPROM(uint16_t len);			//очистить EEPROM (кол-во байт)
StatusMotor CalibrSteps(void);			//замер шагов от начала до отрезки и запись в EEPROM

void RunStepMotor(int steps,uint8_t speed,uint32_t accel, int8_t num_opt, uint8_t status ,uint16_t timeout, uint8_t *flag);
void RunMotor(GPIO_TypeDef* DRAW_A,uint16_t  PIN_A, GPIO_TypeDef* DRAW_B, uint16_t  PIN_B,uint16_t speed_kd,long steps_ust, int16_t current, int8_t num_opt, uint8_t status , uint16_t timeout, uint8_t *flag);	//запуск колекторного двигателя StartMotorShtamp(кол-во шагов, уставка датчика тока, таймаут)
void StopMotor(GPIO_TypeDef* DRAW_A,uint16_t  PIN_A, GPIO_TypeDef* DRAW_B, uint16_t  PIN_B);
HAL_StatusTypeDef Read_I2C(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t len);	//чтение  по интерфейсу I2C
HAL_StatusTypeDef Write_I2C(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t len);	//запись по интерфейсу I2C
void PortRead(I2C_HandleTypeDef *hi2c, uint8_t DevAddress, uint8_t *port);	//чтение портов расширения
void WriteMtk(const char* mt);
void DWT_Init(void);
void delay_micros(uint32_t us);
//void executeCommand(string data);
void InitDev(void);
void Msgint(int val);
//extern void Msg(string message);


#endif /* INC_DEF_FUNC_H_ */

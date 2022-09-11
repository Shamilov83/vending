/*
 * main_func.c
 *
 *  Created on: 25 янв. 2022 г.
 *      Author: shamilov
 *
 *   биты input_UR
 *   0 - оптодатчик наличия фото
 *   1 - оптодатчик
 *   2 - оптодатчик магнита
 *   3 - нож открыт
 *   4 - шт. открыт
 *   5 - нож закрыт
 *   6 -
 *   7 -
 *
 *   биты input_pult
 *   0 - ШД вперед
 *   1 - ШД назад
 *   2 - Нож вниз
 *   3 - Нож вверх
 *   4 -
 *   5 -
 *   6 -
 *   7 -
 *
 *  //адреса I2C устройств//
 * ИК-приемники - 0x47
 * Пульт - 0x43
 * Память - 0x50
 *
 */

#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "def.h"
#include <string>
#include <cstring>
#include "usbd_cdc_if.h"
#include "stm32f1xx_hal.h"

using namespace std;

void executeCommand(string data_rx);

void Msg(string message);	//отправка сообщения в USB
void Msgint(int val);		//отправка целочисленных данных в USB
//флаги
uint8_t fl_er;				//флаг завершения с ошибкой
uint8_t fl_rx;				//принята команда

bool fl_run_pr = 0;			//флаг выпонения программы. выставляется после запуска
uint8_t err_tm;
uint8_t fl_run_prg;			//флаг вып прг

char mtk[8];					//
char usb_bef_tx[21];		//массив для передачи
string usb_buf_rx;			//принятая команда
int param[5];				//массив с параметрами команды


uint16_t count_mg = 0;		//счетчик магнита
uint8_t coun_prg = 0;		//счетчик выпонения программы
int count_step;				//счетчик шагов ШД
long count_taho = 0;		//счетчик тахогенератора
uint32_t count_100ms = 0;	//счетчик 100мс
int step;					//абсолютное количество шагов
//int t;

uint16_t cod_ADC_PW;		//код АЦП напр. пит
uint16_t cod_ADC_CS;		//код АЦП датчика тока штампа

uint8_t accel_st;			//уставка ускорения ШД
uint16_t pediod_T1 = 48000;
uint16_t pulse_T1 = pediod_T1/2;
uint8_t fl_accel = 0;		//флаг завершения ускорения
uint8_t flag_stop = 0;		//флаг остановки ШД
//адреса шины I2C
uint8_t bt = 100;			//скорость I2C
uint8_t adr_pult = 0x43;	//адрес пульта
uint8_t adr_ur_sens = 0x47;	//адрес платы фотоэлементов
uint8_t adr_EEPROM = 0x50;	//адрес EEPROM



uint8_t input_UR = 0b11111111;	//неактивное состояние датчиков
uint8_t input_pult = 0b11111111;	//и кнопок

uint16_t usart_buf[10];		//приемный буфер
/*
 * Глобальные разрешение и запрет прерываний.
 * __disable_irq (); // запретить прерывания
 * __enable_irq ();  // разрешить прерывания
 */

void Main_func (uint16_t Steps,uint8_t stor,uint8_t timeout){
/*проверку флага ошибки нужно производить перед вызовом функции*/
#ifdef DEBUG
			Msg("----Start----");
#endif

	m0:		RunMotor(MOT_MAGN, 1000, 10000,  4000, opto_magn, 1 , timeout,"m0");	//подача магнита  (speed_kd,steps_ust,current,num_opt,status ,timeout)


	m1:		WaitForOptoStatus(opto_print_in,1,30000,"m1");						// ожидание фото из принтера

			Pause(1000);

#ifdef DEBUG
			Msg("fl_er = ");
			Msgint(fl_er);
#endif

	m2:		Solenoid(SOL1_ALIGN,1,"m2"); 		// ВЫРАВНИВАТЕЛЬ
			Pause(300);
			Solenoid(SOL1_ALIGN,0,"m2");
			Pause(300);
			Solenoid(SOL1_ALIGN,1,"m2");

			Pause(300);

	m3:		Solenoid(SOL2_PRESS,1,"m3"); 		// ПРИЖИМ

			Pause(300);

	m4:		Solenoid(SOL1_ALIGN,0,"m4"); 		//откл выравниватель


			Pause(300);
									//проезжает N шагов от начала и останавливается перед штампом
	m5:		RunStepMotor(STEP_TO_CUT,120,1, - 1, 0 ,timeout, "m1"); //(steps,speed,accel,num_opt,status,timeout) 1 - закрыта, 0 - открыта 7500

			Pause(300);

			Solenoid(SOL2_PRESS,0,"m3"); 		// ПРИЖИМ откл

			/*
			Pause(500);
			Msg("Step motor revers...");
	m6:		RunStepMotor(-100,120,1, 2, 0 ,100, "m6");//ШД назад
			*/

			Pause(500);

	m9:		Solenoid(SOL3_GLUE,1,"m9");		//склейка

	////////////////////////*отрезка*///////////////////////////////
	/////////////////////////////////////////////////////////////////
			Pause(500);

			RunMotor(MOT_CUT, 1000, -10000,  -1, kv_cut_up, 0 , timeout,"m7");

	m70:	RunMotor(MOT_CUT, 1000, 10000,  -1, kv_cut_down, 0 ,timeout,"m70");
			Pause(500);

	m20:	RunMotor(MOT_CUT, 1000, -10000,  -1, kv_cut_up, 0 , timeout,"m7");


#ifdef DEBUG
			Msg("fl_er = ");
			Msgint(fl_er);
#endif
	////////////////////////////////////////////////////////////////


			Pause(500);
			//старт ШД на N шагов из входных параметров главной функции
	m8:		RunStepMotor(Steps,120,1, -1, 0 ,timeout, "m8"); //(steps,speed,accel,num_opt,status,timeout) 1 - закрыта, 0 - открыта

			Pause(500);

	m90:	Solenoid(SOL3_GLUE,0,"m9");			//склеивание отключить
			Pause(500);

#ifdef DEBUG
			Msg("fl_er = ");
			Msgint(fl_er);
#endif
	//////////////////штамповка///////////////////////////////////////
			RunMotor(MOT_SHTAMP, 1000, -20000,  2000, kv_sht_open, 0 , timeout,"m0");
			Pause(500);
			//шатмп вверх(закр)
			RunMotor(MOT_SHTAMP, 1000, 20000,  900, -1, 0 , timeout,"m0");//при закрытии исключить контроль по концевику или оптодатчику (-1)
			Pause(500);



			//штамп вниз(откр)
			RunMotor(MOT_SHTAMP, 1000, -1000,  900, -kv_sht_open, 0 , timeout,"m0");
			Pause(500);
			//шатмп вверх(закр)
			RunMotor(MOT_SHTAMP, 1000, 20000,  900, -1, 0 , timeout,"m0");//при закрытии исключить контроль по концевику или оптодатчику (-1)
			Pause(500);



			//штамп вниз(откр)
			RunMotor(MOT_SHTAMP, 1000, -20000,  2000, kv_sht_open, 0 , timeout,"m0");
			Pause(500);


	//////////////////////////////////////////////////////////////////
			//выход из штампа
			Pause(500);
				//старт ШД на N шагов
			RunStepMotor(-2000,120,1, -1, 0 ,timeout, "m18");
	m18:	RunStepMotor(20000,120,1, -1, 0 ,timeout, "m18"); //(steps,speed,accel,num_opt,status,timeout) 1 - закрыта, 0 - открыта

	m100:;

	Event_err();
}

/*фото на магните без обрезки*/
void Foto_to_magn(uint16_t Steps,uint8_t stor,uint8_t timeout){

	m0:		RunMotor(MOT_MAGN, 1000, 10000,  4000, opto_magn, 1 , timeout,"m0");	//подача магнита  (speed_kd,steps_ust,current,num_opt,status ,timeout)


	m1:		WaitForOptoStatus(opto_print_in,1,30000,"m1");						// ожидание фото из принтера

			Pause(1000);

#ifdef DEBUG
			Msg("fl_er = ");
			Msgint(fl_er);
#endif

	m2:		Solenoid(SOL1_ALIGN,1,"m2"); 		// ВЫРАВНИВАТЕЛЬ
			Pause(300);
			Solenoid(SOL1_ALIGN,0,"m2");
			Pause(300);
			Solenoid(SOL1_ALIGN,1,"m2");

			Pause(300);

	m3:		Solenoid(SOL2_PRESS,1,"m3"); 		// ПРИЖИМ

			Pause(300);

	m4:		Solenoid(SOL1_ALIGN,0,"m4"); 		//откл выравниватель


			Pause(300);
									//проезжает N шагов от начала и останавливается перед штампом
	m5:		RunStepMotor(STEP_TO_CUT,120,1, - 1, 0 ,timeout, "m1"); //(steps,speed,accel,num_opt,status,timeout) 1 - закрыта, 0 - открыта 7500

			Pause(300);

			Solenoid(SOL2_PRESS,0,"m3"); 		// ПРИЖИМ откл

			/*
			Pause(500);
			Msg("Step motor revers...");
	m6:		RunStepMotor(-100,120,1, 2, 0 ,100, "m6");//ШД назад
			*/

			Pause(500);

	m9:		Solenoid(SOL3_GLUE,1,"m9");		//склейка

	////////////////////////*отрезка*///////////////////////////////
	/////////////////////////////////////////////////////////////////
			Pause(500);

			RunMotor(MOT_CUT, 1000, -10000,  -1, kv_cut_up, 0 , timeout,"m7");

	m70:	RunMotor(MOT_CUT, 1000, 10000,  -1, kv_cut_down, 0 ,timeout,"m70");
			Pause(500);

	m20:	RunMotor(MOT_CUT, 1000, -10000,  -1, kv_cut_up, 0 , timeout,"m7");


#ifdef DEBUG
			Msg("fl_er = ");
			Msgint(fl_er);
#endif
	////////////////////////////////////////////////////////////////


			Pause(500);
			//старт ШД на N шагов из входных параметров главной функции
	m8:		RunStepMotor(Steps,120,1, -1, 0 ,timeout, "m8"); //(steps,speed,accel,num_opt,status,timeout) 1 - закрыта, 0 - открыта

			Pause(500);

	m90:	Solenoid(SOL3_GLUE,0,"m9");			//склеивание отключить
			Pause(500);

			Event_err();
}
/*Программа печати фото без магнита*/
void PrintFoto(void){
	WaitForOptoStatus(opto_print_in,1,30000,"m1"); 		// Оптрон наличия бумаги
	Pause(1000);

	Solenoid(SOL1_ALIGN,1,"m2"); 		// ВЫРАВНИВАТЕЛЬ
	Pause(500);
	Solenoid(SOL1_ALIGN,0,"m2");
	Pause(500);
	Solenoid(SOL1_ALIGN,1,"m2");

	Pause(500);

	Solenoid(SOL2_PRESS,1,"m3"); 		// ПРИЖИМ

	Pause(500);

	Solenoid(SOL1_ALIGN,0,"m4"); 		//откл выравниватель

	Solenoid(SOL3_GLUE,1,"m9");

	RunStepMotor(30000,120,1, -2, 1 ,100, "m5");

	Solenoid(SOL2_PRESS,0,"m3"); 		// ПРИЖИМ

	Solenoid(SOL3_GLUE,0,"m9");

	Event_err();
}


/*функция подачи магнита*/
void MagnFrv(void){

	m0:		RunMotor(MOT_MAGN, 1000, 10000,  4000, 2, 1 , 10,"m0");	//подача магнита  (speed_kd,steps_ust,current,num_opt,status ,timeout)
			Pause(500);
}


/*
 * Функция ожидания фотографии из принтера
 * если за время таймаута фото не вышло из принтера,
 * то установить флаг ошибки и выйти. При срабатывании шунтируется.
 * 1 - наличие фото, 0 - отсутствие
 */
void WaitForOptoStatus(uint8_t num_opt,uint8_t status,uint16_t timeout,const char* mt){
	if(fl_er){
	return;
	}

#ifdef DEBUG
		Msg("Wait opto");
#endif
		WriteMtk(mt);
		//timeout = (timeout*1000);
		count_100ms = 0;

		for(;;){
			PortRead(&hi2c1, adr_ur_sens,&input_UR);

			if(bitRead(input_UR, num_opt) == status) {
#ifdef DEBUG
				Msg("Event opto1");
#endif
				break;
			}
			else if(count_100ms > timeout){
#ifdef DEBUG
				Msg("Timeout opto1");
#endif
				Pause(10);
				fl_er = true;		//если счетчик больше таймаута
#ifdef DEBUG
				Msgint(fl_er);
#endif
				return;
			}

			HAL_Delay(10);
		}

}


/*
status:
0 - 00;
1 - 01;
2 - 10;
3 - 11.
*/

/*функция включения соленойда
 * status: 1-вкл/0-выкл
 */

void Solenoid(GPIO_TypeDef* PORT,uint16_t  PIN,uint8_t status,const char* mt){
	if(!fl_er){
#ifdef DEBUG
		Msg("Sol_");
#endif
		WriteMtk(mt);
		if(status == 0) HAL_GPIO_WritePin(PORT,PIN,GPIO_PIN_SET);
		else HAL_GPIO_WritePin(PORT,PIN,GPIO_PIN_RESET);
	}
}

// кол-во шагов((-) - в обр, .сторону1000 шагов на оборот), скорость(об/мин), ускорение(приращение к скважности (мс/100мс)) timeout(Cек)
// максимальное ускорение это величина скорости.
// 200 шагов/оборот
//
/*
 *
 * значение регистра
 * (1/скор)*(част.такт/prescaller)
 *
 *возвращаемые значения
 * MOT_OK - функция выполнена
 * MOT_ERROR -
 * MOT_BUSY - в процессе выполнения
 * MOT_TIMEOUT - превышено время ожидания срабатывания оптодатчика или датчика тока
 *
 * status: 0 - открыт, 1 - закрыт.
 * если оптодатчик указан со знаком минул, он не контролируется
 */
StatusMotor RunStepMotor(int steps,uint8_t speed,uint32_t accel, int8_t num_opt, uint8_t status ,uint16_t timeout,const char* mt){
if(fl_er){
return MOT_ERROR;
}

#ifdef DEBUG
	Msg("Start Step mot");
#endif

	WriteMtk(mt);
	if(steps > 0)HAL_GPIO_WritePin(DIR_STEP_MOT,GPIO_PIN_RESET); 	//если положительное число, то вперед
	else HAL_GPIO_WritePin(DIR_STEP_MOT,GPIO_PIN_SET);				//иначе, назад

//	pediod_T1 = ((F_cnt*10)/(speed*200)) ;

	count_100ms = 0;
/*
	pulse_T1 = pediod_T1/8;
	accel_st = accel;
*/
	timeout = (timeout*10);//Cек


	step = abs(steps);
	//старт ШД
/*
	TIM1->ARR = pediod_T1;
	TIM1->CCR1 = pulse_T1;
*/
	HAL_GPIO_WritePin( EN_STEP_MOT,GPIO_PIN_SET);				//включить ШД

/*
 * ожидание открытия оптодатчика для обнуления счетчика шагов
 */
	for(;;){
		//Строб ШД.
////////////////////////////////////////////////////////////////
		HAL_GPIO_WritePin( STEP_STEP_MOT,GPIO_PIN_RESET);
		HAL_GPIO_WritePin( STEP_STEP_MOT,GPIO_PIN_SET);
		delay_micros(speed);
		HAL_GPIO_WritePin( STEP_STEP_MOT,GPIO_PIN_RESET);
		delay_micros(speed);
/////////////////////////////////////////////////////////////////
		PortRead(&hi2c1, adr_ur_sens,&input_UR);
		if(bitRead(input_UR, opto_print_in) == 0){
		//count_step = 0;
		break;
		}
		else if(count_100ms > 1000 ){
#ifdef DEBUG
			Msg("MT_TMT1_SM");
#endif
			//fl_er = 1;
			//break;
			return MOT_TIMEOUT;
		}

	}

#ifdef DEBUG
	Msg("count_step = 0");
#endif

	count_step = 0;
	for(;;){

		//Строб ШД
////////////////////////////////////////////////////////////////
		HAL_GPIO_WritePin( STEP_STEP_MOT,GPIO_PIN_RESET);
		HAL_GPIO_WritePin( STEP_STEP_MOT,GPIO_PIN_SET);
		delay_micros(speed);
		HAL_GPIO_WritePin( STEP_STEP_MOT,GPIO_PIN_RESET);
		delay_micros(speed);
		count_step++;
/////////////////////////////////////////////////////////////////

		PortRead(&hi2c1, adr_ur_sens,&input_UR);					//опрос оптодатчиков

		if(num_opt >= 0 ){											//если оспользуется оптодатчик
			PortRead(&hi2c1, adr_ur_sens,&input_UR);
			if(bitRead(input_UR, num_opt) == status){

				HAL_GPIO_WritePin(EN_STEP_MOT,GPIO_PIN_RESET);		//выключить ШД
#ifdef DEBUG
				Msgint(count_step);
#endif
				return MOT_OK;
			}
		}
			if(count_step >= step){
			HAL_GPIO_WritePin(EN_STEP_MOT,GPIO_PIN_RESET);			//выключить ШД
#ifdef DEBUG
			Msg("count_step >= step");
			Msgint(count_step);
#endif
			return MOT_OK;
		}
			else if(count_100ms > timeout ){						//если превышен таймаут
			HAL_GPIO_WritePin(EN_STEP_MOT,GPIO_PIN_RESET);			//выключить ШД

#ifdef DEBUG
			Msg("MT_TMT2_ST");
#endif
			Pause(10);
			fl_er = true;
#ifdef DEBUG
			Msgint(fl_er);
#endif
			return MOT_TIMEOUT;
		}

	}//for



}


/*
 * функция запуска колекторного двигателя
 * входные параметры(порты,скорость,кол-во шагов, уставка датчика тока, оптодатчик,статус,таймаут)
 * скорость: 1-1000 (коэффициент заполнения)
 * количество шагов для штампа указывается необходимое. шаги стчитываются с энкодера. для других КД кол-во шагов 0
 * для ДТ диапазон измерений 0-2049 и 2048-4096 в зависимости от направления тока. усли отрицат, игнор
 * оптодатчик - бит input_UR, если значние отрицательное, то датчик не контролируется
 * статус - ожидаемый статус оптодатчика при стабатывании. 0 - открыт, 1 - закрыт
 * таймаут - время ожидания срабатывания ДТ или оптодатчика (С)
 * GPIO_TypeDef* DRAW_EN,uint16_t  PIN_EN - вывод разрешающего сигнала
 * GPIO_TypeDef* DRAW_DIR, uint16_t  PIN_DIR - вывод сигнала направления
 *
 */


StatusMotor RunMotor(GPIO_TypeDef* DRAW_A,uint16_t  PIN_A, GPIO_TypeDef* DRAW_B, uint16_t  PIN_B, uint16_t speed_kd, long steps_ust,  int16_t current, int8_t num_opt, uint8_t status , uint16_t timeout,const char* mt){

if(fl_er){
return MOT_ERROR;
}			//если не установлен флаг ошибки

#ifdef DEBUG
Msg("Start KD");
#endif

WriteMtk(mt);
count_100ms = 0;		//обнулить счетчик таймаута.
count_taho = 0;
timeout = (timeout*10);
uint16_t  pediod_T3;
pediod_T3 = 1000 ;

if(num_opt >= 0 ){							//если используется датчик
	PortRead(&hi2c1, adr_ur_sens,&input_UR);
	if(bitRead(input_UR, num_opt) == status){//если датчик сработал
		StopMotor(DRAW_A,PIN_A,DRAW_B,PIN_B);
		return MOT_OK;
	}
}

	//задать направление вращения
	if(steps_ust >= 0){
		 HAL_GPIO_WritePin(DRAW_A,PIN_A,GPIO_PIN_SET);
		 HAL_GPIO_WritePin(DRAW_B,PIN_B,GPIO_PIN_RESET);
	}
	else{
		 HAL_GPIO_WritePin(DRAW_A,PIN_A,GPIO_PIN_RESET);
		 HAL_GPIO_WritePin(DRAW_B,PIN_B,GPIO_PIN_SET);
	}
	steps_ust = abs(steps_ust);
	HAL_Delay(5);
	//запуск ШИМ
	TIM3->ARR = pediod_T3;
	TIM3->CCR1 = speed_kd;
	//HAL_TIM_Base_Start_IT(&htim3);
	//ждем когда что-то сработает

		for(;;){

					if(num_opt >= 0 ){								//если оспользуется оптодатчик
						PortRead(&hi2c1, adr_ur_sens,&input_UR);
						if(bitRead(input_UR, num_opt) == status){
							//if(fl_deb == status){
#ifdef DEBUG
							Msg("Status!");
#endif
							StopMotor(DRAW_A,PIN_A,DRAW_B,PIN_B);
							return MOT_OK;
						}
					}
						Pause(500);
					if(current > 0){
						if(cod_ADC_CS >= current){
#ifdef DEBUG
						Msg("cod_ADC_CS >= current");
#endif
						StopMotor(DRAW_A,PIN_A,DRAW_B,PIN_B);
						return MOT_OK;
						}
					}

						if(count_taho >= steps_ust){
#ifdef DEBUG
						Msg("count_taho >= steps_ust");
#endif
						StopMotor(DRAW_A,PIN_A,DRAW_B,PIN_B);
						return MOT_OK;
					}
						if(count_100ms > timeout ){				//если превышен таймаут
						StopMotor(DRAW_A,PIN_A,DRAW_B,PIN_B);
#ifdef DEBUG
						Msg("MT_TMT");
#endif
						fl_er = true;
						Pause(10);
#ifdef DEBUG
						Msgint(fl_er);
#endif
						return MOT_TIMEOUT;
					}

			}



}



/*
 * функция остановки колекторного двигателя
 * входные параметры - номера портов
 */
void StopMotor(GPIO_TypeDef* DRAW_A,uint16_t  PIN_A, GPIO_TypeDef* DRAW_B, uint16_t  PIN_B){
	 HAL_GPIO_WritePin(DRAW_A,PIN_A,GPIO_PIN_SET);
	 HAL_GPIO_WritePin(DRAW_B,PIN_B,GPIO_PIN_SET);
}

/*
 * опрос расширителей портов по шине I2C
 * I2C_HandleTypeDef *hi2c -  это указатель на экземпляр структуры
 * DevAddress - адрес ведомого устройства
 * *port - указатель на массив размером 1 байт
 *
 */

void PortRead(I2C_HandleTypeDef *hi2c, uint8_t DevAddress, uint8_t *port)
{
	 HAL_StatusTypeDef returnValue;
	 returnValue = HAL_I2C_IsDeviceReady(hi2c, DevAddress, 1, 100); //
	 if(returnValue==HAL_OK){
		 HAL_I2C_Master_Receive (hi2c,DevAddress, port, 1, 100);
		 //printf("read port = %d\r\n", val);
	 }
	 //else printf("port no connect = %d\r\n", returnValue);
}

//Разбор принятых данных
string convertCommandToIntList(string &str,int *array ) {
	int8_t i;
	if(str.find(',') != string::npos){					//если команда с параметрами
	size_t index = str.find('(');
	//Msg(" --parsing--  ");
	//HAL_Delay(5);
	string command = str.substr(0, index);				//возвращает подстроку от нулевого элемента до "("
	//Msg(command);
	str = str.substr((index + 1), str.find(')'));		//возвращает подстроку с параметрами команды не включая скобки
	//Msg(str);
	//HAL_Delay(5);
	for (int8_t j = 0; j<=2; j++ ) {
		i  = str.find(',');
		if(str.find(',') == string::npos) i = (str.length()-1);
		string arg = str.substr(0, i);					//возвращает первый элемент параметра команды
		//Msg("arg ="+arg);
		//HAL_Delay(5);
		array[j] = (stoi(arg));							//добавить преобразованный, из строки в инт, элемент связанного списка
		//Msgint(param[j]);
		str = str.substr(i+1);							//возвращает подстроку с указ позиции и до конца строки
		//Msg("str="+str);
		HAL_Delay(5);
	}													//выход из цикла произойдет не добавив последний параметр в связанный список
	return command+="()" ;
	}
	else{
		return str;
	}
}

void executeCommand(string data_rx)
{
	//Msg("--Build arrea--");
	string command = convertCommandToIntList(data_rx, param);
	//Msg("executeCommand...\r\n");
	if (command.find("Reboot()") != string::npos) { 	//если строка найдена. string::npos(-1) возвращается если заданная строка не найдена.
		Msg("Reboot devace...\r\n");
		   __set_FAULTMASK(1);							// Запрещаем все маскируемые прерывания //связь виснет нужен ресет шины
		   NVIC_SystemReset();							// Программный сброс
		   usb_buf_rx.clear();
	}
	else if (command.find("Connect()") != string::npos) {
			Msg("USB Connected...\r\n");
	}
	else if (command.find("RunPrg()") != string::npos) {
			Msg("RunPrgramm...\r\n");
			Main_func (param[0],param[1],param[2]);		//запуск основной программы
	}
	else if(command.find("ResetError()")!= string::npos){
			Msg("ResetError...\r\n");
			fl_er = 0;									//сброс флага ошибки
	}
	else if(command.find("PrintFoto()")!= string::npos){
			PrintFoto();								//печать фото
	}
	else if(command.find("MagnFrv()")!= string::npos){
		RunMotor(MOT_MAGN, 1000, 10000,  4000, opto_magn, 1 , 60,"m100");	//подача магнита
	}
	else if(command.find("Test()")!= string::npos){
				TestDev();		//
	}
	else if(command.find("ShtampOpen()")!= string::npos){
		RunMotor(MOT_SHTAMP, 1000, -20000,  2000, kv_sht_open, 0 , 20,"m100");
	}
	else if(command.find("CutUp()")!= string::npos){
		RunMotor(MOT_CUT, 1000, -10000,  -1, kv_cut_up, 0 , 20,"m100");
	}
	else if(command.find("RunStepMot()")!= string::npos){
		RunStepMotor(param[0],param[1],1, -1, 0 ,param[2], "m100");  //команда запуска ШД на N шагов RunStepMot(N |-N)
	}
	else if(command.find("Request_fl_er()")!= string::npos){		//запрос флага ошибки
		Msgint(fl_er);
	}
	else if(command.find("Foto_to_magn()")!= string::npos){
		Foto_to_magn(param[0],param[1],param[2]);
	}
	else if(command.find("Cut()")!= string::npos){
		RunMotor(MOT_CUT, 1000, 10000,  -1, kv_cut_down, 0 , 60,"m100");
		RunMotor(MOT_CUT, 1000, -10000,  -1, kv_cut_up, 0 , 60,"m100");
	}
	else if(command.find("TestSol()")!= string::npos){
		TestSol();
	}
	else if(command.find("TestInput()")!= string::npos){
		TestInput();
	}
	else if(command.find("ShtampClose()")!= string::npos){
			RunMotor(MOT_SHTAMP, 1000, 5000,  100, -1, 0 , 20,"m100");
		}

	usb_buf_rx.clear();	//очистить переменную
	fl_rx = 0;
}



//функция формирования строковой переменной
void ArreyRx(string data_rx){

	if( data_rx.find(')') != string::npos){					//если найден символ окончания команды
		usb_buf_rx += data_rx;
		//Msg(usb_buf_rx);
		fl_rx = 1;
	return;
	}
	else{
		usb_buf_rx += data_rx;
		fl_rx = 0;
		return;
	}
}


void TestDev(void){




}

void Service(void){
/*
	#define current 1000 //предел срабатывания датчика тока

	PortRead(&hi2c1, adr_ur_sens,&input_UR);
	Pause(5);
	PortRead(&hi2c1, adr_pult,&input_pult);

	if(bitRead(input_pult, stp_fr) == 0){		//ШД вперёд
		TIM1->ARR = 1000;
		TIM1->CCR1 = 500;
		HAL_GPIO_WritePin(DIR_STEP_MOT,GPIO_PIN_SET);
		HAL_GPIO_WritePin( EN_STEP_MOT,GPIO_PIN_SET);
	}
	else if(bitRead(input_pult, stp_fr) == 1){
			TIM1->CCR1 = 0;
			HAL_GPIO_WritePin( EN_STEP_MOT,GPIO_PIN_RESET);
	}
	else if(bitRead(input_pult, stp_bk) == 0){		//ШД назад
			TIM1->ARR = 1000;
			TIM1->CCR1 = 500;
			HAL_GPIO_WritePin(DIR_STEP_MOT,GPIO_PIN_RESET);
			HAL_GPIO_WritePin( EN_STEP_MOT,GPIO_PIN_SET);
	}
	else if(bitRead(input_pult, stp_bk) == 1){
			TIM1->CCR1 = 0;
			HAL_GPIO_WritePin( EN_STEP_MOT,GPIO_PIN_RESET);
	}
	else if(bitRead(input_pult, sht_opn) == 0){		//открыть штамп
			if(bitRead(input_UR, kv_sht_open) == 1){			//открывать пока не сработает концевик
				HAL_GPIO_WritePin( DRAW_KD1_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin( DRAW_KD1_B,GPIO_PIN_SET);
			}
			else {
				HAL_GPIO_WritePin( DRAW_KD1_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin( DRAW_KD1_B,GPIO_PIN_RESET);
				Msg("sht_open");
				Pause(10);
			}
	}
	else if(bitRead(input_pult, sht_opn) == 1){
			HAL_GPIO_WritePin( DRAW_KD1_A,GPIO_PIN_RESET);
			HAL_GPIO_WritePin( DRAW_KD1_B,GPIO_PIN_RESET);
	}
	else if(bitRead(input_pult, sht_cls) == 0){		//закрыть штамп
			HAL_GPIO_WritePin( DRAW_KD1_A,GPIO_PIN_SET);
			HAL_GPIO_WritePin( DRAW_KD1_B,GPIO_PIN_RESET);

			if(cod_ADC_CS >= current){
				HAL_GPIO_WritePin( DRAW_KD1_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin( DRAW_KD1_B,GPIO_PIN_RESET);
				Msg("sht_close");
				Pause(10);
			}
	}
	else  if(bitRead(input_pult, sht_cls) == 1){
			HAL_GPIO_WritePin( DRAW_KD1_A,GPIO_PIN_RESET);
			HAL_GPIO_WritePin( DRAW_KD1_B,GPIO_PIN_RESET);
	}
	else  if(bitRead(input_pult, mg_fr) == 0){		//магнит вперед
			HAL_GPIO_WritePin( DRAW_KD2_A,GPIO_PIN_SET);
			HAL_GPIO_WritePin( DRAW_KD2_B,GPIO_PIN_RESET);
			Msg("mg_fr");
			Pause(10);
	}
	else  if(bitRead(input_pult, mg_fr) == 1){
			HAL_GPIO_WritePin( DRAW_KD2_A,GPIO_PIN_RESET);
			HAL_GPIO_WritePin( DRAW_KD2_B,GPIO_PIN_RESET);
	}
	else  if(bitRead(input_pult, mg_bk) == 0){		//магнит назад
			HAL_GPIO_WritePin( DRAW_KD2_A,GPIO_PIN_RESET);
			HAL_GPIO_WritePin( DRAW_KD2_B,GPIO_PIN_SET);
			Msg("mg_bk");
			Pause(10);
	}
	else  if(bitRead(input_pult, mg_bk) == 1){
			HAL_GPIO_WritePin( DRAW_KD2_A,GPIO_PIN_RESET);
			HAL_GPIO_WritePin( DRAW_KD2_B,GPIO_PIN_RESET);
	}
	else  if(bitRead(input_pult, cut_cls) == 0){		//нож вниз
			if(bitRead(input_UR, 5) == 1){					//если не сработал концевой вкл. двиг
				HAL_GPIO_WritePin( DRAW_KD3_A,GPIO_PIN_SET);
				HAL_GPIO_WritePin( DRAW_KD3_B,GPIO_PIN_RESET);

			}
			else {
				HAL_GPIO_WritePin( DRAW_KD3_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin( DRAW_KD3_B,GPIO_PIN_RESET);
				Msg("cut_close");
				Pause(10);
			}
	}
	else  if(bitRead(input_pult, cut_cls) == 1){
			HAL_GPIO_WritePin( DRAW_KD3_A,GPIO_PIN_RESET);
			HAL_GPIO_WritePin( DRAW_KD3_B,GPIO_PIN_RESET);
	}
	else  if(bitRead(input_pult, cut_opn) == 0){		//нож вверх
			if(bitRead(input_UR, 3) == 0){
				HAL_GPIO_WritePin( DRAW_KD3_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin( DRAW_KD3_B,GPIO_PIN_SET);
			}
			else{
				HAL_GPIO_WritePin( DRAW_KD3_A,GPIO_PIN_RESET);
				HAL_GPIO_WritePin( DRAW_KD3_B,GPIO_PIN_RESET);
				Msg("cut_open");
				Pause(10);
		}
	}
	else  if(bitRead(input_pult,cut_opn) == 1){
			HAL_GPIO_WritePin( DRAW_KD3_A,GPIO_PIN_RESET);
			HAL_GPIO_WritePin( DRAW_KD3_B,GPIO_PIN_RESET);
	}

*/
}


//инициализация выводов устройств
void InitDev(void){

	//соленойды
	HAL_GPIO_WritePin(SOL1_ALIGN,GPIO_PIN_SET);
	HAL_GPIO_WritePin(SOL2_PRESS,GPIO_PIN_SET);
	HAL_GPIO_WritePin(SOL3_GLUE,GPIO_PIN_SET);
	HAL_GPIO_WritePin(SOL4_EJECT,GPIO_PIN_SET);
	//моторы
	HAL_GPIO_WritePin(DRAW_KD1_A,GPIO_PIN_SET);
	HAL_GPIO_WritePin(DRAW_KD1_B,GPIO_PIN_SET);

	HAL_GPIO_WritePin(DRAW_KD2_A,GPIO_PIN_SET);
	HAL_GPIO_WritePin(DRAW_KD2_B,GPIO_PIN_SET);

	HAL_GPIO_WritePin(DRAW_KD3_A,GPIO_PIN_SET);
	HAL_GPIO_WritePin(DRAW_KD3_B,GPIO_PIN_SET);

	HAL_GPIO_WritePin(EN_STEP_MOT,GPIO_PIN_SET);
	HAL_GPIO_WritePin(DIR_STEP_MOT,GPIO_PIN_SET);

	//сброс шины USB
	HAL_GPIO_WritePin(Res_USB,GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(Res_USB,GPIO_PIN_SET);

	RunMotor(MOT_SHTAMP, 1000, -20000,  3000, kv_sht_open, 0 , 60,"m0");	//открыть штамп
	RunMotor(MOT_CUT, 1000, -10000,  -1, kv_cut_up, 0 , 60,"m7");			//поднять нож
}


/*функция записи метки в массив*/
void WriteMtk(const char* mt){
	string str(mt);
	strcpy(mtk, str.c_str());
}

/*отправка сообщений в USB*/
void Msg(string message){
/*
 * Функция c_str () используется для возврата указателя на массив, который содержит завершенную нулем последовательность символов, представляющую текущее значение строки.
 */
		message += "\r\n";
		int n = message.length(); 				//длинна строковой переменной
		char char_array [n + 1];				//
		strcpy(char_array, message.c_str());	//копирование
	  	CDC_Transmit_FS((unsigned char*)char_array, strlen(char_array));
	  	return;
	  }
/*функция для проверки массива с параметрами*/
void Msgint(int val){
	char arrey[16];								//промежуточный массив
	string str = itoa(val,arrey,10);			//itoa(переменная,промеж.массив,сист.счисл.)
	Msg(str);
}

/*инициализация DWT*/
void DWT_Init(void)
{
    SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; 	// разрешаем использовать счётчик
    DWT_CONTROL |= DWT_CTRL_CYCCNTENA_Msk;   	// запускаем счётчик
}

void delay_micros(uint32_t us)
{
    uint32_t us_count_tic =  us * (SystemCoreClock / 1000000); // получаем кол-во тактов за 1 мкс и умножаем на наше значение
    DWT->CYCCNT = 0U; 							// обнуляем счётчик
    while(DWT->CYCCNT < us_count_tic);
}


/*
 * функции работы с I2C
 */
HAL_StatusTypeDef Read_I2C(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t len)
   {
       HAL_StatusTypeDef returnValue;
       uint8_t addr[2];
       /* вычисляется MSB и LSB части адреса памяти */
       addr[0] = (uint8_t) ((MemAddress & 0xFF00) >> 8);
       addr[1] = (uint8_t) (MemAddress & 0xFF);
       /* Отправить адрес ячейки памяти, с которой начинаем читать данные */
       returnValue = HAL_I2C_Master_Transmit(hi2c, DevAddress, addr, 2, HAL_MAX_DELAY);
       if(returnValue != HAL_OK)
           return returnValue;
       /* получить данные из EEPROM */
       returnValue = HAL_I2C_Master_Receive(hi2c, DevAddress, pData, len, HAL_MAX_DELAY);
       return returnValue;
   }
HAL_StatusTypeDef Write_I2C(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData, uint16_t len)
   {
       HAL_StatusTypeDef returnValue;
       uint8_t *data;
       /* Сначала выделяется временный буфер для хранения целевой памяти
               * адрес и данные для хранения */
       data = (uint8_t*)malloc(sizeof(uint8_t)*(len+2));
       /* вычислить MSB и LSB части адреса памяти */
       data[0] = (uint8_t) ((MemAddress & 0xFF00) >> 8);
       data[1] = (uint8_t) (MemAddress & 0xFF);
       /* копировать содержимое массива pData во временный буфер */
       memcpy(data+2, pData, len);
       /* готовность передать буфер по шине I2C */
       returnValue = HAL_I2C_Master_Transmit(hi2c, DevAddress, data, len + 2, HAL_MAX_DELAY);
       if(returnValue != HAL_OK)
           return returnValue;
       free(data);
       /* Ожидание пока EEPROM сохранит данные в памяти */
       while(HAL_I2C_Master_Transmit(hi2c, DevAddress, 0, 0, HAL_MAX_DELAY) != HAL_OK);
       return HAL_OK;
   }

void Event_err(void){
	if (1 == fl_er){
		HAL_GPIO_WritePin(SOL1_ALIGN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(SOL2_PRESS,GPIO_PIN_SET);
		HAL_GPIO_WritePin(SOL3_GLUE,GPIO_PIN_SET);
		HAL_GPIO_WritePin(SOL4_EJECT,GPIO_PIN_SET);
		HAL_GPIO_WritePin(EN_STEP_MOT,GPIO_PIN_RESET);				//выключить ШД
	}
}

void TestSol(void){
	//соленойды
	HAL_GPIO_WritePin(SOL1_ALIGN,GPIO_PIN_RESET);
	Pause(500);
	HAL_GPIO_WritePin(SOL1_ALIGN,GPIO_PIN_SET);
	Pause(500);
	HAL_GPIO_WritePin(SOL2_PRESS,GPIO_PIN_RESET);
	Pause(500);
	HAL_GPIO_WritePin(SOL2_PRESS,GPIO_PIN_SET);
	Pause(500);
	HAL_GPIO_WritePin(SOL3_GLUE,GPIO_PIN_RESET);
	Pause(500);
	HAL_GPIO_WritePin(SOL3_GLUE,GPIO_PIN_SET);
	Pause(500);
	HAL_GPIO_WritePin(SOL4_EJECT,GPIO_PIN_RESET);
	Pause(500);
	HAL_GPIO_WritePin(SOL4_EJECT,GPIO_PIN_SET);
}

/*   биты input_UR
*   0 - оптодатчик наличия фото
*   1 - оптодатчик
*   2 - оптодатчик магнита
*   3 - нож открыт
*   4 - шт. открыт
*   5 - нож закрыт
*   6 -
*   7 -
*/

void TestInput(void){
	PortRead(&hi2c1, adr_ur_sens,&input_UR);			//опрос оптодатчиков
	if(bitRead(input_UR, 0) == 0){
		Msg("Opto_foto open");
	}
	else{
		Msg("Opto_foto close");
	}

	if(bitRead(input_UR, 1) == 0){
		Msg("Opto_ open");
	}
	else{
		Msg("Opto_ close");
	}

	if(bitRead(input_UR, 2) == 0){
		Msg("Opto_magn open");
	}
	else{
		Msg("Opto_magn close");
	}

	if(bitRead(input_UR, 3) == 0){
		Msg("Cut_ open");
	}

	if(bitRead(input_UR, 4) == 0){
		Msg("Sht_open");
	}

	if(bitRead(input_UR, 5) == 0){
		Msg("Cut_close");
	}

}

/*чтение структуры из EEPROM*/
void ReadEEPROM(void){


}

/*запись структуры в EEPROM*/
void WriteEEPROM(void){


}



















